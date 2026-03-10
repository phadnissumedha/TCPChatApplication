Client-Server Chat Application (C++)

--> A terminal-based client-server chat application built using C++ and Linux socket programming.
It enables real-time communication between a server and a client using TCP sockets, with logging, configuration management, and graceful shutdown handling.
_________________________________________________________________________________________________________________________________
Features :

1. TCP socket-based communication

2. Configuration using .ini file

3. Logging system with timestamps

4. Chat history storage

5. Graceful shutdown using signals

6. Error handling for network failures

7. Input validation for empty messages

8. Client–server exit command support

__________________________________________________________________________________________________________________________________

Architecture : 
          +------------------+
          |      CLIENT      |
          |------------------|
          |  SENDS MESSAGE   |
          |  RECEIVES REPLY  |
          +---------+--------+
                    |
                    |  TCP SOCKET CONNECTION
                    |
          +---------V--------+
          |      SERVER      |
          |------------------|
          | RECEIVES MESSAGE |
          | SENDS RESPONSE   |
          | LOGS ACTIVITY    |
          +------------------+
________________________________________________________________________________________________________________________________
Folder Structure :

chatApplication/
│
├── src/
│   ├── server.cpp
│   ├── client.cpp
│   ├── logger.cpp
│   ├── config.cpp
│   └── socketUtils.cpp
│
├── include/
│   ├── logger.h
│   ├── config.h
│   └── socketUtils.h
│
├── logs/
│   ├── app.log
│   └── chatHistory.txt
│
├── build/
│   ├── server
│   └── client
│
├── config.ini
├── Makefile
└── README.md

_____________________________________________________________________________________________________________________________________

Configuration :

--> The application reads configuration from config.ini.

Build Instructions :

--> Compile the project using Makefile.

commands : make  / make server / make client
These command will generate build/server and build/client.

clean build files : make clean

Running application :

step 1 --> start server : ./build/server
	   Server will start listening on the configured port.
	   
step 2 --> start Client in another terminal : ./build/client
	   Client connects to the server using the configured IP and port.
			   
______________________________________________________________________________________________________________________________________			   

Exit methods :

The application can be closed in multiple ways.
1. Exit command :
--> exit

2. Ctrl + C : 
--> Gracefully shuts down the application using signal handling.
	   
3. Network Disconnect
--> If the client or server disconnects unexpectedly, the application detects the failure and exits safely.
_______________________________________________________________________________________________________________________________________

Logging System :

All logs are stored in : logs/app.log

Chat History :

All chat messages are stored in : logs/chatHistory.txt


________________________________________________________________________________________________________________________________________
Error Handling :

The application handles the following scenarios -

1. Missing configuration values

2. Socket creation failure

3. Port binding failure

4. Client connection failure

5. Network disconnection

6. Empty messages

_______________________________________________________________________________________________________________________________________

Technologies Used : 

C++

Linux Socket Programming

TCP/IP Networking

Signal Handling

Makefile Build System

_______________________________________________________________________________________________________________________________________

Learning Outcomes :

This project helps understand -

1. low-level networking in C++

2. TCP socket lifecycle

3. Linux signal handling

4. configuration-driven applications

5. structured logging

_______________________________________________________________________________________________________________________________________
