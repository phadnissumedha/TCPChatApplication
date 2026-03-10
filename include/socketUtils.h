#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

int createSocket();
void sendMessage(int socket, const char* message);
int receiveMessage(int socket, char* buffer, int size);

#endif