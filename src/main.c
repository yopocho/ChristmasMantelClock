/* Zephyr */
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/rtc.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/crc.h>
/* LVGL */
#include <lvgl.h>
/* UI */
#include "ui/ui.h"
/* C */
#include <string.h>
#include <stdio.h>
#include <stdint.h>
 
/* Defines */
#define LOG_LEVEL LOG_LEVEL_DBG
#define WIDTH 240 // Display width
#define HEIGHT 240 // Display height
#define FRAME_TIME_TARGET 50 // ms. 20 FPS for a clock is plenty
#define PWM_PERIOD PWM_USEC(10) // us
#define SETTINGS_MAGIC 0xFEEDBEEF // Search data for flash journal

/* LVGL */
lv_indev_t * indev;

/* PWM */
uint8_t brightness;

/* Logging */
LOG_MODULE_REGISTER(logging_mantelclock, LOG_LEVEL_DBG);

/* Required global variables for EEZ UI */
char time_hr_global[100] = { 0 };
char time_min_global[100] = { 0 };

/* RTC Calender */
static struct rtc_time tm = {
	.tm_year = 2026 - 1900,
	.tm_mon = 01 - 1,
	.tm_mday = 9,
	.tm_hour = 11,
	.tm_min = 44,
	.tm_sec = 24,
};

typedef enum {
	CLOCK_TYPE_DIGITAL,
	CLOCK_TYPE_ANALOG
} clock_types;

/* Enum for current screen indidication */
typedef enum  {
	SCREEN_DIGITAL_CLOCK,
	SCREEN_ANALOG_CLOCK,
	SCREEN_DIGITAL_CLOCK_SET_TIME,
	SCREEN_ANALOG_CLOCK_SET_TIME,
	SCREEN_MENU,
	SCREEN_NONE,
	SCREEN_PREVIOUS
} screens;

/* Enum for possible UI colours (cherry-picked from LVGL Main Palette) */
typedef enum{
	White,
	Black,
	Red,
	Pink,
	Purple,
	Indigo,
	Blue,
	Cyan,
	Teal,
	Green,
	Lime,
	Yellow,
	Amber,
	Orange,
	Brown,
	Gray
} colours_t;

lv_color_t temp_selection_background_colour;
lv_color_t temp_selection_text_colour;

/* Set initial screen as SCREEN_DIGITAL_CLOCK (TODO: For now, this has to be retrieved from user settings eventually) */
screens current_screen = SCREEN_NONE;
screens next_screen = SCREEN_NONE;
screens previous_screen = SCREEN_NONE;

struct rtc_time current_time;

/* User Settings struct typedef*/
typedef struct __packed user_settings_t {
	uint8_t brightness;
	clock_types clock_type;
	colours_t background_colour;
	colours_t text_colour;
	uint8_t time_hr;
	uint8_t time_min;
} user_settings_t;

/* Flash */
static const struct flash_area *fa;
static size_t write_offset;

/* Flash settings */
struct __packed settings_record {
	uint32_t magic;
	uint16_t version;
	uint16_t length;
	uint32_t crc;
	struct user_settings_t settings;
};

/* Default user settings*/
// TODO: For now, must all be read from NVM
user_settings_t user_settings = {
	.brightness = 80,
	.clock_type = CLOCK_TYPE_DIGITAL,
	.background_colour = Orange,
	.text_colour = White,
	.time_hr = 14,
	.time_min = 49
};

/* Get devices from devicetree */
static const struct gpio_dt_spec dbg_led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct pwm_dt_spec  LCD_kathode_pwm = PWM_DT_SPEC_GET(DT_ALIAS(kathodepwm));
static const struct device *GC9A01 = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
static const struct device *const rtc = DEVICE_DT_GET(DT_ALIAS(rtc));
static const struct device *indev_dt = DEVICE_DT_GET(DT_COMPAT_GET_ANY_STATUS_OKAY(zephyr_lvgl_keypad_input));

/* Prototypes */
static int set_date_time(const struct device *rtc, struct rtc_time *settable_time);
static int get_date_time(const struct device *rtc, struct rtc_time *target_time);
static void display_time(void);
static int setup_dt(void);
static int setup_lvgl(void);

bool settings_flash_load(user_settings_t *out) {
	size_t off = 0;
	bool found = false;

	while (off + sizeof(struct settings_record) <= fa->fa_size) {
		struct settings_record rec;

		flash_area_read(fa, off, &rec, sizeof(rec));

		if(rec.magic != SETTINGS_MAGIC) {
			break;
		}

		if(rec.length == sizeof(user_settings_t)) {
			uint32_t crc = crc32_ieee(&rec.settings, sizeof(user_settings_t));
			if(crc == rec.crc) {
				*out = rec.settings;
				found = true;
			}
		}

		off += ROUND_UP(sizeof(rec), 4);
	}
	return found;
}

static size_t flash_find_tail(void) {
	size_t off = 0;

	while (off + sizeof(struct settings_record) <= fa->fa_size) {
		struct settings_record rec;

		flash_area_read(fa, off, &rec, sizeof(rec));

		if(rec.magic != SETTINGS_MAGIC || rec.length != sizeof(user_settings_t)) {
			break;
		}

		uint32_t crc = crc32_ieee(&rec.settings, sizeof(user_settings_t));

		if(crc != rec.crc) {
			break;
		}

		off += ROUND_UP(sizeof(rec), 4);
	}

	return off;
}

void settings_flash_init(void) {
	flash_area_open(FIXED_PARTITION_ID(user_storage), &fa);
	write_offset = flash_find_tail();
}

int settings_flash_save(const user_settings_t *in) {
	int ret;

	struct settings_record rec = {
		.magic = SETTINGS_MAGIC,
		.version = 1,
		.length = sizeof(user_settings_t),
		.crc = crc32_ieee(in, sizeof(user_settings_t)),
		.settings = *in
	};

	size_t rec_size = ROUND_UP(sizeof(rec), 4);

	if (write_offset + rec_size > fa->fa_size) {
		flash_area_erase(fa, 0, fa->fa_size);
		write_offset = 0;
	}

	ret = flash_area_write(fa, write_offset, &rec, sizeof(rec));
	if(ret) return -1;
	write_offset += rec_size;

	return 0;
}

/* Required implementations for EEZ UI */
const char *get_var_time_hr_global() {
    return time_hr_global;
}

void set_var_time_hr_global(const char *value) {
    strncpy(time_hr_global, value, sizeof(time_hr_global) / sizeof(char));
    time_hr_global[sizeof(time_hr_global) / sizeof(char) - 1] = 0;
}

const char *get_var_time_min_global() {
    return time_min_global;
}

void set_var_time_min_global(const char *value) {
    strncpy(time_min_global, value, sizeof(time_min_global) / sizeof(char));
    time_min_global[sizeof(time_min_global) / sizeof(char) - 1] = 0;
}

void action_change_screen(lv_event_t *e) {
	screens temp_screen = (screens) lv_event_get_user_data(e);
	LOG_DBG("Action change screen called with target screen: %d", temp_screen);	

	if(temp_screen == SCREEN_PREVIOUS) {
		next_screen = previous_screen;
		return;
	}

	if(current_screen != temp_screen) {
		previous_screen = current_screen;
    	next_screen = temp_screen;
	}
}

void action_digital_clock_set_time_save(lv_event_t *e) {
    // TODO: This implementation changes when adhering to the user set Clock Type (digital/analog)
	if(previous_screen == SCREEN_DIGITAL_CLOCK) {
		next_screen = SCREEN_DIGITAL_CLOCK;
	}
	else if(previous_screen == SCREEN_ANALOG_CLOCK) {
		next_screen = SCREEN_ANALOG_CLOCK;
	}
	LOG_DBG("Digital clock set time save button pressed");
}

void action_menu_save(lv_event_t * e) {
	// TODO: Implement saving brightness and such to NVM
	// Get the value of the spinbox and rollers
	LOG_DBG("Menu save button pressed");

	user_settings.background_colour = lv_roller_get_selected(objects.roller_menu_background_colour);
	user_settings.text_colour = lv_roller_get_selected(objects.roller_menu_text_colour);
	user_settings.brightness = lv_spinbox_get_value(objects.spinbox_menu_brightness);
	user_settings.clock_type = lv_roller_get_selected(objects.roller_menu_clock_type);

	int ret = settings_flash_save(&user_settings);
	if(ret < 0) LOG_ERR("Writing to flash failed!");
}

void action_menu_text_colour_value_changed(lv_event_t *e) {
    // TODO: If new styles get added, this function has to get updated
	// TODO: This functionality has to be moved to a seperate function so it can be used by other parts of the program
	uint8_t roller_index = lv_roller_get_selected(objects.roller_menu_text_colour);
	lv_style_t * temp_style_spinboxes_menu = get_style_style_spinboxes_menu_MAIN_DEFAULT();
	lv_style_t * temp_style_spinboxes_digital_clock = get_style_style_spinboxes_digital_clock_MAIN_DEFAULT();
	lv_style_t * temp_scale_analog_clock = get_style_style_scale_analog_clock_MAIN_DEFAULT();
	lv_style_t * temp_style_rollers_menu = get_style_style_rollers_menu_MAIN_DEFAULT();
	lv_style_t * temp_style_labels_menu = get_style_style_labels_menu_MAIN_DEFAULT();
	lv_style_t * temp_style_labels_digital_clock = get_style_style_labels_digital_clock_MAIN_DEFAULT();
	lv_style_t * temp_style_dropdown_menu_clock_type = get_style_style_dropdown_menu_clock_type_MAIN_DEFAULT();
	lv_style_t * temp_style_buttons_menu = get_style_style_buttons_menu_MAIN_DEFAULT();
	lv_style_t * temp_style_buttons_digital_clock_set_time = get_style_style_buttons_digital_clock_set_time_MAIN_DEFAULT();

	LOG_DBG("Menu text colour value changed with value: %d", roller_index);

	switch(roller_index) {
		case White:
			lv_style_set_text_color(temp_style_spinboxes_menu, lv_color_white());
			lv_style_set_text_color(temp_style_spinboxes_digital_clock, lv_color_white());
			lv_style_set_text_color(temp_scale_analog_clock, lv_color_white());
			lv_style_set_text_color(temp_style_rollers_menu, lv_color_white());
			lv_style_set_text_color(temp_style_labels_menu, lv_color_white());
			lv_style_set_text_color(temp_style_labels_digital_clock, lv_color_white());
			lv_style_set_text_color(temp_style_dropdown_menu_clock_type, lv_color_white());
			lv_style_set_text_color(temp_style_buttons_menu, lv_color_white());
			lv_style_set_text_color(temp_style_buttons_digital_clock_set_time, lv_color_white());
			break;
		case Black:
			lv_style_set_text_color(temp_style_spinboxes_menu, lv_color_black());
			lv_style_set_text_color(temp_style_spinboxes_digital_clock, lv_color_black());
			lv_style_set_text_color(temp_scale_analog_clock, lv_color_black());
			lv_style_set_text_color(temp_style_rollers_menu, lv_color_black());
			lv_style_set_text_color(temp_style_labels_menu, lv_color_black());
			lv_style_set_text_color(temp_style_labels_digital_clock, lv_color_black());
			lv_style_set_text_color(temp_style_dropdown_menu_clock_type, lv_color_black());
			lv_style_set_text_color(temp_style_buttons_menu, lv_color_black());
			lv_style_set_text_color(temp_style_buttons_digital_clock_set_time, lv_color_black());
			break;
		case Red:
			lv_style_set_text_color(temp_style_spinboxes_menu, lv_palette_main(LV_PALETTE_RED));
			lv_style_set_text_color(temp_style_spinboxes_digital_clock, lv_palette_main(LV_PALETTE_RED));
			lv_style_set_text_color(temp_scale_analog_clock, lv_palette_main(LV_PALETTE_RED));
			lv_style_set_text_color(temp_style_rollers_menu, lv_palette_main(LV_PALETTE_RED));
			lv_style_set_text_color(temp_style_labels_menu, lv_palette_main(LV_PALETTE_RED));
			lv_style_set_text_color(temp_style_labels_digital_clock, lv_palette_main(LV_PALETTE_RED));
			lv_style_set_text_color(temp_style_dropdown_menu_clock_type, lv_palette_main(LV_PALETTE_RED));
			lv_style_set_text_color(temp_style_buttons_menu, lv_palette_main(LV_PALETTE_RED));
			lv_style_set_text_color(temp_style_buttons_digital_clock_set_time, lv_palette_main(LV_PALETTE_RED));
			break;
		case Pink:
			lv_style_set_text_color(temp_style_spinboxes_menu, lv_palette_main(LV_PALETTE_PINK));
			lv_style_set_text_color(temp_style_spinboxes_digital_clock, lv_palette_main(LV_PALETTE_PINK));
			lv_style_set_text_color(temp_scale_analog_clock, lv_palette_main(LV_PALETTE_PINK));
			lv_style_set_text_color(temp_style_rollers_menu, lv_palette_main(LV_PALETTE_PINK));
			lv_style_set_text_color(temp_style_labels_menu, lv_palette_main(LV_PALETTE_PINK));
			lv_style_set_text_color(temp_style_labels_digital_clock, lv_palette_main(LV_PALETTE_PINK));
			lv_style_set_text_color(temp_style_dropdown_menu_clock_type, lv_palette_main(LV_PALETTE_PINK));
			lv_style_set_text_color(temp_style_buttons_menu, lv_palette_main(LV_PALETTE_PINK));
			lv_style_set_text_color(temp_style_buttons_digital_clock_set_time, lv_palette_main(LV_PALETTE_PINK));
			break;
		case Purple:
			lv_style_set_text_color(temp_style_spinboxes_menu, lv_palette_main(LV_PALETTE_PURPLE));
			lv_style_set_text_color(temp_style_spinboxes_digital_clock, lv_palette_main(LV_PALETTE_PURPLE));
			lv_style_set_text_color(temp_scale_analog_clock, lv_palette_main(LV_PALETTE_PURPLE));
			lv_style_set_text_color(temp_style_rollers_menu, lv_palette_main(LV_PALETTE_PURPLE));
			lv_style_set_text_color(temp_style_labels_menu, lv_palette_main(LV_PALETTE_PURPLE));
			lv_style_set_text_color(temp_style_labels_digital_clock, lv_palette_main(LV_PALETTE_PURPLE));
			lv_style_set_text_color(temp_style_dropdown_menu_clock_type, lv_palette_main(LV_PALETTE_PURPLE));
			lv_style_set_text_color(temp_style_buttons_menu, lv_palette_main(LV_PALETTE_PURPLE));
			lv_style_set_text_color(temp_style_buttons_digital_clock_set_time, lv_palette_main(LV_PALETTE_PURPLE));
			break;
		case Indigo:
			lv_style_set_text_color(temp_style_spinboxes_menu, lv_palette_main(LV_PALETTE_INDIGO));
			lv_style_set_text_color(temp_style_spinboxes_digital_clock, lv_palette_main(LV_PALETTE_INDIGO));
			lv_style_set_text_color(temp_scale_analog_clock, lv_palette_main(LV_PALETTE_INDIGO));
			lv_style_set_text_color(temp_style_rollers_menu, lv_palette_main(LV_PALETTE_INDIGO));
			lv_style_set_text_color(temp_style_labels_menu, lv_palette_main(LV_PALETTE_INDIGO));
			lv_style_set_text_color(temp_style_labels_digital_clock, lv_palette_main(LV_PALETTE_INDIGO));
			lv_style_set_text_color(temp_style_dropdown_menu_clock_type, lv_palette_main(LV_PALETTE_INDIGO));
			lv_style_set_text_color(temp_style_buttons_menu, lv_palette_main(LV_PALETTE_INDIGO));
			lv_style_set_text_color(temp_style_buttons_digital_clock_set_time, lv_palette_main(LV_PALETTE_INDIGO));
			break;
		case Blue:
			lv_style_set_text_color(temp_style_spinboxes_menu, lv_palette_main(LV_PALETTE_BLUE));
			lv_style_set_text_color(temp_style_spinboxes_digital_clock, lv_palette_main(LV_PALETTE_BLUE));
			lv_style_set_text_color(temp_scale_analog_clock, lv_palette_main(LV_PALETTE_BLUE));
			lv_style_set_text_color(temp_style_rollers_menu, lv_palette_main(LV_PALETTE_BLUE));
			lv_style_set_text_color(temp_style_labels_menu, lv_palette_main(LV_PALETTE_BLUE));
			lv_style_set_text_color(temp_style_labels_digital_clock, lv_palette_main(LV_PALETTE_BLUE));
			lv_style_set_text_color(temp_style_dropdown_menu_clock_type, lv_palette_main(LV_PALETTE_BLUE));
			lv_style_set_text_color(temp_style_buttons_menu, lv_palette_main(LV_PALETTE_BLUE));
			lv_style_set_text_color(temp_style_buttons_digital_clock_set_time, lv_palette_main(LV_PALETTE_BLUE));
			break;
		case Cyan:
			lv_style_set_text_color(temp_style_spinboxes_menu, lv_palette_main(LV_PALETTE_CYAN));
			lv_style_set_text_color(temp_style_spinboxes_digital_clock, lv_palette_main(LV_PALETTE_CYAN));
			lv_style_set_text_color(temp_scale_analog_clock, lv_palette_main(LV_PALETTE_CYAN));
			lv_style_set_text_color(temp_style_rollers_menu, lv_palette_main(LV_PALETTE_CYAN));
			lv_style_set_text_color(temp_style_labels_menu, lv_palette_main(LV_PALETTE_CYAN));
			lv_style_set_text_color(temp_style_labels_digital_clock, lv_palette_main(LV_PALETTE_CYAN));
			lv_style_set_text_color(temp_style_dropdown_menu_clock_type, lv_palette_main(LV_PALETTE_CYAN));
			lv_style_set_text_color(temp_style_buttons_menu, lv_palette_main(LV_PALETTE_CYAN));
			lv_style_set_text_color(temp_style_buttons_digital_clock_set_time, lv_palette_main(LV_PALETTE_CYAN));
			break;
		case Teal:
			lv_style_set_text_color(temp_style_spinboxes_menu, lv_palette_main(LV_PALETTE_TEAL));
			lv_style_set_text_color(temp_style_spinboxes_digital_clock, lv_palette_main(LV_PALETTE_TEAL));
			lv_style_set_text_color(temp_scale_analog_clock, lv_palette_main(LV_PALETTE_TEAL));
			lv_style_set_text_color(temp_style_rollers_menu, lv_palette_main(LV_PALETTE_TEAL));
			lv_style_set_text_color(temp_style_labels_menu, lv_palette_main(LV_PALETTE_TEAL));
			lv_style_set_text_color(temp_style_labels_digital_clock, lv_palette_main(LV_PALETTE_TEAL));
			lv_style_set_text_color(temp_style_dropdown_menu_clock_type, lv_palette_main(LV_PALETTE_TEAL));
			lv_style_set_text_color(temp_style_buttons_menu, lv_palette_main(LV_PALETTE_TEAL));
			lv_style_set_text_color(temp_style_buttons_digital_clock_set_time, lv_palette_main(LV_PALETTE_TEAL));
			break;
		case Green:
			lv_style_set_text_color(temp_style_spinboxes_menu, lv_palette_main(LV_PALETTE_GREEN));
			lv_style_set_text_color(temp_style_spinboxes_digital_clock, lv_palette_main(LV_PALETTE_GREEN));
			lv_style_set_text_color(temp_scale_analog_clock, lv_palette_main(LV_PALETTE_GREEN));
			lv_style_set_text_color(temp_style_rollers_menu, lv_palette_main(LV_PALETTE_GREEN));
			lv_style_set_text_color(temp_style_labels_menu, lv_palette_main(LV_PALETTE_GREEN));
			lv_style_set_text_color(temp_style_labels_digital_clock, lv_palette_main(LV_PALETTE_GREEN));
			lv_style_set_text_color(temp_style_dropdown_menu_clock_type, lv_palette_main(LV_PALETTE_GREEN));
			lv_style_set_text_color(temp_style_buttons_menu, lv_palette_main(LV_PALETTE_GREEN));
			lv_style_set_text_color(temp_style_buttons_digital_clock_set_time, lv_palette_main(LV_PALETTE_GREEN));
			break;
		case Lime:
			lv_style_set_text_color(temp_style_spinboxes_menu, lv_palette_main(LV_PALETTE_LIME));
			lv_style_set_text_color(temp_style_spinboxes_digital_clock, lv_palette_main(LV_PALETTE_LIME));
			lv_style_set_text_color(temp_scale_analog_clock, lv_palette_main(LV_PALETTE_LIME));
			lv_style_set_text_color(temp_style_rollers_menu, lv_palette_main(LV_PALETTE_LIME));
			lv_style_set_text_color(temp_style_labels_menu, lv_palette_main(LV_PALETTE_LIME));
			lv_style_set_text_color(temp_style_labels_digital_clock, lv_palette_main(LV_PALETTE_LIME));
			lv_style_set_text_color(temp_style_dropdown_menu_clock_type, lv_palette_main(LV_PALETTE_LIME));
			lv_style_set_text_color(temp_style_buttons_menu, lv_palette_main(LV_PALETTE_LIME));
			lv_style_set_text_color(temp_style_buttons_digital_clock_set_time, lv_palette_main(LV_PALETTE_LIME));
			break;
		case Yellow:
			lv_style_set_text_color(temp_style_spinboxes_menu, lv_palette_main(LV_PALETTE_YELLOW));
			lv_style_set_text_color(temp_style_spinboxes_digital_clock, lv_palette_main(LV_PALETTE_YELLOW));
			lv_style_set_text_color(temp_scale_analog_clock, lv_palette_main(LV_PALETTE_YELLOW));
			lv_style_set_text_color(temp_style_rollers_menu, lv_palette_main(LV_PALETTE_YELLOW));
			lv_style_set_text_color(temp_style_labels_menu, lv_palette_main(LV_PALETTE_YELLOW));
			lv_style_set_text_color(temp_style_labels_digital_clock, lv_palette_main(LV_PALETTE_YELLOW));
			lv_style_set_text_color(temp_style_dropdown_menu_clock_type, lv_palette_main(LV_PALETTE_YELLOW));
			lv_style_set_text_color(temp_style_buttons_menu, lv_palette_main(LV_PALETTE_YELLOW));
			lv_style_set_text_color(temp_style_buttons_digital_clock_set_time, lv_palette_main(LV_PALETTE_YELLOW));
			break;
		case Amber:
			lv_style_set_text_color(temp_style_spinboxes_menu, lv_palette_main(LV_PALETTE_AMBER));
			lv_style_set_text_color(temp_style_spinboxes_digital_clock, lv_palette_main(LV_PALETTE_AMBER));
			lv_style_set_text_color(temp_scale_analog_clock, lv_palette_main(LV_PALETTE_AMBER));
			lv_style_set_text_color(temp_style_rollers_menu, lv_palette_main(LV_PALETTE_AMBER));
			lv_style_set_text_color(temp_style_labels_menu, lv_palette_main(LV_PALETTE_AMBER));
			lv_style_set_text_color(temp_style_labels_digital_clock, lv_palette_main(LV_PALETTE_AMBER));
			lv_style_set_text_color(temp_style_dropdown_menu_clock_type, lv_palette_main(LV_PALETTE_AMBER));
			lv_style_set_text_color(temp_style_buttons_menu, lv_palette_main(LV_PALETTE_AMBER));
			lv_style_set_text_color(temp_style_buttons_digital_clock_set_time, lv_palette_main(LV_PALETTE_AMBER));
			break;
		case Orange:
			lv_style_set_text_color(temp_style_spinboxes_menu, lv_palette_main(LV_PALETTE_ORANGE));
			lv_style_set_text_color(temp_style_spinboxes_digital_clock, lv_palette_main(LV_PALETTE_ORANGE));
			lv_style_set_text_color(temp_scale_analog_clock, lv_palette_main(LV_PALETTE_ORANGE));
			lv_style_set_text_color(temp_style_rollers_menu, lv_palette_main(LV_PALETTE_ORANGE));
			lv_style_set_text_color(temp_style_labels_menu, lv_palette_main(LV_PALETTE_ORANGE));
			lv_style_set_text_color(temp_style_labels_digital_clock, lv_palette_main(LV_PALETTE_ORANGE));
			lv_style_set_text_color(temp_style_dropdown_menu_clock_type, lv_palette_main(LV_PALETTE_ORANGE));
			lv_style_set_text_color(temp_style_buttons_menu, lv_palette_main(LV_PALETTE_ORANGE));
			lv_style_set_text_color(temp_style_buttons_digital_clock_set_time, lv_palette_main(LV_PALETTE_ORANGE));
			break;
		case Brown:
			lv_style_set_text_color(temp_style_spinboxes_menu, lv_palette_main(LV_PALETTE_BROWN));
			lv_style_set_text_color(temp_style_spinboxes_digital_clock, lv_palette_main(LV_PALETTE_BROWN));
			lv_style_set_text_color(temp_scale_analog_clock, lv_palette_main(LV_PALETTE_BROWN));
			lv_style_set_text_color(temp_style_rollers_menu, lv_palette_main(LV_PALETTE_BROWN));
			lv_style_set_text_color(temp_style_labels_menu, lv_palette_main(LV_PALETTE_BROWN));
			lv_style_set_text_color(temp_style_labels_digital_clock, lv_palette_main(LV_PALETTE_BROWN));
			lv_style_set_text_color(temp_style_dropdown_menu_clock_type, lv_palette_main(LV_PALETTE_BROWN));
			lv_style_set_text_color(temp_style_buttons_menu, lv_palette_main(LV_PALETTE_BROWN));
			lv_style_set_text_color(temp_style_buttons_digital_clock_set_time, lv_palette_main(LV_PALETTE_BROWN));
			break;
		case Gray:
			lv_style_set_text_color(temp_style_spinboxes_menu, lv_palette_main(LV_PALETTE_GREY));
			lv_style_set_text_color(temp_style_spinboxes_digital_clock, lv_palette_main(LV_PALETTE_GREY));
			lv_style_set_text_color(temp_scale_analog_clock, lv_palette_main(LV_PALETTE_GREY));
			lv_style_set_text_color(temp_style_rollers_menu, lv_palette_main(LV_PALETTE_GREY));
			lv_style_set_text_color(temp_style_labels_menu, lv_palette_main(LV_PALETTE_GREY));
			lv_style_set_text_color(temp_style_labels_digital_clock, lv_palette_main(LV_PALETTE_GREY));
			lv_style_set_text_color(temp_style_dropdown_menu_clock_type, lv_palette_main(LV_PALETTE_GREY));
			lv_style_set_text_color(temp_style_buttons_menu, lv_palette_main(LV_PALETTE_GREY));	
			lv_style_set_text_color(temp_style_buttons_digital_clock_set_time, lv_palette_main(LV_PALETTE_GREY));
			break;
		default:
			// Do nothing
			break;
	}

	lv_obj_report_style_change(NULL); // Only do this if the solution below does not work
	// lv_obj_report_style_change(&temp_style_spinboxes_menu);
	// lv_obj_report_style_change(&temp_style_spinboxes_digital_clock);
	// lv_obj_report_style_change(&temp_scale_analog_clock);
	// lv_obj_report_style_change(&temp_style_rollers_menu);
	// lv_obj_report_style_change(&temp_style_labels_menu);
	// lv_obj_report_style_change(&temp_style_labels_digital_clock);
	// lv_obj_report_style_change(&temp_style_dropdown_menu_clock_type);
	// lv_obj_report_style_change(&temp_style_buttons_menu);
	// lv_obj_report_style_change(&temp_style_buttons_digital_clock_set_time);
}

void action_menu_background_colour_value_changed(lv_event_t *e) {
	uint8_t roller_index = lv_roller_get_selected(objects.roller_menu_background_colour);
	LOG_DBG("Menu background colour value changed with value: %d", roller_index);
	switch(roller_index) {
		case White:
			lv_obj_set_style_bg_color(objects.scr_menu, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_analog_clock, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_digital_clock, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
			break;
		case Black:
			lv_obj_set_style_bg_color(objects.scr_menu, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_analog_clock, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_digital_clock, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);
			break;
		case Red:
			lv_obj_set_style_bg_color(objects.scr_menu, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_analog_clock, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_digital_clock, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN | LV_STATE_DEFAULT);
			break;
		case Pink:
			lv_obj_set_style_bg_color(objects.scr_menu, lv_palette_main(LV_PALETTE_PINK), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_analog_clock, lv_palette_main(LV_PALETTE_PINK), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_digital_clock, lv_palette_main(LV_PALETTE_PINK), LV_PART_MAIN | LV_STATE_DEFAULT);
			break;
		case Purple:
			lv_obj_set_style_bg_color(objects.scr_menu, lv_palette_main(LV_PALETTE_PURPLE), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_analog_clock, lv_palette_main(LV_PALETTE_PURPLE), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_digital_clock, lv_palette_main(LV_PALETTE_PURPLE), LV_PART_MAIN | LV_STATE_DEFAULT);
			break;
		case Indigo:
			lv_obj_set_style_bg_color(objects.scr_menu, lv_palette_main(LV_PALETTE_INDIGO), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_analog_clock, lv_palette_main(LV_PALETTE_INDIGO), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_digital_clock, lv_palette_main(LV_PALETTE_INDIGO), LV_PART_MAIN | LV_STATE_DEFAULT);
			break;
		case Blue:
			lv_obj_set_style_bg_color(objects.scr_menu, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_analog_clock, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_digital_clock, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN | LV_STATE_DEFAULT);
			break;
		case Cyan:
			lv_obj_set_style_bg_color(objects.scr_menu, lv_palette_main(LV_PALETTE_CYAN), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_analog_clock, lv_palette_main(LV_PALETTE_CYAN), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_digital_clock, lv_palette_main(LV_PALETTE_CYAN), LV_PART_MAIN | LV_STATE_DEFAULT);
			break;
		case Teal:
			lv_obj_set_style_bg_color(objects.scr_menu, lv_palette_main(LV_PALETTE_TEAL), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_analog_clock, lv_palette_main(LV_PALETTE_TEAL), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_digital_clock, lv_palette_main(LV_PALETTE_TEAL), LV_PART_MAIN | LV_STATE_DEFAULT);
			break;
		case Green:
			lv_obj_set_style_bg_color(objects.scr_menu, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_analog_clock, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_digital_clock, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN | LV_STATE_DEFAULT);
			break;
		case Lime:
			lv_obj_set_style_bg_color(objects.scr_menu, lv_palette_main(LV_PALETTE_LIME), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_analog_clock, lv_palette_main(LV_PALETTE_LIME), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_digital_clock, lv_palette_main(LV_PALETTE_LIME), LV_PART_MAIN | LV_STATE_DEFAULT);
			break;
		case Yellow:
			lv_obj_set_style_bg_color(objects.scr_menu, lv_palette_main(LV_PALETTE_YELLOW), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_analog_clock, lv_palette_main(LV_PALETTE_YELLOW), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_digital_clock, lv_palette_main(LV_PALETTE_YELLOW), LV_PART_MAIN | LV_STATE_DEFAULT);
			break;
		case Amber:
			lv_obj_set_style_bg_color(objects.scr_menu, lv_palette_main(LV_PALETTE_AMBER), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_analog_clock, lv_palette_main(LV_PALETTE_AMBER), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_digital_clock, lv_palette_main(LV_PALETTE_AMBER), LV_PART_MAIN | LV_STATE_DEFAULT);
			break;
		case Orange:
			lv_obj_set_style_bg_color(objects.scr_menu, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_analog_clock, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_digital_clock, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_MAIN | LV_STATE_DEFAULT);
			break;
		case Brown:
			lv_obj_set_style_bg_color(objects.scr_menu, lv_palette_main(LV_PALETTE_BROWN), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_analog_clock, lv_palette_main(LV_PALETTE_BROWN), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_digital_clock, lv_palette_main(LV_PALETTE_BROWN), LV_PART_MAIN | LV_STATE_DEFAULT);
			break;
		case Gray:
			lv_obj_set_style_bg_color(objects.scr_menu, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_analog_clock, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN | LV_STATE_DEFAULT);
			lv_obj_set_style_bg_color(objects.scr_digital_clock, lv_palette_main(LV_PALETTE_GREY), LV_PART_MAIN | LV_STATE_DEFAULT);
			break;
		default:
			break;
	}
	lv_obj_invalidate(objects.scr_menu);
	lv_obj_invalidate(objects.scr_analog_clock);
	lv_obj_invalidate(objects.scr_digital_clock);
}

void action_menu_clock_type_value_changed(lv_event_t *e) {
    // TODO: Implement action menu_clock_type_value_changed here
	LOG_DBG("Menu clock type value changed with value: %d", lv_roller_get_selected(objects.roller_menu_clock_type));
}

void action_menu_brightness_value_changed(lv_event_t *e) {
    // TODO: Implement action menu_brightness_value_changed here
	float temp_brightness = lv_spinbox_get_value(objects.spinbox_menu_brightness);
	LOG_DBG("Menu clock type value changed with value: %d", temp_brightness);
	uint32_t pulse_width_us = PWM_PERIOD * (temp_brightness / 100);
	LOG_DBG("New pulse width: %d", pulse_width_us);
	pwm_set_dt(&LCD_kathode_pwm, PWM_PERIOD, (int) PWM_PERIOD * (temp_brightness / 100));
}

/**
 * @brief Set the current time of the RTC device pointed at by *rtc using the tm struct
 * 
 * @retval Zephyr retval
 */
static int set_date_time(const struct device *rtc, struct rtc_time *settable_time)
{
	int ret = 0;

	ret = rtc_set_time(rtc, settable_time);
	if (ret < 0) {
		LOG_ERR("Cannot write date time: %d\n", ret);
		return ret;
	}

	return ret;
}

/**
 * @brief Read the current time from the RTC device pointed at by *rtc
 * 
 * @retval Zephyr retval
*/
static int get_date_time(const struct device *rtc, struct rtc_time *target_time)
{
	int ret = 0;

	ret = rtc_get_time(rtc, target_time);
	if (ret < 0) {
		LOG_ERR("Cannot read date time: %d\n", ret);
		return ret;
	}

	// LOG_INF("RTC date and time: %04d-%02d-%02d %02d:%02d:%02d\n", target_time->tm_year + 1900,
	//        target_time->tm_mon + 1, target_time->tm_mday, target_time->tm_hour, target_time->tm_min, target_time->tm_sec);

	return ret;
}

static void display_time(void) {
	char temp_time_str_hr[3];
	char temp_time_str_min[3];
	if(current_screen == SCREEN_DIGITAL_CLOCK) {
		get_date_time(rtc, &current_time);
		sprintf(temp_time_str_hr, "%02d", current_time.tm_hour);
		sprintf(temp_time_str_min, "%02d", current_time.tm_min);
		set_var_time_hr_global(temp_time_str_hr); // Update EEZ UI global hour variable
		set_var_time_min_global(temp_time_str_min); // Update EEZ UI global minute variable
	}
	else if(current_screen == SCREEN_ANALOG_CLOCK) {
		// TODO: Add analog clock time update (implementation for time showing through a scale widget probably, which needs rotational angles for the hands)
	}

}

/**
 * @brief Setup function for the devices from the devicetree
 * 
 * @retval Zephyr retval
 */
static int setup_dt(void) {
	int ret = 0;

	/* Check if debug GPIO is ready, but not imperative */
	if(!gpio_is_ready_dt(&dbg_led)) {
		LOG_ERR("GPIO device is not ready\n");
	}

	/* Enable GPIOs and set outputs active */
	ret = gpio_pin_configure_dt(&dbg_led, GPIO_OUTPUT_INACTIVE); // Turn on LED
    if (ret < 0) {
		LOG_ERR("GPIO configuring failed\n");
        return ret;
    }

	/* Check if display is ready */
	while (!device_is_ready(GC9A01)) {
		LOG_ERR("Display device is not ready\n");
		k_sleep(K_MSEC(500));
		// return ret;
	}

	/* Check if display kathode PWM is ready */
	if (!pwm_is_ready_dt(&LCD_kathode_pwm)) {
		LOG_ERR("Kathode PWM device is not ready\n");
		return ret;
	}

    /* Check if the RTC is ready */
	if (!device_is_ready(rtc)) {
		LOG_ERR("RTC device is not ready\n");
		return ret;
	}

	/* Check if the "keypad" (2 buttons) is ready */
	if (!device_is_ready(indev_dt)) {
		LOG_ERR("Keypad device is not ready\n");
		return ret;
	}

	/* Set the RTC calender*/
	ret = set_date_time(rtc, &tm);
	if (ret < 0) {
		LOG_ERR("RTC set_date_time failed\n");
        return ret;
    }

	/* Init display brightness from user settings */
	ret = pwm_set_dt(&LCD_kathode_pwm, PWM_PERIOD, PWM_PERIOD * ((float) user_settings.brightness / (float) 100));
	if (ret < 0) {
		LOG_ERR("PWM setting failed\n");
		return ret;
	}

	return 0;
}

/**
 * @brief Setup function for LVGL widgets and devices
 * 
 * @retval Zephyr retval
 */
static int setup_lvgl(void) {
	int ret;

	/* Register the 2 buttons as keypad indev */
	indev = lv_indev_get_next(NULL);
	if(indev == NULL) {
		LOG_ERR("LVGL indev_get_next failed\n");
		return -1;
	}

	/* Init the UI with the indev */
	ui_create_groups();
	lv_indev_set_group(indev, groups.group_digital_clock);

	/* Init UI */
	ui_init();

	/* Disable blanking the display to prevent having to redraw */
	ret = display_blanking_off(GC9A01);
	k_sleep(K_MSEC(120));
	if (ret < 0 && ret != -ENOSYS) {
		LOG_ERR("Failed to turn blanking off (error %d)", ret);
		return ret;
	}

	display_time();

	/* Set initial screen */
	if(user_settings.clock_type == CLOCK_TYPE_DIGITAL) next_screen = SCREEN_DIGITAL_CLOCK;
	else if(user_settings.clock_type == CLOCK_TYPE_ANALOG) next_screen = SCREEN_ANALOG_CLOCK;
	else return -1;

	/* Set initial values from user settings */
	lv_spinbox_set_value(objects.spinbox_menu_brightness, user_settings.brightness);
	lv_roller_set_selected(objects.roller_menu_background_colour, user_settings.background_colour, LV_ANIM_OFF);
	lv_roller_set_selected(objects.roller_menu_text_colour, user_settings.text_colour, LV_ANIM_OFF);
	lv_roller_set_selected(objects.roller_menu_clock_type, user_settings.clock_type, LV_ANIM_OFF);

	return 0;
}

/**
 * @brief Main function loop
 * 
 * @retval -1 if failed
 */
int main(void)
{
	// TODO: NEED the user settings a.s.a.p. as I need to use it in many places throughout the setup (RTC setting, init values etc.)
	int ret;
	/* devicetree setup*/
	ret = setup_dt();	
	if (ret < 0) {
		LOG_ERR("setup_dt failed. Err: %d\n", ret);
    //     return -1;
    }
	LOG_INF("Devicetree setup complete");

	/* LVGL and EEZ UI setup */
	ret = setup_lvgl();
	if (ret < 0) {
		LOG_ERR("setup_lvgl failed. Err: %d\n", ret);
    //     return -1;
    }
	LOG_INF("LVGL setup complete");

	/* Flash setup */
	settings_flash_init();

	/* Read settings from flash */
	if(settings_flash_load(&user_settings)) {
		LOG_INF("Loaded settings from flash");
		LOG_INF("%d, %d, %d, %d, %d, %d", 
				user_settings.brightness, 
				user_settings.clock_type, 
				user_settings.background_colour, 
				user_settings.text_colour, 
				user_settings.time_hr, 
				user_settings.time_min);
	}
	else LOG_WRN("Failed to retreive settings from flash");

	LOG_DBG("Group digital_clock: %d", groups.group_digital_clock);
	LOG_DBG("Group digital_clock_set_time: %d", groups.group_digital_clock_set_time);
	LOG_DBG("sizeof(user_settings_t): %d", sizeof(user_settings_t));

	/* MAIN LOOP */
	LOG_INF("Running");
	while (1) {
		/* Switch screens manually, updated through EEZ UI action callback */
		if(next_screen != current_screen) {
			switch(next_screen) {
				case SCREEN_DIGITAL_CLOCK:
					LOG_DBG("Switching to SCREEN_DIGITAL_CLOCK");
					if(lv_screen_active() != objects.scr_digital_clock) loadScreen(SCREEN_ID_SCR_DIGITAL_CLOCK);
					lv_indev_set_group(indev, groups.group_digital_clock);
					lv_obj_remove_flag(objects.cont_digital_clock, LV_OBJ_FLAG_HIDDEN);
					lv_obj_add_flag(objects.cont_digital_clock_set_time, LV_OBJ_FLAG_HIDDEN);
					lv_obj_add_flag(objects.cont_buttons_digital_clock_set_time, LV_OBJ_FLAG_HIDDEN);
					lv_group_focus_obj(objects.label_time_hr_digital_clock);
					previous_screen = current_screen;
					current_screen = SCREEN_DIGITAL_CLOCK;
					break;
				case SCREEN_ANALOG_CLOCK:
					LOG_DBG("Switching to SCREEN_ANALOG_CLOCK");
					if(lv_screen_active() != objects.scr_analog_clock) loadScreen(SCREEN_ID_SCR_ANALOG_CLOCK);
					current_screen = SCREEN_ANALOG_CLOCK;
					// TODO: Add group for analog clock
					break;
				case SCREEN_DIGITAL_CLOCK_SET_TIME:
					LOG_DBG("Switching to SCREEN_DIGITAL_CLOCK_SET_TIME");
					if(lv_screen_active() != objects.scr_digital_clock) loadScreen(SCREEN_ID_SCR_DIGITAL_CLOCK);
					lv_indev_set_group(indev, groups.group_digital_clock_set_time);
					lv_obj_add_flag(objects.cont_digital_clock, LV_OBJ_FLAG_HIDDEN);
					lv_obj_remove_flag(objects.cont_digital_clock_set_time, LV_OBJ_FLAG_HIDDEN);
					lv_obj_remove_flag(objects.cont_buttons_digital_clock_set_time, LV_OBJ_FLAG_HIDDEN);
					lv_group_focus_obj(objects.spinbox_hr_digital_clock_set_time);
					previous_screen = current_screen;
					current_screen = SCREEN_DIGITAL_CLOCK_SET_TIME;
					break;
				case SCREEN_ANALOG_CLOCK_SET_TIME:
					LOG_DBG("Switching to SCREEN_ANALOG_CLOCK_SET_TIME");
					if(lv_screen_active() != objects.scr_analog_clock) loadScreen(SCREEN_ID_SCR_ANALOG_CLOCK);
					previous_screen = current_screen;
					current_screen = SCREEN_ANALOG_CLOCK_SET_TIME;
					break;
				case SCREEN_MENU:
					LOG_DBG("Switching to SCREEN_MENU");
					if(lv_screen_active() != objects.scr_menu) loadScreen(SCREEN_ID_SCR_MENU);
					temp_selection_text_colour = lv_obj_get_style_text_color(objects.spinbox_menu_brightness, LV_PART_MAIN); // Save the current text colour in case user cancels
					temp_selection_background_colour = lv_obj_get_style_bg_color(objects.scr_menu, LV_PART_MAIN); // Save the current background colour in case user cancels
					lv_indev_set_group(indev, groups.group_menu);
					lv_group_focus_obj(objects.spinbox_menu_brightness);
					previous_screen = current_screen;
					current_screen = SCREEN_MENU;
					break;
				default:
					// Do nothing
					break;
			}
		}

		lv_task_handler(); // Handle LVGL-related tasks
		ui_tick(); // Handle EEZ UI-related tasks

		/* Update the time on widgets which are relevant for the current screen */
		if((current_screen == SCREEN_DIGITAL_CLOCK) || (current_screen == SCREEN_ANALOG_CLOCK)) {
			display_time();
		}

		/* Time for other Zephyr-related threads */
		k_sleep(K_MSEC(FRAME_TIME_TARGET));
	}

	return -1;
}

/* TODO: List of improvements
 * Save user settings to NVM (brightness, clock type, colours)
 * Read settings from NVM whenever switching between screens, states, on boot, just really whenever :')
 *  > Probably better to read settings on boot once and store in a (admittedly large) global struct or something
 * Optimize LVGL config (CONFIG_LV_CONF_MINIMAL=y)
 * Optimize memory allocation through config
 * Increase the SPI speed to maximum stable(!)
 */