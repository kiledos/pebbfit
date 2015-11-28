#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
TextLayer *text_layer;

static int s_uptime = 0; //Timer using tick_handler
static int start = 0; //Start/stop the timer


static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
 text_layer_set_text(s_time_layer, "Workout start");
 
 start = 1;
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  s_uptime = 0;
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(s_time_layer, "Workout Finished");
  start = 0;
}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  // Use a long-lived buffer
  static char s_uptime_buffer[32];
  
  // Get time since launch
  int seconds = s_uptime % 60;
  int minutes = (s_uptime % 3600) / 60;
  int hours = s_uptime / 3600;

  // Update the TextLayer
  snprintf(s_uptime_buffer, sizeof(s_uptime_buffer), "Timer: %dh %dm %ds", hours, minutes, seconds);
  text_layer_set_text(s_time_layer, s_uptime_buffer);

  // Increment s_uptime
  if (start==1)
    s_uptime++;
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(GRect(0,0, bounds.size.w, bounds.size.h));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_text(s_time_layer, "Timer: 0h 0m 0s");
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}
 

static void main_window_unload(Window *window) {
text_layer_destroy(s_time_layer);
}

static void data_handler(AccelData *data, uint32_t num_samples) {
  // Long lived buffer
  static char s_buffer[128];

  // Compose string of all data for 3 samples
  snprintf(s_buffer, sizeof(s_buffer), 
    "N X,Y,Z\n0 %d,%d,%d\n1 %d,%d,%d\n2 %d,%d,%d", 
    data[0].x, data[0].y, data[0].z, 
    data[1].x, data[1].y, data[1].z, 
    data[2].x, data[2].y, data[2].z
  );

  //Show the data
  printf("%s",s_buffer);
}

void init(void) {
  s_main_window = window_create();  
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  text_layer = text_layer_create(GRect(0, 0, 144, 20));
  window_stack_push(s_main_window, true);
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);  
 
  //Register click handlers
  window_set_click_config_provider(s_main_window, click_config_provider);
  
  //Start accel service
  uint32_t num_samples = 3;  
  accel_data_service_subscribe(num_samples, data_handler);
  accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
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
