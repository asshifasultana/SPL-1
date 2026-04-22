#include<stdio.h>
#include"network.h"
#include<winsock2.h>


void sendImage(const char *filename, const char *ip,
                    int port, const char *username) {

    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;

    WSAStartup(MAKEWORD(2,2), &wsa);        //initializes windows socket api
    sock = socket(AF_INET, SOCK_STREAM, 0);  //creates tcp socket,TCP without pipeline, ACK, error checking

    //sets receivers ip and port
    server.sin_family      = AF_INET;
    server.sin_port        = htons(port);
    server.sin_addr.s_addr = inet_addr(ip);   //converts ip to binary format

    printf("[*] Connecting to %s at %s:%d...\n", username, ip, port);


    //try to connect with the receiver, if receiver not alive, connection fails
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Connection failed!\n");
        closesocket(sock);
        WSACleanup();
        return;
    }

    printf("Connected!\n");

    //after connecting, telling about the sender
    char senderName[50] = "Asshifa";
    send(sock, senderName, sizeof(senderName), 0);

    FILE *f = fopen(filename, "rb");
    if (!f) return;

    //open the file, reads size, send to receiver so receiver knows what to expect
    fseek(f, 0, SEEK_END);
    long fileSize = ftell(f);
    rewind(f);

    send(sock, (char*)&fileSize, sizeof(long), 0);

    unsigned char buffer[4096];
    int bytesRead;
    long totalSent = 0;

    //reads 4096 bytes
    //sends a chunk of data
    //repeat
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), f)) > 0) {
        send(sock, buffer, bytesRead, 0);
        totalSent += bytesRead;
    }

    printf("Sent %ld bytes\n", totalSent);

    //closes file and connection
    //shut down the whole networking system
    fclose(f);
    closesocket(sock);
    WSACleanup();
}

void sendToUser(const char *filename){
    char ip[50];
    char username[100];
    int port = 2222;

    printf("Send Stego Image \n");

    printf("Enter receiver username:");
    scanf("%s",username);

    printf("\n Enter user IP:");
    scanf("%s",ip);

    printf("\n Enter port number:");
    scanf("%d",&port);

    char confirm;
    printf("\n Confirm sending?(y/n)");
    scanf(" %c",&confirm);

    if(confirm=='y' || confirm=='Y'){
        sendImage(filename,ip,port,username);
    }

    else{
        printf("\n Send Cancelled");
        return;
    }
}





