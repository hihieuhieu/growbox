#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>

const int esp8266Tx = 2; // TX pin of ESP8266 connected to pin 2 of Arduino
const int esp8266Rx = 3; // RX pin of ESP8266 connected to pin 3 of Arduino

SoftwareSerial espSerial(esp8266Tx, esp8266Rx);

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
  delay(1000);
  espSerial.println("Hello");
}

void loop() {
  // Code for ESP8266's main operations (if any)
}
