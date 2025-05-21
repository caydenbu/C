#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
int main() {

  int server_fd, new_socket;
  ssize_t valread;
  struct sockaddr_in address;
  int opt = 1;
  socklen_t addrlen = sizeof(address);
  char buffer[1024] = {0};

  // --------- Full socket connection to server ---------- //

  // Create the socket
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Attach socket to port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Bind socket
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  // Listen
  if (listen(server_fd, 3) < 0) {
    perror("listen fail");
    exit(EXIT_FAILURE);
  }
  // Accept & new socket
  if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) <
      0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }

  // --------- Send and Read message ---------- //

  char *message = "Hello from server \n";

  // Send and read
  while (1) {
    valread = read(new_socket, buffer, 1024 - 1);

    if (valread <= 0)
      break; // client left

    buffer[valread] = '\0';
    printf("Client:%s\n", buffer);

    // send(new_socket, message, strlen(message), 0);
    send(new_socket, "\n", 1,
         0); // blank line reply (just so client is stuck waiting for response)
  }

  // Close the remaining sockets
  close(new_socket);
  close(server_fd);

  return 0;
}
