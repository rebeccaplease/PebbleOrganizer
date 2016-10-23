#include <pebble.h>

static Window *s_window;	
	
// Keys for AppMessage Dictionary
// These should correspond to the values you defined in appinfo.json/Settings
enum {
	MESSAGE_KEY_Event = 1, // Event object will be a dict of title, start time, end time
  MESSAGE_KEY_Task = 2, // Task object will be a dict of title (string), suggested start and end time in minutes (uint16), materials for class (string)

  OBJ_KEY_Title = 3,
  OBJ_KEY_StartTime = 4,
  OBJ_KEY_EndTime = 5,
  OBJ_KEY_Materials = 6
  
};



// Write message to buffer & send
static void send_message(void) {
	DictionaryIterator *iter;
  
	AppMessageResult result = app_message_outbox_begin(&iter);
  if(result == APP_MSG_OK) {
  	dict_write_cstring(iter, MESSAGE_KEY_Event, "I'm a Pebble!");
  	dict_write_end(iter);
    // Send this message
    result = app_message_outbox_send();
    if(result != APP_MSG_OK) {
      APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int)result);
    }
    else {
      // The outbox cannot be used right now
      APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing the outbox: %d", (int)result);
    }
  }
}

// Called when a message is received from PebbleKitJS
static void in_received_handler(DictionaryIterator *iter, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "in received handler");
  Tuple *tuple = dict_read_first(iter);
  while (tuple) {
    switch (tuple->key) { 
      case OBJ_KEY_StartTime: 
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Start: %s", tuple->value->cstring); 
        int a = atoi(tuple->value->cstring);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "%d", a); 
        break;
      case OBJ_KEY_EndTime: 
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Received End: %s", tuple->value->cstring); 
        break;
      case OBJ_KEY_Title: 
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Title: %s", tuple->value->cstring); 
        break;      
      case OBJ_KEY_Materials:
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Materials: %s", tuple->value->cstring);
        break;
    }
    tuple = dict_read_next(iter);
  }
  //send_message();
}

// Called when an incoming message from PebbleKitJS is dropped
static void in_dropped_handler(AppMessageResult reason, void *context) {	
// A message was received, but had to be dropped
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped. Reason: %d", (int)reason);
}

// Called when PebbleKitJS does not acknowledge receipt of a message
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
  // The message just sent failed to be delivered
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message send failed. Reason: %d", (int)reason);
}

static void init(void) {
	s_window = window_create();
	window_stack_push(s_window, true);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "init!");
	
	// Register AppMessage handlers
	app_message_register_inbox_received(in_received_handler); 
	app_message_register_inbox_dropped(in_dropped_handler); 
	app_message_register_outbox_failed(out_failed_handler);

  // Initialize AppMessage inbox and outbox buffers with a suitable size
  const int inbox_size = 256;
  const int outbox_size = 256;
	app_message_open(inbox_size, outbox_size);
}

static void deinit(void) {
	app_message_deregister_callbacks();
	window_destroy(s_window);
}

int main( void ) {
	init();
	app_event_loop();
	deinit();
}