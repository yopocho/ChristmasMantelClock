#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_change_screen(lv_event_t * e);
extern void action_digital_clock_set_time_save(lv_event_t * e);
extern void action_menu_save(lv_event_t * e);
extern void action_menu_clock_type_value_changed(lv_event_t * e);
extern void action_menu_background_colour_value_changed(lv_event_t * e);
extern void action_menu_text_colour_value_changed(lv_event_t * e);
extern void action_menu_brightness_value_changed(lv_event_t * e);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/