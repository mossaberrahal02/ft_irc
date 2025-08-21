#include "server.hpp"
#include <iostream>
#include <sys/socket.h>

Server::Server() {
    std::cout << "Server initialized." << std::endl;
}
Server::Server(int port) : port(port) {
    std::cout << "Server initialized on port " << port << "." << std::endl;
}
Server::Server(int port, const std::string& password) : port(port), password(password) 
{
}
Server::Server(const Server& other) : port(other.port), password(other.password) {
}
Server& Server::operator=(const Server& other) {
    if (this != &other) {
        port = other.port;
        password = other.password;
    }
    return *this;
}

void Server::start() {
    std::cout << "Password: " << password << std::endl;
    std::cout << "port: " << port << std::endl;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == 0) {
        std::cerr << "Socket creation failed." << std::endl;
        return;
    }
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Setsockopt failed." << std::endl;
        return;
    }
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Bind failed." << std::endl;
        return;
    }

    if (listen(serverSocket, 3) < 0) {
        std::cerr << "Listen failed." << std::endl;
        return;
    }
    std::cout << "Server started. Listening on port " << port << "..." << std::endl;
}

void Server::stop() {
    std::cout << "Server stopped." << std::endl;
}
void Server::setPort(int p) {
    port = p;
    std::cout << "Port set to " << port << "." << std::endl;
}
int Server::getPort() const {
    return port;
}
Server::~Server() {
    std::cout << "Server destroyed." << std::endl;
}
