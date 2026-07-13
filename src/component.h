/** PROJECT MINT
 *  @file   component.h
 *  @author G.C. Looa
 *  @date   2026-06-22
 *  @brief  Header file for component definitions.
 */

#ifndef COMPONENT_H
#define COMPONENT_H

#define STATUSBAR_PART_COUNT 3
#define SCREEN_CONTENT_COUNT 4

typedef enum {
    STATUSBAR,
    CONTENT,
    NAVBAR
} screen_part_t;

typedef enum {
    WEATHER,
    CLOCK,
    SYMBOL
} statusbar_part_t;

typedef enum {
    RADIO,
    MEDIA,
    PHONE,
    SETTINGS
} screen_content_t;

typedef enum {
    SHUFFLE,
    PREV,
    PLAY,
    NEXT,
    QUEUE
} widget_control_index_t;

typedef struct {
    int32_t size;
    uint32_t bg_color;
    uint32_t text_color;
    const char * label;
} widget_control_t;

void statusbar_init(lv_obj_t * screen_statusbar);
void content_init(lv_obj_t * screen_content);
void navbar_init(lv_obj_t * screen_navbar);

void content_radio(lv_obj_t * screen_content);
void content_media(lv_obj_t * screen_content);
void content_phone(lv_obj_t * screen_content);
void content_settings(lv_obj_t * screen_content);

#endif
