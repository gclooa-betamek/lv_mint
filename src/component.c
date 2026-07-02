/** PROJECT MINT
 *  @file   component.c
 *  @author G.C. Looa
 *  @date   2026-06-22
 *  @brief  Component definitions for the LVGL simulator.
 */

#include "lvgl/lvgl.h"
#include "color.h"
#include "component.h"
#include "style.h"
#include "util.h"

static bool statusbar_init_flag = false;
static bool content_init_flag = false;
static bool navbar_init_flag = false;

/*******************
 * MAIN COMPONENTS
 *******************/

/* Create status bar elements. */
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

    /* Align WEATHER element to the left. */
    lv_obj_set_flex_align(statusbar[WEATHER], LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_t * label_weather = lv_label_create(statusbar[WEATHER]);
    lv_label_set_text(label_weather, "25°C");

    /* Align CLOCK element to the center. */
    lv_obj_set_flex_align(statusbar[CLOCK], LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_t * label_clock = lv_label_create(statusbar[CLOCK]);
    lv_label_set_text(label_clock, " ");

    /* Align SYMBOL element to the right. */
    lv_obj_set_flex_align(statusbar[SYMBOL], LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_t * label_symbol_bluetooth = lv_label_create(statusbar[SYMBOL]);
    lv_label_set_text(label_symbol_bluetooth, LV_SYMBOL_BLUETOOTH);
    lv_obj_t * label_symbol_wifi = lv_label_create(statusbar[SYMBOL]);
    lv_label_set_text(label_symbol_wifi, LV_SYMBOL_WIFI);

    /* Initiate the clock. */
    lv_timer_create(clock_timer_callback, 1000, label_clock);

    statusbar_init_flag = true;
}

/* Create content screens. Each screen is delegated to their own helper function below. */
void content_init(lv_obj_t * screen_content)
{
    if (content_init_flag) return;

    content_radio(screen_content);
    content_media(screen_content);
    content_phone(screen_content);
    content_settings(screen_content);

    /* Display radio screen by default. */
    lv_obj_add_flag(lv_obj_get_child(screen_content, RADIO), LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(lv_obj_get_child(screen_content, MEDIA), LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(lv_obj_get_child(screen_content, PHONE), LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(lv_obj_get_child(screen_content, SETTINGS), LV_OBJ_FLAG_HIDDEN);
    lv_obj_remove_flag(lv_obj_get_child(screen_content, RADIO), LV_OBJ_FLAG_HIDDEN);

    content_init_flag = true;
}

/* Create navigation bar buttons corresponding to each content screen. */
void navbar_init(lv_obj_t * screen_navbar)
{
    if (navbar_init_flag) return;

    lv_obj_t * screen = lv_obj_get_parent(screen_navbar);
    lv_obj_t * screen_content = lv_obj_get_child(screen, CONTENT);
    lv_obj_t * content[SCREEN_CONTENT_COUNT];

    content[RADIO] = lv_obj_get_child(screen_content, RADIO);
    content[MEDIA] = lv_obj_get_child(screen_content, MEDIA);
    content[PHONE] = lv_obj_get_child(screen_content, PHONE);
    content[SETTINGS] = lv_obj_get_child(screen_content, SETTINGS);

    lv_obj_t * navbar[SCREEN_CONTENT_COUNT];

    for(int i = 0; i < SCREEN_CONTENT_COUNT; i++) {
        navbar[i] = lv_btn_create(screen_navbar);
        lv_obj_set_height(navbar[i], LV_PCT(100));
        lv_obj_set_flex_grow(navbar[i], 1);
        lv_obj_add_style(navbar[i], &style_base_button, LV_PART_MAIN);
        lv_obj_add_style(navbar[i], &style_navbar_button_hover, LV_STATE_HOVERED);
        lv_obj_add_style(navbar[i], &style_navbar_button, LV_STATE_CHECKED);
        lv_obj_t * label = lv_label_create(navbar[i]);
        switch(i) {
        case RADIO:
            lv_label_set_text(label, LV_SYMBOL_HOME "   Radio");
            lv_obj_add_event_cb(navbar[i], button_event_callback, LV_EVENT_ALL, content[RADIO]);
            break;
        case MEDIA:
            lv_label_set_text(label, LV_SYMBOL_AUDIO "   Media");
            lv_obj_add_event_cb(navbar[i], button_event_callback, LV_EVENT_ALL, content[MEDIA]);
            break;
        case PHONE:
            lv_label_set_text(label, LV_SYMBOL_CALL "   Phone");
            lv_obj_add_event_cb(navbar[i], button_event_callback, LV_EVENT_ALL, content[PHONE]);
            break;
        case SETTINGS:
            lv_label_set_text(label, LV_SYMBOL_SETTINGS "   Settings");
            lv_obj_add_event_cb(navbar[i], button_event_callback, LV_EVENT_ALL, content[SETTINGS]);
            break;
        }
        lv_obj_center(label);
    }
    lv_obj_add_state(navbar[RADIO], LV_STATE_CHECKED);

    navbar_init_flag = true;
}

/*******************
 * CONTENT SCREENS
 *******************/

void content_radio(lv_obj_t * screen_content)
{
    /* Radio content container */
    lv_obj_t * radio = lv_obj_create(screen_content);
    lv_obj_add_style(radio, &style_base, LV_PART_MAIN);
    lv_obj_add_style(radio, &style_content_part, LV_PART_MAIN);

    /* Channel display part */
    lv_obj_t * part_channel = lv_obj_create(radio);
    lv_obj_set_grid_cell(part_channel, LV_GRID_ALIGN_STRETCH, 0, 2, LV_GRID_ALIGN_STRETCH, 0, 2);
    lv_obj_set_style_bg_grad(part_channel, &grad_dsc_sky_blue, LV_PART_MAIN);
    lv_obj_add_style(part_channel, &style_base_widget, LV_PART_MAIN);
}

void content_media(lv_obj_t * screen_content)
{
    lv_obj_t * media = lv_obj_create(screen_content);
    // lv_obj_set_layout(media, LV_LAYOUT_FLEX);
    lv_obj_set_size(media, SCREEN_WIDTH, LV_PCT(100));
    lv_obj_add_style(media, &style_base, LV_PART_MAIN);
    lv_obj_add_style(media, &style_content, LV_PART_MAIN);

    lv_obj_t * label = lv_label_create(media);
    lv_label_set_text(label, "Media");
    lv_obj_center(label);
}

void content_phone(lv_obj_t * screen_content)
{
    lv_obj_t * phone = lv_obj_create(screen_content);
    // lv_obj_set_layout(phone, LV_LAYOUT_FLEX);
    lv_obj_set_size(phone, SCREEN_WIDTH, LV_PCT(100));
    lv_obj_add_style(phone, &style_base, LV_PART_MAIN);
    lv_obj_add_style(phone, &style_content, LV_PART_MAIN);

    lv_obj_t * label = lv_label_create(phone);
    lv_label_set_text(label, "Phone");
    lv_obj_center(label);
}

void content_settings(lv_obj_t * screen_content)
{
    lv_obj_t * settings = lv_obj_create(screen_content);
    // lv_obj_set_layout(settings, LV_LAYOUT_FLEX);
    lv_obj_set_size(settings, SCREEN_WIDTH, LV_PCT(100));
    lv_obj_add_style(settings, &style_base, LV_PART_MAIN);
    lv_obj_add_style(settings, &style_content, LV_PART_MAIN);

    lv_obj_t * label = lv_label_create(settings);
    lv_label_set_text(label, "Settings");
    lv_obj_center(label);
}
