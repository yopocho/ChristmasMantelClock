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
/* LVGL */
#include <lvgl.h>
/* C */
#include <string.h>
#include <stdio.h>

/* Defines */
#define LOG_LEVEL LOG_LEVEL_DBG
#define WIDTH 240 // Display width
#define HEIGHT 240 // Display height
#define FRAME_TIME_TARGET 50 // ms. 20 FPS for a clock is plenty
#define PWM_PERIOD PWM_USEC(10) // us

/* PWM */
uint8_t brightness;

/* Logging */
LOG_MODULE_REGISTER(logging_mantelclock, LOG_LEVEL_DBG);

/* LVGL */
/* Screens */
lv_obj_t * scr_digital_clock;
lv_obj_t * scr_analog_clock;
lv_obj_t * scr_digital_clock_set_time;
lv_obj_t * scr_analog_clock_set_time;
lv_obj_t * scr_menu;


/* Style definitions */
lv_style_t style_spinbox_time;
lv_style_t style_label_time;
lv_style_t style_object_selector;
lv_style_t style_parent_digital_clock;
lv_style_t style_colon;

/* Widget groups */
lv_group_t * group_digital_clock;
lv_group_t * group_digital_clock_set_time;
lv_group_t * group_analog_clock;
lv_group_t * group_analog_clock_set_time;
lv_group_t * group_menu;


/* Objects and such */
lv_indev_t * indev;

lv_obj_t * spinbox_hr;
lv_obj_t * spinbox_min;

lv_obj_t * label_hr;
lv_obj_t * label_min;

lv_obj_t * label_colon;

/* RTC Calender */
static struct rtc_time tm = {
	.tm_year = 2026 - 1900,
	.tm_mon = 01 - 1,
	.tm_mday = 06,
	.tm_hour = 18,
	.tm_min = 03,
	.tm_sec = 24,
};

/* Enum for clear current screen indidication */
typedef enum  {
	SCREEN_DIGITAL_CLOCK,
	SCREEN_ANALOG_CLOCK,
	SCREEN_DIGITAL_CLOCK_SET_TIME,
	SCREEN_ANALOG_CLOCK_SET_TIME,
	SCREEN_MENU
} screens;

/* Set initial screen as SCREEN_DIGITAL_CLOCK (TODO: For now, this has to be retrieved from user settings eventually) */
screens current_screen = SCREEN_DIGITAL_CLOCK;
screens next_screen = SCREEN_DIGITAL_CLOCK;
screens previous_screen = SCREEN_DIGITAL_CLOCK;


struct rtc_time current_time;

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
// void user_interaction_cb(lv_event_t * event);

static void spinbox_interaction_cb(lv_event_t * e) 
{
	ARG_UNUSED(e);
	// lv_event_code_t code = lv_event_get_code(e);
    // lv_obj_t * obj = lv_event_get_target(e);
	// lv_obj_t * temp_screen = lv_screen_active();
	gpio_pin_toggle_dt(&dbg_led);
}

/**
 * @brief Callback function for user interaction with spinboxes on scr_digital_clock
 * 
 */
static void label_interaction_cb(lv_event_t * e) 
{	
	ARG_UNUSED(e);
	gpio_pin_toggle_dt(&dbg_led);

	next_screen = SCREEN_DIGITAL_CLOCK_SET_TIME;
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
	if(current_screen == SCREEN_DIGITAL_CLOCK) {
		char temp_time_str_hr[3];
		char temp_time_str_min[3];
		get_date_time(rtc, &current_time);
		LOG_INF("Current time: %d:%d:%d", current_time.tm_hour, current_time.tm_min, current_time.tm_sec);
		sprintf(temp_time_str_hr, "%02d", current_time.tm_hour);
		sprintf(temp_time_str_min, "%02d", current_time.tm_min);
		lv_label_set_text(label_hr, temp_time_str_hr);
		lv_label_set_text(label_min, temp_time_str_min);
	}
	else if(current_screen == SCREEN_ANALOG_CLOCK) {
		//TODO: Time setting for analog clock, something with angles for lines for the hands of the clock
	}
	else {
		// Do nothing in scr_menu, scr_digital_clock_set_time and scr_analog_clock_set_time
		return;
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

	/* Set display brightness to 100 */
	// brightness = 100;
	ret = pwm_set_dt(&LCD_kathode_pwm, PWM_PERIOD, PWM_PERIOD);
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

	/* Init the different screens */
	scr_digital_clock = lv_obj_create(NULL);
	scr_analog_clock = lv_obj_create(NULL);
	scr_digital_clock_set_time = lv_obj_create(NULL);
	scr_analog_clock_set_time = lv_obj_create(NULL);
	scr_menu = lv_obj_create(NULL);

	/* Create the main group to which indevs and widgets can be added and processed */
	group_digital_clock = lv_group_create();
	group_digital_clock_set_time = lv_group_create();

	/* Register the 2 buttons as keypad indev */
	indev = lv_indev_get_next(NULL);
	lv_indev_set_group(indev, group_digital_clock);

	/* GENERAL WIDGETS FOR DIGITAL CLOCK */
	/* Init the digital clock widgets in the parent container in order of render (left to right) */
	label_hr = lv_label_create(scr_digital_clock);
	label_colon = lv_label_create(scr_digital_clock);
	label_min = lv_label_create(scr_digital_clock);

	spinbox_hr = lv_spinbox_create(scr_digital_clock_set_time);
	spinbox_min = lv_spinbox_create(scr_digital_clock_set_time);
	
	/* Add widgets to groups */
	// TODO: These have to get added to a group when necessary and in the right order with the colon!
	lv_group_add_obj(group_digital_clock, label_hr);
	lv_group_add_obj(group_digital_clock, label_min);

	/* Define the object selector style */
	lv_style_init(&style_object_selector);
	lv_style_set_radius(&style_object_selector, 0); //Square?
	// lv_style_set_outline_color(&style_object_selector, lv_color_white());
	lv_style_set_outline_opa(&style_object_selector, LV_OPA_100);

	/* Define the style of parent_digital_clock */
	lv_style_init(&style_parent_digital_clock);
	lv_style_set_bg_color(&style_parent_digital_clock, lv_color_black());
	lv_style_set_layout(&style_parent_digital_clock, LV_LAYOUT_FLEX);
	lv_style_set_flex_flow(&style_parent_digital_clock, LV_FLEX_FLOW_ROW);

	/* Set container alignment */
	lv_obj_set_flex_align(scr_digital_clock, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_flex_align(scr_digital_clock_set_time, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

	/* Init style for digital time labels */
	lv_style_init(&style_label_time);
	lv_style_set_width(&style_spinbox_time, lv_pct(30));
    lv_style_set_height(&style_spinbox_time, lv_pct(100));
	lv_style_set_pad_all(&style_label_time, 0);
	lv_style_set_bg_opa(&style_label_time, LV_OPA_TRANSP);
	lv_style_set_text_align(&style_label_time, LV_TEXT_ALIGN_CENTER);
	lv_style_set_text_color(&style_label_time, lv_color_white());
	// lv_style_set_border_opa(&style_label_time, LV_OPA_TRANSP);
	lv_style_set_margin_all(&style_label_time, 0);
	// lv_style_set_outline_opa(&style_label_time, LV_OPA_TRANSP);

	/* Init spinbox for each section of time (hh:mm:ss) */
    lv_spinbox_set_range(spinbox_hr, 0, 23);
	lv_spinbox_set_range(spinbox_min, 0, 59);
    lv_spinbox_set_digit_count(spinbox_hr, 2);
	lv_spinbox_set_digit_count(spinbox_min, 2);
	lv_spinbox_set_step(spinbox_hr, 1);
	lv_spinbox_set_step(spinbox_min, 1);
	lv_spinbox_set_rollover(spinbox_hr, true);
	lv_spinbox_set_rollover(spinbox_min, true);

	/* Define the spinbox style */
	lv_style_init(&style_spinbox_time);
	lv_style_set_width(&style_spinbox_time, lv_pct(30));
    lv_style_set_height(&style_spinbox_time, lv_pct(100));
	lv_style_set_pad_all(&style_spinbox_time, 0);
	lv_style_set_bg_opa(&style_spinbox_time, LV_OPA_TRANSP);
	lv_style_set_text_align(&style_spinbox_time, LV_TEXT_ALIGN_CENTER);
	lv_style_set_text_color(&style_spinbox_time, lv_color_white());
	// lv_style_set_text_color(&style_spinbox_time, lv_palette_main(LV_PALETTE_DEEP_PURPLE));
	lv_style_set_border_opa(&style_spinbox_time, LV_OPA_TRANSP);
	lv_style_set_outline_color(&style_spinbox_time, lv_color_white());
	// lv_style_set_outline_opa(&style_spinbox_time, LV_OPA_TRANSP);
	lv_style_set_margin_all(&style_spinbox_time, 0);

	/* Define colon-seperator style */
	lv_style_init(&style_colon);
	lv_style_set_pad_all(&style_colon, 0);
	lv_style_set_bg_opa(&style_colon, LV_OPA_TRANSP);
	lv_style_set_text_align(&style_colon, LV_TEXT_ALIGN_CENTER);
	lv_style_set_text_color(&style_colon, lv_color_white());
	lv_style_set_border_opa(&style_colon, LV_OPA_TRANSP);
	lv_style_set_outline_color(&style_colon, lv_color_white());
	lv_style_set_margin_all(&style_colon, 0);

	/* Assign the time style to the spinboxes */
	lv_obj_add_style(spinbox_hr, &style_spinbox_time, 0);
	lv_obj_add_style(spinbox_min, &style_spinbox_time, 0);

	/* Assign the time style to the time labels */
	lv_obj_add_style(label_hr, &style_label_time, 0);
	lv_obj_add_style(label_min, &style_label_time, 0);

	/* Assign the time style the colon seperator */
	lv_obj_add_style(label_colon, &style_colon, 0);

	/* Assign the style_parent_digital_clock to the parent container */
	lv_obj_add_style(scr_digital_clock, &style_parent_digital_clock, 0);

	/* TODO: TEMP, scr_digital_clock_set_time probably needs its own style */
	lv_obj_add_style(scr_digital_clock_set_time, &style_parent_digital_clock, 0);

	/* Assign the object selector style to relevant selector(s) */
	lv_obj_add_style(spinbox_hr, &style_object_selector, LV_STATE_FOCUS_KEY);
	lv_obj_add_style(spinbox_min, &style_object_selector, LV_STATE_FOCUS_KEY);

	// /* Remove the cursor from the spinboxes */
	lv_obj_add_style(spinbox_hr, &style_spinbox_time, LV_PART_CURSOR);
	lv_obj_add_style(spinbox_min, &style_spinbox_time, LV_PART_CURSOR);

	/* Disable scrolling in the spinboxes */
	lv_obj_clear_flag(spinbox_hr, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_ONE);
	lv_obj_clear_flag(spinbox_min, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_ONE);

	/* Disable scrolling in the time labels */
	lv_obj_clear_flag(label_hr, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_ONE);
	lv_obj_clear_flag(label_min, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_ONE);

	/* Assign time spinboxes initial value */
	lv_spinbox_set_value(spinbox_hr, tm.tm_hour);
	lv_spinbox_set_value(spinbox_min, tm.tm_min);

	/* Assign the time labels initial value */
	char temp_time_str_hr[3];
	char temp_time_str_min[3];
	sprintf(temp_time_str_hr, "%02d", tm.tm_hour);
	sprintf(temp_time_str_min, "%02d", tm.tm_min);

	/* Assign the time labels initial value */
	lv_label_set_text(label_hr, temp_time_str_hr);
	lv_label_set_text(label_min, temp_time_str_min);

	/* Assign the seperator label initial value */
	lv_label_set_text(label_colon, ":");

	/* Add the LV_EVENT_FOCUSED event to the spinboxes */
	lv_obj_add_event(spinbox_hr, spinbox_interaction_cb, LV_EVENT_VALUE_CHANGED | LV_EVENT_FOCUSED | LV_EVENT_DEFOCUSED | LV_EVENT_CLICKED, NULL);
	lv_obj_add_event(spinbox_min, spinbox_interaction_cb, LV_EVENT_VALUE_CHANGED | LV_EVENT_FOCUSED | LV_EVENT_DEFOCUSED | LV_EVENT_CLICKED, NULL);

	/* Attach cb when spinboxes are (de)focused */
	lv_obj_add_event_cb(spinbox_hr, spinbox_interaction_cb, LV_EVENT_VALUE_CHANGED | LV_EVENT_FOCUSED | LV_EVENT_DEFOCUSED | LV_EVENT_CLICKED, NULL);
	lv_obj_add_event_cb(spinbox_min, spinbox_interaction_cb, LV_EVENT_VALUE_CHANGED | LV_EVENT_FOCUSED | LV_EVENT_DEFOCUSED | LV_EVENT_CLICKED, NULL);

	/* Add relevant states to labels that they don't have by default */
	lv_obj_add_state(label_hr, LV_STATE_FOCUS_KEY | LV_STATE_FOCUSED);
	lv_obj_add_state(label_min, LV_STATE_FOCUS_KEY | LV_STATE_FOCUSED);

	/* Add cb when labels are (de)focused */
	lv_obj_add_event_cb(label_hr, label_interaction_cb, LV_EVENT_ALL | LV_EVENT_VALUE_CHANGED | LV_EVENT_FOCUSED | LV_EVENT_DEFOCUSED | LV_EVENT_CLICKED | LV_EVENT_KEY, NULL);
	lv_obj_add_event_cb(label_min, label_interaction_cb, LV_EVENT_ALL | LV_EVENT_VALUE_CHANGED | LV_EVENT_FOCUSED | LV_EVENT_DEFOCUSED | LV_EVENT_CLICKED | LV_EVENT_KEY, NULL);
	
	/* Enable clicking on labels (TODO: Might be redundant) */
	lv_obj_add_flag(label_hr, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_CLICK_FOCUSABLE);
	lv_obj_add_flag(label_min, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_CLICK_FOCUSABLE);
	
	/* Add events to the time labels for UI reaction to label_interaction_cb */
	lv_obj_add_event(label_hr, label_interaction_cb, LV_EVENT_FOCUSED, NULL);
	lv_obj_add_event(label_min, label_interaction_cb, LV_EVENT_FOCUSED, NULL);

	/* Add LV_STATE_DISABLED to the spinboxes to prevent them from being edited by the user while not displayed*/
	lv_obj_add_state(spinbox_hr, LV_STATE_DISABLED);
	lv_obj_add_state(spinbox_min, LV_STATE_DISABLED);

	/* Load scr_digital_clock as default on start-up (for now, TODO, add user configureables and save them to NVM) */
	lv_screen_load(scr_digital_clock);

	lv_task_handler();

	/* Disable blanking the display to prevent having to redraw */
	ret = display_blanking_off(GC9A01);
	k_sleep(K_MSEC(120));
	if (ret < 0 && ret != -ENOSYS) {
		LOG_ERR("Failed to turn blanking off (error %d)", ret);
		return ret;
	}

	display_time();

	return 0;
}

/**
 * @brief Main function loop
 * 
 * @retval -1 if failed
 */
int main(void)
{
	int ret;
	/* devicetree setup*/
	ret = setup_dt();	
	if (ret < 0) {
		LOG_ERR("setup_dt failed. Err: %d\n", ret);
    //     return -1;
    }
	LOG_INF("Devicetree setup complete");

	ret = setup_lvgl();
	if (ret < 0) {
		LOG_ERR("setup_lvgl failed. Err: %d\n", ret);
    //     return -1;
    }
	LOG_INF("LVGL setup complete");

	/* MAIN LOOP */
	LOG_INF("Running");
	while (1) {
		/* Update the time on widgets which are relevant for the current screen */
		display_time();

		/* Switch screens based on user input*/
		switch(next_screen) {
			case SCREEN_DIGITAL_CLOCK:
				if(current_screen != SCREEN_DIGITAL_CLOCK) {
					LOG_DBG("Switching to SCREEN_DIGITAL_CLOCK");
					lv_screen_load(scr_digital_clock);
					current_screen = SCREEN_DIGITAL_CLOCK;
				}
				break;
			case SCREEN_ANALOG_CLOCK:
				if(current_screen != SCREEN_ANALOG_CLOCK) {
					LOG_DBG("Switching to SCREEN_ANALOG_CLOCK");
					lv_screen_load(scr_analog_clock);
					current_screen = SCREEN_ANALOG_CLOCK;
				}
				break;
			case SCREEN_DIGITAL_CLOCK_SET_TIME:
				if(current_screen != SCREEN_DIGITAL_CLOCK_SET_TIME) {
					LOG_DBG("Switching to SCREEN_DIGITAL_CLOCK_SET_TIME");
					lv_screen_load(scr_digital_clock_set_time);
					lv_indev_set_group(indev, group_digital_clock_set_time);
					lv_obj_set_parent(label_colon, scr_digital_clock_set_time);
					lv_obj_remove_state(spinbox_hr, LV_STATE_DISABLED);
					lv_obj_remove_state(spinbox_min, LV_STATE_DISABLED);
					lv_group_add_obj(group_digital_clock_set_time, spinbox_hr);
					lv_group_add_obj(group_digital_clock_set_time, spinbox_min);
					// lv_obj_add_state(spinbox_hr, LV_STATE_FOCUSED);
					current_screen = SCREEN_DIGITAL_CLOCK_SET_TIME;
				}
				break;
			case SCREEN_ANALOG_CLOCK_SET_TIME:
				if(current_screen != SCREEN_ANALOG_CLOCK_SET_TIME) {
					LOG_DBG("Switching to SCREEN_ANALOG_CLOCK_SET_TIME");
					lv_screen_load(scr_analog_clock_set_time);
					current_screen = SCREEN_ANALOG_CLOCK_SET_TIME;
				}
				break;
			case SCREEN_MENU:
				if(current_screen != SCREEN_MENU) {
					LOG_DBG("Switching to SCREEN_MENU");
					lv_screen_load(scr_menu);
					current_screen = SCREEN_MENU;
				}
				break;
			default:
				// Do nothing
				break;
		}

		lv_task_handler(); // Handle LVGL-related tasks
		LOG_DBG("Current screen: %d", current_screen);
		k_sleep(K_MSEC(FRAME_TIME_TARGET)); // Time for other threads
	}

	return -1;
}

/* TODO:
 * Add callbacks of some kind for brightness adjustment
 * Add callbacks for theme switching
 * Write and read preferred brightness and theme to NVM
 * 
 */