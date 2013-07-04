#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

enum {
  CMD_KEY = 0x0, // TUPLE_INTEGER
};

enum {
  CMD_UP = 0x01,
  CMD_DOWN = 0x02,
};


#define MY_UUID {0xEC, 0x7E, 0xE5, 0xC6, 0x8D, 0xDF, 0x40, 0x89, 0xAA, 0x84, 0xC3, 0x39, 0x6A, 0x11, 0xCC, 0x95}
PBL_APP_INFO(MY_UUID,
             "GPS SMS Time", "Pebble",
             3, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

Window window;
TextLayer timeLayer;
static bool callbacks_registered;
static AppMessageCallbacksNode app_callbacks;

static void app_send_failed(DictionaryIterator* failed, AppMessageResult reason, void* context) {
  // TODO: error handling
}

static void app_received_msg(DictionaryIterator* received, void* context) {
  vibes_short_pulse();
}

bool register_callbacks() {
	if (callbacks_registered) {
		if (app_message_deregister_callbacks(&app_callbacks) == APP_MSG_OK)
			callbacks_registered = false;
	}
	if (!callbacks_registered) {
		app_callbacks = (AppMessageCallbacksNode){
			.callbacks = {
				.out_failed = app_send_failed,
        .in_received = app_received_msg
			},
			.context = NULL
		};
		if (app_message_register_callbacks(&app_callbacks) == APP_MSG_OK) {
      callbacks_registered = true;
    }
	}
	return callbacks_registered;
}

static void send_cmd(uint8_t cmd) {
  Tuplet value = TupletInteger(CMD_KEY, cmd);
  
  DictionaryIterator *iter;
  app_message_out_get(&iter);
  
  if (iter == NULL)
    return;
  
  dict_write_tuplet(iter, &value);
  dict_write_end(iter);
  
  app_message_out_send();
  app_message_out_release();
}

void up_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  
  send_cmd(CMD_UP);
  vibes_short_pulse();
}

void down_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  
  send_cmd(CMD_DOWN);
}

void click_config_provider(ClickConfig **config, Window *window) {
  
  config[BUTTON_ID_UP]->click.handler = (ClickHandler) up_single_click_handler;
  config[BUTTON_ID_UP]->click.repeat_interval_ms = 100;
  
  config[BUTTON_ID_DOWN]->click.handler = (ClickHandler) down_single_click_handler;
  config[BUTTON_ID_DOWN]->click.repeat_interval_ms = 100;
}

void handle_second_tick(AppContextRef ctx, PebbleTickEvent *t) {
  
  static char timeText[] = "00:00:00"; // Needs to be static because it's used by the system later.
  
  PblTm currentTime;
  
  
  get_time(&currentTime);
  
  string_format_time(timeText, sizeof(timeText), "%T", &currentTime);
  
  text_layer_set_text(&timeLayer, timeText);
}

void handle_init(AppContextRef ctx) {
  
  window_init(&window, "GPS SMS Time");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorBlack);
  
  text_layer_init(&timeLayer, GRect(29, 54, 144-40 /* width */, 168-54 /* height */));
  text_layer_set_text_color(&timeLayer, GColorWhite);
  text_layer_set_background_color(&timeLayer, GColorClear);
  text_layer_set_font(&timeLayer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  
  handle_second_tick(ctx, NULL);
  
  layer_add_child(&window.layer, &timeLayer.layer);
  
  register_callbacks();
  window_set_click_config_provider(&window, (ClickConfigProvider) click_config_provider);
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .tick_info = {
      .tick_handler = &handle_second_tick,
      .tick_units = SECOND_UNIT
    },
    .messaging_info = {
      .buffer_sizes = {
        .inbound = 256,
        .outbound = 256,
      }
    }
  };
  app_event_loop(params, &handlers);
}
