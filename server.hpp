#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <cstdlib>
#include <poll.h>
#include <vector>
#include "Client.hpp"

class Server {
    private:
        int port;
        int serverSocket, newSocket;
        struct sockaddr_in serverAddress;
        struct sockaddr_in clientAddress;
        std::string password;
        struct pollfd new_cli;
        std::vector<struct pollfd> poll_fds;
        std::vector<Client> clients;

    public:
        Server();
        ~Server();
        Server(int p);
        Server(int port, const std::string& password);
        Server(const Server& other);
        Server& operator=(const Server& other);
        
        void setPort(int p);
        int getPort() const;
        int getServerSocket() const;
        std::string getPassword() const;
        std::vector<struct pollfd> &getPollFds();
        Client * getClientBySocket(int clientSocket);

        void initServerSocket();
        void stop();
        void Error(const std::string& message);
        void addNewClientToPollFds(int clientSocket);
        void coreServerLoop();
        void receiveNewData(int clientSocket);

        std::vector<std::string> splitBufferContent(const std::string &str);
};