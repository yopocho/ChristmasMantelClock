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
/* UI */
#include "ui/ui.h"
/* C */
#include <string.h>
#include <stdio.h>

/* Defines */
#define LOG_LEVEL LOG_LEVEL_DBG
#define WIDTH 240 // Display width
#define HEIGHT 240 // Display height
#define FRAME_TIME_TARGET 50 // ms. 20 FPS for a clock is plenty
#define PWM_PERIOD PWM_USEC(10) // us

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
	.tm_mday = 06,
	.tm_hour = 18,
	.tm_min = 03,
	.tm_sec = 24,
};

/* Enum for current screen indidication */
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
	if(current_screen != next_screen) {
		previous_screen = current_screen;
    	next_screen = (screens) lv_event_get_user_data(e);
	}
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

	/* LVGL and EEZ UI setup */
	ret = setup_lvgl();
	if (ret < 0) {
		LOG_ERR("setup_lvgl failed. Err: %d\n", ret);
    //     return -1;
    }
	LOG_INF("LVGL setup complete");

	/* MAIN LOOP */
	LOG_INF("Running");
	while (1) {
		/* Switch screens manually, updated through EEZ UI action callback */
		if(next_screen != current_screen) {
			switch(next_screen) {
				case SCREEN_DIGITAL_CLOCK:
					LOG_DBG("Switching to SCREEN_DIGITAL_CLOCK");
					current_screen = SCREEN_DIGITAL_CLOCK;
					lv_indev_set_group(indev, groups.group_digital_clock);
					loadScreen(SCREEN_ID_SCR_DIGITAL_CLOCK);
					break;
				case SCREEN_ANALOG_CLOCK:
					LOG_DBG("Switching to SCREEN_ANALOG_CLOCK");
					current_screen = SCREEN_ANALOG_CLOCK;
					loadScreen(SCREEN_ID_SCR_ANALOG_CLOCK);
					break;
				case SCREEN_DIGITAL_CLOCK_SET_TIME:
					LOG_DBG("Switching to SCREEN_DIGITAL_CLOCK_SET_TIME");
					current_screen = SCREEN_DIGITAL_CLOCK_SET_TIME;
					lv_indev_set_group(indev, groups.group_digital_clock_set_time);
					loadScreen(SCREEN_ID_SCR_DIGITAL_CLOCK_SET_TIME);
					break;
				case SCREEN_ANALOG_CLOCK_SET_TIME:
					LOG_DBG("Switching to SCREEN_ANALOG_CLOCK_SET_TIME");
					current_screen = SCREEN_ANALOG_CLOCK_SET_TIME;
					loadScreen(SCREEN_ID_SCR_ANALOG_CLOCK_SET_TIME);
					break;
				case SCREEN_MENU:
					LOG_DBG("Switching to SCREEN_MENU");
					current_screen = SCREEN_MENU;
					loadScreen(SCREEN_ID_SCR_MENU);
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
		// LOG_DBG("Current screen: %d", current_screen);
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