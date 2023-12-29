// stm_glheit.cpp

#include "stm_glheight.h"

// float stm_glheight_upper_short = 1000;    // todo: check this value
// float stm_glheight_lower_short = 1000;    // todo: check this value
// float stm_glheight_short_treshold = 50 ;  // todo: check on this treshold
// bool stm_glheight_short_checkmark = false;
int stm_glheight_motor_delay = 200;      //µs
bool stm_glheight_motor_running = false;


/*
bool stm_glheight_islimit_reached(){
    // did growlight reach its minimum or maximum height?
    stm_glheight_upper_short = analogRead(APIN_STM_CH_U);
    stm_glheight_lower_short = analogRead(APIN_STM_CH_L);
    if (stm_glheight_upper_short < stm_glheight_short_treshold || stm_glheight_lower_short < stm_glheight_short_treshold){
        stm_glheight_short_checkmark = true;
    } else {stm_glheight_short_checkmark = false;}
    return stm_glheight_short_checkmark;
}
*/

void stm_stage_initialization(){
    digitalWrite(DPIN_STM_DIR,HIGH); // todo check if HIGH relates to up-movement
    stm_glheight_motor_running = true;
    while (stm_glheight_motor_running){
        _stm_glheight_run_motor();
        stm_stage_current_position();
        if (stage_position != -1){
            stm_glheight_stop();
            break;
        }
    }
}

void stm_stage_current_position(){
    if (digitalRead(DPIN_LB_1) == LOW){
        stage_position = 1;
    }
    else if (digitalRead(DPIN_LB_2) == LOW){
        stage_position = 2;
    }
    else if (digitalRead(DPIN_LB_3) == LOW){
        stage_position = 3;
    }
    else if (digitalRead(DPIN_LB_4) == LOW){
        stage_position = 4;
    }
    else if (digitalRead(DPIN_LB_5) == LOW){
        stage_position = 5;
    }
    else {
        stage_position = -1;
    }
}

// toggle motor enbl
void stm_glheight_toggle(String mode){
    int state = (mode == "on") ? LOW : (mode == "off") ? HIGH : -1;
    digitalWrite(DPIN_STM_ENBL, state ? HIGH : LOW);
}

void stm_glheight_speed(String speed){
    // adjust rpm of motor: higher delay_time -> slower motor
    if (speed == "slow"){
        stm_glheight_motor_delay = 1000;
    }
    else if (speed == "medium"){
        stm_glheight_motor_delay = 500;
    }
    else if (speed == "fast"){
        stm_glheight_motor_delay = 100;
    }
    else {printf("Not a valid speed statement.");}
}

/*
Move stage up or down

Parameters: 
    direction: {"up", "down"}
*/
void stm_glheight_move(String direction, int position){
    // if direction was set to "up", pin_value will get assigned HIGH. else LOW. -1 as placeholder value for any other condition
    int pin_value = (direction == "up") ? HIGH : ((direction == "down") ? LOW : -1); // todo : set HIGH or LOW depending on actual movement
    digitalWrite(DPIN_STM_DIR, pin_value); 
    stm_glheight_motor_running = true;

    while (stm_glheight_motor_running){
        _stm_glheight_run_motor();

        // if user query: Stop motor
        if (Serial.available() > 0 && Serial.read() == 'S'){ // todo: rewrite such that it fits with RXTX with ESP8266
            stm_glheight_stop();
            break;
        }

        stm_stage_current_position();

        if (stage_position == position){
            stm_glheight_stop();
        }
/*
        // growlight reached maximum
        if (stm_glheight_islimit_reached() == true){ // todo: check case: if there is a contact, i.e. growlight is at a maximum position, does it break instantly?
            stm_glheight_stop();
            break;
        }
*/

    }
}

/*
Start motor movement
*/
void _stm_glheight_run_motor(){
    digitalWrite(DPIN_STM_PUL, HIGH);
    delayMicroseconds(stm_glheight_motor_delay);
    digitalWrite(DPIN_STM_PUL, LOW);
    delayMicroseconds(stm_glheight_motor_delay);
}

/*
Stop motor movement
*/
void stm_glheight_stop(){
    stm_glheight_motor_running = false;
}
