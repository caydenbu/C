#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080

int main(int argc, char const* argv[]){

    int status, valread, client_fd;
    struct sockaddr_in serv_addr;

    char buffer[1024] = { 0 };


    // --------- Full socket connection to server ---------- //

    // Socket creation & check
    if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket failed");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // converting addresses from text to binary
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0){
        printf("invalid adress \n");
        return -1;
    }

    // Connect to the server socket address
    if((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0){
        printf("Connection Failed \n");
        return -1;
    }

    // --------- Send and Read message ---------- //
    char message[1024];

    while(1){
        printf("> ");
        fgets(message, sizeof(message), stdin);

        // Exit condition
        if (strncmp(message, "exit", 4)==0){
            break;
        }

        send(client_fd, message, strlen(message), 0);

        valread = read(client_fd, buffer, 1024 - 1 /* remove null operator*/);
        buffer[valread] = '\0';
    }

    // close the socket
    close(client_fd);

    return 0;
}
