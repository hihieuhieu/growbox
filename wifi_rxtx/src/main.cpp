/*

refer to: https://github.com/mo-thunderz/Esp32WifiPart3/blob/main/Arduino/ESP32WebserverLedControl/ESP32WebserverLedControl.ino

*/

// #include <ESP8266WiFi.h>
// #include <ESP8266WebServer.h>
// #include <SoftwareSerial.h>

// const int esp8266Tx = 2; // TX pin of ESP8266 connected to pin 2 of Arduino
// const int esp8266Rx = 3; // RX pin of ESP8266 connected to pin 3 of Arduino

// SoftwareSerial espSerial(esp8266Tx, esp8266Rx);

// void setup() {
//   Serial.begin(9600);
//   espSerial.begin(9600);
//   delay(1000);
//   espSerial.println("Hello");
// }

// void loop() {
//   // Code for ESP8266's main operations (if any)
// }

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include "webserver.h"

const char* ssid = "Vodafone-2AEC";
const char* password = "LmEemZcea9xJRk2K";

const int esp8266Tx = 2; // TX pin of ESP8266 connected to pin 2 of Arduino
const int esp8266Rx = 3; // RX pin of ESP8266 connected to pin 3 of Arduino

SoftwareSerial espSerial(esp8266Tx, esp8266Rx);

ESP8266WebServer server(80);
WebSocketsServer webSocket(81);

String webpage = "include html";

int tx_to_arduino = 99;
int rx_from_arduino = 99;

void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length);

void setup(){
  Serial.begin(115200);
  espSerial.begin(9600);

  delay(1000);
  espSerial.println("Hello");

  WiFi.begin(ssid, password);

  Serial.println("Initialize connection to WiFi. SSID: " + String(ssid));

  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }

  Serial.println("Connected. IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    server.send(200, "text/html", webpage);
  });

  server.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop(){
  server.handleClient();
  webSocket.loop();

}

