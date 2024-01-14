// csms.cpp

#include "csms.h"

float csms_level(int sensor){
    switch (sensor){
        case 1:
            return analogRead(APIN_CSMS_1);
        case 2:
            return analogRead(APIN_CSMS_2);
        case 3:
            return analogRead(APIN_CSMS_3);
        default:
            return -99999;
        }
}
