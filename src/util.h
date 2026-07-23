/** PROJECT MINT
 *  @file   util.h
 *  @author G.C. Looa
 *  @date   2026-06-25
 *  @brief  Header file for utility functions.
 */

#ifndef UTIL_H
#define UTIL_H

typedef struct {
    int32_t      frequency;
    const char * station;
    const char * song;
} radio_label_t;

typedef struct {
    const char * song;
    const char * artist;
} media_label_t;

typedef struct {
    /* NOTE: Icon to be added in future implementation. */
    const char * name;
    const char * number;
} contact_info_t;

extern const radio_label_t radio_label[];
extern const media_label_t media_label[];
extern const contact_info_t contact_info[];
extern const size_t contact_info_count;

extern lv_subject_t subject_screen_content;
extern lv_subject_t subject_radio_slider_int;
extern lv_subject_t subject_radio_slider_str;
extern lv_subject_t subject_radio_station;
extern lv_subject_t subject_radio_song;
extern lv_subject_t subject_media_song;
extern lv_subject_t subject_media_artist;

extern char subject_radio_slider_buffer[8];
extern char subject_radio_station_buffer[16];
extern char subject_radio_song_buffer[256];
extern char subject_media_song_buffer[256];
extern char subject_media_artist_buffer[128];

void clock_timer_callback(lv_timer_t * timer);
void radio_timer_callback(lv_timer_t * timer);
void scroll_timer_callback(lv_timer_t * timer);

void screen_observer_callback(lv_observer_t * observer, lv_subject_t * subject);
void navbar_event_callback(lv_event_t * e);
void tuner_event_callback(lv_event_t * e);
void tuner_draw_callback(lv_event_t * e);
void station_event_callback(lv_event_t * e);
void keypad_event_callback(lv_event_t * e);
void search_event_callback(lv_event_t * e);
void contact_event_callback(lv_event_t * e);

void util_set_long_mode(label_bundle_t * label_bundle, lv_label_long_mode_t long_mode);

#endif
