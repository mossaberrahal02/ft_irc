#include "Client.hpp"

Client::Client() 
{
    this->clientSocket = -1;
    this->clientNickName = "";
    this->isOperator = false;
    this->isRegistered = false;
    this->isAuthenticated = false;
    this->ipAddress = "";
    this->clientUserName = "";
}

Client::~Client() {
}

Client::Client(int clientSocket, std::string clientNickName, std::string clientUserName)
    : clientSocket(clientSocket), clientNickName(clientNickName), clientUserName(clientUserName) {
}

Client::Client(int socket, const struct sockaddr_in& address)
    : clientSocket(socket), clientAddress(address), isOperator(false), isRegistered(false), isAuthenticated(false) {
}

Client::Client(const Client& other)
    : clientSocket(other.clientSocket), clientAddress(other.clientAddress), clientNickName(other.clientNickName),
      isOperator(other.isOperator), isRegistered(other.isRegistered), isAuthenticated(other.isAuthenticated),
      ipAddress(other.ipAddress), clientUserName(other.clientUserName) {
}

Client& Client::operator=(const Client& other) {
    if (this != &other) {
        clientSocket = other.clientSocket;
        clientAddress = other.clientAddress;
        clientNickName = other.clientNickName;
        isOperator = other.isOperator;
        isRegistered = other.isRegistered;
        isAuthenticated = other.isAuthenticated;
        ipAddress = other.ipAddress;
        clientUserName = other.clientUserName;
    }
    return *this;
}

void Client::setClientSocket(int socket) {
    clientSocket = socket;
}

int Client::getClientSocket() const {
    return clientSocket;
}

void Client::setClientAddress(const struct sockaddr_in& address) {
    clientAddress = address;
}

struct sockaddr_in Client::getClientAddress() const {
    return clientAddress;
}

void Client::setClientNickName(const std::string& clientNickName) {
    this->clientNickName = clientNickName;
}

std::string Client::getClientNickName() const {
    return clientNickName;
}

void Client::setIsOperator(bool isOp) {
    isOperator = isOp;
}

bool Client::getIsOperator() const {
    return isOperator;
}

void Client::setIsRegistered(bool isReg) {
    isRegistered = isReg;
}

bool Client::getIsRegistered() const {
    return isRegistered;
}

void Client::setIsAuthenticated(bool isAuth) {
    isAuthenticated = isAuth;
}

bool Client::getIsAuthenticated() const {
    return isAuthenticated;
}
void Client::setIpAddress(const std::string& ip) {
    ipAddress = ip;
}

std::string Client::getIpAddress() const {
    return ipAddress;
}

void Client::setClientUserName(const std::string& user) {
    clientUserName = user;
}

std::string Client::getClientUserName() const {
    return clientUserName;
}