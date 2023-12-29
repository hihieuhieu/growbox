/*

This file serves as config file as assignment of each pin to
its respectively connected hardware (e.g. DHT11). Further, it
sets pinModes.

*/

#ifndef CONFIG_H
#define CONFIG_H

#include <DHT.h>

//exchange pins by actual pins

// Temperature and humidity sensor DHT11
#define DPIN_DHT11_INSIDE 99
#define DPIN_DHT11_OUTSIDE 99
#define DHTTYPE DHT11

// Capacitive soil moisture sensor
#define APIN_CSMS_1 99
#define APIN_CSMS_2 99
#define APIN_CSMS_3 99

// Step motor pins
#define DPIN_STM_ENBL 99       // enable
#define DPIN_STM_DIR 99        // direction
#define DPIN_STM_PUL 99        // pulse
// #define DPIN_STM_CH_ENBL 99    // output voltage to check if stage has reached maximum
// #define APIN_STM_CH_U 99       // did stage reach upper bound? (short?)
// #define APIN_STM_CH_L 99       // did stage reach lower bound? (short?)
extern float stm_glheight_upper_short;
extern float stm_glheight_lower_short;
extern float stm_glheight_short_treshold;
extern bool stm_glheight_short_checkmark;   // upper or lower bound reached?
extern int stm_glheight_motor_delay;        // motor delay. higher --> slower rotation, µs
extern bool stm_glheight_motor_running;     // is motor running?

// Light barriers
#define DPIN_LB_1 99
#define DPIN_LB_2 99
#define DPIN_LB_3 99
#define DPIN_LB_4 99
#define DPIN_LB_5 99
extern int stage_position;

// Relais
#define DPIN_REL_GL1 99        // growlight
#define DPIN_REL_GL2 99        // growlight
#define DPIN_REL_GLX 99        // the cheap growlight
#define DPIN_REL_FAN1 99       // fan1
#define DPIN_REL_FAN2 99       // fan2
#define DPIN_REL_FAN3 99       // fan3
#define DPIN_REL_STM 99        // enable motor driver power supply
extern int rel_gl1_on;
extern int rel_gl1_off;
extern int rel_gl2_on;
extern int rel_gl2_off;
extern int rel_gl3_on;
extern int rel_gl3_off;

extern int rel_fan1_on;
extern int rel_fan1_off;
extern int rel_fan2_on;
extern int rel_fan2_off;
extern int rel_fan3_on;
extern int rel_fan3_off;


// ESP8266 Serial connection
#define RX_FROM_ESP8266 99
#define TX_TO_ESP8266 99

// Water pump
// ...





#endif