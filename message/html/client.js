console.log("hello");

const USER_NAME = "Bob";

// Input Listener
const input = document.getElementById('messageInput');
input.addEventListener('keydown', function(event) {
    if (event.key === 'Enter') {
        createMessage(input.value);
        input.value = "";
    }
});

function sendMessage(text){
    createMessage(text);
    // send to server
}

function recieveMessage(user, text){
    // get text from server
    createMessage(text);
}


function createMessage(text){
    const MESSAGE_CONTAINER = document.getElementById("chat-chat");
    
    // Create full message
    const MESSAGE = document.createElement("div");
    MESSAGE.classList = "chat-message";

    const MESSAGE_NAME = document.createElement("p");
    MESSAGE_NAME.classList = "chat-message-name";
    MESSAGE_NAME.innerHTML = USER_NAME;
    MESSAGE.append(MESSAGE_NAME);

    const MESSAGE_TEXT = document.createElement("p");
    MESSAGE_TEXT.classList = "chat-message-text";
    MESSAGE_TEXT.innerHTML = text;
    MESSAGE.append(MESSAGE_TEXT);
    
    // Append messge to message list
    MESSAGE_CONTAINER.prepend(MESSAGE);
}

// Web Socket Information
const ws = new WebSocket('ws://localhost:3000');

ws.onopen = () => {
  console.log('WebSocket connected');
};
