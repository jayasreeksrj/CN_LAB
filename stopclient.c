#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <time.h>

#define PORT 8080
#define MAX_FRAMES 100

void stop_and_wait_client(SOCKET server_socket, int total_frames) {
    for (int i = 0; i < total_frames; i++) {
        char buffer[1024] = {0};

        // Send frame
        printf("Client sending Frame %d...\n", i + 1);
        send(server_socket, "Frame", strlen("Frame") + 1, 0);

        // Simulate waiting for ACK with a timeout
        int recv_result = recv(server_socket, buffer, sizeof(buffer), 0);
        if (recv_result > 0) {
            if (strcmp(buffer, "ACK") == 0) {
                printf("Client received ACK for Frame %d.\n", i + 1);
            } else {
                printf("Unexpected response: %s\n", buffer);
            }
        } else {
            printf("No ACK received for Frame %d, resending...\n", i + 1);
            i--; // Resend the same frame
        }
    }
}

int main() {
    WSADATA wsaData;
    SOCKET client_socket;
    struct sockaddr_in server_address;

    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr); // Connect to localhost

    // Connect to server
    connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    printf("Connected to server.\n");

    int total_frames;
    printf("Enter the total number of frames to send: ");
    scanf("%d", &total_frames);

    stop_and_wait_client(client_socket, total_frames);

    closesocket(client_socket);
    WSACleanup();
    return 0;
}
