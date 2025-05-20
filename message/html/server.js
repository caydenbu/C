const WebSocket = require("ws");
const ws = new WebSocket.Server({ port: 3000 });

// Websocket connections status
ws.onopen = () => {
    addToConsole("Node server connected...");
};

ws.onerror = (error) => {
    addToConsole("WebSocket error");
};

ws.onclose = () => {
    addToConsole("Node server closed.");
};

ws.onmessage = (event) => {
    addToConsole(event.data);
};
