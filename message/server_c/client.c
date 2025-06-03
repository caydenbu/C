#include "raylib.h"
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Components
#include "component/components.h"

#define SERVER_IP "127.0.0.1"
#define PORT 8080

#define MAX_MESSAGES 10

// Shared state
char *messages[MAX_MESSAGES] = {0};
int messageCount = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int client_fd;

// Receiver thread
void *receiver_thread(void *arg) {
    char buffer[1024];
    while (1) {
        ssize_t valread = read(client_fd, buffer, sizeof(buffer) - 1);
        if (valread <= 0)
            break;
        buffer[valread] = '\0';

        pthread_mutex_lock(&lock);
        if (messageCount < MAX_MESSAGES) {
            messages[messageCount++] = strdup(buffer);
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    struct sockaddr_in serv_addr;

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        printf("invalid address\n");
        return 1;
    }

    if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <
        0) {
        printf("Connection Failed\n");
        return 1;
    }

    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receiver_thread, NULL);

    // Raylib
    InitWindow(800, 600, "Messaging Project");
    SetTargetFPS(60);

    int tick = 0;
    char *text = malloc(1024);
    text[0] = '\0';

    while (!WindowShouldClose()) {
        tick++;

        // Input handling
        char key = GetCharPressed();
        while (key > 0) {
            int len = strlen(text);
            if (len < 1022) {
                text[len] = key;
                text[len + 1] = '\0';
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            int len = strlen(text);
            if (len > 0)
                text[len - 1] = '\0';
        }

        if (IsKeyPressed(KEY_ENTER)) {
            send(client_fd, text, strlen(text), 0);
            free(text);
            text = malloc(1024);
            text[0] = '\0';
        }

        // Drawing
        BeginDrawing();
        ClearBackground((Color){10, 10, 10, 255});
        DrawInputBox(text, tick);

        pthread_mutex_lock(&lock);
        for (int i = 0; i < messageCount; i++) {
            DrawText(messages[i], 10, 40 + i * 32, 28, WHITE);
        }
        pthread_mutex_unlock(&lock);

        EndDrawing();
    }

    // Cleanup
    pthread_cancel(recv_thread);
    pthread_join(recv_thread, NULL);

    for (int i = 0; i < messageCount; i++) {
        free(messages[i]);
    }

    free(text);
    CloseWindow();
    close(client_fd);
    return 0;
}
