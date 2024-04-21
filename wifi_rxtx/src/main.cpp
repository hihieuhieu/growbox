/*

refer to: https://github.com/mo-thunderz/Esp32WifiPart3/blob/main/Arduino/ESP32WebserverLedControl/ESP32WebserverLedControl.ino

*/

#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>
#include <EEPROM.h>
#include <NTPClient.h>
#include <WiFiUdp.h>


const char* ssid = "Vodafone-2AEC";
const char* password = "LmEemZcea9xJRk2K";

const char* ntpServer = "pool.ntp.org"; // NTP server
const long  gmtOffset_sec = 3600; // Offset from GMT in seconds (Eastern European Time: GMT+2)
const int   daylightOffset_sec = 0; // Daylight offset in seconds

WebSocketsServer webSocket = WebSocketsServer(81);
ESP8266WebServer server(80);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, gmtOffset_sec, daylightOffset_sec);

void handleWebSocketMessage(uint8_t num, String message) {
  // Serial.print("Send following message: ");
  Serial.println(message);
}

String readFile(String path) {
  String content = "";
  if (LittleFS.exists(path)) {
    File file = LittleFS.open(path, "r");
    if (file) {
      content = file.readString();
      file.close();
    }
  }
  return content;
}

void handleRoot() {
  server.send(200, "text/html", readFile("/webinterface.html"));
}

void handleJs() {
  server.send(200, "text/javascript", readFile("/webinterface.js"));
}

void handleCss() {
  server.send(200, "text/css", readFile("/webinterface.css"));
}

void handleNotFound() {
  server.send(404, "text/plain", "Not Found");
}

void printIPAddress() {
  Serial.println("WebSocket Server IP Address:");
  Serial.println(WiFi.localIP());
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_CONNECTED:
      Serial.printf("[%u] WebSocket Connected\n", num);
      break;
    case WStype_DISCONNECTED:
      Serial.printf("[%u] WebSocket Disonnected\n", num);
      break;
    case WStype_TEXT:
      handleWebSocketMessage(num, String((char *)payload));
      break;
  }
}

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize LittleFS (SPIFFS)
  if (!LittleFS.begin()) {
    Serial.println("Failed to mount LittleFS");
    return;
  }


  // Serve HTML/CSS/JS files
  server.on("/", HTTP_GET, handleRoot);
  server.on("/webinterface.js", HTTP_GET, handleJs);
  server.on("/webinterface.css", HTTP_GET, handleCss);
  server.onNotFound(handleNotFound);
  server.begin();

  // Start WebSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
  server.handleClient();

  // timeClient.update();

  // Get the current time
  // unsigned long epochTime = timeClient.getEpochTime(); // Epoch time (seconds since January 1, 1970)
  // Serial.println(epochTime); // Print epoch time (for testing)

  // Serial.flush();
  if (Serial.available() > 0) {
    String message = Serial.readStringUntil('\n');
    // Serial.println(message);

    webSocket.broadcastTXT(message);

    if (message == "IP") {
      printIPAddress();
    }
  }

  delay(1000);
}

