/** PROJECT MINT
 *  @file   util.h
 *  @author G.C. Looa
 *  @date   2026-06-25
 *  @brief  Header file for utility functions.
 */

#ifndef UTIL_H
#define UTIL_H

extern lv_subject_t subject_radio_slider;
extern char subject_radio_slider_buffer[8];

void clock_timer_callback(lv_timer_t * timer);
void button_event_callback(lv_event_t * e);
void widget_tuner_event_callback(lv_event_t * e);
void widget_tuner_draw_callback(lv_event_t * e);
void phone_keypad_event_callback(lv_event_t * e);

#endif
