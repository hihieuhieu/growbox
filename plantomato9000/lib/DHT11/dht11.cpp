// dht11.cpp

#include "dht11.h"

// initialize sensor
void dht11_initialize(DHT &dht){
    dht.begin();
}

// read temperature
float dht11_temperature(DHT &dht){
    return dht.readTemperature();
}

// read humidity
float dht11_humidity(DHT &dht){
    return dht.readHumidity();
}
