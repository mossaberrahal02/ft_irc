#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <cstdlib>

class Server {
    private:
        int port;
        int serverSocket, newSocket;
        struct sockaddr_in serverAddress;
        std::string password;
    public:
        Server();
        ~Server();
        Server(int p);
        Server(int port, const std::string& password);
        Server(const Server& other);
        Server& operator=(const Server& other);
        
        void setPort(int p);
        int getPort() const;

        void start();
        void stop();
};