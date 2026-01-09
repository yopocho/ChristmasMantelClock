#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: style_buttons_digital_clock_set_time
lv_style_t *get_style_style_buttons_digital_clock_set_time_MAIN_DEFAULT();
lv_style_t *get_style_style_buttons_digital_clock_set_time_MAIN_FOCUS_KEY();
lv_style_t *get_style_style_buttons_digital_clock_set_time_MAIN_FOCUSED();
void add_style_style_buttons_digital_clock_set_time(lv_obj_t *obj);
void remove_style_style_buttons_digital_clock_set_time(lv_obj_t *obj);

// Style: style_spinboxes_digital_time
lv_style_t *get_style_style_spinboxes_digital_time_MAIN_DEFAULT();
lv_style_t *get_style_style_spinboxes_digital_time_MAIN_FOCUS_KEY();
lv_style_t *get_style_style_spinboxes_digital_time_MAIN_FOCUSED();
lv_style_t *get_style_style_spinboxes_digital_time_CURSOR_DEFAULT();
void add_style_style_spinboxes_digital_time(lv_obj_t *obj);
void remove_style_style_spinboxes_digital_time(lv_obj_t *obj);

// Style: style_labels_digital_clock
lv_style_t *get_style_style_labels_digital_clock_MAIN_DEFAULT();
void add_style_style_labels_digital_clock(lv_obj_t *obj);
void remove_style_style_labels_digital_clock(lv_obj_t *obj);



#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/