set(srcs    
           
            "lvgl_images/hum_icon.c"
            "lvgl_images/tmp_icon.c"
            "ws_ble_central.c")

idf_component_register(SRCS "lvgl_menu_ui.c" "main.c" "${srcs}"
                            INCLUDE_DIRS ".")
