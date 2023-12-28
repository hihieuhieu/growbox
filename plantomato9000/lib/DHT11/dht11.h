/*

dht11.h

Functions regarding the temperature / humidity sensor DHT11.
Initialization, temperature_read, humidity_read. The functions
take an object of the class DHT as input. The respective objects
are declared within the config.h file. Basically, it reads the
register of the created DHT object and uses it as input parameter
for the respective functions.

*/
#ifndef DHT11_H
#define DHT11_H

#include <Arduino.h>
#include <DHT.h>

void dht11_initialize(DHT &dht);
float dht11_temperature(DHT &dht);
float dht11_humidity(DHT &dht);

#endif