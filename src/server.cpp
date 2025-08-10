#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
    int server_fd, client_fd;

    char buffer[1024];

    struct sockaddr_in address;
    int address_len = sizeof(address);

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)      // AF_INET for IPv4, SOCK_STREAM for TCP
    {
        std::cout << "failed creating socket" << std::endl;
        return (1);
    }
    else
        std::cout << "socket is created" << std::endl;

    std::memset(&address, 0, sizeof(address));
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_family = AF_INET;
    address.sin_port = htons(12345);
    if(bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0)
    {
        std::cout << "failed binding" << std::endl;
        return (1);
    }

    listen(server_fd, 1);

    
    return (0);
}