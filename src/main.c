/*
 * main.c
 * Sets up a Window object and pushes it onto the stack.
 */

#include <pebble.h>
  
#ifdef PBL_COLOR
  #define BACKGROUND_COLOR GColorBlue
#else
  #define BACKGROUND_COLOR GColorBlack
#endif

static Window *s_main_window;
static GFont s_res_gothic_28_bold;
static TextLayer *title_text_layer;
static TextLayer *instructions_text_layer;

enum {
  STOPS_KEY = 0x0,
};

static void request_stops(void) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "REquesting logz!");
  
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

//   if (!iter) {
//     // Error creating outbound message
//     APP_LOG(APP_LOG_LEVEL_DEBUG, "err creating outbound message");
//     return;
//   }

  int value = 1;
  dict_write_int(iter, 0, &value, sizeof(int), true);
  dict_write_end(iter);

  app_message_outbox_send();
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Message received!");
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void main_window_load(Window *window) {  
  // Get the root layer
  Layer *window_layer = window_get_root_layer(window);
  //GRect bounds = layer_get_bounds(window_layer);
  
  s_res_gothic_28_bold = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);

  // Create title.
  title_text_layer = text_layer_create(GRect(42, 12, 60, 100));
  text_layer_set_text(title_text_layer, "Big Blue Bus");
  text_layer_set_background_color(title_text_layer, BACKGROUND_COLOR);
  text_layer_set_text_color(title_text_layer, GColorWhite);
  text_layer_set_font(title_text_layer, s_res_gothic_28_bold);
  text_layer_set_text_alignment(title_text_layer, GTextAlignmentCenter);
  
  // Create instructions (screen is 168 x 144 pixels).
  instructions_text_layer = text_layer_create(GRect(8, 116, 128, 45));
  text_layer_set_text(instructions_text_layer, "Press wait while we load nearby bus data...");
  text_layer_set_background_color(instructions_text_layer, BACKGROUND_COLOR);
  text_layer_set_text_color(instructions_text_layer, GColorWhite);
  
  layer_add_child(window_layer, text_layer_get_layer(title_text_layer));
  layer_add_child(window_layer, text_layer_get_layer(instructions_text_layer));
  
  // Send an empty app message.
  request_stops();
}

static void main_window_unload(Window *window) {
  // Destroy Window's child Layers here
  text_layer_destroy(title_text_layer);
  text_layer_destroy(instructions_text_layer);
}

static void init() {
  // Register app message handlers
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Init buffer
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  // Create main Window
  s_main_window = window_create();
  window_set_background_color(s_main_window, BACKGROUND_COLOR);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
  
  
}

static void deinit() {
  // Destroy main Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
