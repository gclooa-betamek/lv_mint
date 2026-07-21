/** PROJECT MINT
 *  @file   component.c
 *  @author G.C. Looa
 *  @date   2026-06-22
 *  @brief  Component definitions for the LVGL simulator.
 */

#include <stdio.h>
#include "lvgl/lvgl.h"
#include "color.h"
#include "component.h"
#include "style.h"
#include "util.h"

static void contact_list(lv_obj_t * list, lv_obj_t * input);

static const char * label_navbar_button[] = {
    LV_SYMBOL_HOME "   Radio",
    LV_SYMBOL_AUDIO "   Media",
    LV_SYMBOL_CALL "   Phone",
    LV_SYMBOL_SETTINGS "   Settings"
};

static const char * label_radio_button[] = {
    LV_SYMBOL_PREV,
    LV_SYMBOL_LEFT,
    LV_SYMBOL_RIGHT,
    LV_SYMBOL_NEXT
};

static const widget_control_t widget_control_button[] = {
    {80, 0x1a1a1a, 0xffffff, LV_SYMBOL_SHUFFLE},
    {80, 0xffffff, 0x1a1a1a, LV_SYMBOL_PREV},
    {95, 0xffffff, 0x1a1a1a, LV_SYMBOL_PLAY},
    {80, 0xffffff, 0x1a1a1a, LV_SYMBOL_NEXT},
    {80, 0x1a1a1a, 0xffffff, LV_SYMBOL_LIST},
};

static label_bundle_t label_bundle;

static bool statusbar_init_flag = false;
static bool content_init_flag = false;
static bool navbar_init_flag = false;

/******************************************************************************
 * MAIN COMPONENTS
 ******************************************************************************/

/* Initialize subjects. */
void subject_init()
{
    /* Subject for current screen displayed. */
    lv_subject_init_int(&subject_screen_content, RADIO);

    /* Subject for radio tuner value. */
    lv_subject_init_int(&subject_radio_slider_int, radio_label[2].frequency);
    lv_subject_init_string(
        &subject_radio_slider_str, subject_radio_slider_buffer, NULL,
        sizeof(subject_radio_slider_buffer), "92.9"
    );

    /* Subject for radio metadata. */
    lv_subject_init_string(
        &subject_radio_station, subject_radio_station_buffer, NULL,
        sizeof(subject_radio_station_buffer), radio_label[2].station
    );
    lv_subject_init_string(
        &subject_radio_song, subject_radio_song_buffer, NULL,
        sizeof(subject_radio_song_buffer), radio_label[2].song
    );

    /* Subject for media metadata. */
    lv_subject_init_string(
        &subject_media_song, subject_media_song_buffer, NULL,
        sizeof(subject_media_song_buffer), media_label[2].song
    );
    lv_subject_init_string(
        &subject_media_artist, subject_media_artist_buffer, NULL,
        sizeof(subject_media_artist_buffer), media_label[2].artist
    );

    /* Initialize observer for current screen displayed. */
    lv_subject_add_observer(&subject_screen_content, screen_observer_callback, &label_bundle);
}

/* Create status bar elements. */
void statusbar_init(lv_obj_t * screen_statusbar)
{
    if (statusbar_init_flag) return;

    lv_obj_t * statusbar[STATUSBAR_PART_COUNT];

    for (int i = 0; i < STATUSBAR_PART_COUNT; i++) {
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

    for (int i = 0; i < SCREEN_CONTENT_COUNT; i++) {
        navbar[i] = lv_btn_create(screen_navbar);
        lv_obj_set_height(navbar[i], LV_PCT(100));
        lv_obj_set_flex_grow(navbar[i], 1);
        lv_obj_add_style(navbar[i], &style_base_button, LV_PART_MAIN);
        lv_obj_add_style(navbar[i], &style_base_button_hover, LV_STATE_HOVERED);
        lv_obj_add_style(navbar[i], &style_navbar_button, LV_STATE_CHECKED);

        lv_obj_t * label = lv_label_create(navbar[i]);
        lv_label_set_text(label, label_navbar_button[i]);
        lv_obj_add_event_cb(navbar[i], navbar_event_callback, LV_EVENT_CLICKED, content[i]);
        lv_obj_center(label);
    }
    lv_obj_add_state(navbar[RADIO], LV_STATE_CHECKED);

    navbar_init_flag = true;
}

/******************************************************************************
 * SCREEN COMPONENTS
 ******************************************************************************/

/* Radio content screen *******************************************************/
void content_radio(lv_obj_t * screen_content)
{
    /* Radio content container */
    lv_obj_t * radio = lv_obj_create(screen_content);
    lv_obj_add_style(radio, &style_base, LV_PART_MAIN);
    lv_obj_add_style(radio, &style_content_part, LV_PART_MAIN);
    lv_obj_add_style(radio, &style_content_radio, LV_PART_MAIN);

    /* Channel display part */
    lv_obj_t * widget_channel = lv_obj_create(radio);
    lv_obj_set_grid_cell(widget_channel, LV_GRID_ALIGN_STRETCH, 0, 4, LV_GRID_ALIGN_STRETCH, 0, 4);
    lv_obj_add_style(widget_channel, &style_base_widget, LV_PART_MAIN);
    lv_obj_add_style(widget_channel, &style_widget_channel, LV_PART_MAIN);

    label_bundle.radio_frequency = lv_label_create(widget_channel);
    lv_label_set_long_mode(label_bundle.radio_frequency, LV_LABEL_LONG_MODE_CLIP);
    lv_label_bind_text(label_bundle.radio_frequency, &subject_radio_slider_str, "%s");
    lv_obj_set_style_text_font(label_bundle.radio_frequency, &lv_font_montserrat_48, LV_PART_MAIN);
    lv_obj_set_size(label_bundle.radio_frequency, LV_PCT(100), LV_SIZE_CONTENT);

    label_bundle.radio_station = lv_label_create(widget_channel);
    lv_label_set_long_mode(label_bundle.radio_station, LV_LABEL_LONG_MODE_CLIP);
    lv_label_bind_text(label_bundle.radio_station, &subject_radio_station, "%s");
    lv_obj_set_style_text_font(label_bundle.radio_station, &lv_font_montserrat_32, LV_PART_MAIN);
    lv_obj_set_size(label_bundle.radio_station, LV_PCT(100), LV_SIZE_CONTENT);

    label_bundle.radio_song = lv_label_create(widget_channel);
    lv_label_set_long_mode(label_bundle.radio_song, LV_LABEL_LONG_MODE_CLIP);
    lv_label_bind_text(label_bundle.radio_song, &subject_radio_song, "%s");
    lv_obj_set_style_text_font(label_bundle.radio_song, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_set_style_anim_duration(label_bundle.radio_song, 30000, 0);
    lv_obj_set_size(label_bundle.radio_song, LV_PCT(100), LV_SIZE_CONTENT);

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
    lv_slider_set_value(widget_tuner_slider, 929, LV_ANIM_OFF);

    lv_obj_add_event_cb(widget_tuner_slider, tuner_event_callback, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(widget_tuner, tuner_draw_callback, LV_EVENT_DRAW_MAIN, NULL);

    /* Station buttons */
    lv_obj_t * widget_station = lv_obj_create(radio);
    lv_obj_set_layout(widget_station, LV_LAYOUT_GRID);
    lv_obj_set_grid_cell(widget_station, LV_GRID_ALIGN_STRETCH, 4, 4, LV_GRID_ALIGN_STRETCH, 0, 6);
    lv_obj_add_style(widget_station, &style_base, LV_PART_MAIN);
    lv_obj_add_style(widget_station, &style_widget_station, LV_PART_MAIN);

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 2; col++) {
            int index = row * 2 + col;
            lv_obj_t * button = lv_btn_create(widget_station);
            lv_obj_set_grid_cell(button, LV_GRID_ALIGN_STRETCH, col * 2, 2, LV_GRID_ALIGN_STRETCH, row * 2, 2);
            lv_obj_add_style(button, &style_base_button, LV_PART_MAIN);
            lv_obj_add_style(button, &style_base_button_hover, LV_STATE_HOVERED);
            lv_obj_add_style(button, &style_navbar_button, LV_STATE_CHECKED);
            lv_obj_add_event_cb(button, station_event_callback, LV_EVENT_CLICKED, widget_tuner_slider);

            lv_obj_t * label = lv_label_create(button);
            lv_label_set_text(label, radio_label[index].station);
            lv_obj_center(label);
        }
    }
}

/* Media content screen *******************************************************/
void content_media(lv_obj_t * screen_content)
{
    /* Media content container */
    lv_obj_t * media = lv_obj_create(screen_content);
    lv_obj_add_style(media, &style_base, LV_PART_MAIN);
    lv_obj_add_style(media, &style_content_part, LV_PART_MAIN);
    lv_obj_add_style(media, &style_content_media, LV_PART_MAIN);

    lv_obj_update_layout(media);

    /* Album art widget */
    lv_obj_t * album_art = lv_obj_create(media);
    lv_obj_add_style(album_art, &style_base, LV_PART_MAIN);
    lv_obj_add_style(album_art, &style_base_widget, LV_PART_MAIN);
    lv_obj_add_style(album_art, &style_widget_album_art, LV_PART_MAIN);
    lv_obj_set_size(album_art, lv_obj_get_content_height(media) * 90 / 100, LV_PCT(90));

    /* Display default album art */
    LV_IMAGE_DECLARE(album_art_betamek);
    lv_obj_t * art = lv_image_create(album_art);
    lv_image_set_src(art, &album_art_betamek);
    lv_obj_set_style_opa(art, LV_OPA_30, LV_PART_MAIN);
    lv_obj_set_style_image_recolor(art, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_style_image_recolor_opa(art, LV_OPA_100, LV_PART_MAIN);
    lv_obj_set_align(art, LV_ALIGN_CENTER);

    /* Demo decoration settings */
    lv_obj_remove_flag(album_art, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(album_art, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_x(art, 100);
    lv_obj_set_y(art, 50);
    lv_image_set_scale(art, 100);

    /* Player container */
    lv_obj_t * player = lv_obj_create(media);
    lv_obj_add_style(player, &style_base, LV_PART_MAIN);
    lv_obj_add_style(player, &style_base_widget, LV_PART_MAIN);
    lv_obj_add_style(player, &style_widget_player, LV_PART_MAIN);
    lv_obj_set_flex_grow(player, 1);
    lv_obj_set_flex_align(player, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_height(player, LV_PCT(90));

    /* Player metadata */
    label_bundle.media_song = lv_label_create(player);
    lv_label_set_long_mode(label_bundle.media_song, LV_LABEL_LONG_MODE_CLIP);
    lv_label_set_text(label_bundle.media_song, media_label[2].song);
    lv_obj_set_style_text_font(label_bundle.media_song, &lv_font_montserrat_48, LV_PART_MAIN);
    lv_obj_set_size(label_bundle.media_song, LV_PCT(100), LV_SIZE_CONTENT);

    label_bundle.media_artist = lv_label_create(player);
    lv_label_set_long_mode(label_bundle.media_artist, LV_LABEL_LONG_MODE_CLIP);
    lv_label_set_text(label_bundle.media_artist, media_label[2].artist);
    lv_obj_set_style_text_font(label_bundle.media_artist, &lv_font_montserrat_32, LV_PART_MAIN);
    lv_obj_set_size(label_bundle.media_artist, LV_PCT(100), LV_SIZE_CONTENT);

    /* Player progress bar */
    lv_obj_t * widget_progress = lv_obj_create(player);
    lv_obj_add_style(widget_progress, &style_base, LV_PART_MAIN);
    lv_obj_add_style(widget_progress, &style_base_widget, LV_PART_MAIN);
    lv_obj_add_style(widget_progress, &style_widget_progress, LV_PART_MAIN);
    lv_obj_set_layout(widget_progress, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(widget_progress, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(widget_progress, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_width(widget_progress, LV_PCT(100));

    lv_obj_t * widget_progress_timer_start = lv_label_create(widget_progress);
    lv_label_set_text(widget_progress_timer_start, "00:00");
    lv_obj_set_style_text_font(widget_progress_timer_start, &lv_font_montserrat_16, LV_PART_MAIN);

    lv_obj_t * widget_progress_slider = lv_slider_create(widget_progress);
    lv_slider_set_value(widget_progress_slider, 33, LV_ANIM_OFF);
    lv_obj_set_style_bg_color(widget_progress_slider, lv_color_hex(0x777777), LV_PART_MAIN);
    lv_obj_set_style_bg_color(widget_progress_slider, lv_color_hex(0xffffff), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(widget_progress_slider, lv_color_hex(0xffffff), LV_PART_KNOB);
    lv_obj_set_style_height(widget_progress_slider, 4, LV_PART_MAIN);
    lv_obj_set_flex_grow(widget_progress_slider, 1);

    lv_obj_t * widget_progress_timer_end = lv_label_create(widget_progress);
    lv_label_set_text(widget_progress_timer_end, "00:00");
    lv_obj_set_style_text_font(widget_progress_timer_end, &lv_font_montserrat_16, LV_PART_MAIN);

    /* Player controls container */
    lv_obj_t * widget_control = lv_obj_create(player);
    lv_obj_add_style(widget_control, &style_base, LV_PART_MAIN);
    lv_obj_add_style(widget_control, &style_base_widget, LV_PART_MAIN);
    lv_obj_set_layout(widget_control, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(widget_control, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(widget_control, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_size(widget_control, LV_PCT(100), LV_PCT(30));

    lv_obj_update_layout(widget_control);

    /* Player controls */
    for (int i = 0; i < 5; i++) {
        lv_obj_t * button = lv_btn_create(widget_control);
        lv_obj_set_size(button, lv_obj_get_content_height(widget_control) * widget_control_button[i].size / 100, LV_PCT(widget_control_button[i].size));
        lv_obj_set_style_radius(button, LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_bg_color(button, lv_color_hex(widget_control_button[i].bg_color), 0);
        lv_obj_set_style_text_color(button, lv_color_hex(widget_control_button[i].text_color), 0);
        lv_obj_set_style_text_font(button, &lv_font_montserrat_32, 0);
        lv_obj_set_style_margin_hor(button, lv_obj_get_content_height(widget_control) * 20 / 100, 0);
        lv_obj_add_style(button, &style_base_button, LV_PART_MAIN);

        lv_obj_t * label = lv_label_create(button);
        lv_label_set_text(label, widget_control_button[i].label);
        lv_obj_center(label);
    }
}

/* Phone content screen *******************************************************/
void content_phone(lv_obj_t * screen_content)
{
    /* Phone content container */
    lv_obj_t * phone = lv_obj_create(screen_content);
    lv_obj_add_style(phone, &style_base, LV_PART_MAIN);
    lv_obj_add_style(phone, &style_content_part, LV_PART_MAIN);
    lv_obj_add_style(phone, &style_content_phone, LV_PART_MAIN);

    /* Phone keypad */
    lv_obj_t * keypad = lv_buttonmatrix_create(phone);
    lv_obj_add_style(keypad, &style_base, LV_PART_MAIN);
    lv_obj_add_style(keypad, &style_base_widget, LV_PART_MAIN);
    lv_obj_add_style(keypad, &style_widget_keypad, LV_PART_MAIN);
    lv_obj_add_style(keypad, &style_base_button, LV_PART_ITEMS);
    lv_obj_set_style_text_font(keypad, &lv_font_montserrat_48, LV_PART_ITEMS);
    lv_obj_set_style_text_color(keypad, lv_color_hex(0xffffff), LV_PART_ITEMS);
    lv_obj_set_height(keypad, LV_PCT(90));
    lv_obj_set_flex_grow(keypad, 1);
    static const char * map[] = {
        "1", "2", "3", "\n",
        "4", "5", "6", "\n",
        "7", "8", "9", "\n",
        LV_SYMBOL_BACKSPACE,
        "0", LV_SYMBOL_CALL,
        NULL
    };
    lv_buttonmatrix_set_map(keypad, map);

    /* Contact list container */
    lv_obj_t * contact = lv_obj_create(phone);
    lv_obj_add_style(contact, &style_base, LV_PART_MAIN);
    lv_obj_add_style(contact, &style_base_widget, LV_PART_MAIN);
    lv_obj_add_style(contact, &style_widget_contact, LV_PART_MAIN);
    lv_obj_set_height(contact, LV_PCT(90));
    lv_obj_set_flex_grow(contact, 1);

    /* Contact list input field */
    lv_obj_t * input = lv_textarea_create(contact);
    lv_obj_set_grid_cell(input, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_add_style(input, &style_base_button, LV_PART_MAIN);
    lv_obj_add_style(input, &style_widget_input, LV_PART_MAIN);
    lv_obj_set_style_text_font(input, &lv_font_montserrat_48, LV_PART_MAIN);
    lv_obj_set_style_text_color(input, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_flex_grow(input, 1);
    lv_obj_set_scrollbar_mode(input, LV_SCROLLBAR_MODE_OFF);

    lv_textarea_set_one_line(input, true);
    lv_obj_add_event_cb(keypad, keypad_event_callback, LV_EVENT_VALUE_CHANGED, input);

    /* Contact list container */
    /* NOTE: Hardcoded for now, an interface to real data will be implemented in a future file. */
    lv_obj_t * list = lv_obj_create(contact);
    lv_obj_set_grid_cell(list, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 3);
    lv_obj_add_style(list, &style_base, LV_PART_MAIN);
    lv_obj_add_style(list, &style_widget_list, LV_PART_MAIN);
    contact_list(list, input);
}

/* Settings content screen ****************************************************/
void content_settings(lv_obj_t * screen_content)
{
    lv_obj_t * settings = lv_obj_create(screen_content);
    lv_obj_add_style(settings, &style_base, LV_PART_MAIN);
    lv_obj_add_style(settings, &style_content_part, LV_PART_MAIN);

    lv_obj_t * label = lv_label_create(settings);
    lv_label_set_text(label, "Settings");
}

/******************************************************************************
 * MISCELLANEOUS HELPER COMPONENTS
 ******************************************************************************/

/* Contact cards */
static void contact_list(lv_obj_t * list, lv_obj_t * input)
{
    lv_obj_t * card = lv_obj_create(list);
    lv_obj_set_size(card, LV_PCT(100), lv_obj_get_height(input));
    lv_obj_add_style(card, &style_base_button, LV_PART_MAIN);
    lv_obj_set_style_text_font(card, &lv_font_montserrat_32, LV_PART_MAIN);
    lv_obj_set_style_text_color(card, lv_color_hex(0xffffff), LV_PART_MAIN);
}
