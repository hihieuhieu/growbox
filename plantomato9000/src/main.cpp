#include <Arduino.h>
#include <ArduinoJson.h>
#include <config.h>

#include <stdio.h>

#include <SPI.h>
#include <RTClib.h>

#include "dht11.h"
#include "stm_glheight.h"
#include "time.h"
#include "relais.h"
#include "csms.h"

// Initialize DHT objects
DHT dht_inside(DPIN_DHT11_INSIDE, DHTTYPE);
DHT dht_outside(DPIN_DHT11_OUTSIDE, DHTTYPE);

DynamicJsonDocument doc(1024);

int gl1_on = 8*60;
int gl2_on = 11*60;
int  gl3_on = 18*60;
int  gl1_off = 20*60;
int  gl2_off = 16*60;
int  gl3_off = 20*60;

int fan_on = 10; //mins
int fan_off = 20;

int time_now = 1*60+39; //! init every time after reuploading this code to current time (hour*60+minute)
unsigned long previousMillis = 0;
const long interval = 60000; // once per min


void initialize_pins();
void act_stage_position();
void act_toggle_growlight();
void act_toggle_fan();
void send_dht_csms_data();
void update_growlight_toggle_times();
void update_fan_toggle_interval();
void auto_toggle_growlight();
void auto_toggle_fan();

void setup() {
  // initialize required pins as output
  Serial.begin(9600);
  Serial1.begin(9600);
  initialize_pins();
  Serial.println("Pins initialized.");
  // initialize stage position
  // stm_stage_initialization();
  // initialize DHT11 sensors
  dht11_initialize(dht_inside);
  dht11_initialize(dht_outside);
  Serial.println("I AM READY!!!!!!");
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    time_now = time_now+1;

    if (time_now > 24*60){
      time_now = 0;
    }
  Serial.println(time_now);
  }


  if (Serial1.available() > 0){
    String rx_string = Serial1.readStringUntil('\n');
    Serial.println("Received JSON string: " + rx_string);

    DeserializationError error = deserializeJson(doc, rx_string);

    if (error){
      Serial.print("Error parsing JSON: ");
      Serial.println(error.c_str());
    }
    else {
      const char *message_type = doc["message_type"];
      // debugging
      // Serial.println(message_type);

      if (strcmp(message_type, "act_stage_position") == 0)
      {

      }
      else if (strcmp(message_type, "act_toggle_growlight") == 0)
      {
        //debugging
        // Serial.println("act_toggle_growlight");
        act_toggle_growlight();
      }
      else if (strcmp(message_type, "act_toggle_fan") == 0)
      {
        act_toggle_fan();
      }
      // else if (strcmp(message_type, "config_auto_fan_toggle") == 0)
      // {

      // }
      else if (strcmp(message_type, "config_growlight_toggle_times") == 0)
      {
        update_growlight_toggle_times();
      }
      else if (strcmp(message_type, "config_fan_onoff_interval") == 0)
      {
        update_fan_toggle_interval();
      }
      else if (strcmp(message_type, "query_dht_csms_data") == 0)
      {
        send_dht_csms_data();
      }
    }

  }
  auto_toggle_growlight();
  auto_toggle_fan();
}

// pin modes
void initialize_pins(){
    pinMode(DPIN_STM_ENBL,OUTPUT);
    pinMode(DPIN_STM_DIR, OUTPUT);
    pinMode(DPIN_STM_PUL, OUTPUT);
    // pinMode(DPIN_STM_CH_ENBL, OUTPUT);
    pinMode(DPIN_REL_GL1, OUTPUT);
    pinMode(DPIN_REL_GL2, OUTPUT);
    pinMode(DPIN_REL_GLX, OUTPUT);
    pinMode(DPIN_REL_FAN1, OUTPUT);
    pinMode(DPIN_REL_FAN2, OUTPUT);
    pinMode(DPIN_REL_FAN3, OUTPUT);
    pinMode(DPIN_REL_STM, OUTPUT);
    pinMode(DPIN_LB_1, INPUT);
    pinMode(DPIN_LB_2, INPUT);
}


void act_stage_position(){
  int stage_position = doc["stage_position"];
  Serial.println(stage_position);
  if (stage_position >= 1 && stage_position <= 4){
      // rel_enbl_stm("on");
      stm_glheight_toggle("on");

      int current_position = stm_stage_current_position();

      delay(500);
      if (current_position < stage_position){
        stm_glheight_move("up",stage_position);
      }
      else if (current_position > stage_position){
        stm_glheight_move("down",stage_position);
      }
      // rel_enbl_stm("off");
      stm_glheight_toggle("off");
  } else {printf("Not a valid stage position.");}
}


void act_toggle_growlight(){
  int growlight = doc["growlight"];
  const char *state = doc["state"];
  Serial.println(growlight);
  Serial.println(state);
  rel_toggle_gl(growlight, state);
}


void act_toggle_fan(){
  int fan = doc["fan"];
  const char *state = doc["state"];
  Serial.println(fan);
  Serial.println(state);
  rel_toggle_fan(fan, state);
}

void send_dht_csms_data(){
  float temperature_inside = dht11_temperature(dht_inside);
  float temperature_outside = dht11_temperature(dht_outside);
  float humidity_inside = dht11_humidity(dht_inside);
  float humidity_outside = dht11_humidity(dht_outside);
  float soil_moisture_1 = csms_level(1);
  float soil_moisture_2 = csms_level(2);
  float soil_moisture_3 = csms_level(3);

  StaticJsonDocument<128> jsonDocument;
  jsonDocument["temperature_inside"] = temperature_inside;
  jsonDocument["temperature_outside"] = temperature_outside;
  jsonDocument["humidity_inside"] = humidity_inside;
  jsonDocument["humidity_outside"] = humidity_outside;
  jsonDocument["soil_moisture_1"] = soil_moisture_1;
  jsonDocument["soil_moisture_2"] = soil_moisture_2;
  jsonDocument["soil_moisture_3"] = soil_moisture_3;

  String jsonString;
  serializeJson(jsonDocument, jsonString);
  Serial1.println(jsonString);

}

void update_growlight_toggle_times(){
  gl1_on = doc["gl1_on"];
  gl2_on = doc["gl2_on"];
  gl3_on = doc["gl3_on"];
  gl1_off = doc["gl1_off"];
  gl2_off = doc["gl2_off"];
  gl3_off = doc["gl3_off"];
  // Serial.println(gl1_on);
  // Serial.println(gl1_off);

}

void update_fan_toggle_interval(){
  fan_on = doc["fan_on"];
  fan_off = doc["fan_off"];
}

void auto_toggle_growlight(){
  // Serial.println("Exc autotogglegl");
  if (time_now > gl1_on && time_now < gl1_off ){
    rel_toggle_gl(1, "on");
    // Serial.println("Turned on 1");
  }
  if (time_now > gl2_on && time_now < gl2_off){
    rel_toggle_gl(2, "on");
    // Serial.println("Turned on 2");
  }
  if (time_now > gl3_on && time_now < gl3_off){
    rel_toggle_gl(3, "on");
    // Serial.println("Turned on 3");
  }
  if (time_now > gl1_off){
    rel_toggle_gl(1, "off");
    // Serial.println("Turned off 1");
  }
  if (time_now > gl2_off){
    rel_toggle_gl(2, "off");
  }
  if (time_now > gl3_off){
    rel_toggle_gl(3, "off");
  }
}

void auto_toggle_fan(){

}

