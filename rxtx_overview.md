<!-- ## Arduino Mega -> ESP8266: Strings on serial communication (UART)
- `inside_{temp_in}_{humd_in}_outside_{temp_out}_{humd_out}`
    - String, DHT11 data
- `csms1_{moist1}_csms2_{moist2}_csms3_{moist3}`
    - String, CSMS data


## ESP8266 -> Arduino Mega: Strings on serial communication (UART)
- `stage_to_{1,2,3,4,5}`
    - move stage to position x
- `set_stage_speed_{slow,medium,fast}`
    - set stage movement speed
- `update_gl_toggle_{int}_{int}_{int}_{int}_{int}_{int}_{int}_{int}`
    - update growlight toggle times. {GL1_ON, GL1_OFF, GL2_ON, GL2_OFF, GL3_ON, GL3_OFF}. int ∈ ℕ∈[0,24]
- `manually_toggle_growlight_{on,off}_{1,2,3}`
    - toggle growlight manually
- `update_fan_toggle_{int}_{int}_{int}_{int}_{int}_{int}_{int}_{int}`
    - update growlight toggle times. {FAN1_ON, FAN1_OFF, FAN2_ON, FAN2_OFF, FAN3_ON, FAN3_OFF}. int ∈ ℕ∈[0,24]
- `manually_toggle_fan_{on,off}_{1,2,3}`
    - toggle fan manually
- `read_dht11`
    - query read all dht11
- `read_csms`
    - query read all csms

# OR TURN ALL STRINGS TO JSON OBJECTS -->

# Communication interface and methods

- Arduino <--> ESP8266: UART, JSON strings, serial connection
- ESP8266 <--> Client: websockets, JSON strings, WiFi

## Send JSON strings Arduino <--> ESP8266:
```
...
Serial.begin(9600);
...

// create JSON document
StaticJsonDocument<200> doc;
// write keyvalues into keys
object["key1"] = keyval1;
object["key2"] = "keyval2";
// send over serial connection 'Serial'
serializeJson(doc, Serial);
```

## Receive JSON strings Arduino <--> ESP8266:
```
...
Serial.begin(9600);
...
if (Serial.available()>0){
    // create JSON document. 
    // Must be larger than senders, as it must store more strings
    StaticJsonDocument<300> doc;
    // Read JSON document received on port 'Serial'
    // included exception handling
    DeserializationError err = deserializeJson(doc, Serial1);
    if (err == DeserializationError:Ok){
        // integer expected
        keyval1 = doc["key1"].as<int>();
        // String or so expected. CHECK!!
        keyval2 = doc["key2"].as<long>();
    }
    else {
        // Print error to the "debug" serial port
        Serial.print("deserializeJson() returned ");
        Serial.println(err.c_str());
    
        // Flush all bytes in the serial port buffer
        while (Serial.available() > 0)
            Serial.read();
    }
}
```

## Send JSON strings ESP8266 --> Client
```
void sendJson(String l_type, String l_value) {
    String jsonString = "";                           // create a JSON string for sending data to the client
    StaticJsonDocument<200> doc;                      // create JSON container
    JsonObject object = doc.to<JsonObject>();         // create a JSON Object
    object["type"] = l_type;                          // write data into the JSON object -> I used "type" to identify if LED_selected or LED_intensity is sent and "value" for the actual value
    object["value"] = l_value;
    serializeJson(doc, jsonString);                // convert JSON object to string
    webSocket.broadcastTXT(jsonString);               // send JSON string to all clients
}
```

## Arduino Mega --> ESP8266 | ESP8266 --> Client
- DHT11 data:
    - `object["temp_in"] = temp_in`
    - `object["humd_in"] = humd_in`
    - `object["temp_out"] = temp_out`
    - `object["humd_out"] = humd_out`
- CSMS data:
    - `object["csms1"] = moist1`
    - `object["csms2"] = moist2`
    - `object["csms3"] = moist3`

## Client --> ESP8266 | ESP8266 --> Arduino Mega
- Move growlight stage
    - `object["stage_position"] = {1,2,3,4,5}`
- Set stage speed
    - `object["stage_speed"] = {"slow", "medium", "fast"}`
- Update growlight toggle times
    - `object["gl1_on"] = {int | int ∈ ℕ ∧ int ∈ [0,24]}`
    - `object["gl1_off"] = {int | int ∈ ℕ ∧ int ∈ [0,24]}`
    - `object["gl2_on"] = {int | int ∈ ℕ ∧ int ∈ [0,24]}`
    - `object["gl2_off"] = {int | int ∈ ℕ ∧ int ∈ [0,24]}`
    - `object["gl3_on"] = {int | int ∈ ℕ ∧ int ∈ [0,24]}`
    - `object["gl4_off"] = {int | int ∈ ℕ ∧ int ∈ [0,24]}`
- Manually toggle growlight
    - `object["growlight"] = {1,2,3}`
    - `object["state"] = {"on","off"}`
- Update fan toggle times
    - `object["fan1_on"] = {int | int ∈ ℕ ∧ int ∈ [0,24]}`
    - `object["fan1_off"] = {int | int ∈ ℕ ∧ int ∈ [0,24]}`
    - `object["fan2_on"] = {int | int ∈ ℕ ∧ int ∈ [0,24]}`
    - `object["fan2_off"] = {int | int ∈ ℕ ∧ int ∈ [0,24]}`
    - `object["fan3_on"] = {int | int ∈ ℕ ∧ int ∈ [0,24]}`
    - `object["fan4_off"] = {int | int ∈ ℕ ∧ int ∈ [0,24]}`
- Manually toggle fan
    - `object["fan"] = {1,2,3}`
    - `object["state"] = {"on","off"}`
- Query DHT11 measurement
    - `object["DHT11"] = 1`
- Query CSMS measurement
    - `object["CSMS"] = 1`


