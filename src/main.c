#include <pebble.h>

static Window *s_main_window;
TextLayer *text_layer;

static void main_window_load(Window *window) {

}

static void main_window_unload(Window *window) {

}

void init(void) {
  s_main_window = window_create();
  static TextLayer *s_time_layer;
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  text_layer = text_layer_create(GRect(0, 0, 144, 20));
  window_stack_push(s_main_window, true);
}

void deinit(void) {
  text_layer_destroy(text_layer);
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
