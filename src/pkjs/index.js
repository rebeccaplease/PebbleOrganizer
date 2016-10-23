// Function to send a message to the Pebble using AppMessage API
// We are currently only sending a message using the "status" appKey defined in appinfo.json/Settings
function sendMessage() {
  for(var i = 0; i < 5; i++){
    
    var event =  {"OBJ_KEY_Title":"Physics", "OBJ_KEY_StartTime": "60", "OBJ_KEY_EndTime": "120"};
    //var outer_dictionary  =  {"MESSAGE_KEY_Event1" : event1}; 
  	Pebble.sendAppMessage(event, messageSuccessHandler, messageFailureHandler);
  }
}

// Called when the message send attempt succeeds
function messageSuccessHandler() {
  console.log("Message send succeeded.");  
}

// Called when the message send attempt fails
function messageFailureHandler(err) {
  console.log("Message send failed." + JSON.stringify(err));
}

// Called when JS is ready
Pebble.addEventListener("ready", function(e) {
  console.log("JS is ready!");
  sendMessage();
});
												
// Called when incoming message from the Pebble is received
// We are currently only checking the "message" appKey defined in appinfo.json/Settings
Pebble.addEventListener("appmessage", function(e) {
  console.log("Received Message: " + e.payload.message);
});

