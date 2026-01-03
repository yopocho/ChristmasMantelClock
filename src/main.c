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
/* Style definitions */
static lv_style_t style_spinbox_time;
static lv_style_t style_object_selector;

/* Widget groups */
static lv_group_t * g;

/* Objects and such */
char current_time_str[9];
char temp_time_str_hr[3];
char temp_time_str_min[3];
char temp_time_str_sec[3];
lv_obj_t *hello_world_label;
lv_obj_t *current_time_label;
lv_indev_t * indev;

lv_obj_t * spinbox_hr;
lv_obj_t * spinbox_min;
lv_obj_t * spinbox_sec;

/* RTC Calender */
static struct rtc_time tm = {
	.tm_year = 2026 - 1900,
	.tm_mon = 01 - 1,
	.tm_mday = 03,
	.tm_hour = 18,
	.tm_min = 0,
	.tm_sec = 0,
};

struct rtc_time current_time;

/* Get devices from devicetree */
static const struct gpio_dt_spec dbg_led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct pwm_dt_spec  LCD_kathode_pwm = PWM_DT_SPEC_GET(DT_ALIAS(kathodepwm)); //TODO: Fix PWM device AAAAA
static const struct device *GC9A01 = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
static const struct device *const rtc = DEVICE_DT_GET(DT_ALIAS(rtc));
static const struct device *lvgl_keypad = DEVICE_DT_GET(DT_COMPAT_GET_ANY_STATUS_OKAY(zephyr_lvgl_keypad_input));

/* Prototypes */
static int set_date_time(const struct device *rtc, struct rtc_time *settable_time);
static int get_date_time(const struct device *rtc, struct rtc_time *target_time);
static void set_time_label(void);
static int setup_dt(void);
static int setup_lvgl(void);
void process_keypad(lv_indev_t * indev, lv_indev_data_t * data);


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

	LOG_INF("RTC date and time: %04d-%02d-%02d %02d:%02d:%02d\n", target_time->tm_year + 1900,
	       target_time->tm_mon + 1, target_time->tm_mday, target_time->tm_hour, target_time->tm_min, target_time->tm_sec);

	return ret;
}

static void set_time_label(void) {
	// get_date_time(rtc, &current_time);
	// LOG_INF("Current time: %d:%d:%d", current_time.tm_hour, current_time.tm_min, current_time.tm_sec);
	// sprintf(temp_time_str_hr, "%02d", current_time.tm_hour);
	// sprintf(temp_time_str_min, "%02d", current_time.tm_min);
	// sprintf(temp_time_str_sec, "%02d", current_time.tm_sec);
	// sprintf(current_time_str, "%s%s%s%s%s", temp_time_str_hr, ":", temp_time_str_min, ":", temp_time_str_sec);
	// lv_label_set_text(current_time_label, current_time_str);
}

// void process_keypad(lv_indev_t * indev, lv_indev_data_t * data) {
//   if(key_pressed()) {
//      data->key = my_last_key();            /* Get the last pressed or released key */
//      data->state = LV_INDEV_STATE_PRESSED;
//   } else {
//      data->state = LV_INDEV_STATE_RELEASED;
//   }
// }

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

	/* Create the main group to which indevs and widgets can be added and processed */
	g = lv_group_create();
	
	/* Set initial display BG color */
	lv_obj_set_style_bg_color(lv_screen_active(), lv_color_black(), 0);

	/* Register the 2 buttons as keypad indev */
	indev = lv_indev_get_next(NULL);
	lv_indev_set_group(indev, g);

	/* Init spinbox for each section of time (hh:mm:ss) */
	spinbox_hr = lv_spinbox_create(lv_screen_active());
	spinbox_min = lv_spinbox_create(lv_screen_active());
	spinbox_sec = lv_spinbox_create(lv_screen_active());
	lv_group_add_obj(g, spinbox_hr);
	lv_group_add_obj(g, spinbox_min);
	lv_group_add_obj(g, spinbox_sec);
    lv_spinbox_set_range(spinbox_hr, 0, 23);
	lv_spinbox_set_range(spinbox_min, 0, 59);
	lv_spinbox_set_range(spinbox_sec, 0, 59);
    lv_spinbox_set_digit_count(spinbox_hr, 2);
	lv_spinbox_set_digit_count(spinbox_min, 2);
	lv_spinbox_set_digit_count(spinbox_sec, 2);
	lv_spinbox_set_step(spinbox_hr, 1);
	lv_spinbox_set_step(spinbox_min, 1);
	lv_spinbox_set_step(spinbox_sec, 1);
    // lv_obj_set_width(spinbox_hr, 75);
	// lv_obj_set_width(spinbox_min, 75);
	// lv_obj_set_width(spinbox_sec, 75);
	lv_spinbox_set_rollover(spinbox_hr, true);
	lv_spinbox_set_rollover(spinbox_min, true);
	lv_spinbox_set_rollover(spinbox_sec, true);

	/* Define the spinbox style */
	lv_style_init(&style_spinbox_time);
	lv_style_set_width(&style_spinbox_time, 75); // Maybe lv_pct(25) or smthn, with reduced margin perhaps ????
    lv_style_set_height(&style_spinbox_time, lv_pct(20));
	lv_style_set_pad_all(&style_spinbox_time, 0);
	lv_style_set_bg_opa(&style_spinbox_time, LV_OPA_TRANSP);
	lv_style_set_text_align(&style_spinbox_time, LV_TEXT_ALIGN_CENTER);
	lv_style_set_text_color(&style_spinbox_time, lv_color_white());
	lv_style_set_border_opa(&style_spinbox_time, LV_OPA_TRANSP);
	lv_style_set_outline_color(&style_spinbox_time, lv_color_white());
	lv_obj_set_align(spinbox_hr, LV_ALIGN_LEFT_MID);
	lv_obj_set_align(spinbox_min, LV_ALIGN_CENTER);
	lv_obj_set_align(spinbox_sec, LV_ALIGN_RIGHT_MID);
	// lv_style_set_margin_all(&style_spinbox_time, 5);

	/* Assign the time style to the spinboxes */
	lv_obj_add_style(spinbox_hr, &style_spinbox_time, 0);
	lv_obj_add_style(spinbox_min, &style_spinbox_time, 0);
	lv_obj_add_style(spinbox_sec, &style_spinbox_time, 0);

	/* Remove the cursor from the spinboxes */
	lv_obj_add_style(spinbox_hr, &style_spinbox_time, LV_PART_CURSOR);
	lv_obj_add_style(spinbox_min, &style_spinbox_time, LV_PART_CURSOR);
	lv_obj_add_style(spinbox_sec, &style_spinbox_time, LV_PART_CURSOR);

	/* Define the object selector style */
	lv_style_init(&style_object_selector);
	lv_style_set_radius(&style_object_selector, 0); //Square?
	lv_style_set_outline_color(&style_object_selector, lv_color_white());
	lv_style_set_outline_opa(&style_object_selector, LV_OPA_100);

	/* Assign the object selector style to relevant selector(s) */
	lv_obj_add_style(spinbox_hr, &style_object_selector, LV_STATE_FOCUS_KEY);
	lv_obj_add_style(spinbox_min, &style_object_selector, LV_STATE_FOCUS_KEY);
	lv_obj_add_style(spinbox_sec, &style_object_selector, LV_STATE_FOCUS_KEY);

	/* Assign time spinboxes initial value */
	lv_spinbox_set_value(spinbox_hr, tm.tm_hour);
	lv_spinbox_set_value(spinbox_hr, tm.tm_min);
	lv_spinbox_set_value(spinbox_hr, tm.tm_sec);

	// set_time_label(); Deprecated rn

	lv_task_handler();

	/* Disable blanking the display to prevent having to redraw */
	ret = display_blanking_off(GC9A01);
	k_sleep(K_MSEC(120));
	if (ret < 0 && ret != -ENOSYS) {
		LOG_ERR("Failed to turn blanking off (error %d)", ret);
		return ret;
	}
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
		// set_time_label(); // Update the time label
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