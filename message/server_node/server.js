const fs = require("fs");
const http = require("http");
const WebSocket = require("ws");
const net = require("net");

// TCP server
const TCP_PORT = 8080;

let cClient = null;

// TCP server for client.c
const tcpServer = net.createServer((socket) => {
    console.log("🟢 client.c connected to TCP server");
    cClient = socket;

    socket.on("end", () => {
        console.log("🔴 client.c disconnected");
        cClient = null;
    });

    socket.on("error", (err) => {
        console.error("TCP Socket error:", err.message);
        cClient = null;
    });
});

tcpServer.listen(TCP_PORT, () => {
    console.log(`TCP server listening on port ${TCP_PORT}`);
});

const pipePath = "/tmp/server_pipe";

// Create basic HTTP server (needed for WebSocket)
const server = http.createServer();
const wss = new WebSocket.Server({ server });

wss.on("connection", (ws) => {
    console.log("WebSocket client connected");
});

// Open the named pipe for reading
const pipe = fs.createReadStream(pipePath);

pipe.on("data", (chunk) => {
    const message = chunk.toString();
    console.log("From pipe:", message);

    // Broadcast to all WebSocket clients
    wss.clients.forEach((client) => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(message);
        }
    });
});

pipe.on("error", (err) => {
    console.error("Pipe error:", err.message);
});

// Start HTTP/WebSocket server
server.listen(3000, () => {
    console.log("WebSocket server running on ws://localhost:3000");
});
