#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <string.h>

char incomingPackets[100];

int main(){
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET udpSocket2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // declaring udp socket
    struct sockaddr_in name1;
    name1.sin_family = AF_INET;
    name1.sin_addr.s_addr = INADDR_ANY;
    name1.sin_port = htons(9999);
    struct sockaddr_in senderAddr;
    int senderAddrSize = sizeof(senderAddr);
    bind(udpSocket2, (struct sockaddr *)&name1, sizeof(name1)); //we binded the socket to our local host

    int bytesRecived = recvfrom(udpSocket2, incomingPackets, sizeof(incomingPackets), 0, (struct sockaddr *)&senderAddr, &senderAddrSize);
    
    if(bytesRecived > 0){
        printf("A packet was recived");
    }else{
        printf("SOCKET_ERROR");
    };

    WSACleanup();

    return 0;
}