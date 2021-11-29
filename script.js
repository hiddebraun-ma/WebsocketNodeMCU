const websocketURL = 'ws://192.168.1.38:81';

let connectBtn, disconnectBtn, controls, messages, status;
let socket;

const start = () => {
    connectBtn = document.getElementById('connect');
    disconnectBtn = document.getElementById('disconnect');
    controls = document.getElementById('controls');
    messages = document.getElementById('messages');
    status = document.getElementById('status');

    toggleLedBtn = document.getElementById('toggle-led');
    toggleLedBtn.addEventListener('click', toggleLed);

    connectBtn.addEventListener('click', connectToServer);
    disconnectBtn.addEventListener('click', disconnectFromServer);
}

const connectToServer = () => {
    socket = new WebSocket(websocketURL);

    // Connection opened
    socket.addEventListener('open', onSocketOpen);

    // Listen for messages
    socket.addEventListener('message', onSocketMessage);

    // Listen for errors
    socket.addEventListener('error', onSocketError);

    // Listen for disconnectBtn
    socket.addEventListener('close', onSocketClosed);

}

const disconnectFromServer = () => {
    socket.close();
}

const onSocketOpen = (event) => {
    console.log('Socket open:', event);
    status.className = 'connected';
    connectBtn.style.display = 'none';
    disconnectBtn.style.display = 'flex';
    controls.style.display = 'block';
    logMessage("Connected to " + event.currentTarget.url);
}

const onSocketMessage = (event) => {
    console.log('Message from server', event);
    logMessage("Message: " + event.data);
}

const onSocketError = (event) => {
    console.error('Socket error', event);
    logMessage("Error: " + event);
}

const onSocketClosed = (event) => {
    console.log('Socket closed', event);
    status.className = null;
    connectBtn.style.display = 'inline';
    disconnectBtn.style.display = 'none';
    controls.style.display = 'none';
    logMessage("Disconnected from " + event.currentTarget.url);
}

const logMessage = (message) => {
    messages.innerHTML = message + '<br/>' + messages.innerHTML;
}

const toggleLed = () => {
    let cmd = { command: 'toggle-led' };
    socket.send(JSON.stringify(cmd));
}

window.addEventListener('DOMContentLoaded', start);