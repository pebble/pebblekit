#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x15, 0x42, 0xCD, 0xA0, 0xEB, 0x9C, 0x4E, 0xA4, 0xAE, 0x82, 0x5D, 0xB0, 0xB2, 0xB7, 0x20, 0xDF }
PBL_APP_INFO(MY_UUID,
             "Scroll Demo", "Demo Corp",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);


Window window;

// This is a scroll layer
ScrollLayer scroll_layer;

// We also use a text layer to scroll in the scroll layer
TextLayer text_layer;

// The scroll layer can other things in it such as an invert layer
InverterLayer inverter_layer;

// Lorum ipsum to have something to scroll
char scroll_text[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam quam tellus, fermentu  m quis vulputate quis, vestibulum interdum sapien. Vestibulum lobortis pellentesque pretium. Quisque ultricies purus e  u orci convallis lacinia. Cras a urna mi. Donec convallis ante id dui dapibus nec ullamcorper erat egestas. Aenean a m  auris a sapien commodo lacinia. Sed posuere mi vel risus congue ornare. Curabitur leo nisi, euismod ut pellentesque se  d, suscipit sit amet lorem. Aliquam eget sem vitae sem aliquam ornare. In sem sapien, imperdiet eget pharetra a, lacin  ia ac justo. Suspendisse at ante nec felis facilisis eleifend.";

const int vert_scroll_text_padding = 4;


// Setup the scroll layer on window load
// We do this here in order to be able to get the max used text size
void window_load(Window *me) {
  const GRect max_text_bounds = GRect(0, 0, 144, 2000);

  // Initialize the scroll layer
  scroll_layer_init(&scroll_layer, me->layer.bounds);

  // This binds the scroll layer to the window so that up and down map to scrolling
  // You may use scroll_layer_set_callbacks to add or override interactivity
  scroll_layer_set_click_config_onto_window(&scroll_layer, me);

  // Set the initial max size
  scroll_layer_set_content_size(&scroll_layer, max_text_bounds.size);

  // Initialize the text layer
  text_layer_init(&text_layer, max_text_bounds);
  text_layer_set_text(&text_layer, scroll_text);

  // Change the font to a nice readable one
  // This is system font; you can inspect pebble_fonts.h for all system fonts
  // or you can take a look at feature_custom_font to add your own font
  text_layer_set_font(&text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));

  // Trim text layer and scroll content to fit text box
  GSize max_size = text_layer_get_max_used_size(app_get_current_graphics_context(), &text_layer);
  text_layer_set_size(&text_layer, max_size);
  scroll_layer_set_content_size(&scroll_layer, GSize(144, max_size.h + vert_scroll_text_padding));

  // Add the layers for display
  scroll_layer_add_child(&scroll_layer, &text_layer.layer);

  // The inverter layer will highlight some text
  inverter_layer_init(&inverter_layer, GRect(0, 28, window.layer.frame.size.w, 28));
  scroll_layer_add_child(&scroll_layer, &inverter_layer.layer);

  layer_add_child(&me->layer, &scroll_layer.layer);
}


void handle_init(AppContextRef ctx) {
  window_init(&window, "Scroll Demo");
  window_set_window_handlers(&window, (WindowHandlers) {
    .load = window_load,
  });
  window_stack_push(&window, true /* Animated */);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init
  };
  app_event_loop(params, &handlers);
}
