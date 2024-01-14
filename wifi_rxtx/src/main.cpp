/*

refer to: https://github.com/mo-thunderz/Esp32WifiPart3/blob/main/Arduino/ESP32WebserverLedControl/ESP32WebserverLedControl.ino

*/

#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>

const char* ssid = "Vodafone-2AEC";
const char* password = "LmEemZcea9xJRk2K";
const int webSocketPort = 81;

WebSocketsServer webSocket = WebSocketsServer(webSocketPort);
ESP8266WebServer server(80);

void handleWebSocketMessage(uint8_t num, String message) {
  Serial.print("Received message from client #");
  Serial.print(num);
  Serial.print(": ");
  Serial.println(message);

  // Parse JSON
  DynamicJsonDocument jsonDoc(1024);  // Adjust the size based on your JSON structure
  DeserializationError error = deserializeJson(jsonDoc, message);
  
  if (error) {
    Serial.print("JSON parsing failed! Error code: ");
    Serial.println(error.c_str());
    return;
  }

  // Send each key-value pair over Serial1
  for (JsonPair keyValue : jsonDoc.as<JsonObject>()) {
    const char *key = keyValue.key().c_str();

    // Determine the data type of the value
    if (keyValue.value().is<const char*>()) {
      const char *value = keyValue.value().as<const char*>();
      Serial1.print(key);
      Serial1.print(": ");
      Serial1.println(value);
    } else if (keyValue.value().is<float>()) {
      float value = keyValue.value().as<float>();
      Serial1.print(key);
      Serial1.print(": ");
      Serial1.println(value, 6); // Assuming 6 decimal places, modify as needed
    } else if (keyValue.value().is<int>()) {
      int value = keyValue.value().as<int>();
      Serial1.print(key);
      Serial1.print(": ");
      Serial1.println(value);
    }
  }
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
  // Send the HTML content
  server.send(200, "text/html", readFile("/webinterface.html"));
}

void handleJs() {
  // Send the JS content
  server.send(200, "text/javascript", readFile("/webinterface.js"));
}

void handleCss() {
  // Send the CSS content
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
  Serial.begin(115200);
  Serial1.begin(9600);

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

  // Start WebSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  // Serve HTML/CSS/JS files
  server.on("/", HTTP_GET, handleRoot);
  server.on("/webinterface.js", HTTP_GET, handleJs);
  server.on("/webinterface.css", HTTP_GET, handleCss);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  webSocket.loop();
  server.handleClient();

  // Check for serial input
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "IP") {
      printIPAddress();
    }
  }
}

