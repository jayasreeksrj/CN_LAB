#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 43451
#define BUFFER_SIZE 1024

// Function to respond to predefined questions
void respondToQuestion(char *question, char *response) {
    if (strcmp(question, "hello") == 0) {
        strcpy(response, "Hello! How can I help you?");
    } else if (strcmp(question, "how are you") == 0) {
        strcpy(response, "I'm doing great, thank you! How about you?");
    } else if (strcmp(question, "what is your name") == 0) {
        strcpy(response, "I am your friendly server!");
    } else if (strcmp(question, "what time is it") == 0) {
        strcpy(response, "It’s always a good time to learn something new!");
    } else if (strcmp(question, "tell me a joke") == 0) {
        strcpy(response, "Why don’t scientists trust atoms? Because they make up everything!");
    } else if (strcmp(question, "what can you do") == 0) {
        strcpy(response, "I can answer your questions, do simple calculations, and tell you jokes!");
    } else if (strcmp(question, "who created you") == 0) {
        strcpy(response, "I was created by a dedicated programmer using C and sockets!");
    } else if (strcmp(question, "what is the meaning of life") == 0) {
        strcpy(response, "42... according to *The Hitchhiker's Guide to the Galaxy*.");
    } else if (strcmp(question, "can you help me") == 0) {
        strcpy(response, "Of course! Ask me anything.");
    } else if (strcmp(question, "what is the weather like") == 0) {
        strcpy(response, "I'm not connected to the internet, so I can't check. But I hope it's sunny!");
    } else if (strcmp(question, "what is your favorite color") == 0) {
        strcpy(response, "I like all colors equally! I’m a program, after all.");
    } else if (strcmp(question, "what is 2 + 2") == 0) {
        strcpy(response, "2 + 2 is 4. Easy math!");
    } else if (strcmp(question, "how old are you") == 0) {
        strcpy(response, "I was born the moment my code started running!");
    } else if (strcmp(question, "what do you like to do") == 0) {
        strcpy(response, "I enjoy helping people with questions and making them smile with jokes!");
    } else if (strcmp(question, "exit") == 0) {
        strcpy(response, "Goodbye!");
    } else {
        strcpy(response, "I didn't understand the question.");
    }
}

int main() {
    WSADATA wsaData;
    SOCKET serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLen, recvLen;
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create a socket
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        return 1;
    }

    // Set up server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        return 1;
    }

    printf("UDP Server is running on port %d\n", SERVER_PORT);

    // Wait for client messages
    while (1) {
        clientAddrLen = sizeof(clientAddr);
        recvLen = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&clientAddr, &clientAddrLen);
        buffer[recvLen] = '\0'; // Null-terminate the received string
        printf("Client: %s\n", buffer);

        // Respond to the received question
        respondToQuestion(buffer, response);

        // Send the response back to the client
        sendto(serverSocket, response, BUFFER_SIZE, 0, (struct sockaddr*)&clientAddr, clientAddrLen);

        // If the client sends "exit", break the loop
        if (strcmp(buffer, "exit") == 0) {
            printf("Client requested to exit.\n");
            break;
        }
    }

    // Clean up and close the socket
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
