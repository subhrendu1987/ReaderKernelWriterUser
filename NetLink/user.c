#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_PORT 12345
#define SERVER_ADDR "127.0.0.1"

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char message[] = "Hello from user program";
    char buffer[1024];
    ssize_t bytes_sent, bytes_received;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Failed to create socket");
        return EXIT_FAILURE;
    }

    // Set up server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_ADDR, &(server_addr.sin_addr)) <= 0) {
        perror("Invalid server address");
        return EXIT_FAILURE;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to connect to server");
        return EXIT_FAILURE;
    }

    // Send message to kernel module
    bytes_sent = send(sock, message, strlen(message), 0);
    if (bytes_sent < 0) {
        perror("Failed to send message");
        return EXIT_FAILURE;
    }
    printf("Sent message to kernel module: %s\n", message);

    // Receive response from kernel module
    bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received < 0) {
        perror("Failed to receive response");
        return EXIT_FAILURE;
    }
    buffer[bytes_received] = '\0';
    printf("Received response from kernel module: %s\n", buffer);

    // Close the socket
    close(sock);

    return EXIT_SUCCESS;
}
