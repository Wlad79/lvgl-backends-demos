/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 PHYTEC Messtechnik GmbH
 */

#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/wayland/wayland.h"
//#include "lv_drivers/x11/x11.h"
#include <unistd.h>

#if USE_SDL
// https://forum.lvgl.io/t/how-to-use-sdl-dirver/10201/4
#include <lvgl.h>
#include <sdl/sdl.h>

bool hal_init(void)
{
    sdl_init();

    /*Create a display buffer*/
    static lv_disp_draw_buf_t disp_buf1;

    void * buf1 = lv_mem_alloc(SDL_HOR_RES * SDL_VER_RES * sizeof(lv_color_t));
    lv_disp_draw_buf_init(&disp_buf1, buf1, NULL, SDL_HOR_RES * SDL_VER_RES);

    /*Create a display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv); /*Basic initialization*/
    disp_drv.draw_buf = &disp_buf1;
    disp_drv.flush_cb = sdl_display_flush;
    disp_drv.hor_res = SDL_HOR_RES;
    disp_drv.ver_res = SDL_VER_RES;

    lv_disp_t * disp = lv_disp_drv_register(&disp_drv);

    /* Register input */
    static lv_indev_drv_t lv_indev_drv_pointer;
    lv_indev_drv_pointer.type = LV_INDEV_TYPE_POINTER;
    lv_indev_drv_pointer.read_cb = sdl_mouse_read;
    lv_indev_t *mouse_indev = lv_indev_drv_register(&lv_indev_drv_pointer);


    lv_theme_t * th = lv_theme_default_init(disp,
                                            lv_palette_main(LV_PALETTE_BLUE),
                                            lv_palette_main(LV_PALETTE_RED),
                                            LV_THEME_DEFAULT_DARK,
                                            LV_FONT_DEFAULT);
    lv_disp_set_theme(disp, th);

    lv_group_t * g = lv_group_create();
    lv_group_set_default(g);
    LV_LOG_USER("SDL driver set.");
    return true;
}
#endif

int main(void)
{
    /* LVGL init */
    lv_init();
#if USE_SDL
    hal_init();
#endif
#ifndef USE_WAYLAND
    /* Wayland display init */
    lv_wayland_init();
    lv_disp_t * disp = lv_wayland_create_window(400, 400, "window", NULL);
    lv_wayland_window_set_fullscreen(disp, true);

    /* Set a cursor for the touchscreen */
    lv_indev_t * touchscreen = lv_wayland_get_touchscreen(disp);
    LV_IMG_DECLARE(mouse_cursor_icon)
    lv_obj_t * cursor_obj = lv_img_create(lv_scr_act()); /* Create an image object for the cursor  */
    lv_img_set_src(cursor_obj, &mouse_cursor_icon); /* Set the image source */
    lv_indev_set_cursor(touchscreen, cursor_obj); /* Connect the image object to the driver */
#endif

#ifndef USE_X11
    /* initialize X11 display driver */
    lv_display_t * disp = lv_x11_window_create("LVGL X11 Simulation", 480, 480);
    //lv_display_t * disp = lv_x11_window_create("LVGL X11 Simulation", monitor_hor_res, monitor_ver_res);

    /* initialize X11 input drivers (for keyboard, mouse & mousewheel) */
    lv_x11_inputs_create(disp, NULL);
#endif

#ifndef USE_FBDEV
    lv_display_t *disp = lv_linux_fbdev_create();
    lv_linux_fbdev_set_file(disp, "/dev/fb0");
#endif

    /* Search connected keyboard and attach it to the driver */
    lv_group_t * g = lv_group_create();
    lv_group_set_default(g);

    lv_indev_t * cur_drv = NULL;
    for (;;) {
        cur_drv = lv_indev_get_next(cur_drv);
        if (!cur_drv) {
            break;
        }

        if (cur_drv->driver->type == LV_INDEV_TYPE_KEYPAD) {
            lv_indev_set_group(cur_drv, g);
        }

        if (cur_drv->driver->type == LV_INDEV_TYPE_ENCODER) {
            lv_indev_set_group(cur_drv, g);
        }
    }

    /* Create a Demo (activate in project-root/lv_conf.h */
    lv_demo_widgets();
    //lv_demo_benchmark();          // OK
    //lv_demo_stress();             // seems to be OK
    // lv_demo_keypad_encoder();     // works, but I haven't an encoder
    //lv_demo_music();              // NOK
    // lv_demo_printer();


    while(1) {
        lv_tick_inc(5);
        lv_timer_handler();
        usleep(5000);
    }

    return 0;
}
