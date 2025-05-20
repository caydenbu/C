#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define JS_PORT 8080
#define C_PORT 9090

#include <fcntl.h>  // open
#include <stdio.h>  // perror
#include <string.h> // strlen, snprintf
#include <unistd.h> // write, close

void send_to_pipe(const char *buffer) {
  int fd = open("/tmp/server_pipe", O_WRONLY | O_NONBLOCK);
  char message[1024];

  snprintf(message, sizeof(message), "Client: %s\n", buffer);
  ssize_t written = write(fd, message, strlen(message));
}

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
  address.sin_port = htons(C_PORT);

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

  // Send and read
  while (1) {
    valread = read(new_socket, buffer, 1024 - 1);

    if (valread <= 0)
      break; // client left
    buffer[valread] = '\0';

    //  Send to websocket server
    send_to_pipe(buffer);

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
