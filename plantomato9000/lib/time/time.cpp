// time.cpp

#include <TimeLib.h>
#include "time.h"

int hour_val = 0;
int minute_val = 0;
int second_val = 0;

void initialize_time(){
    // write function such that the user inputs the current time once the setup initializes
    // ! necessary? or just start the bums when its exactly 12AM
}

//! check how exactly this works. need delay?
void current_time(){
    if (timeStatus() != timeNotSet){
        setTime(hour(),minute(),second(),day(),month(),year());
    }

    hour_val = hour();
    minute_val = minute();
    second_val = second();

}