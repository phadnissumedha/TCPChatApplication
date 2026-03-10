CXX = g++
CXXFLAGS = -Iinclude -Wall

SERVER_SRC = src/server.cpp src/logger.cpp src/config.cpp src/socketUtils.cpp
CLIENT_SRC = src/client.cpp src/logger.cpp src/config.cpp src/socketUtils.cpp

SERVER_OUT = build/server
CLIENT_OUT = build/client

all: server client

server:
	mkdir -p build
	$(CXX) $(SERVER_SRC) $(CXXFLAGS) -o $(SERVER_OUT)

client:
	mkdir -p build
	$(CXX) $(CLIENT_SRC) $(CXXFLAGS) -o $(CLIENT_OUT)

clean:
	rm -rf build