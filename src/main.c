/** PROJECT MINT
 *  @file   main.c
 *  @author G.C. Looa
 *  @date   2026-04-20
 *  @brief  Main driver function for the LVGL simulator.
 */

/*********************
 *      INCLUDES
 *********************/

#ifndef _DEFAULT_SOURCE
  #define _DEFAULT_SOURCE /* needed for usleep() */
#endif

#include <stdlib.h>
#include <stdio.h>

/** G.C. Looa 15/06/2026
 * The following block of code provided by the original LVGL simulator project was
 * meant to provide a cross-platform way to determine which sleep function to use.
 * However, since we need to include Windows.h for SetProcessDPIAware(), we will
 * include Windows.h regardless of compiler, and ignore the preprocessing.

#ifdef _MSC_VER
  #include <Windows.h>
#else
  #include <unistd.h>
  #include <pthread.h>
#endif

 */
#include <Windows.h>
#include <SDL.h>

#include "lvgl/lvgl.h"
#include "hal/hal.h"
#include "style.h"
#include "component.h"

/*********************
 *      DEFINES
 *********************/

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

#define SCREEN_STATUSBAR_PCT 10
#define SCREEN_CONTENT_PCT   70
#define SCREEN_NAVBAR_PCT    20

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

#if LV_USE_OS != LV_OS_FREERTOS

int main(int argc, char **argv)
{
  (void)argc; /*Unused*/
  (void)argv; /*Unused*/

  /* Set DPI awareness to ignore Windows scale setting */
  SetProcessDPIAware();

  /*Initialize LVGL*/
  lv_init();

  /*Initialize the HAL (display, input devices, tick) for LVGL*/
  sdl_hal_init(SCREEN_WIDTH, SCREEN_HEIGHT);

  /*Initialize styles*/
  style_init();

  /* Hide system cursor */
  // SDL_ShowCursor(SDL_DISABLE);

  /* Initialize screen */
  lv_obj_t * screen = lv_obj_create(NULL);
  lv_obj_set_layout(screen, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
  lv_obj_add_style(screen, &style_base, LV_PART_MAIN);
  lv_screen_load(screen);

  /* Create screen partitions */
  lv_obj_t * screen_statusbar = lv_obj_create(screen);
  lv_obj_set_layout(screen_statusbar, LV_LAYOUT_FLEX);
  lv_obj_set_size(screen_statusbar, SCREEN_WIDTH, LV_PCT(SCREEN_STATUSBAR_PCT));
  lv_obj_add_style(screen_statusbar, &style_base, LV_PART_MAIN);
  lv_obj_add_style(screen_statusbar, &style_statusbar, LV_PART_MAIN);

  lv_obj_t * screen_content = lv_obj_create(screen);
  lv_obj_set_layout(screen_content, LV_LAYOUT_FLEX);
  lv_obj_set_size(screen_content, SCREEN_WIDTH, LV_PCT(SCREEN_CONTENT_PCT));
  lv_obj_add_style(screen_content, &style_base, LV_PART_MAIN);
  lv_obj_add_style(screen_content, &style_content, LV_PART_MAIN);

  lv_obj_t * screen_navbar = lv_obj_create(screen);
  lv_obj_set_layout(screen_navbar, LV_LAYOUT_FLEX);
  lv_obj_set_size(screen_navbar, SCREEN_WIDTH, LV_PCT(SCREEN_NAVBAR_PCT));
  lv_obj_add_style(screen_navbar, &style_base, LV_PART_MAIN);
  lv_obj_add_style(screen_navbar, &style_navbar, LV_PART_MAIN);
  lv_obj_set_layout(screen_navbar, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(screen_navbar, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(screen_navbar, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  /* Create statusbar */
  statusbar_init(screen_statusbar);

  /* Create navbar buttons */
  navbar_init(screen_navbar);

  while(1) {
    /* Periodically call the lv_task handler.
     * It could be done in a timer interrupt or an OS task too.*/
    uint32_t sleep_time_ms = lv_timer_handler();
    if(sleep_time_ms == LV_NO_TIMER_READY){
	    sleep_time_ms =  LV_DEF_REFR_PERIOD;
    }
/**
 * Same as the previous note on line 19, we will use Sleep()
 * regardless of compiler, and ignore the preprocessing.

#ifdef _MSC_VER
    Sleep(sleep_time_ms);
#else
    usleep(sleep_time_ms * 1000);
#endif

 */
    Sleep(sleep_time_ms);
  }

  return 0;
}


#endif

/**********************
 *   STATIC FUNCTIONS
 **********************/
