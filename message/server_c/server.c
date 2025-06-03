#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define MAX_CLIENTS 10

int clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t client_lock = PTHREAD_MUTEX_INITIALIZER;

void broadcast(const char *msg, int sender_fd) {
    pthread_mutex_lock(&client_lock);
    for (int i = 0; i < client_count; i++) {
        send(clients[i], msg, strlen(msg), 0);
    }
    pthread_mutex_unlock(&client_lock);
}

void *handle_client(void *arg) {
    int client_fd = *(int *)arg;
    free(arg);
    char buffer[1024];

    while (1) {
        ssize_t len = read(client_fd, buffer, sizeof(buffer) - 1);
        if (len <= 0)
            break;

        buffer[len] = '\0';
        broadcast(buffer, client_fd); // send to everyone else
    }

    pthread_mutex_lock(&client_lock);
    for (int i = 0; i < client_count; i++) {
        if (clients[i] == client_fd) {
            clients[i] = clients[--client_count];
            break;
        }
    }
    pthread_mutex_unlock(&client_lock);

    close(client_fd);
    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, MAX_CLIENTS);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (new_socket < 0)
            continue;

        pthread_mutex_lock(&client_lock);
        if (client_count < MAX_CLIENTS) {
            clients[client_count++] = new_socket;

            int *pclient = malloc(sizeof(int));
            *pclient = new_socket;
            pthread_t tid;
            pthread_create(&tid, NULL, handle_client, pclient);
            pthread_detach(tid);
        } else {
            close(new_socket); // too many clients
        }
        pthread_mutex_unlock(&client_lock);
    }

    close(server_fd);
    return 0;
}
