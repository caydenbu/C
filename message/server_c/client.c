#include "raylib.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080

int main(int argc, char const *argv[]) {

  // --------- Full socket connection to server ---------- //
  int status, valread, client_fd;
  struct sockaddr_in serv_addr;

  char buffer[1024] = {0};

  if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
    printf("invalid address\n");
    return -1;
  }

  if ((status = connect(client_fd, (struct sockaddr *)&serv_addr,
                        sizeof(serv_addr))) < 0) {
    printf("Connection Failed\n");
    return -1;
  }

  // --------- Raylib window ---------- //
  InitWindow(800, 600, "Client Window");

  SetTargetFPS(60);

  // --------- Game and network loop ---------- //
  while (!WindowShouldClose()) {
    // Handle network input
    if (IsKeyPressed(KEY_ENTER)) {
      const char *message = "Hello\n";
      send(client_fd, message, strlen(message), 0);
      valread = read(client_fd, buffer, sizeof(buffer) - 1);
      buffer[valread] = '\0';
    }

    // Draw
    BeginDrawing();
    ClearBackground(RAYWHITE);
    EndDrawing();
  }

  // Cleanup
  CloseWindow();
  close(client_fd);

  return 0;
}
