/* Zephyr */
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <zephyr/device.h>
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
#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#define WIDTH 240 // Display width
#define HEIGHT 240 // Display height
#define FRAME_TIME_TARGET 50 // ms. 20 FPS for a clock is plenty
#define PWM_PERIOD PWM_USEC(10) // us

/* PWM */
uint8_t brightness;

/* LVGL */
static lv_obj_t * list_clock_theme;
static lv_style_t style_list_clock_theme;
static lv_style_t style_background;
static lv_style_t style_numbers;
static lv_style_t style_text_border;
static lv_style_t style_display_border;
static char current_time_str[15] = {0}; //TODO: Check tijdens testen of deze array gwn naar 8-9 lang kan ipv 15
static lv_obj_t *hello_world_label;
static lv_obj_t *current_time_label;

/* RTC Calender */
static struct rtc_time tm = {
	.tm_year = 2025 - 1900,
	.tm_mon = 12 - 1,
	.tm_mday = 21,
	.tm_hour = 20,
	.tm_min = 17,
	.tm_sec = 0,
};

static struct rtc_time current_time;

/* Get devices from devicetree */
static const struct gpio_dt_spec dbg_led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec LCD_PSU_EN = GPIO_DT_SPEC_GET(DT_ALIAS(lcd_psu_en), gpios);
static const struct pwm_dt_spec  LCD_kathode_pwm = PWM_DT_SPEC_GET(DT_ALIAS(kathodepwm)); //TODO: Fix PWM device AAAAA
static const struct device *GC9A01 = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
// static const struct device *const rtc = DEVICE_DT_GET(DT_ALIAS(rtc));
static const struct device *lvgl_keypad = DEVICE_DT_GET(DT_COMPAT_GET_ANY_STATUS_OKAY(zephyr_lvgl_keypad_input));

/* Prototypes */
static int set_date_time(const struct device *rtc, struct rtc_time *settable_time);
static int get_date_time(const struct device *rtc, struct rtc_time *target_time);
static int setup_dt(void);
static int setup_lvgl(void);


/**
 * @brief Set the current time of the RTC device pointed at by *rtc using the tm struct
 * 
 * @retval Zephyr retval
 */
// static int set_date_time(const struct device *rtc, struct rtc_time *settable_time)
// {
// 	int ret = 0;

// 	ret = rtc_set_time(rtc, settable_time);
// 	if (ret < 0) {
// 		// printk("Cannot write date time: %d\n", ret);
// 		return ret;
// 	}
// 	return ret;
// }

/**
 * @brief Read the current time from the RTC device pointed at by *rtc
 * 
 * @retval Zephyr retval
//  */
// static int get_date_time(const struct device *rtc, struct rtc_time *target_time)
// {
// 	int ret = 0;

// 	ret = rtc_get_time(rtc, target_time);
// 	if (ret < 0) {
// 		// printk("Cannot read date time: %d\n", ret);
// 		return ret;
// 	}

// 	// printk("RTC date and time: %04d-%02d-%02d %02d:%02d:%02d\n", target_time->tm_year + 1900,
// 	//        target_time->tm_mon + 1, target_time->tm_mday, target_time->tm_hour, target_time->tm_min, target_time->tm_sec);

// 	return ret;
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
		printk("GPIO device is not ready\n");
	}

	/* Enable GPIOs and set outputs active */
	ret = gpio_pin_configure_dt(&dbg_led, GPIO_OUTPUT_ACTIVE); // Turn on LED
	ret = gpio_pin_configure_dt(&LCD_PSU_EN, GPIO_OUTPUT_ACTIVE); // Turn on display
    // if (ret < 0) {
	// 	printk("GPIO configuring failed\n");
    //     return ret;
    // }

	// /* Check if display is ready */
	if (!device_is_ready(GC9A01)) {
	// 	printk("Display device is not ready\n");
	// 	return ret;
	}

	// /* Check if display kathode PWM is ready */
	if (!pwm_is_ready_dt(&LCD_kathode_pwm)) {
	// 	printk("Kathode PWM device is not ready\n");
	// 	return ret;
	}

    // /* Check if the RTC is ready */
	// if (!device_is_ready(rtc)) {
	// 	printk("RTC device is not ready\n");
	// 	return ret;
	// }

	// /* Check if the "keypad" (2 buttons) is ready */
	if (!device_is_ready(lvgl_keypad)) {
	// 	printk("Keypad device is not ready\n");
	// 	return ret;
	}

	// /* Set the RTC calender*/
	// set_date_time(rtc, &tm);

	// /* Set display brightness to 100 */
	// brightness = 100;
	pwm_set_dt(&LCD_kathode_pwm, PWM_PERIOD, PWM_PERIOD / 2u);

	return 0;
}

/**
 * @brief Setup function for LVGL widgets and devices
 * 
 * @retval Zephyr retval
 */
static int setup_lvgl(void) {
	int ret;

	/* LVGL HELLO WORLD SNIPPET */
	hello_world_label = lv_label_create(lv_screen_active());
	lv_label_set_text(hello_world_label, "Hello world!");
	lv_obj_align(hello_world_label, LV_ALIGN_CENTER, 0, 0);

	// current_time_label = lv_label_create(lv_screen_active());
	// lv_obj_align(current_time_label, LV_ALIGN_BOTTOM_MID, 0, 0);

	lv_task_handler();
	ret = display_blanking_off(GC9A01);
	// if (ret < 0 && ret != -ENOSYS) {
	// 	// LOG_ERR("Failed to turn blanking off (error %d)", ret);
	// 	return ret;
	// }
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
	// if (ret < 0) {
	// 	printk("setup_dt failed. Err: %d\n", ret);
    //     return -1;
    // }

	ret = setup_lvgl();
	// if (ret < 0) {
	// 	printk("setup_lvgl failed. Err: %d\n", ret);
    //     return -1;
    // }

	/* MAIN LOOP */
	while (1) {
		// get_date_time(rtc, &current_time);
		// sprintf(current_time_str, "%02d", current_time.tm_hour);
		// current_time_str[3] = ':';
		// sprintf(current_time_str + strlen(current_time_str), "%02d", current_time.tm_min);
		// current_time_str[6] = ':';
		// sprintf(current_time_str + strlen(current_time_str), "%02d", current_time.tm_sec);
		// lv_label_set_text(current_time_label, current_time_str);
		// sprintf(current_time_str, "%d", ret);
		// lv_label_set_text(current_time_label, current_time_str);
		lv_label_set_text(hello_world_label, "Hello world!");
		// lv_obj_align(hello_world_label, LV_ALIGN_CENTER, 0, 0);
		lv_task_handler();
		gpio_pin_toggle_dt(&dbg_led);
		// k_sleep(K_MSEC(FRAME_TIME_TARGET));
		k_sleep(K_MSEC(50));
	}

	return -1;
}

/* TODO:
 * Add callbacks of some kind for brightness adjustment
 * Add callbacks for theme switching
 * Write and read preferred brightness and theme to NVM
 * 
 */