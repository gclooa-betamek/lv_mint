/** PROJECT MINT
 *  @file   component.c
 *  @author G.C. Looa
 *  @date   2026-06-22
 *  @brief  Component definitions for the LVGL simulator.
 */

#include "lvgl/lvgl.h"
#include "component.h"
#include "style.h"
#include "util.h"

#define STATUSBAR_PART_COUNT 3
#define NAVBAR_BUTTON_COUNT  4

typedef enum statusbar_part{
    WEATHER,
    CLOCK,
    SYMBOL
} statusbar_part_t;

typedef enum navbar_button{
    RADIO,
    MEDIA,
    PHONE,
    SETTINGS
} navbar_button_t;

static bool statusbar_init_flag = false;
static bool content_init_flag = false;
static bool navbar_init_flag = false;

void statusbar_init(lv_obj_t * screen_statusbar)
{
    if (statusbar_init_flag) return;

    lv_obj_t * statusbar[STATUSBAR_PART_COUNT];

    for(int i = 0; i < STATUSBAR_PART_COUNT; i++) {
        statusbar[i] = lv_obj_create(screen_statusbar);
        lv_obj_set_flex_grow(statusbar[i], 1);
        lv_obj_set_flex_flow(statusbar[i], LV_FLEX_FLOW_ROW);
        lv_obj_set_height(statusbar[i], LV_PCT(100));
        lv_obj_add_style(statusbar[i], &style_base, LV_PART_MAIN);
        lv_obj_add_style(statusbar[i], &style_statusbar_part, LV_PART_MAIN);
    }
    lv_obj_set_flex_align(statusbar[WEATHER], LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_t * label_weather = lv_label_create(statusbar[WEATHER]);
    lv_label_set_text(label_weather, "25°C");

    lv_obj_set_flex_align(statusbar[CLOCK], LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_t * label_clock = lv_label_create(statusbar[CLOCK]);
    lv_label_set_text(label_clock, "10:10 AM");

    lv_obj_set_flex_align(statusbar[SYMBOL], LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_t * label_symbol_bluetooth = lv_label_create(statusbar[SYMBOL]);
    lv_label_set_text(label_symbol_bluetooth, LV_SYMBOL_BLUETOOTH);
    lv_obj_t * label_symbol_wifi = lv_label_create(statusbar[SYMBOL]);
    lv_label_set_text(label_symbol_wifi, LV_SYMBOL_WIFI);

    lv_timer_create(clock_timer_callback, 1000, label_clock);

    statusbar_init_flag = true;
}

void content_init(lv_obj_t * screen_content)
{
    if (content_init_flag) return;

    content_init_flag = true;
}

void navbar_init(lv_obj_t * screen_navbar)
{
    if (navbar_init_flag) return;

    lv_obj_t * navbar[NAVBAR_BUTTON_COUNT];

    for(int i = 0; i < NAVBAR_BUTTON_COUNT; i++) {
        navbar[i] = lv_btn_create(screen_navbar);
        lv_obj_set_height(navbar[i], LV_PCT(100));
        lv_obj_set_flex_grow(navbar[i], 1);
        lv_obj_add_style(navbar[i], &style_base_button, LV_PART_MAIN);
        lv_obj_t * label = lv_label_create(navbar[i]);
        switch(i) {
        case RADIO:
            lv_label_set_text(label, LV_SYMBOL_HOME "   Radio");
            break;
        case MEDIA:
            lv_label_set_text(label, LV_SYMBOL_AUDIO "   Media");
            break;
        case PHONE:
            lv_label_set_text(label, LV_SYMBOL_CALL "   Phone");
            break;
        case SETTINGS:
            lv_label_set_text(label, LV_SYMBOL_SETTINGS "   Settings");
            break;
        }
        lv_obj_center(label);
    }
    navbar_init_flag = true;
}
