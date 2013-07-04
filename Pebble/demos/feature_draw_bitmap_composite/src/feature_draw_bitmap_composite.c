/*

   Demonstrate graphics bitmap compositing.

*/

#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0xDC, 0x11, 0x22, 0x7F, 0x6E, 0xC1, 0x4E, 0x58, 0x89, 0xE4, 0x98, 0x70, 0x0E, 0xD0, 0x79, 0x9C }
PBL_APP_INFO(MY_UUID,
             "GCompOp Demo", "Demo Corp",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);


typedef struct GCompOpInfo {
  char *name;
  GCompOp op;
} GCompOpInfo;

// These are the compositing modes available
GCompOpInfo gcompops[] = {
  {"GCompOpAssign", GCompOpAssign},
  {"GCompOpAssignInverted", GCompOpAssignInverted},
  {"GCompOpOr", GCompOpOr},
  {"GCompOpAnd", GCompOpAnd},
  {"GCompOpClear", GCompOpClear},
  {"GCompOpSet", GCompOpSet}
};

int current_gcompop = 0;

const int num_gcompops = ARRAY_LENGTH(gcompops);

Window window;

Layer layer;

// We will use a heap bitmap to composite with a large circle
HeapBitmap image;


// This is a layer update callback where compositing will take place
void layer_update_callback(Layer *me, GContext* ctx) {
  // Display the name of the current compositing operation
  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_text_draw(ctx,
    gcompops[current_gcompop].name,
    fonts_get_system_font(FONT_KEY_GOTHIC_18),
    me->frame,
    GTextOverflowModeTrailingEllipsis,
    GTextAlignmentCenter,
    NULL);

  // Draw the large circle the image will composite with
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_circle(ctx, GPoint(window.layer.frame.size.w/2, window.layer.frame.size.h+110), 180);

  // Use the image size to help center the image
  GRect destination = image.bmp.bounds;

  // Center horizontally using the window frame size
  destination.origin.x = (window.layer.frame.size.w-destination.size.w)/2;
  destination.origin.y = 50;

  // Set the current compositing operation
  // This will only cause bitmaps to composite
  graphics_context_set_compositing_mode(ctx, gcompops[current_gcompop].op);

  // Draw the bitmap; it will use current compositing operation set
  graphics_draw_bitmap_in_rect(ctx, &image.bmp, destination);
}


void handle_init(AppContextRef ctx) {
  // If you specified resources in resource_map.json, use this
  // to prepare the resources for use within your app
  resource_init_current_app(&APP_RESOURCES);

  // Then use the respective resource loader to obtain the resource for use
  // In this case, we load the image
  heap_bitmap_init(&image, RESOURCE_ID_IMAGE_PUG);

  window_init(&window, "Draw Bitmap");
  window_stack_push(&window, true /* Animated */);

  // Initialize the layer
  layer_init(&layer, window.layer.frame);

  // Set up the update layer callback
  layer_set_update_proc(&layer, layer_update_callback);

  // Add the layer to the window for display
  layer_add_child(&window.layer, &layer);
}


void handle_deinit(AppContextRef ctx) {
  // Cleanup the image
  heap_bitmap_deinit(&image);
}


// This is a quick and dirty input handler for button controls.
// It's recommended to use a click config provider for button controls instead.
// Click config providers provide more fine-grained button controls.
// You would only want to use input handler for an application-wide override.
// Check out app_vibe_variety for an example of click config provider usage.
void handle_button_down(AppContextRef ctx, PebbleButtonEvent *event) {
  switch (event->button_id) {
    default: break;
    // Show the previous bitmap compositing operation
    case BUTTON_ID_UP: if (--current_gcompop < 0) { current_gcompop = num_gcompops-1; } break;
    // Show the next bitmap compositing operation
    case BUTTON_ID_DOWN: if (++current_gcompop >= num_gcompops) { current_gcompop = 0; } break;
  }
  // Mark the layer for an update
  layer_mark_dirty(&layer);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &handle_deinit,
    .input_handlers = {
      .buttons = {
        .down = &handle_button_down
      }
    }
  };
  app_event_loop(params, &handlers);
}
