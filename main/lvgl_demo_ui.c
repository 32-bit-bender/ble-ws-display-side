/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

// This demo UI is adapted from LVGL official example: https://docs.lvgl.io/master/widgets/extra/meter.html#simple-meter
#include "lv_conf.h"
#include "lvgl.h"
#include "esp_log.h"


static lv_style_t style_con_label;
static lv_style_t style_con2_label;
static lv_style_t style_switch;
static lv_style_t style_back_label;
static lv_style_t style_tmp_page_bg;
static lv_style_t style_hum_page_bg;
static lv_style_t style_container_text;
static lv_style_t style_data_label;


static void style_init (void)
{
  lv_style_init(&style_con_label);
  lv_style_set_radius(&style_con_label, 10);
  lv_style_set_bg_opa(&style_con_label, LV_OPA_COVER);
  lv_style_set_bg_color(&style_con_label, lv_palette_lighten(LV_PALETTE_ORANGE, 4));
  lv_style_set_bg_grad_color(&style_con_label, lv_palette_main(LV_PALETTE_ORANGE));
  lv_style_set_bg_grad_dir(&style_con_label, LV_GRAD_DIR_VER);
  lv_style_set_border_color(&style_con_label, lv_color_black());
  lv_style_set_border_opa(&style_con_label, LV_OPA_20);
  lv_style_set_border_width(&style_con_label, 2);
  lv_style_set_text_color(&style_con_label, lv_color_black());
  lv_style_set_text_font(&style_con_label,&lv_font_montserrat_28); //changed the font just to make the size bigger (from 14 to 32)
  lv_style_set_text_align(&style_con_label, LV_TEXT_ALIGN_CENTER);

  lv_style_init(&style_con2_label);
  lv_style_set_radius(&style_con2_label, 10);
  lv_style_set_bg_opa(&style_con2_label, LV_OPA_COVER);
  lv_style_set_bg_color(&style_con2_label, lv_palette_lighten(LV_PALETTE_CYAN, 4));
  lv_style_set_bg_grad_color(&style_con2_label, lv_palette_main(LV_PALETTE_CYAN));
  lv_style_set_bg_grad_dir(&style_con2_label, LV_GRAD_DIR_VER);
  lv_style_set_border_color(&style_con2_label, lv_color_black());
  lv_style_set_border_opa(&style_con2_label, LV_OPA_20);
  lv_style_set_border_width(&style_con2_label, 2);
  lv_style_set_text_color(&style_con2_label, lv_color_black());
  lv_style_set_text_font(&style_con2_label,&lv_font_montserrat_28); //changed the font just to make the size bigger (from 14 to 32)
  lv_style_set_text_align(&style_con2_label, LV_TEXT_ALIGN_CENTER);

  //Style for the bluetooth toggle switch in default state
  lv_style_init(&style_switch);
  lv_style_set_bg_color(&style_switch, lv_palette_darken(LV_PALETTE_GREY, 2));
  lv_style_set_bg_grad_color(&style_switch, lv_palette_darken(LV_PALETTE_GREY, 4));

  //Style for the back button in the temperature page
  lv_style_init(&style_back_label);
  lv_style_set_radius(&style_back_label, 10);
  lv_style_set_bg_opa(&style_back_label, LV_OPA_COVER);
  lv_style_set_bg_color(&style_back_label, lv_palette_lighten(LV_PALETTE_LIGHT_BLUE, 4));
  lv_style_set_bg_grad_color(&style_back_label, lv_palette_main(LV_PALETTE_LIGHT_BLUE));
  lv_style_set_bg_grad_dir(&style_back_label, LV_GRAD_DIR_VER);
  lv_style_set_border_color(&style_back_label, lv_color_black());
  lv_style_set_border_opa(&style_back_label, LV_OPA_20);
  lv_style_set_border_width(&style_back_label, 2);
  lv_style_set_text_color(&style_back_label, lv_color_black());
  lv_style_set_text_font(&style_back_label,&lv_font_montserrat_18);
  lv_style_set_text_align(&style_back_label, LV_TEXT_ALIGN_CENTER);

  //Style for Temperature page background
  lv_style_init(&style_tmp_page_bg);
  lv_style_set_radius(&style_tmp_page_bg, 10);
  lv_style_set_bg_opa(&style_tmp_page_bg, LV_OPA_COVER);
  lv_style_set_bg_color(&style_tmp_page_bg, lv_palette_lighten(LV_PALETTE_ORANGE, 5));

  //Style for Temperature page background
  lv_style_init(&style_hum_page_bg);
  lv_style_set_radius(&style_hum_page_bg, 10);
  lv_style_set_bg_opa(&style_hum_page_bg, LV_OPA_COVER);
  lv_style_set_bg_color(&style_hum_page_bg, lv_palette_lighten(LV_PALETTE_CYAN, 5));

  //Style for container1 main text
  lv_style_init(&style_container_text);
  lv_style_set_text_font(&style_container_text,&lv_font_montserrat_18); 
  lv_style_set_text_align(&style_container_text, LV_TEXT_ALIGN_CENTER);

  //Temperature and humidity data labels
  lv_style_init(&style_data_label);
  lv_style_set_text_font(&style_data_label,&lv_font_montserrat_18); 
  //lv_style_set_text_align(&style_data_label,LV_TEXT); 
}

static void event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        LV_UNUSED(obj);
        LV_LOG_USER("State: %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
    }
}

void lv_my_menu(lv_disp_t *disp)
{
    lv_obj_t *scr = lv_disp_get_scr_act(disp);
    lv_obj_t *live_tmp_label;
    lv_obj_t *live_hum_label;
    style_init();

    /*Create a menu object*/
    lv_obj_t * menu = lv_menu_create(scr);
    lv_obj_set_size(menu, lv_disp_get_physical_hor_res(disp), lv_disp_get_physical_ver_res(disp)*3/4);
    lv_obj_align(menu, LV_ALIGN_BOTTOM_MID,0,0);

    /*Modify the header*/
    lv_obj_t * back_btn = lv_menu_get_main_header_back_btn(menu);
    lv_obj_t * back_button_label = lv_label_create(back_btn);
    lv_obj_set_size(back_button_label,100,30);
    lv_label_set_text(back_button_label, "Back");
    lv_obj_add_style(back_button_label, &style_back_label,0);
    lv_obj_t * cont;
    lv_obj_t * label;

    /*Create sub pages*/
    lv_obj_t * sub_1_page = lv_menu_page_create(menu, "Page 1");
    lv_obj_add_style(sub_1_page, &style_tmp_page_bg,0);
    
    //cont = lv_menu_cont_create(sub_1_page);
    label = lv_label_create(sub_1_page);
    lv_obj_add_style(label, &style_container_text,0);
    lv_obj_set_content_height(label, 50); //gives this label a certain height so that i can spaceit away from other elements
    lv_label_set_text(label, "Live Temperature\nReadings:");

    // /********CREATING TEMPERATURE IMAGE *********/
    LV_IMG_DECLARE(tmp_icon);
    lv_obj_t * img1 = lv_img_create(sub_1_page);
    lv_obj_set_content_height(img1, 100); //gives this label a certain height so that i can spaceit away from other elements
    lv_img_set_src(img1, &tmp_icon);

    /*******CREATING TEMPERATURE LIVE VALUE********/
    live_tmp_label= lv_label_create(sub_1_page);
    lv_label_set_text(live_tmp_label,"<TEMPERATURE> °C");
    lv_obj_add_style(live_tmp_label, &style_data_label,0);

    /*******SECOND SUBPAGE******/
    lv_obj_t * sub_2_page = lv_menu_page_create(menu, "");
    label = lv_label_create(sub_2_page);
    lv_obj_set_content_height(label, 50); //gives this label a certain height so that i can spaceit away from other elements
    lv_label_set_text(label, "Live Humidity\nReadings:");
    lv_obj_add_style(label, &style_container_text,0);
    lv_obj_add_style(sub_2_page, &style_hum_page_bg,0);  

    // /********CREATING HUMIDITY IMAGE *********/
    LV_IMG_DECLARE(hum_icon);
    lv_obj_t * img2 = lv_img_create(sub_2_page);
    lv_obj_set_content_height(img2, 80); //gives this label a certain height so that i can spaceit away from other elements
    lv_img_set_src(img2, &hum_icon);

    /*******CREATING HUMIDITY LIVE VALUE********/
    live_hum_label= lv_label_create(sub_2_page);
    lv_label_set_text(live_hum_label,"<HUMIDITY> g/m3");
    lv_obj_add_style(live_hum_label, &style_data_label,0);

    /*Create a main page*/
    lv_obj_t * main_page = lv_menu_page_create(menu, NULL);

    cont = lv_menu_cont_create(main_page);
    label = lv_label_create(cont);
    lv_obj_center(cont);
    lv_obj_set_size(cont, lv_disp_get_physical_hor_res(disp), lv_disp_get_physical_ver_res(disp)*3/9);
    lv_label_set_text(label, "Check Temperature");
    lv_obj_set_flex_grow(label, 1);
    lv_menu_set_load_page_event(menu, cont, sub_1_page);
    lv_obj_add_style(cont, &style_con_label, 0);

    cont = lv_menu_cont_create(main_page);
    label = lv_label_create(cont);
    lv_obj_center(cont);
    lv_obj_set_size(cont, lv_disp_get_physical_hor_res(disp), lv_disp_get_physical_ver_res(disp)*3/9);
    lv_label_set_text(label, "Check Humidity");
    lv_obj_set_flex_grow(label, 1);
    lv_menu_set_load_page_event(menu, cont, sub_2_page);
    lv_obj_add_style(cont, &style_con2_label, 0);

    lv_menu_set_page(menu, main_page);
    // /*************SWITCH CREATION******************/
    lv_obj_t * sw;
    sw = lv_switch_create(scr);
    lv_obj_add_event_cb(sw, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_flag(sw, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_align(sw, LV_ALIGN_TOP_LEFT,10,20);
    lv_obj_set_size(sw, 80, 40);
    lv_obj_add_style(sw, &style_switch, LV_STATE_DEFAULT);

}