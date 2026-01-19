#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations



// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_TIME_HR_GLOBAL = 0,
    FLOW_GLOBAL_VARIABLE_TIME_MIN_GLOBAL = 1
};

// Native global variables

extern const char *get_var_time_hr_global();
extern void set_var_time_hr_global(const char *value);
extern const char *get_var_time_min_global();
extern void set_var_time_min_global(const char *value);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/