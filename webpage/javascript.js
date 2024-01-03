var array_length = 10;
var xValues = [];
for (let i = 0; i < array_length; i++) {
  xValues[i] = i;
}

var yValues = [];

const temperature_chart = new Chart("temperature_chart", {
  type: "line",
  data: {
    labels: xValues,
    datasets: [{
      fill: false,
      lineTension: 0,
      backgroundColor: "rgba(0,0,255,1.0)",
      borderColor: "rgba(0,0,255,0.1)",
      borderWidth: 2, //linewidth
      data: yValues
    }]
  },
  options: {
    legend: {display: true},
    scales: {
      yAxes: [{
        scaleLabel: {
            display: true,
            labelString: 'Temperature (°C)'
        },
        ticks: {min: 5, max:35}}],
      xAxes: [{
            display: false
        },
        {
            scaleLabel: {
                display: true,
                labelString: 'Time [hh:mm]'
            }
        }]

    }
  }
});


const humidity_chart = new Chart("humidity_chart", {
  type: "line",
  data: {
    labels: xValues,
    datasets: [{
      fill: false,
      lineTension: 0,
      backgroundColor: "rgba(0,0,255,1.0)",
      borderColor: "rgba(0,0,255,0.1)",
      borderWidth: 2, //linewidth
      data: yValues
    }]
  },
  options: {
    legend: {display: true},
    scales: {
      yAxes: [{
        scaleLabel: {
            display: true,
            labelString: 'Humidity [%]'
        },
        ticks: {min: 0, max:100}}],
      xAxes: [{
            display: false
        },
        {
            scaleLabel: {
                display: true,
                labelString: 'Time [hh:mm]'
            }
        }]

    }
  }
});


const moisture_chart = new Chart("moisture_chart", {
type: "line",
data: {
labels: xValues,
datasets: [{
  fill: false,
  lineTension: 0,
  backgroundColor: "rgba(0,0,255,1.0)",
  borderColor: "rgba(0,0,255,0.1)",
  borderWidth: 2, //linewidth
  data: yValues
}]
},
options: {
legend: {display: true},
scales: {
  yAxes: [{
    scaleLabel: {
        display: true,
        labelString: 'Moisture [a.u.]',
        fontSize: 14,
        fontStyle: 'bold',
        fontColor: '#333'
    },
    ticks: {
        min: 0, 
        max:500,
        fontSize: 16,
        fontStyle: 'bold',
        fontColor: '#333'
      }
    
    }],
  xAxes: [{
        display: false,
    },
    {
        scaleLabel: {
            display: true,
            labelString: 'Time [hh:mm]',
            fontSize: 14,
            fontStyle: 'bold',
            fontColor: '#333'
        },
        ticks: {
          min: 0, 
          max: 24,
          fontSize: 16,
          fontStyle: 'bold',
          fontColor: '#333'
        }
    }]

}
}
});

/*Make selective checkbox: unchecks other checkbox when one is chosen*/
document.addEventListener("DOMContentLoaded", function() {
  const checkboxes = document.querySelectorAll('.selective_radio');

  checkboxes.forEach(function(checkbox) {
    checkbox.addEventListener('change', function() {
      if (this.checked) {
        checkboxes.forEach(function(otherCheckbox) {
          if (otherCheckbox !== checkbox) {
            otherCheckbox.checked = false;
          }
        });
      }
    });
  });
});


// Get main switch and other switches
const all_growlight_Switches = document.getElementById('all_growlight_Switches');
const growlight_switch_1 = document.getElementById('growlight_switch_1');
const growlight_switch_2 = document.getElementById('growlight_switch_2');
const growlight_switch_3 = document.getElementById('growlight_switch_3');

// Function to check and update main switch state
function update_all_growlight_Switches() {
  const all_growlight_SwitchesArray = [growlight_switch_1, growlight_switch_2, growlight_switch_3];
  const all_growlight_SwitchesChecked = all_growlight_SwitchesArray.every(switchElem => switchElem.checked);
  
  all_growlight_Switches.checked = all_growlight_SwitchesChecked;
  
  // Check if any switch is unchecked, then uncheck the main switch
  if (!all_growlight_SwitchesChecked) {
    all_growlight_Switches.checked = false;
  }
  
  // Check if all switches are checked, then check the main switch
  if (all_growlight_SwitchesChecked) {
    all_growlight_Switches.checked = true;
  }
}

// Add event listeners to individual switches
growlight_switch_1.addEventListener('change', update_all_growlight_Switches);
growlight_switch_2.addEventListener('change', update_all_growlight_Switches);
growlight_switch_3.addEventListener('change', update_all_growlight_Switches);

// Add event listener to main switch for convenience (optional)
all_growlight_Switches.addEventListener('change', function() {
  const state = this.checked;
  growlight_switch_1.checked = state;
  growlight_switch_2.checked = state;
  growlight_switch_3.checked = state;
});

// Initialize the main switch based on the state of other switches
update_all_growlight_Switches();




/*--random stuff from template*/
var output = document.getElementById('ID_RANDOM_INTENSITY_VALUE');

var Socket;

function init() {
  Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
  Socket.onmessage = function(event) {
    processCommand(event);
  };
}

function processCommand(event) {
  var obj = JSON.parse(event.data);
  var type = obj.type;
  if (type.localeCompare("graph_update") == 0) {
    console.log(obj.value);
    var temp_values = obj.value;
    for(var i=0, length = temp_values.length; i < length; i++) {
      temp_values[i] = temp_values[i] / 10;
    }
    myChart.data.datasets[0].data = temp_values;
    myChart.update();
  }
}
window.onload = function(event) {
  init();
}