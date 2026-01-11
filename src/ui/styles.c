#include "styles.h"
#include "images.h"
#include "fonts.h"

#include "ui.h"
#include "screens.h"

//
// Style: style_buttons_digital_clock_set_time
//

void init_style_style_buttons_digital_clock_set_time_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_pad_top(style, 0);
    lv_style_set_pad_bottom(style, 0);
    lv_style_set_pad_left(style, 0);
    lv_style_set_pad_right(style, 0);
    lv_style_set_pad_row(style, 0);
    lv_style_set_pad_column(style, 0);
    lv_style_set_bg_opa(style, 0);
    lv_style_set_text_font(style, &ui_font_courier_new_20);
    lv_style_set_radius(style, 0);
    lv_style_set_translate_y(style, -60);
    lv_style_set_margin_left(style, 4);
    lv_style_set_margin_right(style, 4);
};

lv_style_t *get_style_style_buttons_digital_clock_set_time_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_style_buttons_digital_clock_set_time_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_style_buttons_digital_clock_set_time_MAIN_FOCUS_KEY(lv_style_t *style) {
    lv_style_set_outline_opa(style, 0);
    lv_style_set_border_color(style, lv_color_hex(0xffffffff));
    lv_style_set_border_side(style, LV_BORDER_SIDE_BOTTOM);
    lv_style_set_border_width(style, 2);
};

lv_style_t *get_style_style_buttons_digital_clock_set_time_MAIN_FOCUS_KEY() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_style_buttons_digital_clock_set_time_MAIN_FOCUS_KEY(style);
    }
    return style;
};

void init_style_style_buttons_digital_clock_set_time_MAIN_FOCUSED(lv_style_t *style) {
    lv_style_set_border_color(style, lv_color_hex(0xffffffff));
    lv_style_set_border_width(style, 2);
    lv_style_set_border_side(style, LV_BORDER_SIDE_BOTTOM);
};

lv_style_t *get_style_style_buttons_digital_clock_set_time_MAIN_FOCUSED() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_style_buttons_digital_clock_set_time_MAIN_FOCUSED(style);
    }
    return style;
};

void add_style_style_buttons_digital_clock_set_time(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_style_buttons_digital_clock_set_time_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_style_buttons_digital_clock_set_time_MAIN_FOCUS_KEY(), LV_PART_MAIN | LV_STATE_FOCUS_KEY);
    lv_obj_add_style(obj, get_style_style_buttons_digital_clock_set_time_MAIN_FOCUSED(), LV_PART_MAIN | LV_STATE_FOCUSED);
};

void remove_style_style_buttons_digital_clock_set_time(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_style_buttons_digital_clock_set_time_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_style_buttons_digital_clock_set_time_MAIN_FOCUS_KEY(), LV_PART_MAIN | LV_STATE_FOCUS_KEY);
    lv_obj_remove_style(obj, get_style_style_buttons_digital_clock_set_time_MAIN_FOCUSED(), LV_PART_MAIN | LV_STATE_FOCUSED);
};

//
// Style: style_spinboxes_digital_clock
//

void init_style_style_spinboxes_digital_clock_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_font(style, &ui_font_courier_new_72);
    lv_style_set_radius(style, 0);
    lv_style_set_bg_opa(style, 0);
    lv_style_set_border_opa(style, 0);
    lv_style_set_pad_top(style, 0);
    lv_style_set_pad_bottom(style, 0);
    lv_style_set_pad_left(style, 0);
    lv_style_set_pad_right(style, 0);
    lv_style_set_pad_row(style, 0);
    lv_style_set_pad_column(style, 0);
    lv_style_set_text_align(style, LV_TEXT_ALIGN_LEFT);
    lv_style_set_text_color(style, lv_color_hex(0xffffffff));
};

lv_style_t *get_style_style_spinboxes_digital_clock_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_style_spinboxes_digital_clock_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_style_spinboxes_digital_clock_MAIN_FOCUS_KEY(lv_style_t *style) {
    lv_style_set_outline_opa(style, 0);
    lv_style_set_border_color(style, lv_color_hex(0xffffffff));
    lv_style_set_border_opa(style, 255);
    lv_style_set_border_width(style, 2);
    lv_style_set_border_side(style, LV_BORDER_SIDE_BOTTOM);
};

lv_style_t *get_style_style_spinboxes_digital_clock_MAIN_FOCUS_KEY() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_style_spinboxes_digital_clock_MAIN_FOCUS_KEY(style);
    }
    return style;
};

void init_style_style_spinboxes_digital_clock_MAIN_FOCUSED(lv_style_t *style) {
    lv_style_set_border_color(style, lv_color_hex(0xffffffff));
    lv_style_set_border_width(style, 2);
    lv_style_set_border_opa(style, 255);
    lv_style_set_border_side(style, LV_BORDER_SIDE_BOTTOM);
};

lv_style_t *get_style_style_spinboxes_digital_clock_MAIN_FOCUSED() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_style_spinboxes_digital_clock_MAIN_FOCUSED(style);
    }
    return style;
};

void init_style_style_spinboxes_digital_clock_CURSOR_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_opa(style, 0);
};

lv_style_t *get_style_style_spinboxes_digital_clock_CURSOR_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_style_spinboxes_digital_clock_CURSOR_DEFAULT(style);
    }
    return style;
};

void add_style_style_spinboxes_digital_clock(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_style_spinboxes_digital_clock_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_style_spinboxes_digital_clock_MAIN_FOCUS_KEY(), LV_PART_MAIN | LV_STATE_FOCUS_KEY);
    lv_obj_add_style(obj, get_style_style_spinboxes_digital_clock_MAIN_FOCUSED(), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_add_style(obj, get_style_style_spinboxes_digital_clock_CURSOR_DEFAULT(), LV_PART_CURSOR | LV_STATE_DEFAULT);
};

void remove_style_style_spinboxes_digital_clock(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_style_spinboxes_digital_clock_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_style_spinboxes_digital_clock_MAIN_FOCUS_KEY(), LV_PART_MAIN | LV_STATE_FOCUS_KEY);
    lv_obj_remove_style(obj, get_style_style_spinboxes_digital_clock_MAIN_FOCUSED(), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_remove_style(obj, get_style_style_spinboxes_digital_clock_CURSOR_DEFAULT(), LV_PART_CURSOR | LV_STATE_DEFAULT);
};

//
// Style: style_labels_digital_clock
//

void init_style_style_labels_digital_clock_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_font(style, &ui_font_courier_new_72);
    lv_style_set_text_color(style, lv_color_hex(0xffffffff));
};

lv_style_t *get_style_style_labels_digital_clock_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_style_labels_digital_clock_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_style_labels_digital_clock(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_style_labels_digital_clock_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_style_labels_digital_clock(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_style_labels_digital_clock_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: style_labels_menu
//

void init_style_style_labels_menu_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(0xffffffff));
    lv_style_set_text_font(style, &ui_font_courier_new_16);
};

lv_style_t *get_style_style_labels_menu_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_style_labels_menu_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_style_labels_menu(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_style_labels_menu_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_style_labels_menu(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_style_labels_menu_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: style_spinboxes_menu
//

void init_style_style_spinboxes_menu_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(0xffffffff));
    lv_style_set_text_font(style, &ui_font_courier_new_16);
    lv_style_set_border_opa(style, 0);
    lv_style_set_bg_opa(style, 0);
    lv_style_set_radius(style, 0);
    lv_style_set_pad_top(style, 0);
    lv_style_set_pad_bottom(style, 0);
    lv_style_set_pad_left(style, 0);
    lv_style_set_pad_right(style, 0);
    lv_style_set_pad_row(style, 0);
    lv_style_set_pad_column(style, 0);
};

lv_style_t *get_style_style_spinboxes_menu_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_style_spinboxes_menu_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_style_spinboxes_menu_MAIN_FOCUS_KEY(lv_style_t *style) {
    lv_style_set_border_color(style, lv_color_hex(0xffffffff));
    lv_style_set_border_opa(style, 255);
    lv_style_set_border_width(style, 2);
    lv_style_set_border_side(style, LV_BORDER_SIDE_BOTTOM);
    lv_style_set_outline_opa(style, 0);
};

lv_style_t *get_style_style_spinboxes_menu_MAIN_FOCUS_KEY() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_style_spinboxes_menu_MAIN_FOCUS_KEY(style);
    }
    return style;
};

void init_style_style_spinboxes_menu_CURSOR_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_opa(style, 0);
};

lv_style_t *get_style_style_spinboxes_menu_CURSOR_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_style_spinboxes_menu_CURSOR_DEFAULT(style);
    }
    return style;
};

void init_style_style_spinboxes_menu_SELECTED_DEFAULT(lv_style_t *style) {
    lv_style_set_text_align(style, LV_TEXT_ALIGN_LEFT);
};

lv_style_t *get_style_style_spinboxes_menu_SELECTED_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_style_spinboxes_menu_SELECTED_DEFAULT(style);
    }
    return style;
};

void add_style_style_spinboxes_menu(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_style_spinboxes_menu_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_style_spinboxes_menu_MAIN_FOCUS_KEY(), LV_PART_MAIN | LV_STATE_FOCUS_KEY);
    lv_obj_add_style(obj, get_style_style_spinboxes_menu_CURSOR_DEFAULT(), LV_PART_CURSOR | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_style_spinboxes_menu_SELECTED_DEFAULT(), LV_PART_SELECTED | LV_STATE_DEFAULT);
};

void remove_style_style_spinboxes_menu(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_style_spinboxes_menu_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_style_spinboxes_menu_MAIN_FOCUS_KEY(), LV_PART_MAIN | LV_STATE_FOCUS_KEY);
    lv_obj_remove_style(obj, get_style_style_spinboxes_menu_CURSOR_DEFAULT(), LV_PART_CURSOR | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_style_spinboxes_menu_SELECTED_DEFAULT(), LV_PART_SELECTED | LV_STATE_DEFAULT);
};

//
// Style: style_dropdown_menu_clock_type
//

void init_style_style_dropdown_menu_clock_type_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_font(style, &ui_font_courier_new_16);
    lv_style_set_text_color(style, lv_color_hex(0xffffffff));
    lv_style_set_bg_opa(style, 0);
    lv_style_set_radius(style, 0);
    lv_style_set_pad_top(style, 0);
    lv_style_set_pad_bottom(style, 0);
    lv_style_set_pad_left(style, 0);
    lv_style_set_pad_right(style, 0);
    lv_style_set_pad_row(style, 0);
    lv_style_set_pad_column(style, 0);
    lv_style_set_border_opa(style, 0);
    lv_style_set_text_opa(style, 255);
    lv_style_set_max_height(style, 536870911);
};

lv_style_t *get_style_style_dropdown_menu_clock_type_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_style_dropdown_menu_clock_type_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_style_dropdown_menu_clock_type_MAIN_FOCUS_KEY(lv_style_t *style) {
    lv_style_set_border_color(style, lv_color_hex(0xffffffff));
    lv_style_set_border_opa(style, 255);
    lv_style_set_border_width(style, 2);
    lv_style_set_border_side(style, LV_BORDER_SIDE_BOTTOM);
    lv_style_set_outline_opa(style, 0);
};

lv_style_t *get_style_style_dropdown_menu_clock_type_MAIN_FOCUS_KEY() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_style_dropdown_menu_clock_type_MAIN_FOCUS_KEY(style);
    }
    return style;
};

void add_style_style_dropdown_menu_clock_type(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_style_dropdown_menu_clock_type_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_style_dropdown_menu_clock_type_MAIN_FOCUS_KEY(), LV_PART_MAIN | LV_STATE_FOCUS_KEY);
};

void remove_style_style_dropdown_menu_clock_type(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_style_dropdown_menu_clock_type_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_style_dropdown_menu_clock_type_MAIN_FOCUS_KEY(), LV_PART_MAIN | LV_STATE_FOCUS_KEY);
};

//
// Style: style_buttons_menu
//

void init_style_style_buttons_menu_MAIN_FOCUS_KEY(lv_style_t *style) {
    lv_style_set_outline_opa(style, 0);
    lv_style_set_border_color(style, lv_color_hex(0xffffffff));
    lv_style_set_border_opa(style, 255);
    lv_style_set_border_width(style, 2);
    lv_style_set_border_side(style, LV_BORDER_SIDE_BOTTOM);
};

lv_style_t *get_style_style_buttons_menu_MAIN_FOCUS_KEY() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_style_buttons_menu_MAIN_FOCUS_KEY(style);
    }
    return style;
};

void init_style_style_buttons_menu_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_opa(style, 0);
    lv_style_set_text_color(style, lv_color_hex(0xffffffff));
    lv_style_set_text_font(style, &ui_font_courier_new_20);
    lv_style_set_pad_top(style, 0);
    lv_style_set_pad_bottom(style, 0);
    lv_style_set_pad_left(style, 0);
    lv_style_set_pad_right(style, 0);
    lv_style_set_pad_row(style, 0);
    lv_style_set_pad_column(style, 0);
    lv_style_set_radius(style, 0);
    lv_style_set_margin_left(style, 4);
    lv_style_set_margin_right(style, 4);
};

lv_style_t *get_style_style_buttons_menu_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_style_buttons_menu_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_style_buttons_menu(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_style_buttons_menu_MAIN_FOCUS_KEY(), LV_PART_MAIN | LV_STATE_FOCUS_KEY);
    lv_obj_add_style(obj, get_style_style_buttons_menu_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_style_buttons_menu(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_style_buttons_menu_MAIN_FOCUS_KEY(), LV_PART_MAIN | LV_STATE_FOCUS_KEY);
    lv_obj_remove_style(obj, get_style_style_buttons_menu_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: style_scale_analog_clock
//

void init_style_style_scale_analog_clock_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_align(style, LV_ALIGN_CENTER);
    lv_style_set_text_font(style, &ui_font_courier_new_16);
    lv_style_set_text_color(style, lv_color_hex(0xffffffff));
};

lv_style_t *get_style_style_scale_analog_clock_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_style_scale_analog_clock_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_style_scale_analog_clock(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_style_scale_analog_clock_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_style_scale_analog_clock(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_style_scale_analog_clock_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: style_rollers_menu
//

void init_style_style_rollers_menu_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_pad_top(style, 0);
    lv_style_set_pad_bottom(style, 0);
    lv_style_set_pad_left(style, 0);
    lv_style_set_pad_right(style, 0);
    lv_style_set_pad_row(style, 0);
    lv_style_set_pad_column(style, 0);
    lv_style_set_bg_opa(style, 0);
    lv_style_set_radius(style, 0);
    lv_style_set_clip_corner(style, true);
    lv_style_set_border_opa(style, 0);
    lv_style_set_text_font(style, &ui_font_courier_new_16);
    lv_style_set_text_color(style, lv_color_hex(0xffffffff));
};

lv_style_t *get_style_style_rollers_menu_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_style_rollers_menu_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_style_rollers_menu_MAIN_FOCUS_KEY(lv_style_t *style) {
    lv_style_set_border_color(style, lv_color_hex(0xffffffff));
    lv_style_set_border_opa(style, 255);
    lv_style_set_border_width(style, 2);
    lv_style_set_border_side(style, LV_BORDER_SIDE_BOTTOM);
    lv_style_set_outline_opa(style, 0);
};

lv_style_t *get_style_style_rollers_menu_MAIN_FOCUS_KEY() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_style_rollers_menu_MAIN_FOCUS_KEY(style);
    }
    return style;
};

void init_style_style_rollers_menu_SELECTED_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_opa(style, 0);
};

lv_style_t *get_style_style_rollers_menu_SELECTED_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_style_rollers_menu_SELECTED_DEFAULT(style);
    }
    return style;
};

void add_style_style_rollers_menu(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_style_rollers_menu_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_style_rollers_menu_MAIN_FOCUS_KEY(), LV_PART_MAIN | LV_STATE_FOCUS_KEY);
    lv_obj_add_style(obj, get_style_style_rollers_menu_SELECTED_DEFAULT(), LV_PART_SELECTED | LV_STATE_DEFAULT);
};

void remove_style_style_rollers_menu(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_style_rollers_menu_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_style_rollers_menu_MAIN_FOCUS_KEY(), LV_PART_MAIN | LV_STATE_FOCUS_KEY);
    lv_obj_remove_style(obj, get_style_style_rollers_menu_SELECTED_DEFAULT(), LV_PART_SELECTED | LV_STATE_DEFAULT);
};

//
//
//

void add_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*AddStyleFunc)(lv_obj_t *obj);
    static const AddStyleFunc add_style_funcs[] = {
        add_style_style_buttons_digital_clock_set_time,
        add_style_style_spinboxes_digital_clock,
        add_style_style_labels_digital_clock,
        add_style_style_labels_menu,
        add_style_style_spinboxes_menu,
        add_style_style_dropdown_menu_clock_type,
        add_style_style_buttons_menu,
        add_style_style_scale_analog_clock,
        add_style_style_rollers_menu,
    };
    add_style_funcs[styleIndex](obj);
}

void remove_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*RemoveStyleFunc)(lv_obj_t *obj);
    static const RemoveStyleFunc remove_style_funcs[] = {
        remove_style_style_buttons_digital_clock_set_time,
        remove_style_style_spinboxes_digital_clock,
        remove_style_style_labels_digital_clock,
        remove_style_style_labels_menu,
        remove_style_style_spinboxes_menu,
        remove_style_style_dropdown_menu_clock_type,
        remove_style_style_buttons_menu,
        remove_style_style_scale_analog_clock,
        remove_style_style_rollers_menu,
    };
    remove_style_funcs[styleIndex](obj);
}

