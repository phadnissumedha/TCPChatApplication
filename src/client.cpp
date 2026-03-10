#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include "logger.h"
#include "socketUtils.h"
#include "config.h"

using namespace std;

int clientSocket;
void handleSignal(int signal);

int main()
{
    /* Register signal handlers */
    signal(SIGINT, handleSignal);   // Ctrl + C
    signal(SIGTERM, handleSignal);  // kill command
    signal(SIGQUIT, handleSignal);  
    signal(SIGHUP, handleSignal);   // terminal closed

    struct sockaddr_in serverAddr;
    char buffer[1024];

    /* Create socket */
    clientSocket = createSocket();

    string serverIP = getConfigValue("SERVER_IP");
    string portStr = getConfigValue("PORT");

    if(serverIP.empty() || portStr.empty())
    {
        cout << "Config values missing!" << endl;
        LOG_MSG("ERROR","SERVER_IP or PORT missing in config file.");
        return 1;
    }

    int port = stoi(portStr);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    /* Validate IP address */
    if(inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr) <= 0)
    {
        cout << "Invalid server IP address." << endl;
        LOG_MSG("ERROR","Invalid SERVER_IP in config.");
        return 1;
    }

    /* Connect to server */
    if(connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        cout << "Unable to connect to server." << endl;
        LOG_MSG("ERROR","Connection to server failed.");
        return 1;
    }

    LOG_MSG("INFO","Client connected to Server.");

    while(true)
    {

        cout << "Client: ";
        cin.getline(buffer,1024);

        /* Empty message test case */
        if(strlen(buffer) == 0) 
        {
            LOG_MSG("WARNING","Client attempted to send empty message.");
            cout << "Empty message not sent." << endl;
            continue;
        }

        /* Send message */
        sendMessage(clientSocket, buffer);
        LOG_MSG("INFO", string("Message sent to server: ") + buffer);

        memset(buffer,0,sizeof(buffer));

        /* Receive server response */
        int bytesReceived = receiveMessage(clientSocket, buffer, 1024);

        /* Network failure / server disconnect */
        if(bytesReceived <= 0)
        {
            cout << "Server disconnected." << endl;
            LOG_MSG("ERROR","Server connection lost.");
            break;
        }

        /* Server exit command */
        if(strcmp(buffer,"exit") == 0)
        {
            cout << "Server closed the connection." << endl;
            LOG_MSG("INFO","Server initiated connection close.");
            break;
        }

        cout << "Server: " << buffer << endl;
        LOG_MSG("INFO", string("Message received from server: ") + buffer);
    }

    close(clientSocket);

    return 0;
}

void handleSignal(int signal)
{

if(signal == SIGINT)
        LOG_MSG("INFO","SIGINT received (Ctrl+C). Client shutting down...");
    else if(signal == SIGTERM)
        LOG_MSG("INFO","SIGTERM received.Client shutting down...");
    else if(signal == SIGQUIT)
        LOG_MSG("INFO","SIGQUIT received.Client shutting down...");
    else if(signal == SIGHUP)
        LOG_MSG("INFO","SIGHUP received.Client shutting down...");
    else
        LOG_MSG("INFO","Unknown signal received.Client shutting down...");

    cout << "\nClient shutting down safely..." << endl;

    close(clientSocket);

    exit(0);
}