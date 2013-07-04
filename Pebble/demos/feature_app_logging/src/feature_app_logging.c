#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x4E, 0x56, 0x05, 0x4D, 0x06, 0x7A, 0x44, 0xBB, 0xBA, 0x19, 0x02, 0xEB, 0x6C, 0xD8, 0xB5, 0xA1 }
PBL_APP_INFO_SIMPLE(MY_UUID, "Logging Demo", "Demo Corp", 1 /* App version */);

Window window;

TextLayer text;

void select_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Single Click");
}

void select_multi_click_handler(ClickRecognizerRef recognizer, Window *window) {
  const uint16_t count = click_number_of_clicks_counted(recognizer);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Multi-Click: count:%u", count);
}

void select_long_click_handler(ClickRecognizerRef recognizer, Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Long-Click: START");
}

void select_long_click_release_handler(ClickRecognizerRef recognizer, Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Long-Click: RELEASE");
}

void config_provider(ClickConfig **config, Window *window) {
  // single click / repeat-on-hold config:
  config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) select_single_click_handler;
  config[BUTTON_ID_SELECT]->click.repeat_interval_ms = 1000; // "hold-to-repeat" gets overridden if there's a long click handler configured!
  // multi click config:
  config[BUTTON_ID_SELECT]->multi_click.handler = (ClickHandler) select_multi_click_handler;
  config[BUTTON_ID_SELECT]->multi_click.min = 2;
  config[BUTTON_ID_SELECT]->multi_click.max = 10;
  config[BUTTON_ID_SELECT]->multi_click.last_click_only = true;
  // long click config:
  config[BUTTON_ID_SELECT]->long_click.handler = (ClickHandler) select_long_click_handler;
  config[BUTTON_ID_SELECT]->long_click.release_handler = (ClickHandler) select_long_click_release_handler;
  config[BUTTON_ID_SELECT]->long_click.delay_ms = 700;
}

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Logging Demo");

  text_layer_init(&text, GRect(0, 0, 144, 150));
  text_layer_set_text_color(&text, GColorBlack);
  text_layer_set_font(&text, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(&text, GTextAlignmentCenter);
  text_layer_set_text(&text, "Press the select button to try out different clicks and watch your Bluetooth logs");
  layer_add_child(&window.layer, &text.layer);

  window_set_click_config_provider(&window, (ClickConfigProvider) config_provider);

  window_stack_push(&window, true /* Animated */);
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
  };
  app_event_loop(params, &handlers);
}
