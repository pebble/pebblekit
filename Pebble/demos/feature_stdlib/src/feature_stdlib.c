/*

   Demonstrate a few standard library functions available.

 */

#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x16, 0x5D, 0xCE, 0x7B, 0x2F, 0x46, 0x43, 0x39, 0xBD, 0xC2, 0xB0, 0x64, 0x22, 0xF4, 0xDF, 0x23 }
PBL_APP_INFO(MY_UUID,
             "Stdlib Demo", "Demo Corp",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

Window window;

Layer layer;

static char info_text[256] = "info";

static char str_example[32] = "Goodbye!";

static char str_copy[32] = "Hello, World!";

// Can be used to distinguish between multiple timers in your app
#define COOKIE_MY_TIMER 1


void update_layer_callback(Layer *me, GContext *ctx) {
  // Get the amount of seconds and the milliseconds part since the Epoch
  // time_ms also returns the milliseconds, so you can optionally pass in null
  // for any output parameters you don't need
  time_t t = 0;
  uint16_t t_ms = 0;
  time_ms(&t, &t_ms);

  // Get the next pseudo-random number
  int r = rand();

  // Print formatted text into a buffer
  snprintf(info_text, sizeof(info_text),
      "str: %s\n"
      "time: %lu\n"
      "milliseconds: %u\n"
      "rand: %d",
      str_example, t, t_ms, r);

  // Draw the formatted text
  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_text_draw(ctx,
      info_text,
      fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD),
      (GRect){ .origin = GPoint(10, 5), .size = window.layer.frame.size },
      GTextOverflowModeWordWrap,
      GTextAlignmentLeft,
      NULL);
}


void handle_timer(AppContextRef ctx, AppTimerHandle handle, uint32_t cookie) {
  // All timers that resolve send an event here
  // Check if it's the right timer that fired
  if (cookie == COOKIE_MY_TIMER) {
    // Mark the layer dirty to have it update
    layer_mark_dirty(&layer);

    // Start another timer to have this happen again
    app_timer_send_event(ctx, 50 /* milliseconds */, COOKIE_MY_TIMER);
  }
}


void handle_init(AppContextRef ctx) {
  // Seed the pseudo-random number generator with the time
  srand(time(NULL));

  // Setup the window
  window_init(&window, "Demo");
  window_stack_push(&window, true /* Animated */);

  // Setup the layer that will display the text
  layer_init(&layer, (GRect){ .origin = GPointZero, .size = window.layer.frame.size });
  layer.update_proc = update_layer_callback;
  layer_add_child(&window.layer, &layer);

  // Copy the string into the example
  strcpy(str_example, str_copy);

  // Start the timer
  app_timer_send_event(ctx, 50 /* milliseconds */, COOKIE_MY_TIMER);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .timer_handler = &handle_timer
  };
  app_event_loop(params, &handlers);
}
