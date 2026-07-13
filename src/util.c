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
    if (code != LV_EVENT_CLICKED) return;

    lv_obj_t * button = lv_event_get_target(e);
    lv_obj_t * content = lv_event_get_user_data(e);

    /* Get parents to find corresponding siblings. */
    lv_obj_t * screen_navbar = lv_obj_get_parent(button);
    lv_obj_t * screen_content = lv_obj_get_parent(content);

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

void widget_tuner_event_callback(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code != LV_EVENT_VALUE_CHANGED) return;

    lv_obj_t * slider = lv_event_get_target(e);
    int slider_value = lv_slider_get_value(slider);
    char slider_value_buffer[8];
    snprintf(slider_value_buffer, sizeof(slider_value_buffer), "%d.%d", slider_value / 10, slider_value % 10);
    lv_subject_copy_string(&subject_radio_slider, slider_value_buffer);

    printf("DEBUG: Slider value  = %s\n", slider_value_buffer);
    printf("DEBUG: Subject value = %s\n", lv_subject_get_string(&subject_radio_slider));
}

void widget_tuner_draw_callback(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code != LV_EVENT_DRAW_MAIN) return;

    lv_obj_t * tuner = lv_event_get_target(e);
    lv_obj_t * slider = lv_obj_get_child(tuner, 0);

    lv_layer_t * layer = lv_event_get_layer(e);

    lv_area_t area_tuner;
    lv_obj_get_content_coords(tuner, &area_tuner);
    lv_area_t area_slider;
    lv_obj_get_content_coords(slider, &area_slider);
    lv_area_t area;
    lv_area_set(&area, area_slider.x1, area_tuner.y1, area_slider.x2, area_tuner.y2);

    int min = lv_slider_get_min_value(slider);
    int max = lv_slider_get_max_value(slider);
    int value = lv_slider_get_value(slider);

    int width = lv_area_get_width(&area);

    /* Tuner frequency markers */
    for (int freq = min; freq <= max; freq += 5) {
        lv_draw_line_dsc_t marker;
        lv_draw_line_dsc_init(&marker);

        int x = area.x1 + (freq - min) * width / (max - min);
        lv_point_precise_t marker_points[2];
        marker_points[0].x = marker_points[1].x = x;
        marker_points[0].y = area.y1 + 10;
        marker_points[1].y = area.y2 - 10;

        marker.points = marker_points;
        marker.point_cnt = 2;
        marker.width = 5;
        marker.round_start = 1;
        marker.round_end = 1;

        bool major = (freq % 100) == 0;
        if(major) marker.color = lv_color_make(0x2b, 0x2b, 0x2b);
        else marker.color = lv_color_make(0x1a, 0x1a, 0x1a);

        lv_draw_line(layer, &marker);
    }

    /* Tuner cursor */
    lv_draw_line_dsc_t cursor;
    lv_draw_line_dsc_init(&cursor);

    int cursor_x = area.x1 + (value - min) * width / (max - min);
    lv_point_precise_t cursor_points[2];
    cursor_points[0].x = cursor_points[1].x = cursor_x;
    cursor_points[0].y = area.y1;
    cursor_points[1].y = area.y2;

    cursor.points = cursor_points;
    cursor.point_cnt = 2;
    cursor.width = 7;
    cursor.round_start = 1;
    cursor.round_end = 1;
    cursor.color = lv_color_make(255, 0, 0);

    lv_draw_line(layer, &cursor);

    /* Debug message */
    printf("DEBUG: widget_tuner_draw_callback() fired.\n");
}
