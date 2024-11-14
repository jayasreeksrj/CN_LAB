#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int frame_number = 0;
    int total_frames, window_size;

    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        perror("Socket creation failed");
        return 1;
    }

    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        return 1;
    }

    printf("Connected to server.\n");

    // Prompt user for the total number of frames to send and the window size
    printf("Enter the total number of frames to send: ");
    scanf("%d", &total_frames);
    printf("Enter the window size: ");
    scanf("%d", &window_size);

    // Loop to send frames according to the window size
    while (frame_number < total_frames) {
        int send_count = 0;

        // Send frames in the window size
        while (send_count < window_size && frame_number < total_frames) {
            // Create frame message
            snprintf(buffer, sizeof(buffer), "Frame %d", frame_number);
            printf("Client sending: %s\n", buffer);
            send(client_socket, buffer, strlen(buffer) + 1, 0);
            send_count++;
            frame_number++;
        }

        // Wait for ACKs
        for (int i = 0; i < send_count; i++) {
            recv(client_socket, buffer, sizeof(buffer), 0);
            printf("Client received: %s\n", buffer);
        }
    }

    // Cleanup
    closesocket(client_socket);
    WSACleanup();

    return 0;
}
