#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x69, 0xA5, 0x39, 0x87, 0x37, 0x9E, 0x4A, 0xE7, 0xA3, 0xC2, 0xD0, 0x8E, 0x5F, 0xFE, 0x85, 0x11 }
PBL_APP_INFO(MY_UUID,
             "Simple Menu", "Demo Corp",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);


#define NUM_MENU_SECTIONS 2
#define NUM_FIRST_MENU_ITEMS 3
#define NUM_SECOND_MENU_ITEMS 1


Window window;

// This is a simple menu layer
SimpleMenuLayer simple_menu_layer;

// A simple menu layer can have multiple sections
SimpleMenuSection menu_sections[NUM_MENU_SECTIONS];

// Each section is composed of a number of menu items
SimpleMenuItem first_menu_items[NUM_FIRST_MENU_ITEMS];

SimpleMenuItem second_menu_items[NUM_SECOND_MENU_ITEMS];

// Menu items can optionally have icons
HeapBitmap menu_icon_image;

bool special_flag = false;

int hit_count = 0;

// You can capture when the user selects a menu icon with a menu item select callback
void menu_select_callback(int index, void *ctx) {
  // Here we just change the subtitle to a literal string
  first_menu_items[index].subtitle = "You've hit select here!";
  // Mark the layer to be updated
  layer_mark_dirty(simple_menu_layer_get_layer(&simple_menu_layer));
}


// You can specify special callbacks to differentiate functionality of a menu item
void special_select_callback(int index, void *ctx) {
  // Of course, you can do more complicated things in a menu item select callback
  // Here, we have a simple toggle
  special_flag = !special_flag;

  SimpleMenuItem *menu_item = &second_menu_items[index];

  if (special_flag) {
    menu_item->subtitle = "Okay, it's not so special.";
  } else {
    menu_item->subtitle = "Well, maybe a little.";
  }

  if (++hit_count > 5) {
    menu_item->title = "Very Special Item";
  }

  // Mark the layer to be updated
  layer_mark_dirty(simple_menu_layer_get_layer(&simple_menu_layer));
}


// This initializes the menu upon window load
void window_load(Window *me) {
  // We'll have to load the icon before we can use it
  // resource_init_current_app must be called before all asset loading
  heap_bitmap_init(&menu_icon_image, RESOURCE_ID_IMAGE_MENU_ICON_1);

  // Although we already defined NUM_FIRST_MENU_ITEMS, you can define
  // an int as such to easily change the order of menu items later
  int num_a_items = 0;

  // This is an example of how you'd set a simple menu item
  first_menu_items[num_a_items++] = (SimpleMenuItem){
    // You should give each menu item a title and callback
    .title = "First Item",
    .callback = menu_select_callback,
  };
  // The menu items appear in the order saved in the menu items array
  first_menu_items[num_a_items++] = (SimpleMenuItem){
    .title = "Second Item",
    // You can also give menu items a subtitle
    .subtitle = "Here's a subtitle",
    .callback = menu_select_callback,
  };
  first_menu_items[num_a_items++] = (SimpleMenuItem){
    .title = "Third Item",
    .subtitle = "This has an icon",
    .callback = menu_select_callback,
    // This is how you would give a menu item an icon
    .icon = &menu_icon_image.bmp,
  };

  // This initializes the second section
  second_menu_items[0] = (SimpleMenuItem){
    .title = "Special Item",
    // You can use different callbacks for your menu items
    .callback = special_select_callback,
  };

  // Bind the menu items to the corresponding menu sections
  menu_sections[0] = (SimpleMenuSection){
    .num_items = NUM_FIRST_MENU_ITEMS,
    .items = first_menu_items,
  };
  menu_sections[1] = (SimpleMenuSection){
    // Menu sections can also have titles as well
    .title = "Yet Another Section",
    .num_items = NUM_SECOND_MENU_ITEMS,
    .items = second_menu_items,
  };

  // Now we prepare to initialize the simple menu layer
  // We need the bounds to specify the simple menu layer's viewport size
  // In this case, it'll be the same as the window's
  GRect bounds = me->layer.bounds;

  // Initialize the simple menu layer
  simple_menu_layer_init(&simple_menu_layer, bounds, me, menu_sections, NUM_MENU_SECTIONS, NULL);

  // Add it to the window for display
  layer_add_child(&me->layer, simple_menu_layer_get_layer(&simple_menu_layer));
}


// Deinitialize resources on window unload that were initialized on window load
void window_unload(Window *me) {
  // Cleanup the menu icon
  heap_bitmap_deinit(&menu_icon_image);
}


void handle_init(AppContextRef ctx) {
  resource_init_current_app(&APP_RESOURCES);

  window_init(&window, "Demo");
  window_stack_push(&window, true /* Animated */);

  // Setup the window handlers
  window_set_window_handlers(&window, (WindowHandlers){
    .load = window_load,
    .unload = window_unload,
  });
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init
  };
  app_event_loop(params, &handlers);
}
