#include <lvgl.h>

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
	SCREEN_PREVIOUS,
	SCREEN_ACTIVE_CLOCK
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

/* User Settings struct typedef*/
typedef struct __packed user_settings_t {
	uint8_t brightness;
	clock_types clock_type;
	colours_t background_colour;
	colours_t text_colour;
	uint8_t time_hr;
	uint8_t time_min;
} user_settings_t;

extern user_settings_t user_settings;

lv_color_t get_color_from_index(colours_t colour);