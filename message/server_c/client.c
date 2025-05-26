#include "raylib.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Components
#include "component/components.h"

#define SERVER_IP "127.0.0.1"
#define PORT 8080

int main(int argc, char const *argv[]) {

    // --------- Full socket connection to server ---------- //
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;

    char buffer[1024] = {0};

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        //       return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        printf("invalid address\n");
        //     return -1;
    }

    if ((status = connect(client_fd, (struct sockaddr *)&serv_addr,
                          sizeof(serv_addr))) < 0) {
        printf("Connection Failed\n");
        //   return -1;
    }

    // --------- Raylib window ---------- //
    InitWindow(800, 600, "Messaging Project");

    SetTargetFPS(60);
    int tick = 0;

    // --------- Game and network loop ---------- //
    char *text = malloc(1024);
    text[0] = '\0';

    char *CurrentMessage = malloc(1024);
    CurrentMessage[0] = '\0';

    int messages = 0; //  Change this to a dynamic list later

    while (!WindowShouldClose()) {
        tick++;

        if (IsKeyPressed(KEY_ENTER)) {
            // Sends Message
            send(client_fd, text, strlen(text), 0);
            valread = read(client_fd, buffer, sizeof(buffer) - 1);

            // Recieves back message
            messages++;
            buffer[valread] = '\0';
            CurrentMessage = buffer;
            printf("%s\n", buffer);

            // Resets Clients text
            free(text);
            text = malloc(1024);
            text[0] = '\0';
        }

        // UI Init
        Color background;
        background.r = 10;
        background.g = 10;
        background.b = 10;

        // Get Input
        char key = GetCharPressed();

        if (key > 0) {
            text[strlen(text)] = key;
            text[strlen(text) + 1] = '\0';
        }

        // BackSpace
        if (IsKeyPressed(KEY_BACKSPACE)) {
            int len = strlen(text);
            if (len > 0) {
                text[len - 1] = '\0';
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(background);

        DrawInputBox(text, tick);
        DrawText(CurrentMessage, 0, 0, 32, WHITE);

        EndDrawing();
    }

    // Cleanup
    CloseWindow();
    close(client_fd);

    return 0;
}
