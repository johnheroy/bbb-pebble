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
static Window *loading_window;
static TextLayer *loading_text;
static GFont s_res_gothic_28_bold;
static TextLayer *title_text_layer;
static TextLayer *instructions_text_layer;

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
  text_layer_set_text(instructions_text_layer, "Press select to view upcoming bus arrivals near you");
  text_layer_set_background_color(instructions_text_layer, BACKGROUND_COLOR);
  text_layer_set_text_color(instructions_text_layer, GColorWhite);
  
  layer_add_child(window_layer, text_layer_get_layer(title_text_layer));
  layer_add_child(window_layer, text_layer_get_layer(instructions_text_layer));
}

static void main_window_unload(Window *window) {
  // Destroy Window's child Layers here
  // Destroy TextLayer
  text_layer_destroy(title_text_layer);
  text_layer_destroy(instructions_text_layer);

  // Destroy Window
  window_destroy(window);
}

static void loading_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  loading_text = text_layer_create(GRect(42, 12, 60, 100));
  text_layer_set_text(loading_text, "Loading");
  
  layer_add_child(window_layer, text_layer_get_layer(loading_text));
}

static void loading_window_unload(Window *window) {
  window_destroy(window);
}

void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  loading_window = window_create();
  window_set_window_handlers(loading_window, (WindowHandlers) {
    .load = loading_window_load,
    .unload = loading_window_unload
  });
  window_stack_push(loading_window, true);
}

void config_provider(Window *window) {
 // single click / repeat-on-hold config:
  window_single_repeating_click_subscribe(BUTTON_ID_SELECT, 1000, select_single_click_handler);
}

static void init() {
  // Create main Window
  s_main_window = window_create();
  window_set_background_color(s_main_window, BACKGROUND_COLOR);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_set_click_config_provider(s_main_window, (ClickConfigProvider) config_provider);
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
