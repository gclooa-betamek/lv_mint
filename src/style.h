/** PROJECT MINT
 *  @file   style.h
 *  @author G.C. Looa
 *  @date   2026-06-08
 *  @brief  Header file for style definitions.
 */

#ifndef STYLE_H
#define STYLE_H

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

#define SCREEN_STATUSBAR_PCT 10
#define SCREEN_CONTENT_PCT   70
#define SCREEN_NAVBAR_PCT    20

extern lv_style_t style_base;
extern lv_style_t style_base_button;
extern lv_style_t style_base_button_hover;
extern lv_style_t style_base_widget;

extern lv_style_t style_statusbar;
extern lv_style_t style_statusbar_part;

extern lv_style_t style_content;
extern lv_style_t style_content_part;

extern lv_style_t style_navbar;
extern lv_style_t style_navbar_button;

extern lv_style_t style_widget_channel;
extern lv_style_t style_widget_channel_button;

void style_init(void);

#endif
