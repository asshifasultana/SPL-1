#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>                   //networking stuff in windows

int main() {
    WSADATA wsa;
    SOCKET server_sock, client_sock;
    struct sockaddr_in server, client;
    int c;

    // Initialize Winsock
    WSAStartup(MAKEWORD(2,2), &wsa);     //starting windows network

    
    // Creates a socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == INVALID_SOCKET) {
        printf("Socket creation failed!\n");
        return 1;
    }


    // Setup server
    server.sin_family = AF_INET;          //IPv4
    server.sin_addr.s_addr = INADDR_ANY; // listen on all interfaces
    server.sin_port = htons(2222);       // same port as sender


    // Bind
    if (bind(server_sock, (struct sockaddr*)&server, sizeof(server)) < 0) { //attaches socket to port 2222
        printf("Bind failed!\n");
        return 1;
    }


    // Listen
    listen(server_sock, 1);                     //starts waiting for someone to call, 1 means 1 person can wait in queue
    printf("[+] Waiting for incoming connection on port 2222...\n");


    // Accept connection
    c = sizeof(struct sockaddr_in);
    client_sock = accept(server_sock, (struct sockaddr*)&client, &c);
    if (client_sock < 0) {
        printf("Accept failed!\n");
        return 1;
    }

    printf("[+] Connection accepted!\n");


    // Receives sender username and print
    char senderName[50];
    recv(client_sock, senderName, sizeof(senderName), 0);
    printf("[+] Sender: %s\n", senderName);


    // Receive file size
    long fileSize;
    recv(client_sock, (char*)&fileSize, sizeof(long), 0);       //learns how may bytes to accpet
    printf("[+] Receiving file of size: %ld bytes\n", fileSize);


    // Open file to write
    FILE *f = fopen("received.bmp", "wb");
    if (!f) {
        printf("File creation failed!\n");
        return 1;
    }

    // Receive file data
    char buffer[4096];
    int bytesReceived;
    long totalReceived = 0;


    while (totalReceived < fileSize) {   //until full file arrives
        bytesReceived = recv(client_sock, buffer, sizeof(buffer), 0); //receive
        if (bytesReceived <= 0) break;      //sender disconnected,stops immediately

        fwrite(buffer, 1, bytesReceived, f);    //writes to file
        totalReceived += bytesReceived;         //count total data received

        printf("\r[*] Receiving... %.1f%%",
               (float)totalReceived / fileSize * 100);
        fflush(stdout);
    }


    printf("\n[+] File received successfully!\n");
    printf("[+] Saved as: received.bmp\n");


    //closes file and network connection
    fclose(f);
    closesocket(client_sock);
    closesocket(server_sock);
    WSACleanup();

    return 0;
}