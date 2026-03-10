#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include "socketUtils.h"

using namespace std;

int createSocket() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    
    if(sock < 0) {
        cout << "Socket creation failed" << endl;
    }

    return sock;
}

void sendMessage(int socket, const char* message) {
    send(socket, message, strlen(message)+1, 0);
}

int receiveMessage(int socket, char* buffer, int size) {
   int bytesReceived = recv(socket, buffer, size, 0);
   return bytesReceived;
}