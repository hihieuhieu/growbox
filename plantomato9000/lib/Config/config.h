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
#define DPIN_DHT11_INSIDE 51
#define DPIN_DHT11_OUTSIDE 53
#define DHTTYPE DHT11

// Capacitive soil moisture sensor
#define APIN_CSMS_1 13
#define APIN_CSMS_2 14
#define APIN_CSMS_3 15


// Step motor pins
#define DPIN_STM_ENBL 8           //! change for mega. enable
#define DPIN_STM_DIR 9            //! change for mega. direction
#define DPIN_STM_PUL 10           //! change for mega. pulse
extern int stm_glheight_motor_delay;        // motor delay. higher --> slower rotation, µs
extern bool stm_glheight_motor_running;     // is motor running?

// Light barriers
#define DPIN_LB_1 6             //! change for mega.
#define DPIN_LB_2 7             //! change for mega.
#define DPIN_LB_3 99
#define DPIN_LB_4 99
extern int stage_position;

// Relais
#define DPIN_REL_GL1 23        // growlight
#define DPIN_REL_GL2 25        // growlight
#define DPIN_REL_GLX 27        // the cheap growlight
#define DPIN_REL_FAN1 29       // fan1
#define DPIN_REL_FAN2 31       // fan2
#define DPIN_REL_FAN3 33       // fan3
#define DPIN_REL_STM 35        // enable motor driver power supply
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