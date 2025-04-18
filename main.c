#include <winsock2.h>
#include <stdio.h>
#include <windows.h>  // idk if i need this
#include <string.h>

FILE *fptr;
FILE *fptr2;
char ipInput[15];
int listOfPorts[100];
int numberOfPorts = 0;
int j = 0;
int k = 0;
char line[200];
char dummyPacket = 'A';
char buffer[100];


int main() { //start of program
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { //i was dumb for this thanks to docs
        printf("WSAStartup failed with error: %d\n", WSAGetLastError());
        return 1;
    }
    printf("**********Port Scanner**********\n"); //for more beauty
    fptr = fopen("portsTCP.txt", "r"); //opens file
    while (fgets(line, sizeof(line), fptr) != NULL) { //reading TCP ports
        listOfPorts[j] = atoi(line);  // converting strings to int
        j++;
    }
    numberOfPorts = j;
    fclose(fptr);

    printf("Enter IP address: ");
    scanf("%s", ipInput);  // taking input for IP Address
    struct in_addr ipAddr; //checking if ip addres is valid
    ipAddr.s_addr = inet_addr(ipInput);
    if (ipAddr.s_addr == INADDR_NONE) {
        printf("Invalid IP address entered.\n");
        WSACleanup();
        return 1;
    }

    printf("TCP SOCKETS\n");
    for (int i = 0; i < numberOfPorts; i++) {
        SOCKET mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  // declaring socket
        struct sockaddr_in socketAddressInput;  // struct that holds protocol, ports, and user’s IP address
        socketAddressInput.sin_family = AF_INET;
        socketAddressInput.sin_port = htons(listOfPorts[i]);
        socketAddressInput.sin_addr.s_addr = inet_addr(ipInput);
        
        u_long mode = 1;  // 1 to enable non-blocking mode
        ioctlsocket(mySocket, FIONBIO, &mode);
        
        int connectionStatus = connect(mySocket, (struct sockaddr *)&socketAddressInput, sizeof(socketAddressInput));
        if (connectionStatus == 0) {
            printf("Port %d is open\n", listOfPorts[i]);
        } else {
            printf("Port %d is closed\n", listOfPorts[i]);
        }
        closesocket(mySocket);  // Close socket after checking
        Sleep(1300);
    }


    printf("UDP SOCKETS\n");
    fptr2 = fopen("portsUDP.txt", "r");
    while(fgets(line, sizeof(line), fptr2) != NULL){ //reading UDP ports
        listOfPorts[k] = atoi(line); //translating them from string to int
        k++;
    };
    numberOfPorts = k;
    fclose(fptr2);

    for(int k = 0; k < numberOfPorts; k++){
    struct sockaddr_in destinationAddress;
    destinationAddress.sin_family = AF_INET;  // Use IPv4
    destinationAddress.sin_port = htons(listOfPorts[k]);  // Port to send the packet to
    destinationAddress.sin_addr.s_addr = inet_addr("127.0.0.1");  // IP of the UDP server

    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // declaring udp socket
    sendto(udpSocket, &dummyPacket, sizeof(dummyPacket), 0, (struct sockaddr *)&destinationAddress, sizeof(destinationAddress));
    DWORD timeout = 1000; 
    setsockopt(udpSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
    
    int bytesReceived = recvfrom(udpSocket, buffer, sizeof(buffer), 0, NULL, NULL);
    if (bytesReceived > 0){
        printf("Port %d is open\n", listOfPorts[k]);
    }else{ 
        printf("Port %d is closed\n", listOfPorts[k]);
    };
    closesocket(udpSocket);  // Close socket after checking
    Sleep(1300);
    };


    WSACleanup();  // Clean up Winsock
    return 0;
}