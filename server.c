#include<stdio.h>
#include<winsock2.h>    // For Winsock API
#include<ws2tcpip.h>    // For inet_pton and other functions
#include<math.h>
// Link with ws2_32.lib for Windows
#pragma comment(lib,"ws2_32.lib")

void func(SOCKET confd);

int main() {
    WSADATA wsa;
    SOCKET sockfd, confd;
    struct sockaddr_in servaddr, cliaddr;
    int len;

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Winsock initialization failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    printf("Winsock initialized.\n");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    printf("\nSocket is created\n");

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(43451);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR) {
        printf("\nBind failed. Error Code: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    printf("\nBind successfully\n");

    if (listen(sockfd, 5) == SOCKET_ERROR) {
        printf("\nListen failed. Error Code: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    printf("\nListen successfully\n");

    len = sizeof(cliaddr);
    confd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);
    if (confd == INVALID_SOCKET) {
        printf("\nAccept failed. Error Code: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    printf("\nServer accepted successfully\n");

    func(confd);

    closesocket(confd);
    closesocket(sockfd);
    WSACleanup();

    return 0;
}

void func(SOCKET confd) {
    int choice, result;
    double principal, rate, time;
    int num, powerResult = 1;
    
    while (1) {
        // Receive user's choice
        recv(confd, (char*)&choice, sizeof(choice), 0);
        if (choice == 5) break; // Exit choice

        switch (choice) {
            case 1: // Simple Interest
                recv(confd, (char*)&principal, sizeof(principal), 0);
                recv(confd, (char*)&rate, sizeof(rate), 0);
                recv(confd, (char*)&time, sizeof(time), 0);
                result = (principal * rate * time) / 100;
                send(confd, (char*)&result, sizeof(result), 0);
                break;
            case 2: // Compound Interest
                recv(confd, (char*)&principal, sizeof(principal), 0);
                recv(confd, (char*)&rate, sizeof(rate), 0);
                recv(confd, (char*)&time, sizeof(time), 0);
                result = principal * pow((1 + rate / 100), time);
                send(confd, (char*)&result, sizeof(result), 0);
                break;
            case 3: // Power
                recv(confd, (char*)&num, sizeof(num), 0);
                recv(confd, (char*)&powerResult, sizeof(powerResult), 0);
                for (int i = 0; i < num; i++) {
                    powerResult *= num;
                }
                send(confd, (char*)&powerResult, sizeof(powerResult), 0);
                break;
            case 4: // Factorial
                recv(confd, (char*)&num, sizeof(num), 0);
                result = 1;
                for (int i = 1; i <= num; i++) {
                    result *= i;
                }
                send(confd, (char*)&result, sizeof(result), 0);
                break;
            default:
                printf("Invalid choice.\n");
        }
    }
}
