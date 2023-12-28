#include <Arduino.h>
#include <config.h>
#include "dht11.h"
#include "stm_glheight.h"
#include "time.h"
#include "relais.h"

// Initialize DHT objects
DHT dht_inside(DPIN_DHT11_INSIDE, DHTTYPE);
DHT dht_outside(DPIN_DHT11_OUTSIDE, DHTTYPE);

void initialize_pins();

void setup() {
  // initialize required pins as output
  initialize_pins();
  // initialize DHT11 sensors
  dht11_initialize(dht_inside);
  dht11_initialize(dht_outside);

  Serial.begin(9600);
}

void loop() {
  /*
  Run continuously
  */
  current_time();
  rel_toggle_automatically();


  /*
  User inputs
  */
  if (Serial.available() > 0){
    String userInput = Serial.readString();

    // move stage to position {1, 2, 3, 4, 5}
    if (userInput.indexOf("stage_up_to_")){
      int travel_distance_up = userInput.substring(userInput.lastIndexOf('_') + 1).toInt();
      rel_enbl_stm("on");
      stm_glheight_toggle("on");
      delay(500);
      stm_glheight_move("up", );

      
    }

  }
  
}

// pin modes
void initialize_pins(){
    pinMode(DPIN_STM_ENBL,OUTPUT);
    pinMode(DPIN_STM_DIR, OUTPUT);
    pinMode(DPIN_STM_PUL, OUTPUT);
    pinMode(DPIN_STM_CH_ENBL, OUTPUT);
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