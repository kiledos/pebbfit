#include <pebble.h>
#include "extension.h"
static Window *s_main_window;
static TextLayer *s_time_layer;
TextLayer *text_layer;
static int value;
//static int past[5] ={0,0,0,0,0};
//static int future[5]={0,0,0,0,0}

static int s_uptime = 0; //Timer using tick_handler
static int start = 0; //Start/stop the timer
static int period = -1;
static int pv = 1; //Change Period Value Here
static int index =0;
int accarray[20][3];
static int i=0;
static int a[3]={1,2,3};
int ncounter;
static int *final;


static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
 text_layer_set_text(s_time_layer, "Workout start"); 
 start = 1;
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  s_uptime = 0;
  period = 0;
}
static void send_next_data()
{
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);

	for(int i = 0; i < ncounter; i++)
	{
		
			int value = final[i];
			Tuplet t = TupletInteger(i, value);
			dict_write_tuplet(iter, &t);
		
	}
  app_message_outbox_send();
	
	
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(s_time_layer, "Workout Finished");
  final = session();  
  start = 0;
  send_next_data();
}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
 // Increment s_uptime
  if (start==1)
  {
    s_uptime++;
    //printf("Second %d",period);
    if(period<pv)
    {
      period++;
    }
    else
    {
      appendArray(accarray,20);
      //printf("Output %d",check3);
      period=0;
      //int i;
      
      /*for (i=0;i<25;i++){
        realloc(accarray[i],0);
      }*/
      //realloc(accarray,N);
    }
  }
  
  // Use a long-lived buffer
  static char s_uptime_buffer[32];
  
  // Get time since launch
  int seconds = s_uptime % 60;
  int minutes = (s_uptime % 3600) / 60;
  int hours = s_uptime / 3600;
   
  // Update the TextLayer
  snprintf(s_uptime_buffer, sizeof(s_uptime_buffer), "Timer: %dh %dm %ds", hours, minutes, seconds);
  text_layer_set_text(s_time_layer, s_uptime_buffer);
  
  
}

static void data_handler(AccelData *data, uint32_t num_samples) {
 if (start==1)
  {
    for(i=0;i<10;i++)
    {
       index = i + (period*10);
       accarray[index][0] = data[i].x;
       accarray[index][1] = data[i].y;
       accarray[index][2] = data[i].z;
       
       //printf("abcd x:%d y:%d z:%d",accarray[index][0],accarray[index][1],accarray[index][2]);
       
    }
    
   //printf("period:%d",period);
  }
  /*
   Long lived buffer
  static char s_buffer[128];
  
  // Compose string of all data for 3 samples
  snprintf(s_buffer, sizeof(s_buffer), 
    "X,Y,Z\n x:%d,y:%d,z:%d", 
    data[0].x, data[0].y, data[0].z
  );

  //Show the data
  printf("%s",s_buffer);*/
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



void init(void) {
  s_main_window = window_create();  
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  text_layer = text_layer_create(GRect(0, 0, 144, 20));
  window_stack_push(s_main_window, true);
   
 
  //Register click handlers
  window_set_click_config_provider(s_main_window, click_config_provider);
  
  //Start accel service  
  uint32_t num_samples = 10;  
  accel_data_service_subscribe(num_samples, data_handler);
  accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler); 
}

void deinit(void) {  
   tick_timer_service_unsubscribe();  
   accel_data_service_unsubscribe();
  text_layer_destroy(text_layer);
  window_destroy(s_main_window);
}

int main(void){
  //printsomething(a,3);
  init();
  app_event_loop();
  deinit();
}
