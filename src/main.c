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
LOG_MODULE_REGISTER(logging_mantelclock, LOG_LEVEL);

/* LVGL */
/* Screens */
lv_obj_t * scr_digital_clock;
lv_obj_t * scr_analog_clock;
lv_obj_t * scr_digital_clock_set_time;
lv_obj_t * scr_analog_clock_set_time;
lv_obj_t * scr_menu;


/* Style definitions */
static lv_style_t style_spinbox_time;
static lv_style_t style_object_selector;
static lv_style_t style_parent_digital_clock;
static lv_style_t style_colon;

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
lv_obj_t * spinbox_sec;

lv_obj_t * label_colon;

lv_obj_t * parent_digital_clock;
lv_obj_t * parent_digital_clock_set_time;
lv_obj_t * parent_analog_clock;
lv_obj_t * parent_analog_clock_set_time;

/* RTC Calender */
static struct rtc_time tm = {
	.tm_year = 2026 - 1900,
	.tm_mon = 01 - 1,
	.tm_mday = 04,
	.tm_hour = 23,
	.tm_min = 59,
	.tm_sec = 24,
};

struct rtc_time current_time;

/* Get devices from devicetree */
static const struct gpio_dt_spec dbg_led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct pwm_dt_spec  LCD_kathode_pwm = PWM_DT_SPEC_GET(DT_ALIAS(kathodepwm));
static const struct device *GC9A01 = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
static const struct device *const rtc = DEVICE_DT_GET(DT_ALIAS(rtc));
static const struct device *lvgl_keypad = DEVICE_DT_GET(DT_COMPAT_GET_ANY_STATUS_OKAY(zephyr_lvgl_keypad_input));

/* Prototypes */
static int set_date_time(const struct device *rtc, struct rtc_time *settable_time);
static int get_date_time(const struct device *rtc, struct rtc_time *target_time);
static void display_time(void);
static int setup_dt(void);
static int setup_lvgl(void);
static void user_interaction_cb(lv_event_t * event);
// void process_keypad(lv_indev_t * indev, lv_indev_data_t * data);


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
	get_date_time(rtc, &current_time);
	// LOG_INF("Current time: %d:%d:%d", current_time.tm_hour, current_time.tm_min, current_time.tm_sec);
	// sprintf(temp_time_str_hr, "%02d", current_time.tm_hour);
	// sprintf(temp_time_str_min, "%02d", current_time.tm_min);
	// sprintf(temp_time_str_sec, "%02d", current_time.tm_sec);
	lv_spinbox_set_value(spinbox_hr, current_time.tm_hour);
	lv_spinbox_set_value(spinbox_min, current_time.tm_min);
	// lv_spinbox_set_value(spinbox_sec, current_time.tm_sec);
	// sprintf(current_time_str, "%s%s%s%s%s", temp_time_str_hr, ":", temp_time_str_min, ":", temp_time_str_sec);
	// lv_label_set_text(current_time_label, current_time_str);
}

// static void defocused_cb(lv_event_t * event) {
// 	lv_style_set_outline_opa(&style_spinbox_time, LV_OPA_TRANSP);
// 	lv_obj_report_style_change(&style_spinbox_time)
// 	// lv_group_set_defocused
// }

/* FIXME TODO: -> de callback werkt niet meer :'| */
static void user_interaction_cb(lv_event_t * event) {
	lv_indev_t * temp_indev = (lv_indev_t *)lv_event_get_target(event); /*Same as lv_indev_active()*/
	uint32_t pressed_key = 0;
	lv_obj_t * temp_screen = lv_screen_active();

	LOG_DBG("User input detected");

	if(temp_screen == scr_digital_clock) {
		LOG_DBG("Switching active screen to scr_digital_clock_set_time");
		/* Define the object selector style */
		lv_style_init(&style_object_selector);
		lv_style_set_radius(&style_object_selector, 0); //Square?
		lv_style_set_outline_color(&style_object_selector, lv_color_white());
		lv_style_set_outline_opa(&style_object_selector, LV_OPA_100);

		/* Assign the object selector style to relevant selector(s) */
		lv_obj_add_style(spinbox_hr, &style_object_selector, LV_STATE_FOCUS_KEY);
		lv_obj_add_style(spinbox_min, &style_object_selector, LV_STATE_FOCUS_KEY);

		/* Add the spinboxes to the relevant group to allow them to be focussed and edited */
		lv_group_add_obj(group_digital_clock_set_time, spinbox_hr);
		lv_group_add_obj(group_digital_clock_set_time, spinbox_min);

		/* Set correct parents */
		lv_obj_set_parent(spinbox_hr, parent_digital_clock_set_time);
		lv_obj_set_parent(label_colon, parent_digital_clock_set_time);
		lv_obj_set_parent(spinbox_min, parent_digital_clock_set_time);

		/* Report style update */
		lv_obj_report_style_change(&style_spinbox_time);

		/* Bind indev to new group */
		lv_indev_set_group(indev, group_digital_clock_set_time);

		/* Load in the new screen */
		lv_screen_load(scr_digital_clock_set_time);
	}
	else if(temp_screen == scr_analog_clock) {
		LOG_DBG("Switching active screen to scr_analog_clock_set_time");
		// TODO: Hier widgets overzetten
		lv_screen_load(scr_analog_clock_set_time);
	}
	else if((temp_screen == scr_digital_clock_set_time) || (temp_screen == scr_analog_clock_set_time) || (temp_screen == scr_menu)) {
		// Do nothing (for now)
		LOG_DBG("At custom screen, but not handling screen switching here");
		return;
	}
	else {
		// Catch-all just to be sure 
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
	if (!device_is_ready(lvgl_keypad)) { //TODO: Dit moet wss pas NA het initten van zephyr
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
	
	/* Create a container for the digital clock */
	parent_digital_clock = lv_obj_create(scr_digital_clock);
	parent_digital_clock_set_time = lv_obj_create(scr_digital_clock_set_time);
	parent_analog_clock = lv_obj_create(scr_analog_clock);
	parent_analog_clock_set_time = lv_obj_create(scr_analog_clock_set_time);

	/* Set initial display BG color */
	lv_obj_set_style_bg_color(scr_digital_clock, lv_color_black(), 0);

	/* Register the 2 buttons as keypad indev */
	indev = lv_indev_get_next(NULL);
	lv_indev_set_group(indev, group_digital_clock);
	lv_indev_add_event_cb(indev, user_interaction_cb, LV_EVENT_KEY, NULL);

	/* GENERAL WIDGETS FOR DIGITAL CLOCK */
	/* Init the digital clock widgets in the parent container in order of render (left to right) */
	spinbox_hr = lv_spinbox_create(parent_digital_clock);
	label_colon = lv_label_create(parent_digital_clock);
	spinbox_min = lv_spinbox_create(parent_digital_clock);

	/* Add widgets to groups */
	lv_group_add_obj(group_digital_clock, spinbox_hr);
	lv_group_add_obj(group_digital_clock, spinbox_min);

	/* Define the style of parent_digital_clock */
	lv_style_init(&style_parent_digital_clock);
	lv_style_set_width(&style_parent_digital_clock, lv_pct(100));
    lv_style_set_height(&style_parent_digital_clock, lv_pct(50));
	lv_style_set_pad_all(&style_parent_digital_clock, 0);
	lv_style_set_bg_opa(&style_parent_digital_clock, LV_OPA_TRANSP);
	lv_style_set_border_opa(&style_parent_digital_clock, LV_OPA_TRANSP);
	lv_style_set_outline_opa(&style_parent_digital_clock, LV_OPA_TRANSP);
	lv_style_set_align(&style_parent_digital_clock, LV_ALIGN_CENTER);
	lv_style_set_margin_all(&style_parent_digital_clock, 0);
	lv_style_set_layout(&style_parent_digital_clock, LV_LAYOUT_FLEX);
	lv_style_set_flex_flow(&style_parent_digital_clock, LV_FLEX_FLOW_ROW);

	/* Set container alignment */
	lv_obj_set_flex_align(parent_digital_clock, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_obj_set_flex_align(parent_digital_clock_set_time, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

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
	lv_style_set_border_opa(&style_spinbox_time, LV_OPA_TRANSP);
	lv_style_set_outline_color(&style_spinbox_time, lv_color_white());
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

	/* Assign the time style the colon seperator */
	lv_obj_add_style(label_colon, &style_colon, 0);

	/* Assign the style_parent_digital_clock to the parent container */
	lv_obj_add_style(parent_digital_clock, &style_parent_digital_clock, 0);

	/* Remove the cursor from the spinboxes */
	lv_obj_add_style(spinbox_hr, &style_spinbox_time, LV_PART_CURSOR);
	lv_obj_add_style(spinbox_min, &style_spinbox_time, LV_PART_CURSOR);

	/* Disable scrolling in the spinboxes */
	lv_obj_clear_flag(spinbox_hr, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_ONE);
	lv_obj_clear_flag(spinbox_min, LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_ONE);

	/* Assign time spinboxes initial value */
	lv_spinbox_set_value(spinbox_hr, tm.tm_hour);
	lv_spinbox_set_value(spinbox_min, tm.tm_min);

	/* Assign the seperator label initial value */
	lv_label_set_text(label_colon, ":");

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
	while (1) {
		display_time(); // Update the spinboxes
		lv_task_handler(); // Handle LVGL-related tasks
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