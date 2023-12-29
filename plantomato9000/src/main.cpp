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
// void initialize_values();

void setup() {
  // initialize required pins as output
  initialize_pins();
  // // initialize values
  // initialize_values();
  // initialize stage position
  stm_stage_initialization();
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
    // sent string: "stage_to_{1,2,3,4,5}"
    if (userInput.indexOf("stage_to_")){
      int pos = userInput.substring(userInput.lastIndexOf('_') + 1).toInt();
      if (pos >= 1 && pos <= 5){
        rel_enbl_stm("on");
        stm_glheight_toggle("on");
        delay(500);
        if (stage_position < pos){
          stm_glheight_move("up", pos);
        }
        else if (stage_position > pos){
          stm_glheight_move("down", pos);
        }
        rel_enbl_stm("off");
        stm_glheight_toggle("off");
      } else {printf("Not a valid position.");}
    }

    // set motor speed {slow, medium, fast}
    // sent string: "set_stage_speed_{slow, medium, fast}"
    else if (userInput.indexOf("set_stage_speed_")){
      String speed = userInput.substring(userInput.lastIndexOf('_') + 1);
      stm_glheight_speed(speed);
    }
    

  }
  
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


// void initialize_values(){
//   stage_position = -1; // -1 upon initialization. {1, 2, 3, 4, 5} when light barrier is reached

//   rel_gl1_on = 8;
//   rel_gl1_off = 20;
//   rel_gl2_on = 8;
//   rel_gl2_off = 20;
//   rel_gl3_on = 6;
//   rel_gl3_off = 8;

//   rel_fan1_on = 8;
//   rel_fan1_off = 20;
//   rel_fan2_on = 8;
//   rel_fan2_off = 20;
//   rel_fan3_on = 6;
//   rel_fan3_off = 8;
// }