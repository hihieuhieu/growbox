// entry point to websocket interface
var gateway = 'ws://${window.location.hostname}/ws';
// global variable for websocket
var websocket;
// initialize websocket when page loads
window.addEventListener('load',onload)

// calls initWebSocket() to initialize websocket connection with server
function onload(event){
    initWebSocket();
}

function initWebSocket(){
    
}

// sends query dht message to the server
function tx_dht11(){
    websocket.send("read_dht11");
}

// reads the message received from server upon query_dht11
function rx_dht11(event){
    console.log(event.data);
    var object = JSON.parse(event.data);
    var keys = Object.keys(object)
    
}