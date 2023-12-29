#include <Arduino.h>
#include <config.h>

#include <stdio.h>

#include "dht11.h"
#include "stm_glheight.h"
#include "time.h"
#include "relais.h"
#include "csms.h"

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
    String rx_string = Serial.readString();

    // move stage to position {1, 2, 3, 4, 5}
    // received string: "stage_to_{1,2,3,4,5}"
    if (rx_string.indexOf("stage_to_")){
      int pos = rx_string.substring(rx_string.lastIndexOf('_') + 1).toInt();
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
    // received string: "set_stage_speed_{slow, medium, fast}"
    else if (rx_string.indexOf("set_stage_speed_")){
      String speed = rx_string.substring(rx_string.lastIndexOf('_') + 1);
      stm_glheight_speed(speed);
    }
    
    // update growlight toggle hours
    // received string: "update_gl_toggle_{int}_{int}_{int}_{int}_{int}_{int}"
    // int ∈ ℕ∈[0,24]
    else if (rx_string.indexOf("update_gl_toggle_")){
      int sep_1 = rx_string.indexOf('_', 15);
      int sep_2 = rx_string.indexOf('_', sep_1 + 1);
      int sep_3 = rx_string.indexOf('_', sep_2 + 1);
      int sep_4 = rx_string.indexOf('_', sep_3 + 1);
      int sep_5 = rx_string.indexOf('_', sep_4 + 1);
      int sep_6 = rx_string.indexOf('_', sep_5 + 1);

      String str1 = rx_string.substring(sep_1 + 1, sep_2);
      String str2 = rx_string.substring(sep_2 + 1, sep_3);
      String str3 = rx_string.substring(sep_3 + 1, sep_4);
      String str4 = rx_string.substring(sep_4 + 1, sep_5);
      String str5 = rx_string.substring(sep_5 + 1, sep_6);
      String str6 = rx_string.substring(sep_6 + 1);

      rel_gl1_on = str1.toInt();
      rel_gl1_off = str2.toInt();
      rel_gl2_on = str3.toInt();
      rel_gl2_off = str4.toInt();
      rel_gl3_on = str5.toInt();
      rel_gl3_off = str6.toInt();
    }

    // toggle growlight manually
    // received string: "manually_toggle_growlight_{on, off}_{1, 2, 3}"
    else if (rx_string.indexOf("manually_toggle_growlight_")){
      int growlight = rx_string.substring(rx_string.lastIndexOf('_') + 1).toInt();
      int state = (rx_string.indexOf("on")) ? LOW : (rx_string.indexOf("off")) ? HIGH : -1;
      switch (growlight){
        case 1:
          digitalWrite(DPIN_REL_GL1, state);
          break;
        case 2:
          digitalWrite(DPIN_REL_GL2, state);
          break;
        case 3:
          digitalWrite(DPIN_REL_GLX, state);
          break;
        default:
          break;
      }
    }

    // toggle fans manually
    // received string: "manually_toggle_fan_{on, off}_{1, 2, 3}"
    else if (rx_string.indexOf("manually_toggle_fan_")){
      int fan = rx_string.substring(rx_string.lastIndexOf('_') + 1).toInt();
      int state = (rx_string.indexOf("on")) ? LOW : (rx_string.indexOf("off")) ? HIGH : -1;
      switch (fan){
        case 1:
          digitalWrite(DPIN_REL_FAN1, state);
          break;
        case 2:
          digitalWrite(DPIN_REL_FAN2, state);
          break;
        case 3:
          digitalWrite(DPIN_REL_FAN3, state);
          break;
        default:
          break;
      }
    }

    // readout temperature and humidity
    // received string: "read_dht11"
    // transmitted string: "inside_{temperature_in}_{humidity_in}_outside_{temperature_out}_{humidity_out}"
    else if (rx_string.indexOf("read_dht11")){
      float temp_inside = dht11_temperature(dht_inside);
      float temp_outside = dht11_temperature(dht_outside);
      float hum_inside = dht11_humidity(dht_inside);
      float hum_outside = dht11_humidity(dht_outside);
      char string[50]; 
      sprintf(string, "inside_%.1f_%.1f_outside_%.1f_%.1f", (double)temp_inside, (double)hum_inside, (double)temp_outside, (double)hum_outside);
      Serial.println(string);
    }

    // readout soil moisture
    // received string: "read_csms"
    // transmitted string: "csms1_{val1}_csms2_{val2}_csms3_{val3}_"
    else if (rx_string.indexOf("read_csms")){
      float csms1 = csms_level(1);
      float csms2 = csms_level(2);
      float csms3 = csms_level(3);
      char string[50]; 
      sprintf(string, "csms1_%.1f_csms2_%.1f_csms3_%.1f", (double)csms1, (double)csms2, (double)csms3);
      Serial.println(string);
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