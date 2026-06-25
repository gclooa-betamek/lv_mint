/** PROJECT MINT
 *  @file   util.c
 *  @author G.C. Looa
 *  @date   2026-06-25
 *  @brief  Utility functions for the LVGL simulator.
 */

 #include <time.h>
 #include "lvgl/lvgl.h"
 #include "util.h"

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
