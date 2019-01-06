function togglePower() {
    sendMessage("1EE1F00F");
}
function volumeUp() {
    sendMessage("1EE17887");
}
function volumeDown() {
    sendMessage("1EE1F807");
}

function sendMessage(msg) {
    var message = new Paho.MQTT.Message(msg);
    message.destinationName = TOPIC;
    mqttClient.send(message);
}

function connect() {
    mqttClient.connect({
        userName: USERNAME,
        password: PASSWORD,
        keepAliveInterval: 10,
        useSSL: true,
        onSuccess: connected,
        onFailure: connectionFailed,
    });
}

function connected() {
    console.log("Connected");
}

function connectionFailed(response) {
    console.log("Connection failed:" + response.errorMessage);
}

function connectionLost(response) {
    if (response.errorCode !== 0) {
        console.log("Connection lost:" + response.errorMessage);
        Connect();
    }
}

let mqttClient = new Paho.MQTT.Client(HOSTNAME, PORT, CLIENT_ID);
mqttClient.onConnectionLost = connectionLost;
connect();
