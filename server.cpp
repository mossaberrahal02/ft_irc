#include "server.hpp"
#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <arpa/inet.h>

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
void Server::Error(const std::string& message) {
    //close all sockets and free ressources
    /* TODO i have to clear ressources in this function  */
    std::cerr << "Error: " << message << std::endl;
}
void Server::addNewClientToPollFds(int clientSocket) {
    new_cli.fd = clientSocket;
    new_cli.events = POLLIN;
    new_cli.revents = 0;
    poll_fds.push_back(new_cli);
}
void Server::initServerSocket() {

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == 0) 
        return Error("Socket creation failed.");

    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        return Error("Setsockopt failed.");
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
        return Error("Bind failed.");
    if (listen(serverSocket, 3) < 0)
        return Error("Listen failed.");
    
    std::cout << "Server started. Listening on port " << port << "..." << std::endl;
    // Add the server socket to poll_fds
    addNewClientToPollFds(serverSocket);
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
int Server::getServerSocket() const {
    return serverSocket;
}

std::string Server::getPassword() const {
    return password;
}

Server::~Server() {
    std::cout << "Server destroyed." << std::endl;
}

/* struct pollfd {
    int fd;        // File descriptor to monitor
    short events;  // Events to monitor (e.g., POLLIN, POLLOUT)
    short revents; // Events that actually occurred (output)
};

Monitoring Multiple Descriptors: You pass an array of pollfd structures to poll(), each representing a file descriptor you want to monitor (like client sockets or pipes).
Waiting for Events: The program waits for events on these file descriptors. When any of them is ready (e.g., data available for reading), poll() returns, and you can process the event.
Event Reporting: The revents field in each pollfd structure is updated by poll() to indicate what actually happened (e.g., if the socket is ready for reading, revents will contain POLLIN).
 */

std::vector<struct pollfd> &Server::getPollFds()
{
    return poll_fds;
}

void Server::coreServerLoop()
{
    std::cout << "Core server loop started." << std::endl;
    std::cout << "Waiting for client connections..." << std::endl;
    while (1)
    {
        if(poll(getPollFds().data(), getPollFds().size(), -1) < 0) 
            {Error("Poll failed.");exit(EXIT_FAILURE);}
        for (unsigned int i = 0; i < getPollFds().size(); i++)
        {
            if (getPollFds()[i].revents & POLLIN)
            {
                Client tempClient;
                memset(&clientAddress, 0, sizeof(clientAddress));
                socklen_t len = sizeof(clientAddress);
                int incofd = accept(serverSocket, (sockaddr *)&(clientAddress), &len);
                if (incofd == -1)
                    {std::cout << "accept() failed" << std::endl; return;}

                new_cli.fd = incofd;
                new_cli.events = POLLIN;
                new_cli.revents = 0;

                tempClient.setClientSocket(incofd);
                tempClient.setIpAddress(inet_ntoa(clientAddress.sin_addr));

                clients.push_back(tempClient);
                poll_fds.push_back(new_cli);
                std::cout << "New client connected: " << tempClient.getIpAddress() << " on socket " << incofd << std::endl;
            }
        }
    }
}