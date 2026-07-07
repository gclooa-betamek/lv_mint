/** PROJECT MINT
 *  @file   util.c
 *  @author G.C. Looa
 *  @date   2026-06-25
 *  @brief  Utility functions for the LVGL simulator.
 */

 #include <stdio.h>
 #include <time.h>
 #include "lvgl/lvgl.h"
 #include "component.h"
 #include "util.h"

 lv_subject_t subject_radio_slider;
 char subject_radio_slider_buffer[8];

void clock_timer_callback(lv_timer_t * timer)
{
    time_t now = time(NULL);
    struct tm tm_now;
    localtime_s(&tm_now, &now);

    char tm_buffer[16];
    strftime(tm_buffer, sizeof(tm_buffer), "%I:%M %p", &tm_now);

    lv_obj_t * time_label = lv_timer_get_user_data(timer);
    lv_label_set_text(time_label, tm_buffer);
}

void button_event_callback(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * button = lv_event_get_target(e);
    lv_obj_t * content = lv_event_get_user_data(e);

    /* Get parents to find corresponding siblings. */
    lv_obj_t * screen_navbar = lv_obj_get_parent(button);
    lv_obj_t * screen_content = lv_obj_get_parent(content);

    if (code == LV_EVENT_CLICKED) {
        /* Clear LV_STATE_CHECKED for all buttons except the one pressed. (Toggling behavior) */
        lv_obj_remove_state(lv_obj_get_child(screen_navbar, RADIO), LV_STATE_CHECKED);
        lv_obj_remove_state(lv_obj_get_child(screen_navbar, MEDIA), LV_STATE_CHECKED);
        lv_obj_remove_state(lv_obj_get_child(screen_navbar, PHONE), LV_STATE_CHECKED);
        lv_obj_remove_state(lv_obj_get_child(screen_navbar, SETTINGS), LV_STATE_CHECKED);
        lv_obj_add_state(button, LV_STATE_CHECKED);

        /* Hide all content screens except the one selected. */
        lv_obj_add_flag(lv_obj_get_child(screen_content, RADIO), LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(lv_obj_get_child(screen_content, MEDIA), LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(lv_obj_get_child(screen_content, PHONE), LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(lv_obj_get_child(screen_content, SETTINGS), LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_flag(content, LV_OBJ_FLAG_HIDDEN);
    }
}

void radio_slider_event_callback(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code != LV_EVENT_VALUE_CHANGED) return;

    lv_obj_t * slider = lv_event_get_target(e);
    int slider_value = lv_slider_get_value(slider);
    char slider_value_buffer[8];
    snprintf(slider_value_buffer, sizeof(slider_value_buffer), "%d.%d", slider_value / 10, slider_value % 10);

    lv_subject_copy_string(&subject_radio_slider, slider_value_buffer);
    printf("Slider value changed: %s\n", slider_value_buffer);
    printf("Subject value = %s\n", lv_subject_get_string(&subject_radio_slider));
}
