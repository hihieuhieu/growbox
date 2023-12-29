// csms.cpp

#include "csms.h"

float csms_level(int sensor){
    switch (sensor){
    case 1:
        analogRead(APIN_CSMS_1);
        break;
    case 2:
        analogRead(APIN_CSMS_2);
    case 3:
        analogRead(APIN_CSMS_3);
    default:
        break;
    }
}