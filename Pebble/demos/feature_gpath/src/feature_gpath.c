#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x6D, 0x5F, 0x57, 0xF9, 0xB0, 0x1C, 0x4A, 0x6A, 0x88, 0xF0, 0xEC, 0x02, 0x4D, 0x38, 0xBA, 0x41 }
PBL_APP_INFO(MY_UUID,
             "GPath Demo", "Demo Corp",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);


Window window;

Layer path_layer;

// This defines graphics path information to be loaded as a path later
const GPathInfo HOUSE_PATH_POINTS = {
  // This is the amount of points
  11,
  // A path can be concave, but it should twist on itself
  // The points should be defined in clockwise order due to the rendering
  // implementation. Counter-clockwise will work in older firmwares, but
  // it is not officially supported
  (GPoint []) {
    {-40, 0},
    {0, -40},
    {40, 0},
    {28, 0},
    {28, 40},
    {10, 40},
    {10, 16},
    {-10, 16},
    {-10, 40},
    {-28, 40},
    {-28, 0}
  }
};

// This is an example of a path that looks like a compound path
// If you rotate it however, you will see it is a single shape
const GPathInfo INFINITY_RECT_PATH_POINTS = {
  16,
  (GPoint []) {
    {-50, 0},
    {-50, -60},
    {10, -60},
    {10, -20},
    {-10, -20},
    {-10, -40},
    {-30, -40},
    {-30, -20},
    {50, -20},
    {50, 40},
    {-10, 40},
    {-10, 0},
    {10, 0},
    {10, 20},
    {30, 20},
    {30, 0}
  }
};

GPath house_path;

GPath infinity_path;

#define NUM_GRAPHIC_PATHS 2

GPath *graphic_paths[NUM_GRAPHIC_PATHS];

GPath *current_path = &house_path;

int current_path_index = 0;

int path_angle = 0;

bool outline_mode = false;


// This is the layer update callback
// It is called whenever a render update should occur
void path_layer_update_callback(Layer *me, GContext *ctx) {
  (void)me;

  // You can rotate the path before rendering
  gpath_rotate_to(current_path, (TRIG_MAX_ANGLE / 360) * path_angle);

  // There are two ways you can draw a GPath: outline or filled
  // In this example, only one or the other is drawn, but you can draw
  // multiple instances of the same path filled or outline.
  if (outline_mode) {
    // draw outline uses the stroke color
    graphics_context_set_stroke_color(ctx, GColorWhite);
    gpath_draw_outline(ctx, current_path);
  } else {
    // draw filled uses the fill color
    graphics_context_set_fill_color(ctx, GColorWhite);
    gpath_draw_filled(ctx, current_path);
  }
}


int path_angle_add(int angle) {
  return path_angle = (path_angle + angle) % 360;
}


void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "GPath Demo");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorBlack);

  GRect frame = window.layer.frame;

  layer_init(&path_layer, GRect(0, 0, frame.size.w, frame.size.h));
  path_layer.update_proc = path_layer_update_callback;
  layer_add_child(&window.layer, &path_layer);

  // You need to initialize a GPath before you can use it
  // To initialize a GPath, you need to the corresponding GPathInfo
  // You don't need to deinitialize GPaths.
  gpath_init(&house_path, &HOUSE_PATH_POINTS);
  gpath_init(&infinity_path, &INFINITY_RECT_PATH_POINTS);

  // This demo allows you to cycle paths
  // We accomplish this using an array we'll cycle through
  // Try to adding more GPaths to cycle through
  // You'll need to define another GPathInfo
  // Remember to update NUM_GRAPHIC_PATHS accordingly
  graphic_paths[0] = &house_path;
  graphic_paths[1] = &infinity_path;

  // Move all paths to the center of the screen
  for (int i = 0; i < NUM_GRAPHIC_PATHS; i++) {
    gpath_move_to(graphic_paths[i], GPoint(frame.size.w/2, frame.size.h/2));
  }
}


// This is a quick and dirty input handler for button controls.
// It's recommended to use a click config provider for button controls instead.
// Click config providers provide more fine-grained button controls.
// You would only want to use input handler for an application-wide override.
// Check out app_vibe_variety for an example of click config provider usage.
void handle_button_down(AppContextRef ctx, PebbleButtonEvent *event) {
  (void)ctx;
  switch (event->button_id) {
    default: break;
    // Rotate the path counter-clockwise
    case BUTTON_ID_UP: path_angle_add(-10); break;
    // Show the outline of the path when select is held down
    case BUTTON_ID_SELECT: outline_mode = true; break;
    // Rotate the path clockwise
    case BUTTON_ID_DOWN: path_angle_add(10); break;
  }
  // Update the layer
  layer_mark_dirty(&path_layer);
}


void handle_button_up(AppContextRef ctx, PebbleButtonEvent *event) {
  (void)ctx;
  switch (event->button_id) {
    default: break;
    case BUTTON_ID_SELECT:
      // Show the path filled
      outline_mode = false;
      // Cycle to the next path
      current_path_index = (current_path_index+1) % NUM_GRAPHIC_PATHS;
      current_path = graphic_paths[current_path_index];
      break;
  }
  // Update the layer
  layer_mark_dirty(&path_layer);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .input_handlers = {
      .buttons = {
        .down = &handle_button_down,
        .up = &handle_button_up
      }
    },
  };
  app_event_loop(params, &handlers);
}
