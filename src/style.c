/** PROJECT MINT
 *  @file   style.c
 *  @author G.C. Looa
 *  @date   2026-06-08
 *  @brief  Style definitions for the LVGL simulator.
 */

#include "lvgl/lvgl.h"
#include "style.h"
#include "color.h"

static const int32_t grid_col_dsc_array_0[] = {
    LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1),
    LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1),
    LV_GRID_TEMPLATE_LAST
};
static const int32_t grid_row_dsc_array_0[] = {
    LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1),
    LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1),
    LV_GRID_TEMPLATE_LAST
};

lv_style_t style_base;
lv_style_t style_base_button;
lv_style_t style_base_button_hover;
lv_style_t style_base_widget;

lv_style_t style_statusbar;
lv_style_t style_statusbar_part;

lv_style_t style_content;
lv_style_t style_content_part;
lv_style_t style_content_radio;
lv_style_t style_content_media;
lv_style_t style_content_phone;
lv_style_t style_content_settings;

lv_style_t style_navbar;
lv_style_t style_navbar_button;

lv_style_t style_widget_channel;
lv_style_t style_widget_channel_button;

lv_style_t style_widget_tuner;

lv_style_t style_widget_album_art;

static bool style_init_flag = false;

void style_init(void)
{
    if(style_init_flag) return;
    color_init();

    /**
     * BASE STYLES
     */
    lv_style_init(&style_base);
    lv_style_set_text_font(&style_base, &lv_font_montserrat_24);
    lv_style_set_text_color(&style_base, lv_color_make(255, 255, 255));
    lv_style_set_bg_color(&style_base, lv_color_make(0, 0, 0));
    lv_style_set_border_width(&style_base, 0);
    lv_style_set_radius(&style_base, 0);
    lv_style_set_pad_all(&style_base, 0);
    lv_style_set_pad_row(&style_base, 0);
    lv_style_set_pad_column(&style_base, 0);

    lv_style_init(&style_base_button);
    lv_style_set_bg_color(&style_base_button, lv_color_hex(0x1a1a1a));
    lv_style_set_border_width(&style_base_button, 0);
    lv_style_set_radius(&style_base_button, 20);
    lv_style_set_shadow_color(&style_base_button, lv_color_hex(0x000000));
    lv_style_set_shadow_width(&style_base_button, 0);
    lv_style_set_shadow_offset_y(&style_base_button, 0);
    lv_style_set_shadow_opa(&style_base_button, 0);

    lv_style_init(&style_base_button_hover);
    lv_style_set_border_width(&style_base_button_hover, 5);
    lv_style_set_border_color(&style_base_button_hover, lv_color_make(255, 255, 255));

    lv_style_init(&style_base_widget);
    lv_style_set_bg_color(&style_base_widget, lv_color_hex(0x1a1a1a));
    lv_style_set_border_width(&style_base_widget, 0);
    lv_style_set_radius(&style_base_widget, 20);

    /**
     * STATUS BAR STYLES
     */
    lv_style_init(&style_statusbar);
    lv_style_set_bg_grad(&style_statusbar, &grad_dsc_statusbar);
    lv_style_set_size(&style_statusbar, LV_PCT(100), LV_PCT(SCREEN_STATUSBAR_PCT));
    lv_style_set_layout(&style_statusbar, LV_LAYOUT_FLEX);

    lv_style_init(&style_statusbar_part);
    lv_style_set_bg_opa(&style_statusbar_part, LV_OPA_0);
    lv_style_set_pad_all(&style_statusbar_part, 20);
    lv_style_set_pad_row(&style_statusbar_part, 10);
    lv_style_set_pad_column(&style_statusbar_part, 10);

    /**
     * CONTENT STYLES
     */
    lv_style_init(&style_content);
    lv_style_set_bg_grad(&style_content, &grad_dsc_content);
    lv_style_set_size(&style_content, LV_PCT(100), LV_PCT(SCREEN_CONTENT_PCT));
    lv_style_set_layout(&style_content, LV_LAYOUT_FLEX);

    lv_style_init(&style_content_part);
    lv_style_set_bg_opa(&style_content_part, LV_OPA_0);
    lv_style_set_size(&style_content_part, LV_PCT(100), LV_PCT(100));
    lv_style_set_pad_top(&style_content_part, 10);
    lv_style_set_pad_left(&style_content_part, 10);
    lv_style_set_pad_right(&style_content_part, 10);
    lv_style_set_pad_row(&style_content_part, 10);
    lv_style_set_pad_column(&style_content_part, 10);

    lv_style_init(&style_content_radio);
    lv_style_set_layout(&style_content_radio, LV_LAYOUT_GRID);
    lv_style_set_grid_column_dsc_array(&style_content_radio, grid_col_dsc_array_0);
    lv_style_set_grid_row_dsc_array(&style_content_radio, grid_row_dsc_array_0);

    lv_style_init(&style_content_media);
    lv_style_set_layout(&style_content_media, LV_LAYOUT_FLEX);
    lv_style_set_flex_flow(&style_content_media, LV_FLEX_FLOW_ROW);

    lv_style_init(&style_content_phone);
    lv_style_init(&style_content_settings);

    /**
     * NAVIGATION BAR STYLES
     */
    lv_style_init(&style_navbar);
    lv_style_set_size(&style_navbar, LV_PCT(100), LV_PCT(SCREEN_NAVBAR_PCT));
    lv_style_set_layout(&style_navbar, LV_LAYOUT_FLEX);
    lv_style_set_flex_flow(&style_navbar, LV_FLEX_FLOW_ROW);
    lv_style_set_pad_all(&style_navbar, 10);
    lv_style_set_pad_row(&style_navbar, 10);
    lv_style_set_pad_column(&style_navbar, 10);

    lv_style_init(&style_navbar_button);
    lv_style_set_text_color(&style_navbar_button, lv_color_make(0, 0, 0));
    lv_style_set_bg_grad(&style_navbar_button, &grad_dsc_button);

    /**
     * RADIO WIDGET STYLES
     */
    lv_style_init(&style_widget_channel);
    lv_style_set_bg_grad(&style_widget_channel, &grad_dsc_sky_blue);
    lv_style_set_layout(&style_widget_channel, LV_LAYOUT_FLEX);
    lv_style_set_flex_flow(&style_widget_channel, LV_FLEX_FLOW_COLUMN);
    lv_style_set_pad_row(&style_widget_channel, 10);
    lv_style_set_pad_column(&style_widget_channel, 10);
    lv_style_set_pad_all(&style_widget_channel, 20);

    lv_style_init(&style_widget_channel_button);
    lv_style_set_bg_grad(&style_widget_channel_button, &grad_dsc_off_white);

    lv_style_init(&style_widget_tuner);
    lv_style_set_bg_opa(&style_widget_tuner, LV_OPA_0);
    lv_style_set_pad_all(&style_widget_tuner, 20);

    /**
     * MEDIA WIDGET STYLES
     */
    lv_style_init(&style_widget_album_art);
    lv_style_set_bg_grad(&style_widget_album_art, &grad_dsc_sky_blue);

    style_init_flag = true;
}
