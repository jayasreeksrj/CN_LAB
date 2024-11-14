#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    int addr_len = sizeof(client_addr);
    int expected_frame = 0;

    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        perror("Socket creation failed");
        return 1;
    }

    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    // Listen for incoming connections
    listen(server_socket, 3);
    printf("Waiting for client connections...\n");

    // Accept a connection from a client
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
    if (client_socket < 0) {
        perror("Accept failed");
        return 1;
    }

    printf("Client connected.\n");

    // Loop to receive frames
    while (1) {
        // Receive frame
        recv(client_socket, buffer, sizeof(buffer), 0);
        printf("Server received: %s\n", buffer);

        // Send ACK for the received frame
        snprintf(buffer, sizeof(buffer), "ACK for Frame %d", expected_frame);
        send(client_socket, buffer, strlen(buffer) + 1, 0);
        expected_frame++;
    }

    // Cleanup
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
