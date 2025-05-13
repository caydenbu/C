
// Connect Websocket
const ws = new WebSocket('ws://localhost:3000');

// Print info to web terminal
function addToConsole(text){

    const webConsole = document.getElementById("console");
    const p = document.createElement("p")
    p.innerHTML="> " + text;
    webConsole.append(p);
}

// Websocket connections status
ws.onopen = () => {
    addToConsole("Node server connected...");
};

ws.onerror = (error) => {
    addToConsole("WebSocket error (Server may be closed)");
};

ws.onclose = () => {
    addToConsole("Node server closed.");
}
