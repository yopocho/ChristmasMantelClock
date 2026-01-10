#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _groups_t {
    lv_group_t *group_menu;
    lv_group_t *group_digital_clock;
    lv_group_t *group_digital_clock_set_time;
} groups_t;

extern groups_t groups;

void ui_create_groups();

typedef struct _objects_t {
    lv_obj_t *scr_digital_clock;
    lv_obj_t *scr_analog_clock;
    lv_obj_t *scr_menu;
    lv_obj_t *cont_digital_clock_set_time;
    lv_obj_t *spinbox_hr_digital_clock_set_time;
    lv_obj_t *label_colon_digital_clock_1;
    lv_obj_t *spinbox_min_digital_clock_set_time;
    lv_obj_t *cont_buttons_digital_clock_set_time;
    lv_obj_t *button_digital_clock_set_time_cancel;
    lv_obj_t *button_digital_clock_set_time_menu;
    lv_obj_t *button_digital_clock_set_time_ok;
    lv_obj_t *cont_digital_clock;
    lv_obj_t *label_time_hr_digital_clock;
    lv_obj_t *label_colon_digital_clock;
    lv_obj_t *label_time_min_digital_clock;
    lv_obj_t *scale_analog_clock;
    lv_obj_t *cont_scr_menu;
    lv_obj_t *cont_menu_brightness;
    lv_obj_t *label_menu_brightness;
    lv_obj_t *spinbox_menu_brightness;
    lv_obj_t *cont_menu_clock_type;
    lv_obj_t *label_menu_clock_type;
    lv_obj_t *dropdown_menu_clock_type;
    lv_obj_t *cont_menu_background_colour;
    lv_obj_t *label_menu_background_colour;
    lv_obj_t *dropdown_menu_background_colour;
    lv_obj_t *cont_menu_text_colour;
    lv_obj_t *label_menu_text_colour;
    lv_obj_t *dropdown_menu_text_colour;
    lv_obj_t *cont_menu_buttons;
    lv_obj_t *button_menu_cancel;
    lv_obj_t *button_menu_save;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_SCR_DIGITAL_CLOCK = 1,
    SCREEN_ID_SCR_ANALOG_CLOCK = 2,
    SCREEN_ID_SCR_MENU = 3,
};

void create_screen_scr_digital_clock();
void tick_screen_scr_digital_clock();

void create_screen_scr_analog_clock();
void tick_screen_scr_analog_clock();

void create_screen_scr_menu();
void tick_screen_scr_menu();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/