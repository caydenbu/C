#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Global password
const char *password = "SUPERSECRETPASSWORD";

void encrypt(char *data, const char *password) {
        // XOR encryption
    	for (int i = 0; i < 1024; i++) {
        	data[i] ^= password[i % strlen(password)]; 
    	}
}

// Role Play 🤪
typedef struct server{
    char data[1024];
} server;

void sendMessage(char message[], server *destination){

    // Creates empty message of blanks
    char *encryptedMessage = calloc(1024, sizeof(char));
    memset(encryptedMessage, ' ', 1024);

    // copies and encrypts messages
    strcpy(encryptedMessage, message);
    encrypt(encryptedMessage, password);

    // copys encrypted message into server
    memcpy(destination->data, encryptedMessage, 1024);

}

char* recieveMessage(server *destination){

    // gets encrypted message from server
    char *decryptedMessage = malloc(1024);
    memcpy(decryptedMessage, destination->data, 1024);

    // decryptes and return message
    encrypt(decryptedMessage, password);
    return decryptedMessage;
}

int main() {
    	char message[] = "hello I am testing the decryption technique ive totally thought of my self";
        server Server;

        sendMessage(message, &Server);
        char *recievedMessage = recieveMessage(&Server);
    	printf("%.*s\n", 1024, recievedMessage); // No clue what how the hell this works skips null term

    	return 0;
    
}

