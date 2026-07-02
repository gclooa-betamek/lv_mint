/** PROJECT MINT
 *  @file   color.c
 *  @author G.C. Looa
 *  @date   2026-06-19
 *  @brief  Color definitions for the LVGL simulator.
 */

#include "lvgl/lvgl.h"
#include "color.h"

/* Background gradient default */
static const lv_color_t grad_colors_bg[2] = {
    LV_COLOR_MAKE(0x0a, 0x0a, 0x0a),
    LV_COLOR_MAKE(0x20, 0x20, 0x20),
};
/* Button gradient default */
static const lv_color_t grad_colors_button[2] = {
    LV_COLOR_MAKE(0x68, 0xed, 0x89),
    LV_COLOR_MAKE(0x70, 0xd2, 0xdb),
};
/* Sky blue gradient */
static const lv_color_t grad_colors_sky_blue[2] = {
    LV_COLOR_MAKE(0x62, 0xcf, 0xf4),
    LV_COLOR_MAKE(0x2c, 0x67, 0xf2),
};

static const lv_opa_t grad_opa[2] = {
    LV_OPA_100,
    LV_OPA_100,
};

static const uint8_t grad_frac_statusbar[2] = {
    50 * 255 / 100,
    100 * 255 / 100,
};
static const uint8_t grad_frac_content[2] = {
    95 * 255 / 100,
    100 * 255 / 100,
};
static const uint8_t grad_frac_button[2] = {
    75 * 255 / 100,
    100 * 255 / 100,
};

lv_grad_dsc_t grad_dsc_statusbar;
lv_grad_dsc_t grad_dsc_content;
lv_grad_dsc_t grad_dsc_button;
lv_grad_dsc_t grad_dsc_sky_blue;

static bool color_init_flag = false;

void color_init(void)
{
    if(color_init_flag) return;

    lv_grad_init_stops(&grad_dsc_statusbar, grad_colors_bg, grad_opa, grad_frac_statusbar, sizeof(grad_colors_bg) / sizeof(lv_color_t));
    lv_grad_vertical_init(&grad_dsc_statusbar);

    lv_grad_init_stops(&grad_dsc_content, grad_colors_bg, grad_opa, grad_frac_content, sizeof(grad_colors_bg) / sizeof(lv_color_t));
    lv_grad_vertical_init(&grad_dsc_content);

    lv_grad_init_stops(&grad_dsc_button, grad_colors_button, grad_opa, grad_frac_button, sizeof(grad_colors_button) / sizeof(lv_color_t));
    lv_grad_vertical_init(&grad_dsc_button);

    lv_grad_init_stops(&grad_dsc_sky_blue, grad_colors_sky_blue, grad_opa, NULL, sizeof(grad_colors_sky_blue) / sizeof(lv_color_t));
    lv_grad_vertical_init(&grad_dsc_sky_blue);

    color_init_flag = true;
}
