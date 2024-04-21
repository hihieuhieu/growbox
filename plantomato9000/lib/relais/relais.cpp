// relais.cpp

#include "relais.h"
// #include "time.h"


void rel_toggle_gl(int growlight_id, String mode){
    int growlight = -1;
    // Serial.println("Turned on a GL");

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
