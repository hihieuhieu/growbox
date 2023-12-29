/*

stm_glheight.h

The stm_glheight serves to adjust the growlight height using
a step motor.

*/

#ifndef STM_GLHEIGHT_H
#define STM_GLHEIGHT_H

#include <Arduino.h>
#include "config.h"

//bool stm_glheight_islimit_reached();
void stm_stage_initialization();
void stm_glheight_toggle(String);
void stm_glheight_speed(String);
void _stm_glheight_run_motor();
void stm_glheight_move(String, int);
void stm_glheight_stop();
void stm_stage_current_position();

#endif