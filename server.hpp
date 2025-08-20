#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

class Server {
    int port;
    int serverSocket, newSocket;
    struct sockaddr_in address;
public:
    Server();
    ~Server();
    Server(int p);
    Server(const Server& other);
    Server& operator=(const Server& other);
    
    void setPort(int p);
    int getPort() const;

    void start();
    void stop();
};