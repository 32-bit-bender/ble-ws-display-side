set(srcs    
            "lvgl_demo_ui.c"
            "lvgl_images/hum_icon.c"
            "lvgl_images/tmp_icon.c"
            "ws_ble_central.c")

idf_component_register(SRCS "main.c" "${srcs}"
                            INCLUDE_DIRS ".")
