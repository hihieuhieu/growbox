#include "webserver.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>


// client --> server
// receive data from the client
void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length){
  switch (type){
  case WStype_DISCONNECTED:
    Serial.println("Client " + String(num) + "disconnected.");
    break;
  
  case WStype_CONNECTED:
    Serial.println("Client " + String(num) + "connected.");

    // send Jsons with all current values (e.g. GL1 ON) to all newly connected clients
    //sendJson();
    break;
  
  // if client has sent data, type == WStype_TEXT
  // doc contains information
  case WStype_TEXT:
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    else {
      // {growlight, fan, dht11, csms, growlight_height, }
      const char* component = doc["type"];
      if (component == "growlight"){
        // do sth
      }
      else if (component == "fan"){
        // do sth
      }
      const int component_number = doc["value"];
      const char* component_toggle = doc["toggle"];
      const float component_value = doc["value"];

      // include all the stuff what should happen when e.g. button 1 is pressed. I.e. what is printed to serial
      
    }
    Serial.println("");
    break;
  }
}

// server --> client
// send JSON string to the client
void sendJson(String l_type, String l_value) {
    String jsonString = "";                           // create a JSON string for sending data to the client
    StaticJsonDocument<200> doc;                      // create JSON container
    JsonObject object = doc.to<JsonObject>();         // create a JSON Object
    object["type"] = l_type;                          // write data into the JSON object -> I used "type" to identify if LED_selected or LED_intensity is sent and "value" for the actual value
    object["value"] = l_value;
    serializeJson(doc, jsonString);                // convert JSON object to string
    webSocket.broadcastTXT(jsonString);               // send JSON string to all clients
}