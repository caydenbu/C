console.log("hello");

const USER_NAME = "Bob";

// Input Listener
const input = document.getElementById("messageInput");
input.addEventListener("keydown", function(event) {
    if (event.key === "Enter") {
        createMessage(input.value);
        input.value = "";
    }
});

function sendMessage(text) {
    // send to server
}

function recieveMessage(user, text) {
    // get text from server

    createMessage(user, text);
}

function createMessage(user, text) {
    const MESSAGE_CONTAINER = document.getElementById("chat-chat");

    // Create full message
    const MESSAGE = document.createElement("div");
    MESSAGE.classList = "chat-message";

    const MESSAGE_NAME = document.createElement("p");
    MESSAGE_NAME.classList = "chat-message-name";
    MESSAGE_NAME.innerHTML = user;
    MESSAGE.append(MESSAGE_NAME);

    const MESSAGE_TEXT = document.createElement("p");
    MESSAGE_TEXT.classList = "chat-message-text";
    MESSAGE_TEXT.innerHTML = text;
    MESSAGE.append(MESSAGE_TEXT);

    // Append messge to message list
    MESSAGE_CONTAINER.prepend(MESSAGE);
}

// Web Socket Information
const ws = new WebSocket("ws://localhost:8080");
ws.onopen = () => {
    console.log("WebSocket connected");

    const input = document.getElementById("messageInput");
    input.addEventListener("keydown", (e) => {
        if (e.key === "Enter" && input.value.trim()) {
            ws.send(input.value.trim());
            console.log("sent");
            input.value = ""; // clear input
        }
    });
};

ws.onerror = (err) => {
    console.error("WebSocket error", err);
};

ws.onmessage = (event) => {
    console.log(event);
    let message = event.data.split(":");
    createMessage(message[0], message[1]);
};
