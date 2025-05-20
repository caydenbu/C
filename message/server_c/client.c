#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define JS_PORT 3000
#define C_PORT 8080

int main(int argc, char const *argv[]) {

  int status, valread, client_fd, js_fd;
  struct sockaddr_in js_addr, c_addr;

  char buffer[1024] = {0};

  // --------- Full socket connection to server ---------- //

  // Connect to JS server (server.js)
  if ((js_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket js_fd failed");
    return 1;
  }

  js_addr.sin_family = AF_INET;
  js_addr.sin_port = htons(JS_PORT);
  inet_pton(AF_INET, SERVER_IP, &js_addr.sin_addr);

  if (connect(js_fd, (struct sockaddr *)&js_addr, sizeof(js_addr)) < 0) {
    perror("connect to js server failed");
    return 1;
  }

  // C Server

  // Socket creation & check
  if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    return -1;
  }

  c_addr.sin_family = AF_INET;
  c_addr.sin_port = htons(C_PORT);

  // converting addresses from text to binary
  if (inet_pton(AF_INET, SERVER_IP, &c_addr.sin_addr) <= 0) {
    printf("invalid adress \n");
    return -1;
  }

  // Connect to the server socket address
  if ((status = connect(client_fd, (struct sockaddr *)&c_addr,
                        sizeof(c_addr))) < 0) {
    printf("Connection Failed \n");
    return -1;
  }

  // --------- Send and Read message ---------- //
  char message[1024];

  // Read from JS server and forward to real C server
  while (1) {
    int valread = read(js_fd, buffer, sizeof(buffer) - 1);
    if (valread > 0) {
      buffer[valread] = '\0';
      printf("Received from browser: %s\n", buffer);
      send(client_fd, buffer, strlen(buffer), 0);
    }
  }
  // close the socket
  close(client_fd);
  close(js_fd);

  return 0;
}
