#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

groups_t groups;
static bool groups_created = false;

objects_t objects;
lv_obj_t *tick_value_change_obj;
uint32_t active_theme_index = 0;

static void event_handler_cb_scr_digital_clock_scr_digital_clock(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_SCREEN_LOAD_START) {
        // group: group_menu
        lv_group_remove_all_objs(groups.group_menu);
        // group: group_digital_clock
        lv_group_remove_all_objs(groups.group_digital_clock);
        lv_group_add_obj(groups.group_digital_clock, objects.label_time_hr_digital_clock);
        lv_group_add_obj(groups.group_digital_clock, objects.label_time_min_digital_clock);
        // group: group_digital_clock_set_time
        lv_group_remove_all_objs(groups.group_digital_clock_set_time);
        lv_group_add_obj(groups.group_digital_clock_set_time, objects.spinbox_hr_digital_clock_set_time);
        lv_group_add_obj(groups.group_digital_clock_set_time, objects.spinbox_min_digital_clock_set_time);
        lv_group_add_obj(groups.group_digital_clock_set_time, objects.button_digital_clock_set_time_cancel);
        lv_group_add_obj(groups.group_digital_clock_set_time, objects.button_digital_clock_set_time_menu);
        lv_group_add_obj(groups.group_digital_clock_set_time, objects.button_digital_clock_set_time_ok);
        // group: group_analog_clock
        lv_group_remove_all_objs(groups.group_analog_clock);
        // group: grou_analog_clock_set_time
        lv_group_remove_all_objs(groups.grou_analog_clock_set_time);
    }
}

static void event_handler_cb_scr_analog_clock_scr_analog_clock(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_SCREEN_LOAD_START) {
        // group: group_menu
        lv_group_remove_all_objs(groups.group_menu);
        // group: group_digital_clock
        lv_group_remove_all_objs(groups.group_digital_clock);
        // group: group_digital_clock_set_time
        lv_group_remove_all_objs(groups.group_digital_clock_set_time);
        // group: group_analog_clock
        lv_group_remove_all_objs(groups.group_analog_clock);
        // group: grou_analog_clock_set_time
        lv_group_remove_all_objs(groups.grou_analog_clock_set_time);
    }
}

static void event_handler_cb_scr_menu_scr_menu(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_SCREEN_LOAD_START) {
        // group: group_menu
        lv_group_remove_all_objs(groups.group_menu);
        lv_group_add_obj(groups.group_menu, objects.spinbox_menu_brightness);
        lv_group_add_obj(groups.group_menu, objects.roller_menu_clock_type);
        lv_group_add_obj(groups.group_menu, objects.roller_menu_background_colour);
        lv_group_add_obj(groups.group_menu, objects.roller_menu_text_colour);
        lv_group_add_obj(groups.group_menu, objects.button_menu_cancel);
        lv_group_add_obj(groups.group_menu, objects.button_menu_save);
        // group: group_digital_clock
        lv_group_remove_all_objs(groups.group_digital_clock);
        // group: group_digital_clock_set_time
        lv_group_remove_all_objs(groups.group_digital_clock_set_time);
        // group: group_analog_clock
        lv_group_remove_all_objs(groups.group_analog_clock);
        // group: grou_analog_clock_set_time
        lv_group_remove_all_objs(groups.grou_analog_clock_set_time);
    }
}

void create_screen_scr_digital_clock() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.scr_digital_clock = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 240);
    lv_obj_add_event_cb(obj, event_handler_cb_scr_digital_clock_scr_digital_clock, LV_EVENT_ALL, 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // cont_digital_clock_set_time
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cont_digital_clock_set_time = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 240);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_FOCUSED);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_translate_y(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // spinbox_hr_digital_clock_set_time
                    lv_obj_t *obj = lv_spinbox_create(parent_obj);
                    objects.spinbox_hr_digital_clock_set_time = obj;
                    lv_obj_set_pos(obj, -10445, -6613);
                    lv_obj_set_size(obj, LV_PCT(38), LV_PCT(24));
                    lv_spinbox_set_digit_format(obj, 2, 0);
                    lv_spinbox_set_range(obj, 0, 23);
                    lv_spinbox_set_rollover(obj, true);
                    lv_spinbox_set_step(obj, 1);
                    lv_spinbox_set_value(obj, 0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_ON_FOCUS|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_scroll_dir(obj, LV_DIR_NONE);
                    lv_obj_set_scroll_snap_x(obj, LV_SCROLL_SNAP_NONE);
                    lv_obj_set_scroll_snap_y(obj, LV_SCROLL_SNAP_NONE);
                    add_style_style_spinboxes_digital_clock(obj);
                }
                {
                    // label_colon_digital_clock_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_colon_digital_clock_1 = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_PCT(7), LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    add_style_style_labels_digital_clock(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_translate_y(obj, -1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, ":");
                }
                {
                    // spinbox_min_digital_clock_set_time
                    lv_obj_t *obj = lv_spinbox_create(parent_obj);
                    objects.spinbox_min_digital_clock_set_time = obj;
                    lv_obj_set_pos(obj, -4096, -4722);
                    lv_obj_set_size(obj, LV_PCT(38), LV_PCT(24));
                    lv_spinbox_set_digit_format(obj, 2, 0);
                    lv_spinbox_set_range(obj, 0, 59);
                    lv_spinbox_set_rollover(obj, true);
                    lv_spinbox_set_step(obj, 1);
                    lv_spinbox_set_value(obj, 0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_ON_FOCUS|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_scroll_dir(obj, LV_DIR_NONE);
                    lv_obj_set_scroll_snap_x(obj, LV_SCROLL_SNAP_NONE);
                    lv_obj_set_scroll_snap_y(obj, LV_SCROLL_SNAP_NONE);
                    add_style_style_spinboxes_digital_clock(obj);
                }
            }
        }
        {
            // cont_buttons_digital_clock_set_time
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cont_buttons_digital_clock_set_time = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 240);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_END, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // button_digital_clock_set_time_cancel
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.button_digital_clock_set_time_cancel = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_PCT(8));
                    lv_obj_add_event_cb(obj, action_change_screen, LV_EVENT_KEY, (void *)0);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_ON_FOCUS|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_scroll_dir(obj, LV_DIR_NONE);
                    lv_obj_set_scroll_snap_x(obj, LV_SCROLL_SNAP_NONE);
                    lv_obj_set_scroll_snap_y(obj, LV_SCROLL_SNAP_NONE);
                    add_style_style_buttons_digital_clock_set_time(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_change_screen, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
                            lv_label_set_text(obj, "CANCEL");
                        }
                    }
                }
                {
                    // button_digital_clock_set_time_menu
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.button_digital_clock_set_time_menu = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_PCT(8));
                    lv_obj_add_event_cb(obj, action_change_screen, LV_EVENT_KEY, (void *)4);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_ON_FOCUS|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_scroll_dir(obj, LV_DIR_NONE);
                    lv_obj_set_scroll_snap_x(obj, LV_SCROLL_SNAP_NONE);
                    lv_obj_set_scroll_snap_y(obj, LV_SCROLL_SNAP_NONE);
                    add_style_style_buttons_digital_clock_set_time(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_change_screen, LV_EVENT_PRESSED, (void *)4);
                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
                            lv_label_set_text(obj, "MENU");
                        }
                    }
                }
                {
                    // button_digital_clock_set_time_ok
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.button_digital_clock_set_time_ok = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_PCT(8));
                    lv_obj_add_event_cb(obj, action_digital_clock_set_time_save, LV_EVENT_KEY, (void *)0);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_ON_FOCUS|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_scroll_dir(obj, LV_DIR_NONE);
                    lv_obj_set_scroll_snap_x(obj, LV_SCROLL_SNAP_NONE);
                    lv_obj_set_scroll_snap_y(obj, LV_SCROLL_SNAP_NONE);
                    add_style_style_buttons_digital_clock_set_time(obj);
                    lv_obj_set_style_outline_opa(obj, 0, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_FOCUS_KEY);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_add_event_cb(obj, action_digital_clock_set_time_save, LV_EVENT_PRESSED, (void *)0);
                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
                            lv_label_set_text(obj, "SAVE");
                        }
                    }
                }
            }
        }
        {
            // cont_digital_clock
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cont_digital_clock = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 240);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // label_time_hr_digital_clock
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_time_hr_digital_clock = obj;
                    lv_obj_set_pos(obj, 4, 98);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_PCT(20));
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_add_event_cb(obj, action_change_screen, LV_EVENT_KEY, (void *)2);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_scroll_dir(obj, LV_DIR_NONE);
                    lv_obj_set_scroll_snap_x(obj, LV_SCROLL_SNAP_NONE);
                    lv_obj_set_scroll_snap_y(obj, LV_SCROLL_SNAP_NONE);
                    add_style_style_labels_digital_clock(obj);
                    lv_label_set_text(obj, "");
                }
                {
                    // label_colon_digital_clock
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_colon_digital_clock = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_PCT(9), LV_PCT(24));
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_scroll_dir(obj, LV_DIR_NONE);
                    lv_obj_set_scroll_snap_x(obj, LV_SCROLL_SNAP_NONE);
                    lv_obj_set_scroll_snap_y(obj, LV_SCROLL_SNAP_NONE);
                    add_style_style_labels_digital_clock(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_translate_y(obj, -2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_translate_x(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, ":");
                }
                {
                    // label_time_min_digital_clock
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_time_min_digital_clock = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_PCT(20));
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_add_event_cb(obj, action_change_screen, LV_EVENT_KEY, (void *)2);
                    lv_obj_add_event_cb(obj, action_change_screen, LV_EVENT_FOCUSED, (void *)2);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_scroll_dir(obj, LV_DIR_NONE);
                    lv_obj_set_scroll_snap_x(obj, LV_SCROLL_SNAP_NONE);
                    lv_obj_set_scroll_snap_y(obj, LV_SCROLL_SNAP_NONE);
                    add_style_style_labels_digital_clock(obj);
                    lv_label_set_text(obj, "");
                }
            }
        }
    }
    
    tick_screen_scr_digital_clock();
}

void tick_screen_scr_digital_clock() {
    {
        const char *new_val = get_var_time_hr_global();
        const char *cur_val = lv_label_get_text(objects.label_time_hr_digital_clock);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.label_time_hr_digital_clock;
            lv_label_set_text(objects.label_time_hr_digital_clock, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_time_min_global();
        const char *cur_val = lv_label_get_text(objects.label_time_min_digital_clock);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.label_time_min_digital_clock;
            lv_label_set_text(objects.label_time_min_digital_clock, new_val);
            tick_value_change_obj = NULL;
        }
    }
}

void create_screen_scr_analog_clock() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.scr_analog_clock = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 240);
    lv_obj_add_event_cb(obj, event_handler_cb_scr_analog_clock_scr_analog_clock, LV_EVENT_ALL, 0);
    lv_obj_add_state(obj, LV_STATE_DISABLED);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // cont_analog_clock
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cont_analog_clock = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 240);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // scale_analog_clock
                    lv_obj_t *obj = lv_scale_create(parent_obj);
                    objects.scale_analog_clock = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 240, 240);
                    lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
                    lv_scale_set_range(obj, 1, 12);
                    lv_scale_set_total_tick_count(obj, 60);
                    lv_scale_set_major_tick_every(obj, 5);
                    lv_scale_set_label_show(obj, true);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_scroll_dir(obj, LV_DIR_NONE);
                    lv_obj_set_scroll_snap_x(obj, LV_SCROLL_SNAP_NONE);
                    lv_obj_set_scroll_snap_y(obj, LV_SCROLL_SNAP_NONE);
                    add_style_style_scale_analog_clock(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_clip_corner(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_clip_corner(obj, true, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_rounded(obj, false, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_rounded(obj, true, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
            }
        }
    }
    
    tick_screen_scr_analog_clock();
}

void tick_screen_scr_analog_clock() {
}

void create_screen_scr_menu() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.scr_menu = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 240);
    lv_obj_add_event_cb(obj, action_change_screen, LV_EVENT_KEY, (void *)6);
    lv_obj_add_event_cb(obj, event_handler_cb_scr_menu_scr_menu, LV_EVENT_ALL, 0);
    lv_obj_add_state(obj, LV_STATE_DISABLED);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // cont_scr_menu
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cont_scr_menu = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, 200);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // cont_menu_left
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.cont_menu_left = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 110, 140);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_END, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // label_menu_brightness
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.label_menu_brightness = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_style_labels_menu(obj);
                            lv_label_set_text(obj, "Brghtnss:");
                        }
                        {
                            // label_menu_clock_type
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.label_menu_clock_type = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            add_style_style_labels_menu(obj);
                            lv_label_set_text(obj, "Clock:");
                        }
                        {
                            // label_menu_background_colour
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.label_menu_background_colour = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            add_style_style_labels_menu(obj);
                            lv_label_set_text(obj, "Backgrnd:");
                        }
                        {
                            // label_menu_text_colour
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.label_menu_text_colour = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_style_labels_menu(obj);
                            lv_label_set_text(obj, "Text:");
                        }
                    }
                }
                {
                    // cont_menu_right
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.cont_menu_right = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 85, 140);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // spinbox_menu_brightness
                            lv_obj_t *obj = lv_spinbox_create(parent_obj);
                            objects.spinbox_menu_brightness = obj;
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_PCT(40), LV_PCT(13));
                            lv_spinbox_set_digit_format(obj, 3, 0);
                            lv_spinbox_set_range(obj, 10, 100);
                            lv_spinbox_set_rollover(obj, true);
                            lv_spinbox_set_step(obj, 10);
                            lv_spinbox_set_value(obj, 100);
                            lv_obj_add_event_cb(obj, action_menu_brightness_value_changed, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_ON_FOCUS|LV_OBJ_FLAG_SNAPPABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_set_scroll_dir(obj, LV_DIR_NONE);
                            lv_obj_set_scroll_snap_x(obj, LV_SCROLL_SNAP_NONE);
                            lv_obj_set_scroll_snap_y(obj, LV_SCROLL_SNAP_NONE);
                            add_style_style_spinboxes_menu(obj);
                            lv_obj_set_style_margin_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // roller_menu_clock_type
                            lv_obj_t *obj = lv_roller_create(parent_obj);
                            objects.roller_menu_clock_type = obj;
                            lv_obj_set_pos(obj, 0, -325);
                            lv_obj_set_size(obj, LV_PCT(86), 20);
                            lv_roller_set_options(obj, "Digital\nAnalog", LV_ROLLER_MODE_INFINITE);
                            lv_obj_add_event_cb(obj, action_menu_clock_type_value_changed, LV_EVENT_KEY, (void *)0);
                            add_style_style_rollers_menu(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // roller_menu_background_colour
                            lv_obj_t *obj = lv_roller_create(parent_obj);
                            objects.roller_menu_background_colour = obj;
                            lv_obj_set_pos(obj, 0, -363);
                            lv_obj_set_size(obj, LV_PCT(76), 20);
                            lv_roller_set_options(obj, "White\nBlack\nRed\nPink\nPurple\nIndigo\nBlue\nCyan\nTeal\nGreen\nLime\nYellow\nAmber\nOrange\nBrown\nGray", LV_ROLLER_MODE_INFINITE);
                            lv_roller_set_selected(obj, 5, LV_ANIM_OFF);
                            lv_obj_add_event_cb(obj, action_menu_background_colour_value_changed, LV_EVENT_KEY, (void *)0);
                            add_style_style_rollers_menu(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // roller_menu_text_colour
                            lv_obj_t *obj = lv_roller_create(parent_obj);
                            objects.roller_menu_text_colour = obj;
                            lv_obj_set_pos(obj, 0, -335);
                            lv_obj_set_size(obj, LV_PCT(76), 20);
                            lv_roller_set_options(obj, "White\nBlack\nRed\nPink\nPurple\nIndigo\nBlue\nCyan\nTeal\nGreen\nLime\nYellow\nAmber\nOrange\nBrown\nGray", LV_ROLLER_MODE_INFINITE);
                            lv_obj_add_event_cb(obj, action_menu_text_colour_value_changed, LV_EVENT_KEY, (void *)0);
                            add_style_style_rollers_menu(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                    }
                }
            }
        }
        {
            // cont_menu_buttons
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cont_menu_buttons = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 240, LV_PCT(10));
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_translate_y(obj, -40, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // button_menu_cancel
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.button_menu_cancel = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_PCT(80));
                    lv_obj_add_event_cb(obj, action_change_screen, LV_EVENT_KEY, (void *)6);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_ON_FOCUS|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_scroll_dir(obj, LV_DIR_NONE);
                    lv_obj_set_scroll_snap_x(obj, LV_SCROLL_SNAP_NONE);
                    lv_obj_set_scroll_snap_y(obj, LV_SCROLL_SNAP_NONE);
                    add_style_style_buttons_menu(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "CANCEL");
                        }
                    }
                }
                {
                    // button_menu_save
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.button_menu_save = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 50, LV_PCT(80));
                    lv_obj_add_event_cb(obj, action_menu_save, LV_EVENT_KEY, (void *)6);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_ON_FOCUS|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_scroll_dir(obj, LV_DIR_NONE);
                    lv_obj_set_scroll_snap_x(obj, LV_SCROLL_SNAP_NONE);
                    lv_obj_set_scroll_snap_y(obj, LV_SCROLL_SNAP_NONE);
                    add_style_style_buttons_menu(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "SAVE");
                        }
                    }
                }
            }
        }
    }
    
    tick_screen_scr_menu();
}

void tick_screen_scr_menu() {
}


void ui_create_groups() {
    if (!groups_created) {
        groups.group_menu = lv_group_create();
        groups.group_digital_clock = lv_group_create();
        groups.group_digital_clock_set_time = lv_group_create();
        groups.group_analog_clock = lv_group_create();
        groups.grou_analog_clock_set_time = lv_group_create();
        groups_created = true;
    }
}


typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_scr_digital_clock,
    tick_screen_scr_analog_clock,
    tick_screen_scr_menu,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    ui_create_groups();
    
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_scr_digital_clock();
    create_screen_scr_analog_clock();
    create_screen_scr_menu();
}
