/*

relais.h

This file serves for switching the 8 channels of the relais.

*/

#ifndef RELAIS_H
#define RELAIS_H

#include <Arduino.h>
#include "time.h"
#include "config.h"

void rel_update_gl_toggle_hour(int, int, int, int, int, int);
void rel_toggle_gl(int, String);
void rel_update_fan_toggle_hour(int, int, int, int, int, int);
void rel_toggle_fan(int, String);
void rel_enbl_stm(String);
void rel_toggle_automatically();

#endif