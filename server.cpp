#include "server.hpp"

Server::Server() {
    std::cout << "Server initialized." << std::endl;
}Server::Server(int p) : port(p) {
    std::cout << "Server initialized on port " << port << "." << std::endl;
}
Server::Server(const Server& other) : port(other.port) {
}
Server& Server::operator=(const Server& other) {
    if (this != &other) {
        port = other.port;
    }
    return *this;
}

void Server::start() {
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
