#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

class Client {
    private:
        int clientSocket;
        std::string buffer;
        struct sockaddr_in clientAddress;
        std::string clientNickName;
        bool isOperator;
        bool isRegistered;
        bool isAuthenticated;
        std::string ipAddress;
        std::string clientUserName;

    public:
        Client();
        ~Client();
        Client(int clientSocket, std::string clientNickName, std::string clientUserName);
        Client(int socket, const struct sockaddr_in& address);
        Client(const Client& other);
        Client& operator=(const Client& other);

        void setClientSocket(int socket);
        int getClientSocket() const;
        void setClientAddress(const struct sockaddr_in& address);
        struct sockaddr_in getClientAddress() const;
        void setClientNickName(const std::string& clientNickName);
        std::string getClientNickName() const;
        void setIsOperator(bool isOp);
        bool getIsOperator() const;
        void setIsRegistered(bool isReg);
        bool getIsRegistered() const;
        void setIsAuthenticated(bool isAuth);
        bool getIsAuthenticated() const;
        void setIpAddress(const std::string& ip);
        std::string getIpAddress() const;
        void setClientUserName(const std::string& user);
        std::string getClientUserName() const;
        void setBuffer(const std::string& data);
        std::string getBuffer() const;
};