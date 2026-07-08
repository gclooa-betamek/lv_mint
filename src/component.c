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

static const char * label_navbar_button[SCREEN_CONTENT_COUNT] = {
    LV_SYMBOL_HOME "   Radio",
    LV_SYMBOL_AUDIO "   Media",
    LV_SYMBOL_CALL "   Phone",
    LV_SYMBOL_SETTINGS "   Settings"
};

static const char * label_channel_button[6] = {
    "EIGHT FM",
    "Ai FM",
    "Hitz",
    "Mix",
    "Fly FM",
    "Era"
};

static const char * label_radio_button[4] = {
    LV_SYMBOL_PREV,
    LV_SYMBOL_LEFT,
    LV_SYMBOL_RIGHT,
    LV_SYMBOL_NEXT,
};

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

    lv_subject_init_string(
        &subject_radio_slider, subject_radio_slider_buffer, NULL,
        sizeof(subject_radio_slider_buffer), "80.0"
    );

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
        lv_obj_add_style(navbar[i], &style_base_button_hover, LV_STATE_HOVERED);
        lv_obj_add_style(navbar[i], &style_navbar_button, LV_STATE_CHECKED);

        lv_obj_t * label = lv_label_create(navbar[i]);
        lv_label_set_text(label, label_navbar_button[i]);
        lv_obj_add_event_cb(navbar[i], button_event_callback, LV_EVENT_ALL, content[i]);
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
    lv_obj_t * widget_channel = lv_obj_create(radio);
    lv_obj_set_grid_cell(widget_channel, LV_GRID_ALIGN_STRETCH, 0, 4, LV_GRID_ALIGN_STRETCH, 0, 4);
    lv_obj_add_style(widget_channel, &style_base_widget, LV_PART_MAIN);
    lv_obj_add_style(widget_channel, &style_widget_channel, LV_PART_MAIN);

    lv_obj_t * label_channel = lv_label_create(widget_channel);
    lv_label_bind_text(label_channel, &subject_radio_slider, "%s");
    lv_obj_set_style_text_font(label_channel, &lv_font_montserrat_48, LV_PART_MAIN);

    lv_obj_t * label_station = lv_label_create(widget_channel);
    lv_label_set_text(label_station, "Hitz FM");
    lv_obj_set_style_text_font(label_station, &lv_font_montserrat_32, LV_PART_MAIN);

    lv_obj_t * label_song = lv_label_create(widget_channel);
    lv_label_set_text(label_song, "Radiohead - Weird Fishes/Arpeggi");
    lv_obj_set_style_text_font(label_song, &lv_font_montserrat_24, LV_PART_MAIN);

    /* Radio controls */
    for (int i = 0; i < 4; i++) {
        lv_obj_t * widget_channel_button = lv_btn_create(radio);
        lv_obj_set_grid_cell(widget_channel_button, LV_GRID_ALIGN_STRETCH, i, 1, LV_GRID_ALIGN_STRETCH, 4, 2);
        lv_obj_add_style(widget_channel_button, &style_base_button, LV_PART_MAIN);
        lv_obj_add_style(widget_channel_button, &style_base_button_hover, LV_STATE_HOVERED);
        // lv_obj_add_style(widget_channel_button, &style_widget_channel_button, LV_PART_MAIN);

        lv_obj_t * label = lv_label_create(widget_channel_button);
        lv_label_set_text(label, label_radio_button[i]);
        lv_obj_center(label);
    }

    /* Channel buttons */
    lv_obj_t * button[4];
    for (int row = 0; row < 3; row++) {
        for(int col = 0; col < 2; col++) {
            int index = row * 2 + col;
            button[index] = lv_btn_create(radio);
            lv_obj_set_grid_cell(button[index], LV_GRID_ALIGN_STRETCH, col * 2 + 4, 2, LV_GRID_ALIGN_STRETCH, row * 2, 2);
            lv_obj_add_style(button[index], &style_base_button, LV_PART_MAIN);
            lv_obj_add_style(button[index], &style_base_button_hover, LV_STATE_HOVERED);
            lv_obj_add_style(button[index], &style_navbar_button, LV_STATE_CHECKED);

            lv_obj_t * label = lv_label_create(button[index]);
            lv_label_set_text(label, label_channel_button[index]);
            lv_obj_center(label);
        }
    }

    /* Tuner widget */
    lv_obj_t * widget_tuner = lv_obj_create(radio);
    lv_obj_set_layout(widget_tuner, LV_LAYOUT_GRID);
    lv_obj_set_grid_cell(widget_tuner, LV_GRID_ALIGN_STRETCH, 0, 8, LV_GRID_ALIGN_STRETCH, 6, 2);
    lv_obj_add_style(widget_tuner, &style_base_widget, LV_PART_MAIN);
    lv_obj_add_style(widget_tuner, &style_widget_tuner, LV_PART_MAIN);

    lv_obj_t * widget_tuner_slider = lv_slider_create(widget_tuner);
    lv_obj_set_grid_cell(widget_tuner_slider, LV_GRID_ALIGN_CENTER, 0, 8, LV_GRID_ALIGN_STRETCH, 0, 2);
    lv_obj_set_width(widget_tuner_slider, LV_PCT(95));
    lv_obj_set_style_bg_opa(widget_tuner_slider, LV_OPA_0, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(widget_tuner_slider, LV_OPA_0, LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(widget_tuner_slider, LV_OPA_0, LV_PART_KNOB);

    lv_slider_set_range(widget_tuner_slider, 800, 1100);
    lv_obj_add_event_cb(widget_tuner_slider, widget_tuner_event_callback, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(widget_tuner, widget_tuner_draw_callback, LV_EVENT_DRAW_MAIN, NULL);

}

void content_media(lv_obj_t * screen_content)
{
    lv_obj_t * media = lv_obj_create(screen_content);
    lv_obj_set_size(media, SCREEN_WIDTH, LV_PCT(100));
    lv_obj_add_style(media, &style_base, LV_PART_MAIN);
    lv_obj_add_style(media, &style_content_part, LV_PART_MAIN);

    lv_obj_t * label = lv_label_create(media);
    lv_label_set_text(label, "Media");
    lv_obj_center(label);
}

void content_phone(lv_obj_t * screen_content)
{
    lv_obj_t * phone = lv_obj_create(screen_content);
    lv_obj_set_size(phone, SCREEN_WIDTH, LV_PCT(100));
    lv_obj_add_style(phone, &style_base, LV_PART_MAIN);
    lv_obj_add_style(phone, &style_content_part, LV_PART_MAIN);

    lv_obj_t * label = lv_label_create(phone);
    lv_label_set_text(label, "Phone");
    lv_obj_center(label);
}

void content_settings(lv_obj_t * screen_content)
{
    lv_obj_t * settings = lv_obj_create(screen_content);
    lv_obj_set_size(settings, SCREEN_WIDTH, LV_PCT(100));
    lv_obj_add_style(settings, &style_base, LV_PART_MAIN);
    lv_obj_add_style(settings, &style_content_part, LV_PART_MAIN);

    lv_obj_t * label = lv_label_create(settings);
    lv_label_set_text(label, "Settings");
    lv_obj_center(label);
}
