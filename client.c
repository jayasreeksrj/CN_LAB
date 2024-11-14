#include<stdio.h>
#include<winsock2.h>    // For Winsock API
#include<ws2tcpip.h>    // For inet_pton and other functions

#pragma comment(lib,"ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in servaddr;
    int choice;
    double principal, rate, time;
    int num;

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Winsock initialization failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(43451);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Connection to server failed. Error Code: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("Connected to server.\n");

    while (1) {
        printf("----- Calculator Menu -----\n");
        printf("1. Simple Interest\n");
        printf("2. Compound Interest\n");
        printf("3. Power\n");
        printf("4. Factorial\n");
        printf("5. Exit\n");
        printf("---------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        send(sock, (char*)&choice, sizeof(choice), 0);
        if (choice == 5) break; // Exit choice

        switch (choice) {
            case 1: // Simple Interest
                printf("Enter principal: ");
                scanf("%lf", &principal);
                printf("Enter rate: ");
                scanf("%lf", &rate);
                printf("Enter time: ");
                scanf("%lf", &time);
                send(sock, (char*)&principal, sizeof(principal), 0);
                send(sock, (char*)&rate, sizeof(rate), 0);
                send(sock, (char*)&time, sizeof(time), 0);
                break;
            case 2: // Compound Interest
                printf("Enter principal: ");
                scanf("%lf", &principal);
                printf("Enter rate: ");
                scanf("%lf", &rate);
                printf("Enter time: ");
                scanf("%lf", &time);
                send(sock, (char*)&principal, sizeof(principal), 0);
                send(sock, (char*)&rate, sizeof(rate), 0);
                send(sock, (char*)&time, sizeof(time), 0);
                break;
            case 3: // Power
                printf("Enter base: ");
                scanf("%d", &num);
                printf("Enter exponent: ");
                int exponent;
                scanf("%d", &exponent);
                send(sock, (char*)&num, sizeof(num), 0);
                send(sock, (char*)&exponent, sizeof(exponent), 0);
                break;
            case 4: // Factorial
                printf("Enter number: ");
                scanf("%d", &num);
                send(sock, (char*)&num, sizeof(num), 0);
                break;
            default:
                printf("Invalid choice.\n");
                continue;
        }

        // Receive result
        int result;
        recv(sock, (char*)&result, sizeof(result), 0);
        printf("Result: %d\n", result);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
