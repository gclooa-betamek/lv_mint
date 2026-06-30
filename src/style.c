/** PROJECT MINT
 *  @file   style.c
 *  @author G.C. Looa
 *  @date   2026-06-08
 *  @brief  Style definitions for the LVGL simulator.
 */

#include "lvgl/lvgl.h"
#include "style.h"
#include "color.h"

lv_style_t style_base;
lv_style_t style_base_button;
lv_style_t style_statusbar;
lv_style_t style_statusbar_part;
lv_style_t style_content;
lv_style_t style_navbar;
lv_style_t style_navbar_button;
lv_style_t style_navbar_button_hover;

static bool style_init_flag = false;

void style_init(void)
{
    if(style_init_flag) return;
    color_init();

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

    lv_style_init(&style_statusbar);
    lv_style_set_bg_grad(&style_statusbar, &grad_dsc_statusbar);

    lv_style_init(&style_statusbar_part);
    lv_style_set_bg_opa(&style_statusbar_part, LV_OPA_0);
    lv_style_set_pad_all(&style_statusbar_part, 20);
    lv_style_set_pad_row(&style_statusbar_part, 10);
    lv_style_set_pad_column(&style_statusbar_part, 10);

    lv_style_init(&style_content);
    lv_style_set_bg_grad(&style_content, &grad_dsc_content);

    lv_style_init(&style_navbar);
    lv_style_set_pad_all(&style_navbar, 10);
    lv_style_set_pad_row(&style_navbar, 10);
    lv_style_set_pad_column(&style_navbar, 10);

    lv_style_init(&style_navbar_button);
    lv_style_set_text_color(&style_navbar_button, lv_color_make(0, 0, 0));
    lv_style_set_bg_grad(&style_navbar_button, &grad_dsc_button);

    lv_style_init(&style_navbar_button_hover);
    lv_style_set_border_width(&style_navbar_button_hover, 5);
    lv_style_set_border_color(&style_navbar_button_hover, lv_color_make(255, 255, 255));

    style_init_flag = true;
}
