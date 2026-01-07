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
        // group: group_digital_clock
        lv_group_remove_all_objs(groups.group_digital_clock);
        lv_group_add_obj(groups.group_digital_clock, objects.label_time_hr_digital_clock);
        lv_group_add_obj(groups.group_digital_clock, objects.label_time_min_digital_clock);
        // group: group_digital_clock_set_time
        lv_group_remove_all_objs(groups.group_digital_clock_set_time);
    }
}

static void event_handler_cb_scr_digital_clock_set_time_scr_digital_clock_set_time(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_SCREEN_LOAD_START) {
        // group: group_digital_clock
        lv_group_remove_all_objs(groups.group_digital_clock);
        // group: group_digital_clock_set_time
        lv_group_remove_all_objs(groups.group_digital_clock_set_time);
        lv_group_add_obj(groups.group_digital_clock_set_time, objects.spinbox_hr_digital_clock_set_time);
        lv_group_add_obj(groups.group_digital_clock_set_time, objects.spinbox_min_digital_clock_set_time);
    }
}

void create_screen_scr_digital_clock() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.scr_digital_clock = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 240);
    lv_obj_add_event_cb(obj, event_handler_cb_scr_digital_clock_scr_digital_clock, LV_EVENT_ALL, 0);
    {
        lv_obj_t *parent_obj = obj;
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
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_add_event_cb(obj, action_change_screen, LV_EVENT_FOCUSED, (void *)2);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
                {
                    // label_colon_digital_clock
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_colon_digital_clock = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, ":");
                }
                {
                    // label_time_min_digital_clock
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_time_min_digital_clock = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_add_event_cb(obj, action_change_screen, LV_EVENT_FOCUSED, (void *)2);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);
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

void create_screen_scr_digital_clock_set_time() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.scr_digital_clock_set_time = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 240, 240);
    lv_obj_add_event_cb(obj, event_handler_cb_scr_digital_clock_set_time_scr_digital_clock_set_time, LV_EVENT_ALL, 0);
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
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_END, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // spinbox_hr_digital_clock_set_time
                    lv_obj_t *obj = lv_spinbox_create(parent_obj);
                    objects.spinbox_hr_digital_clock_set_time = obj;
                    lv_obj_set_pos(obj, -10445, -6613);
                    lv_obj_set_size(obj, LV_PCT(27), LV_SIZE_CONTENT);
                    lv_spinbox_set_digit_format(obj, 2, 0);
                    lv_spinbox_set_range(obj, 0, 23);
                    lv_spinbox_set_rollover(obj, true);
                    lv_spinbox_set_step(obj, 1);
                    lv_spinbox_set_value(obj, 0);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_ON_FOCUS|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_scroll_dir(obj, LV_DIR_NONE);
                    lv_obj_set_scroll_snap_x(obj, LV_SCROLL_SNAP_NONE);
                    lv_obj_set_scroll_snap_y(obj, LV_SCROLL_SNAP_NONE);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_FOCUS_KEY);
                    lv_obj_set_style_border_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_CURSOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_SELECTED | LV_STATE_FOCUS_KEY);
                    lv_obj_set_style_border_opa(obj, 255, LV_PART_SELECTED | LV_STATE_FOCUS_KEY);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_SELECTED | LV_STATE_FOCUS_KEY);
                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_SELECTED | LV_STATE_FOCUS_KEY);
                }
                {
                    // label_colon_digital_clock_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.label_colon_digital_clock_1 = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, ":");
                }
                {
                    // spinbox_min_digital_clock_set_time
                    lv_obj_t *obj = lv_spinbox_create(parent_obj);
                    objects.spinbox_min_digital_clock_set_time = obj;
                    lv_obj_set_pos(obj, -4096, -4722);
                    lv_obj_set_size(obj, LV_PCT(27), LV_SIZE_CONTENT);
                    lv_spinbox_set_digit_format(obj, 2, 0);
                    lv_spinbox_set_range(obj, 0, 59);
                    lv_spinbox_set_rollover(obj, true);
                    lv_spinbox_set_step(obj, 1);
                    lv_spinbox_set_value(obj, 0);
                    lv_obj_clear_flag(obj, LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_ON_FOCUS|LV_OBJ_FLAG_SNAPPABLE);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    lv_obj_set_scroll_dir(obj, LV_DIR_NONE);
                    lv_obj_set_scroll_snap_x(obj, LV_SCROLL_SNAP_NONE);
                    lv_obj_set_scroll_snap_y(obj, LV_SCROLL_SNAP_NONE);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_FOCUS_KEY);
                    lv_obj_set_style_border_opa(obj, 255, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
                    lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_FOCUS_KEY);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_CURSOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_max_width(obj, 10, LV_PART_CURSOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_SELECTED | LV_STATE_FOCUS_KEY);
                    lv_obj_set_style_border_opa(obj, 255, LV_PART_SELECTED | LV_STATE_FOCUS_KEY);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_SELECTED | LV_STATE_FOCUS_KEY);
                    lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_BOTTOM, LV_PART_SELECTED | LV_STATE_FOCUS_KEY);
                }
            }
        }
    }
    
    tick_screen_scr_digital_clock_set_time();
}

void tick_screen_scr_digital_clock_set_time() {
}


void ui_create_groups() {
    if (!groups_created) {
        groups.group_digital_clock = lv_group_create();
        groups.group_digital_clock_set_time = lv_group_create();
        groups_created = true;
    }
}


typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_scr_digital_clock,
    tick_screen_scr_digital_clock_set_time,
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
    create_screen_scr_digital_clock_set_time();
}
