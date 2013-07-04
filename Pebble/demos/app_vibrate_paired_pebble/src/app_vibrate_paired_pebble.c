#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

// app_message constants

// keys
enum {
  MSG_IN_KEY = 0x00,
  MSG_OUT_KEY = 0x01
};

// cmds
enum {
  MSG_CMD_VIBE = 0x00,
  MSG_CMD_UP = 0x01,
  MSG_CMD_DOWN = 0x02
} MessageCommand;

#define MY_UUID {0x7B, 0xD1, 0x03, 0xD3, 0x0F, 0x87, 0x4D, 0x31, 0xAE, 0xF8, 0xF5, 0x23, 0x7A, 0x79, 0xC7, 0x2E}
PBL_APP_INFO(MY_UUID,
             "Happy Hour", "Pebble",
             3, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

Window s_window;
TextLayer s_time_layer;
static bool callbacks_registered;
static AppMessageCallbacksNode app_callbacks;

static void app_send_failed(DictionaryIterator* failed, AppMessageResult reason, void* context) {
  // TODO: error handling
}

static void app_received_msg(DictionaryIterator* received, void* context) {
	Tuple* cmd_tuple = dict_find(received, MSG_IN_KEY);
  
	if (cmd_tuple == NULL) {
    // TODO: handle invalid message
		return;
	}
	
  uint8_t cmd = cmd_tuple->value->int8;
  
	if (cmd == MSG_CMD_VIBE) {
    vibes_long_pulse();
	}
}

static void app_dropped_msg(void* context, AppMessageResult reason) {
	// TODO: error handling
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
        .in_received = app_received_msg,
				.in_dropped = app_dropped_msg,
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
  Tuplet value = TupletInteger(MSG_OUT_KEY, cmd);
  
  DictionaryIterator *iter;
  app_message_out_get(&iter);
  
  if (iter == NULL) {
    return;
  }
  
  dict_write_tuplet(iter, &value);
  dict_write_end(iter);
  
  app_message_out_send();
  app_message_out_release();
}

void up_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  
  send_cmd(MSG_CMD_UP);
}

void down_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  
  send_cmd(MSG_CMD_DOWN);
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
  
  text_layer_set_text(&s_time_layer, timeText);
}

void handle_init(AppContextRef ctx) {
  
  window_init(&s_window, "Happy Hour");
  window_stack_push(&s_window, true /* Animated */);
  window_set_background_color(&s_window, GColorBlack);
  
  text_layer_init(&s_time_layer, GRect(29, 54, 144-40 /* width */, 168-54 /* height */));
  text_layer_set_text_color(&s_time_layer, GColorWhite);
  text_layer_set_background_color(&s_time_layer, GColorClear);
  text_layer_set_font(&s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  
  handle_second_tick(ctx, NULL);
  
  layer_add_child(&s_window.layer, &s_time_layer.layer);
  
  register_callbacks();
  window_set_click_config_provider(&s_window, (ClickConfigProvider) click_config_provider);
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
