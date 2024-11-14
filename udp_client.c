#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 43451
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    int serverAddrLen;
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create a socket
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        return 1;
    }

    // Set up server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    serverAddrLen = sizeof(serverAddr);

    while (1) {
        // Get input from the user
        printf("Enter your question (type 'exit' to quit): ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;  // Remove the newline character

        // Send message to the server
        sendto(clientSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&serverAddr, serverAddrLen);

        // If the client types 'exit', quit the loop
        if (strcmp(buffer, "exit") == 0) {
            printf("Client is exiting...\n");
            break;
        }

        // Receive the server's response
        recvfrom(clientSocket, response, BUFFER_SIZE, 0, (struct sockaddr*)&serverAddr, &serverAddrLen);
        printf("Server: %s\n", response);
    }

    // Clean up and close the socket
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
