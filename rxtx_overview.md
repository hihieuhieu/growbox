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

## Arduino/ESP8266: Send JSON strings Arduino <--> ESP8266:
C++
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

## Arduino/ESP8266: Receive JSON strings Arduino <--> ESP8266:
C++
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
    DeserializationError err = deserializeJson(doc, Serial);
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

## ESP8266: Send JSON strings ESP8266 --> Client
C++
```
// create container string
String jsonString = "";
// create JSON document
StaticJsonDocument<200> doc;
// convert JSON document to object
JsonObject object = doc.to<JsonObject>();
// wriite keyvalues into keys
object["key1"] = keyvalue1;
object["key2"] = "keyvalue2";
// convert JSON document to string
serializeJson(doc, jsonString);
// send to client
webSocket.sendTXT(jsonString);
```


## Client: JS grid
Javascript
```
// create websocket
const socket = new WebSocket('ws://server_ip:81);

// add event listener: opened connection
socket.addEventListener('open', function (event){
    console.log('Connected to websocket server');
});

// add event listener: message fron server
socket.addEventListener('message', function (event){
    const rx_message = JSON.parse(event.data);
    console.log('Received message: ', rx_message);
});

// add event listener: websocket error
socket.addEventListener('error', function (event){
    console.error('Websocket error: ', event)
});

// add event listener: closed connection
socket.addEventListener('close', function (event) {
    console.log('Connection closed);
});

// add event listeners for all buttons and switches etc. See below
...

```

## Client: Send JSON strings Client --> ESP8266
Javascript

### Radio buttons
html
```
<input type="radio" id="option1" name="options" value="option1">
<label for="option1">Option 1</label><br>
<input type="radio" id="option2" name="options" value="option2">
<label for="option2">Option 2</label><br>
<input type="radio" id="option3" name="options" value="option3">
<label for="option3">Option 3</label><br>
```

JavaScript
```
// selects all radio buttons with common name atrribute
const radioButtons = document.querySelectorAll('input[name="options"]');
// add change event listener to each radio button
radioButtons.forEach(button => {
    button.addEventListener('change', function() {
    // get value of selected radio button
    const selectedOption = document.querySelector('input[name="options"]:checked').value;

    // create JSON object containing selected option
    const data = {
        "selectedOption": selectedOption
    };

    // convert to JSON string
    const jsonString = JSON.stringify(data);

    socket.send(jsonString);    
    });
});
```

### Toggle switch
html
```
<label class="switch">
    <input type="checkbox" id="toggleSwitch">
    <span class="slider round"></span>
</label>
```

Javascript
```
const toggleSwitch = document.getElementById("toggleSwitch");
    
toggleSwitch.addEventListener('change', function() {
    const isSwitchOn = toggleSwitch.checked;
    
    console.log('Toggle Switch State:', isSwitchOn);
    
    const data = {
    "toggleState": isSwitchOn
    };

    const jsonString = JSON.stringify(data);

    socket.send(jsonString);
});
```

### Numeric value
html
```
<input type="number" id="numericField" />
<button onclick="sendData()">Send</button>
```

JavaScript
```
function sendData() {
    const numericValue = document.getElementById('numericField').value;

    // Create a JSON object with the numeric value
    const data = {
    value: parseFloat(numericValue)
    };

    // Convert JSON object to string before sending
    const jsonData = JSON.stringify(data);

    // Send the data over the WebSocket connection
    socket.send(jsonData);
};
```

## Client: Receive JSON strings ESP8266 --> Client

```
socket.onmessage = function(event) {
    const receivedData = JSON.parse(event.data);
    const selectedValue = receivedData.key;
    //assign keyvalue to numeric field, graph, ...
};
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


