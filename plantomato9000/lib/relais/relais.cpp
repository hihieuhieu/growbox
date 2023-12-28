// relais.cpp

#include "relais.h"
#include "time.h"

void rel_update_gl_toggle_hour(int gl1_on, int gl1_off, int gl2_on, int gl2_off, int gl3_on, int gl3_off){
    rel_gl1_on = gl1_on;
    rel_gl1_off = gl1_off;
    rel_gl2_on = gl2_on;
    rel_gl2_off = gl2_off;
    rel_gl3_on = gl3_on;
    rel_gl3_off = gl3_off;
}

void rel_toggle_gl(int growlight_id, String mode){
    int growlight = -1;

    switch (growlight_id){
        case 1:
            growlight = DPIN_REL_GL1;
            break;
        case 2:
            growlight = DPIN_REL_GL2;
            break;
        case 3:
            growlight = DPIN_REL_GLX;
            break;
        default:
            break;
    }

    int state = (mode == "on") ? LOW : (mode == "off") ? HIGH : -1; //! think about exception handling
    digitalWrite(growlight,state);
}

void rel_update_fan_toggle_hour(int fan1_on, int fan1_off, int fan2_on, int fan2_off, int fan3_on, int fan3_off){
    rel_fan1_on = fan1_on;
    rel_fan1_off = fan1_off;
    rel_fan2_on = fan2_on;
    rel_fan2_off = fan2_off;
    rel_fan3_on = fan3_on;
    rel_fan3_off = fan3_off;
}

void rel_toggle_fan(int fan_id, String mode){
    int fan = -1;

    switch (fan_id){
        case 1:
            fan = DPIN_REL_FAN1;
            break;
        case 2:
            fan = DPIN_REL_FAN2;
            break;
        case 3:
            fan = DPIN_REL_FAN3;
            break;
        default:
            break;
    }

    int state = (mode == "on") ? LOW : (mode == "off") ? HIGH : -1; //! think about exception handling
    digitalWrite(fan,state);
}

void rel_enbl_stm(String mode){
    int state = (mode == "on") ? LOW : (mode == "off") ? HIGH : -1;
    digitalWrite(DPIN_REL_STM,state);
}

void rel_toggle_automatically(){
    int state_gl1 = (hour_val >= rel_gl1_on && hour_val < rel_gl1_off) ? LOW : HIGH;
    int state_gl2 = (hour_val >= rel_gl2_on && hour_val < rel_gl2_off) ? LOW : HIGH;
    int state_glx = (hour_val >= rel_gl3_on && hour_val < rel_gl3_off) ? LOW : HIGH;
    int state_fan1 = (hour_val >= rel_fan1_on && hour_val < rel_fan1_off) ? LOW : HIGH;
    int state_fan2 = (hour_val >= rel_fan2_on && hour_val < rel_fan2_off) ? LOW : HIGH;
    int state_fan3 = (hour_val >= rel_fan3_on && hour_val < rel_fan3_off) ? LOW : HIGH;
    
    digitalWrite(DPIN_REL_GL1,state_gl1);
    digitalWrite(DPIN_REL_GL2,state_gl2);
    digitalWrite(DPIN_REL_GLX,state_glx);
    digitalWrite(DPIN_REL_FAN1,state_fan1);
    digitalWrite(DPIN_REL_FAN2,state_fan2);
    digitalWrite(DPIN_REL_FAN3,state_fan3);

}