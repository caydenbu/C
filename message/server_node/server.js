const fs = require('fs');
const WebSocket = require('ws');
const http = require('http');

const pipePath = '/tmp/server_pipe';
const server = http.createServer();
const wss = new WebSocket.Server({ server });

function broadcast(data) {
  wss.clients.forEach(client => {
    if (client.readyState === WebSocket.OPEN) {
      client.send(data);
    }
  });
}

const pipeStream = fs.createReadStream(pipePath);
pipeStream.on('data', chunk => {
  broadcast(chunk.toString());
});

wss.on('connection', ws => {
  ws.send('Connected to WebSocket server.');
});

server.listen(3000, () => {
  console.log('WebSocket listening on ws://localhost:3000');
});

