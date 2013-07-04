#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x0C, 0x13, 0x6B, 0x6E, 0x7F, 0x4C, 0x48, 0x88, 0x92, 0xE2, 0x75, 0x63, 0x92, 0xFC, 0x0F, 0x3B }
PBL_APP_INFO(MY_UUID,
             "Inverter Layer", "Demo Company",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

Window window;

TextLayer text_below;
TextLayer text_above;
InverterLayer inverter_layer;

void handle_init(AppContextRef ctx) {

  window_init(&window, "Inverter Layer");
  window_stack_push(&window, true /* Animated */);

  text_layer_init(&text_below, GRect(0, 54, 144 /* width */, 168-54 /* height */));
  text_layer_set_text(&text_below, "below");
  text_layer_set_font(&text_below, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text_alignment(&text_below, GTextAlignmentCenter);
  layer_add_child(&window.layer, &text_below.layer);

  inverter_layer_init(&inverter_layer, GRect(144/2, 0, 144, 168));
  layer_add_child(&window.layer, &inverter_layer.layer);

  text_layer_init(&text_above, GRect(0, 84, 144 /* width */, 168-84 /* height */));
  text_layer_set_text(&text_above, "above");
  text_layer_set_font(&text_above, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text_alignment(&text_above, GTextAlignmentCenter);
  text_layer_set_background_color(&text_above, GColorClear);
  layer_add_child(&window.layer, &text_above.layer);

}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init
  };
  app_event_loop(params, &handlers);
}
