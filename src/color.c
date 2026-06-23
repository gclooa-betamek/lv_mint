/** PROJECT MINT
 *  @file   color.c
 *  @author G.C. Looa
 *  @date   2026-06-19
 *  @brief  Color definitions for the LVGL simulator.
 */

#include "lvgl/lvgl.h"
#include "color.h"

static const lv_color_t grad_colors[2] = {
    LV_COLOR_MAKE(0x0a, 0x0a, 0x0a),
    LV_COLOR_MAKE(0x1a, 0x1a, 0x1a),
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

lv_grad_dsc_t grad_dsc_statusbar;
lv_grad_dsc_t grad_dsc_content;

static bool color_init_flag = false;

void color_init(void)
{
    if(color_init_flag) return;

    lv_grad_init_stops(&grad_dsc_statusbar, grad_colors, grad_opa, grad_frac_statusbar, sizeof(grad_colors) / sizeof(lv_color_t));
    lv_grad_vertical_init(&grad_dsc_statusbar);

    lv_grad_init_stops(&grad_dsc_content, grad_colors, grad_opa, grad_frac_content, sizeof(grad_colors) / sizeof(lv_color_t));
    lv_grad_vertical_init(&grad_dsc_content);

    color_init_flag = true;
}
