#include <iostream>
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include "socketUtils.h"
#include "logger.h"
#include "config.h"

using namespace std;

void handleSignal(int signal);

/* Timestamp function */
string getTimestamp()
{

    time_t now = time(0);
    struct tm *ltm = localtime(&now);

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);

    return string(buffer);
}

/* Startup validation */
bool validateStartup() 
{

    LOG_MSG("INFO","Starting server configuration validation");

    string port = getConfigValue("PORT");
    string logFile = getConfigValue("LOG_FILE");
    string chatFile = getConfigValue("CHAT_HISTORY");

    if(port.empty())
    {
        LOG_MSG("ERROR","PORT missing in config file.");
        cout << "PORT missing in config file.\n";
        return false;
    }

    LOG_MSG("INFO","PORT configuration loaded successfully.");

    if(logFile.empty()) 
    {
        LOG_MSG("ERROR","LOG_FILE missing in config file.");
        cout << "LOG_FILE missing in config.ini\n";
        return false;
    }

    LOG_MSG("INFO","LOG_FILE configuration loaded successfully.");

    if(chatFile.empty()) 
    {
        LOG_MSG("ERROR","CHAT_HISTORY missing in config file.");
        cout << "CHAT_HISTORY missing in config.ini\n";
        return false;
    }

    LOG_MSG("INFO","CHAT_HISTORY configuration loaded successfully.");

    LOG_MSG("INFO","Server startup validation completed successfully.");

    return true;
}

int main() {

    /* Register signal handlers */
    signal(SIGINT, handleSignal);   // Ctrl + C
    signal(SIGTERM, handleSignal);  // kill command
    signal(SIGQUIT, handleSignal);  
    signal(SIGHUP, handleSignal);   // terminal closed

    if(!validateStartup()) 
    {
        LOG_MSG("ERROR","Server startup validation failed.");
        return 1;
    }

    int server_fd, clientSocket;
    struct sockaddr_in address;
    char buffer[1024];

    string chatFilePath = getConfigValue("CHAT_HISTORY");
    int port = stoi(getConfigValue("PORT"));

    ofstream chatFile(chatFilePath, ios::app);

    if(!chatFile) {
        cout << "Error opening chat history file.\n";
        LOG_MSG("ERROR","Unable to open chat history file.");
        return 1;
    }

    /* Socket creation test case */
    server_fd = createSocket();

    if(server_fd < 0) {
        LOG_MSG("ERROR","Socket creation failed.");
        return 1;
    }

    LOG_MSG("INFO","Server socket created.");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    /* Bind failure test case */

    if(bind(server_fd,(struct sockaddr*)&address,sizeof(address)) < 0) 
    {
        LOG_MSG("ERROR","Bind failed. Port may already be in use.");
        return 1;
    }

    LOG_MSG("INFO","Socket bind successful.");

    /* Listen failure test case */

    if(listen(server_fd,3) < 0) {
        LOG_MSG("ERROR","Listen failed.");
        return 1;
    }

    LOG_MSG("INFO","Server listening on port " + to_string(port));
    LOG_MSG("INFO","Waiting for client connection.");

    cout << "Waiting for client connection...\n";

    clientSocket = accept(server_fd,NULL,NULL);

    /* Client connection failure test case */

    if(clientSocket < 0) {
        LOG_MSG("ERROR","Client connection failed.");
        return 1;
    }

    LOG_MSG("INFO","Client connected successfully.");

    while(true) {

        memset(buffer,0,sizeof(buffer));

        int bytesReceived = receiveMessage(clientSocket,buffer,1024);

        /* Network failure / client disconnect */
        if(bytesReceived <= 0) {

            LOG_MSG("ERROR","Client disconnected or network failure detected.");
            cout << "Client disconnected.\n";
            close(clientSocket);
            break;
        }

        if(strcmp(buffer,"exit") == 0)
        {
            LOG_MSG("INFO","Client requested connection close.");
            cout << "Client exited chat.\n";
            close(clientSocket);
            break;
        }

        /* Empty client message */
        if(strlen(buffer) == 0) {

            LOG_MSG("WARNING","Empty message received from client.");
            continue;
        }

        LOG_MSG("INFO","Client message received: " + string(buffer));

        cout << "Client: " << buffer << endl;

        chatFile << getTimestamp() << " | Client: " << buffer << endl;

        cout << "Server: ";
        cin.getline(buffer,1024);

        if(strcmp(buffer,"exit") == 0)
        {
            LOG_MSG("INFO","Server initiated connection close.");
            sendMessage(clientSocket,"Server closed connection.");
            close(clientSocket);
            break;
        }

        /* Empty server message */
        if(strlen(buffer) == 0)
        {

            LOG_MSG("WARNING","Server attempted to send empty message.");
            cout << "Empty message not sent." << endl;
            continue;
        }

        sendMessage(clientSocket,buffer);

        LOG_MSG("INFO","Server message sent: " + string(buffer));

        chatFile << getTimestamp() << " | Server: " << buffer << endl;
    }

    chatFile.close();
    close(server_fd);

    LOG_MSG("INFO","Server shutdown complete.");

    return 0;
}

/* Graceful shutdown */
void handleSignal(int signal) {

    LOG_MSG("INFO","Server interrupted (Ctrl+C). Shutting down...");
    cout << "\nServer shutting down safely...\n";

    exit(0);
}