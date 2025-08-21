#include "server.hpp"
#include <iostream>

int isValidPort(int port, const char* portStr) {
    if ( port < 1024 || port > 65535 ) 
        return 0;
    for(int i = 0; portStr[i] != '\0'; i++) {
        if (!isdigit(portStr[i])) 
            return 0;
    }
    return 1;
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }

    int port = atoi(argv[1]);
    std::string password = argv[2];

    if(!isValidPort(port, argv[1]))
    {
        std::cerr << "Invalid port number: " << argv[1] << std::endl;
        return 1;
    }

    Server server(port, password);
    server.start();
    return 0;
}