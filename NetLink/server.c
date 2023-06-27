#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 12345

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];
    ssize_t bytes_received;

    // Create server socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Failed to create socket");
        return EXIT_FAILURE;
    }

    // Set up server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind server socket to address and port
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to bind socket");
        return EXIT_FAILURE;
    }

    // Listen for incoming connections
    if (listen(server_sock, 5) < 0) {
        perror("Failed to listen for connections");
        return EXIT_FAILURE;
    }

    printf("Server is listening on port %d\n", SERVER_PORT);

    while (1) {
        // Accept incoming connection
        socklen_t client_addr_len = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_sock < 0) {
            perror("Failed to accept connection");
            return EXIT_FAILURE;
        }

        printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));

        // Receive message from client
        bytes_received = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received < 0) {
            perror("Failed to receive message");
            return EXIT_FAILURE;
        }
        buffer[bytes_received] = '\0';
        printf("Received message from client: %s\n", buffer);

        // Send response back to client
        const char *response = "Hello from server";
        ssize_t bytes_sent = send(client_sock, response, strlen(response), 0);
        if (bytes_sent < 0) {
            perror("Failed to send response");
            return EXIT_FAILURE;
        }

        // Close client socket
        close(client_sock);

        printf("Response sent to client\n");
    }

    // Close server socket
    close(server_sock);

    return EXIT_SUCCESS;
}
