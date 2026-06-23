/** PROJECT MINT
 *  @file   component.c
 *  @author G.C. Looa
 *  @date   2026-06-22
 *  @brief  Component definitions for the LVGL simulator.
 */

#include "lvgl/lvgl.h"
#include "component.h"

static bool component_init_flag = false;

void component_init(void)
{
    if (component_init_flag) return;

    component_init_flag = true;
}
