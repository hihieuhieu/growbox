// INITIALIZATION
// entry point to websocket interface
var socket = new WebSocket("ws://" + window.location.hostname + ":81/");

let dataReceived = false;

// add event listener: opened connection
socket.addEventListener('open', function (event){
    console.log('Connected to websocket server');
});

// add event listener: message from server
socket.addEventListener('message', function (event){
    const rx_message = JSON.parse(event.data);
    console.log('Received');
    dataReceived = true;

    update_DHT_CSMS_data(rx_message);
});

// add event listener: websocket error
socket.addEventListener('error', function (event){
    console.error('Websocket error: ', event)
});

// add event listener: closed connection
socket.addEventListener('close', function (event) {
    console.log('Connection closed');
});

//_________________________________________________________________

// DISPLAY CURRENT VALUES SECTION
function display_value(rx_message) {
    const fieldsMap = {
      'temp_in': 'inside_temperature_field',
      'temp_out': 'outside_temperature_field',
      'humd_in': 'inside_humidity_field',
      'humd_out': 'outside_humidity_field',
      'csms1': 'moisture_sens1_field',
      'csms2': 'moisture_sens2_field',
      'csms3': 'moisture_sens3_field'
    };

    for (const key in rx_message) {
      if (fieldsMap.hasOwnProperty(key)) {
        const fieldId = fieldsMap[key];
        const element = document.getElementById(fieldId);
        if (element) {
          element.value = rx_message[key];
        } else {
          console.error(`Element with ID '${fieldId}' not found.`);
        }
      }
    }
  }


// RADIO BUTTONS - Growlight position selection
// selects all radio buttons with common name atrribute
const radioButtons_growlight_position = document.querySelectorAll('input[name="growlight_position"]');
const numericValueSpan = document.getElementById('numeric-value');
// add change event listener to each radio button
radioButtons_growlight_position.forEach(button => {
    button.addEventListener('change', function() {
    // get value of selected radio button
    const selectedOption = document.querySelector('input[name="growlight_position"]:checked').value;
    // create JSON object containing selected option
    const positionValues = {
      "position1": 1,
      "position2": 2,
      "position3": 3,
      "position4": 4,
    };
    var stage_position_value = positionValues[selectedOption] || 0;

    const data = {
        "message_type"    :    "act_stage_position",
        "stage_position"  :     stage_position_value
    };
    // convert to JSON string
    const jsonString = JSON.stringify(data);
    // send JSON string over websocket
    socket.send(jsonString);
    });
});

// TOGGLE SWITCHES
function timeToMinutes(time) {
  const [hours, minutes] = time.split(':').map(Number);
  return hours * 60 + minutes;
}


// Growlights

function addGrowlight1ToggleListener(growlightId) {
  const growlight_toggle = document.getElementById(growlightId);
  // add event listener 'change'
  var isSwitchOn = "";
  growlight_toggle.addEventListener('change', function() {
      if (growlight_toggle.checked){
        isSwitchOn = "on"
      } else {isSwitchOn = "off"}
      const data = {
          "message_type": "act_toggle_growlight",
          "growlight": 1,
          "state": isSwitchOn
      };
      // convert to JSON string
      const jsonString = JSON.stringify(data);
      // send JSON string over websocket
      console.log(jsonString);
      socket.send(jsonString);
  });
}

function addGrowlight2ToggleListener(growlightId) {
  const growlight_toggle = document.getElementById(growlightId);
  // add event listener 'change'
  var isSwitchOn = "";
  growlight_toggle.addEventListener('change', function() {
      if (growlight_toggle.checked){
        isSwitchOn = "on"
      } else {isSwitchOn = "off"}
      const data = {
          "message_type": "act_toggle_growlight",
          "growlight": 2,
          "state": isSwitchOn
      };
      // convert to JSON string
      const jsonString = JSON.stringify(data);
      // send JSON string over websocket
      console.log(jsonString);
      socket.send(jsonString);
  });
}

function addGrowlight3ToggleListener(growlightId) {
  const growlight_toggle = document.getElementById(growlightId);
  // add event listener 'change'
  var isSwitchOn = "";
  growlight_toggle.addEventListener('change', function() {
      if (growlight_toggle.checked){
        isSwitchOn = "on"
      } else {isSwitchOn = "off"}
      const data = {
          "message_type": "act_toggle_growlight",
          "growlight": 3,
          "state": isSwitchOn
      };
      // convert to JSON string
      const jsonString = JSON.stringify(data);
      // send JSON string over websocket
      console.log(jsonString);
      socket.send(jsonString);
  });
}

// Fans
function addFanToggleListener(fanId) {
  // create object on element id
  const fan_toggle = document.getElementById(fanId);
  var fan_number = 1
  // add event listener 'change'
  var isSwitchOn = "";
  fan_toggle.addEventListener('change', function() {
      if (fan_toggle.checked){
        isSwitchOn = "on"
      } else {isSwitchOn = "off"}
      const data = {
          "message_type"  :   "act_toggle_fan",
          "fan"           :   fan_number,
          "state"         :   isSwitchOn
      };
      const jsonString = JSON.stringify(data);
      socket.send(jsonString);
      console.log(jsonString)
  });
}

function addFanToggleOnOffListener(fanId) {
  const fan_onff_toggle = document.getElementById(fanId);
  fan_number = 1
  var isSwitchOn = "";
  fan_onff_toggle.addEventListener('change', function() {
    if (fan_onff_toggle.checked){
      isSwitchOn = "on";
    } else {isSwitchOn = "off"}
    const data = {
      "message_type"  : "config_fan_onoff_interval",
      "fan"           : fan_number,
      "state"         : isSwitchOn
    }
    const jsonString = JSON.stringify(data)
    socket.send(jsonString)
    console.log(jsonString)
  });
}

// Add event listeners for each growlight and fan toggle switch
addGrowlight1ToggleListener("growlight1_toggle");
addGrowlight2ToggleListener("growlight2_toggle");
addGrowlight3ToggleListener("growlight3_toggle");
addFanToggleListener("fan_toggle");
addFanToggleOnOffListener("fan_onoff_toggle")


// CONFIGURATIONS
document.getElementById('gl_config_submit').addEventListener('click', function() {
  // var jsonData = {
  //   "message_type"  : "config_growlight_toggle_times",
  //   "component"     : "growlight"
  // };

  // // Loop through each configuration group
  // for (let i = 1; i <= 3; i++) {
  //   // Extract on and off values
  //   var onTime = document.getElementById('gl' + i + '_on').value;
  //   var offTime = document.getElementById('gl' + i + '_off').value;

  //   var onMinutes = convertTimeToMinutes(onTime);
  //   var offMinutes = convertTimeToMinutes(offTime);

  //   var partNumberObj = {
  //     "on": onMinutes,
  //     "off": offMinutes
  //   };

  //   jsonData['part_number' + i] = partNumberObj;
  // }

  // var jsonString = JSON.stringify(jsonData);
  const data = {
    "message_type"  : "config_growlight_toggle_times",
    "gl1_on"        : convertTimeToMinutes(document.getElementById('gl1_on').value),
    "gl2_on"        : convertTimeToMinutes(document.getElementById('gl2_on').value),
    "gl3_on"        : convertTimeToMinutes(document.getElementById('gl3_on').value),
    "gl1_off"        : convertTimeToMinutes(document.getElementById('gl1_off').value),
    "gl2_off"        : convertTimeToMinutes(document.getElementById('gl2_off').value),
    "gl3_off"        : convertTimeToMinutes(document.getElementById('gl3_off').value),
  }
  var jsonString = JSON.stringify(data)
  console.log(jsonString);
  socket.send(jsonString);
});



// UPDATE TIME AND DATE
// Function to update date and time on the web page
function updateTime() {
  var now = new Date();
  var dateElement = document.getElementById('date');
  var timeElement = document.getElementById('time');

  // Format date as DD.MM.YYYY
  var optionsDate = { day: '2-digit', month: '2-digit', year: 'numeric' };
  var formattedDate = now.toLocaleDateString('de-DE', optionsDate);

  // Format time as HH:MM:SS
  var optionsTime = { hour: '2-digit', minute: '2-digit', second: '2-digit' };
  var formattedTime = now.toLocaleTimeString('de-DE', optionsTime);

  dateElement.innerHTML = 'Date: ' + formattedDate;
  timeElement.innerHTML = 'Time: ' + formattedTime;
}

// Initialize time variables
var gl1_on = "";
var gl1_off = "";
var gl2_on = "";
var gl2_off = "";
var gl3_on = "";
var gl3_off = "";
var fan_on = "";
var fan_off = "";


// Function to update time variables
function updateVariables_gl() {
  gl1_on = document.getElementById('gl1_on').value;
  gl1_off = document.getElementById('gl1_off').value;
  gl2_on = document.getElementById('gl2_on').value;
  gl2_off = document.getElementById('gl2_off').value;
  gl3_on = document.getElementById('gl3_on').value;
  gl3_off = document.getElementById('gl3_off').value;
}

function updateVariables_fan_onoff() {
  fan_on = document.getElementById('fan_on').value;
  fan_off = document.getElementById('fan_off').value;
}

document.getElementById('fan_onoff_config_submit').addEventListener('click', function() {
  // Update the time variables
  updateVariables_fan_onoff();
  const data = {
    "message_type": "config_fan_onoff_interval",
    "fan_on": fan_on,
    "fan_off": fan_off,
  };
  jsonString = JSON.stringify(data);
  socket.send(jsonString);
  console.log(jsonString)

});


// Query data from DHT and CSMS
function query_DHT_CSMS_data() {
  const data = {
    "message_type": "query_dht_csms_data",
  };
  jsonString = JSON.stringify(data);
  socket.send(jsonString);
  console.log(jsonString)

}

function update_DHT_CSMS_data(rx_message) {
  if (dataReceived){
    console.log("Sensor function activated")
    console.log(rx_message);
    dataReceived = false;

    var temperature_inside = rx_message.temperature_inside;
    var temperature_outside = rx_message.temperature_outside;
    var humidity_inside = rx_message.humidity_inside;
    var humidity_outside = rx_message.humidity_outside;
    var soil_moisture_1 = rx_message.soil_moisture_1;
    var soil_moisture_2 = rx_message.soil_moisture_2;
    var soil_moisture_3 = rx_message.soil_moisture_3;


    updateSensorValue("inside_temperature_field", temperature_inside + "°C");
    updateSensorValue("outside_temperature_field", temperature_outside + "°C");
    updateSensorValue("inside_humidity_field", humidity_inside + "%");
    updateSensorValue("outside_humidity_field", humidity_outside + "%");
    updateSensorValue("moisture_sens1_field", soil_moisture_1 + " [a.u.]");
    updateSensorValue("moisture_sens2_field", soil_moisture_2 + " [a.u.]");
    updateSensorValue("moisture_sens3_field", soil_moisture_3 + " [a.u.]");

  }
  else {console.log('No data received.')}
}

document.getElementById('sensor_data').addEventListener('click', function() {
  query_DHT_CSMS_data();
  update_DHT_CSMS_data();
});

function updateSensorValue(elementId, value) {
  document.getElementById(elementId).textContent = value;
}


function query_and_update_DHT_CSMS(){
  // query_DHT_CSMS_data();
  // update_DHT_CSMS_data();
  console.log("Exected query_and_update_DHT_CSMS")
}


// EXECUTE FUNCTIONS

var query_DHT_CSMS_data_interval = 10000 //30 * 60 * 1000  // 30 minutes * 60 seconds * 1000 ms

window.onload = function() {
  query_and_update_DHT_CSMS();
}

setInterval(query_and_update_DHT_CSMS, query_DHT_CSMS_data_interval);

// Function to convert time to minutes
function convertTimeToMinutes(timeString) {
  var timeParts = timeString.split(':');
  var hours = parseInt(timeParts[0], 10);
  var minutes = parseInt(timeParts[1], 10);
  return hours * 60 + minutes;
}

