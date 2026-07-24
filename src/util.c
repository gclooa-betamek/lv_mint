/** PROJECT MINT
 *  @file   util.c
 *  @author G.C. Looa
 *  @date   2026-06-25
 *  @brief  Utility functions for the LVGL simulator.
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "lvgl/lvgl.h"
#include "component.h"
#include "util.h"

/* Dummy Radio */
const radio_label_t radio_label[] = {
    {881, "EIGHT FM", "Yukopi - Kyoufuu All Back"},
    {893, "Ai FM", "Gorillaz - Tomorrow Comes Today"},
    {929, "Hitz", "Nightcord at 25:00 - Bad Apple!! feat.SEKAI (Hatsune Miku, Yoisaki Kanade, Asahina Mafuyu, Shinonome Ena, Akiyama Mizuki)"},
    {945, "Mix", "Moe Shop - Notice (feat. TORIENA)"},
    {958, "Fly FM", "Have a Nice Life - Bloodhail"},
    {1033, "Era", "Nine Inch Nails - Head Like A Hole"}
};

/* Dummy media */
const media_label_t media_label[] = {
    {"Ruler of Everything", "Tally Hall"},
    {"Dreams (2004 Remaster)", "Fleetwood Mac"},
    {"Can I friend you on Bassbook? lol", "Nanahira & Camellia"}
};

/* Dummy contacts */
const contact_info_t contact_info[] = {
    {"Michael",   "0123456789"},
    {"Gabriel",   "0123336666"},
    {"Raphael",   "0125557777"},
    {"Donatello", "0106668888"},
    {"Leonardo",  "0169996666"},
    {"Adam",      "0196669999"},
    {"Lily",      "0101110000"},
};
const size_t contact_info_count = sizeof(contact_info) / sizeof(contact_info[0]);

const settings_label_t settings_label[] = {
    {LV_SYMBOL_VOLUME_MID, "Audio", "Volume and equalizer"},
    {LV_SYMBOL_IMAGE, "Display", "Brightness and theme"},
    {LV_SYMBOL_BLUETOOTH, "Connectivity", "Bluetooth and devices"},
    {LV_SYMBOL_SETTINGS, "System", "Language, date and time"},
    {LV_SYMBOL_DIRECTORY, "Placeholder", ""},
    {LV_SYMBOL_DIRECTORY, "Placeholder", ""},
    {LV_SYMBOL_DIRECTORY, "Placeholder", ""},
    {LV_SYMBOL_DIRECTORY, "Placeholder", ""},
};
const size_t settings_label_count = sizeof(settings_label) / sizeof(settings_label[0]);


lv_subject_t subject_screen_content;
lv_subject_t subject_radio_slider_int;
lv_subject_t subject_radio_slider_str;
lv_subject_t subject_radio_station;
lv_subject_t subject_radio_song;
lv_subject_t subject_media_song;
lv_subject_t subject_media_artist;

char subject_radio_slider_buffer[8];
char subject_radio_station_buffer[16];
char subject_radio_song_buffer[256];
char subject_media_song_buffer[256];
char subject_media_artist_buffer[128];

static lv_timer_t * radio_timer = NULL;
static lv_timer_t * scroll_timer = NULL;

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

void radio_timer_callback(lv_timer_t * timer)
{
    printf("DEBUG: radio_timer_callback() fired.\n");
    int slider_value = lv_slider_get_value(lv_timer_get_user_data(timer));

    snprintf(subject_radio_station_buffer, sizeof(subject_radio_station_buffer), "%s", "Unknown station");
    snprintf(subject_radio_song_buffer, sizeof(subject_radio_song_buffer), "%s", " ");
    lv_subject_copy_string(&subject_radio_station, subject_radio_station_buffer);
    lv_subject_copy_string(&subject_radio_song, subject_radio_song_buffer);

    for (int i = 0; i < sizeof(radio_label) / sizeof(radio_label[0]); i++) {
        // printf("DEBUG: %d Slider value = %d\n", i, slider_value);
        // printf("DEBUG: %d Index value  = %d\n", i, radio_label[i].frequency);
        if (slider_value == radio_label[i].frequency) {
            snprintf(subject_radio_station_buffer, sizeof(subject_radio_station_buffer), "%s", radio_label[i].station);
            snprintf(subject_radio_song_buffer, sizeof(subject_radio_song_buffer), "%s", radio_label[i].song);
            lv_subject_copy_string(&subject_radio_station, subject_radio_station_buffer);
            lv_subject_copy_string(&subject_radio_song, subject_radio_song_buffer);
            break;
        }
    }

    lv_timer_delete(radio_timer);
    radio_timer = NULL;
}

void scroll_timer_callback(lv_timer_t * timer)
{
    printf("DEBUG: scroll_timer_callback() fired.\n");
    label_bundle_t * label_bundle = lv_timer_get_user_data(timer);
    util_set_long_mode(label_bundle, LV_LABEL_LONG_MODE_SCROLL_CIRCULAR);

    lv_timer_delete(scroll_timer);
    scroll_timer = NULL;
}

void screen_observer_callback(lv_observer_t * observer, lv_subject_t * subject)
{
    printf("DEBUG: screen_observer_callback() fired.\n");
    if (scroll_timer) {
        lv_timer_delete(scroll_timer);
        scroll_timer = NULL;
    }
    label_bundle_t * label_bundle = lv_observer_get_user_data(observer);
    util_set_long_mode(label_bundle, LV_LABEL_LONG_MODE_CLIP);

    scroll_timer = lv_timer_create(scroll_timer_callback, 1000, label_bundle);
}

void navbar_event_callback(lv_event_t * e)
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

    /* Update subject with selected screen. */
    lv_subject_set_int(&subject_screen_content, lv_obj_get_index(content));
}

void tuner_event_callback(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code != LV_EVENT_VALUE_CHANGED) return;

    if (radio_timer) {
        lv_timer_delete(radio_timer);
        radio_timer = NULL;
    }

    snprintf(subject_radio_station_buffer, sizeof(subject_radio_station_buffer), "%s", "Seeking...");
    snprintf(subject_radio_song_buffer, sizeof(subject_radio_song_buffer), "%s", " ");
    lv_subject_copy_string(&subject_radio_station, subject_radio_station_buffer);
    lv_subject_copy_string(&subject_radio_song, subject_radio_song_buffer);

    lv_obj_t * slider = lv_event_get_target(e);
    int slider_value = lv_slider_get_value(slider);
    char slider_value_buffer[8];
    snprintf(slider_value_buffer, sizeof(slider_value_buffer), "%d.%d", slider_value / 10, slider_value % 10);
    lv_subject_copy_string(&subject_radio_slider_str, slider_value_buffer);
    lv_subject_set_int(&subject_radio_slider_int, slider_value);

    // printf("DEBUG: Slider value  = %s\n", slider_value_buffer);
    // printf("DEBUG: Subject value = %s\n", lv_subject_get_string(&subject_radio_slider_str));

    radio_timer = lv_timer_create(radio_timer_callback, 1000, slider);
}

void tuner_draw_callback(lv_event_t * e)
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
        if(major) marker.color = lv_color_hex(0x2b2b2b);
        else marker.color = lv_color_hex(0x1a1a1a);

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
    cursor.color = lv_color_hex(0xff0000);

    lv_draw_line(layer, &cursor);

    // printf("DEBUG: widget_tuner_draw_callback() fired.\n");
}

void station_event_callback(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code != LV_EVENT_CLICKED) return;

    lv_obj_t * station = lv_event_get_target(e);
    lv_obj_t * slider = lv_event_get_user_data(e);

    int index = lv_obj_get_index(station);
    lv_slider_set_value(slider, radio_label[index].frequency, LV_ANIM_ON);

    int slider_value = lv_slider_get_value(slider);
    char slider_value_buffer[8];
    snprintf(slider_value_buffer, sizeof(slider_value_buffer), "%d.%d", slider_value / 10, slider_value % 10);
    lv_subject_copy_string(&subject_radio_slider_str, slider_value_buffer);
    lv_subject_set_int(&subject_radio_slider_int, slider_value);

    snprintf(subject_radio_station_buffer, sizeof(subject_radio_station_buffer), "%s", radio_label[index].station);
    snprintf(subject_radio_song_buffer, sizeof(subject_radio_song_buffer), "%s", radio_label[index].song);
    lv_subject_copy_string(&subject_radio_station, subject_radio_station_buffer);
    lv_subject_copy_string(&subject_radio_song, subject_radio_song_buffer);
}

void keypad_event_callback(lv_event_t * e)
{
    lv_obj_t * key = lv_event_get_target_obj(e);
    lv_obj_t * textarea = (lv_obj_t *)lv_event_get_user_data(e);
    const char * text = lv_buttonmatrix_get_button_text(key, lv_buttonmatrix_get_selected_button(key));
    if (lv_strcmp(text, LV_SYMBOL_BACKSPACE) == 0) lv_textarea_delete_char(textarea);
    else if (lv_strcmp(text, LV_SYMBOL_CALL) == 0) lv_obj_send_event(textarea, LV_EVENT_READY, NULL);
    else lv_textarea_add_text(textarea, text);
}

void search_event_callback(lv_event_t * e)
{
    lv_obj_t * textarea = lv_event_get_target(e);
    lv_obj_t * list = lv_event_get_user_data(e);
    const char * search = lv_textarea_get_text(textarea);

    lv_obj_scroll_to(list, 0, 0, LV_ANIM_OFF);

    for (size_t i = 0; i < contact_info_count; i++) {
        lv_obj_t * card = lv_obj_get_child(list, i);
        if (strstr(contact_info[i].number, search))
            lv_obj_set_flag(card, LV_OBJ_FLAG_HIDDEN, false);
        else
            lv_obj_set_flag(card, LV_OBJ_FLAG_HIDDEN, true);
    }
}

void contact_event_callback(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code != LV_EVENT_CLICKED) return;

    lv_obj_t * card = lv_event_get_target(e);
    lv_obj_t * textarea = (lv_obj_t *)lv_event_get_user_data(e);
    const char * number = contact_info[lv_obj_get_index(card)].number;

    printf("DEBUG: Contact selected: %s\n", number);
    lv_textarea_set_text(textarea, "");
    lv_textarea_add_text(textarea, number);
}

void util_set_long_mode(label_bundle_t * label_bundle, lv_label_long_mode_t long_mode)
{
    lv_label_set_long_mode(label_bundle->radio_frequency, long_mode);
    lv_label_set_long_mode(label_bundle->radio_station, long_mode);
    lv_label_set_long_mode(label_bundle->radio_song, long_mode);
    lv_label_set_long_mode(label_bundle->media_song, long_mode);
    lv_label_set_long_mode(label_bundle->media_artist, long_mode);
}
