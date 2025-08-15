#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
    int server_fd;

    struct sockaddr_in serv_address;
    int serv_address_len = sizeof(serv_address);

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)      // AF_INET for IPv4, SOCK_STREAM for TCP
    {
        std::cout << "failed creating socket" << std::endl;
        return (1);
    }
    else
        std::cout << "socket is created" << std::endl;

    std::memset(&serv_address, 0, serv_address_len);
    serv_address.sin_addr.s_addr = INADDR_ANY;
    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(12345);
    if(bind(server_fd, (struct sockaddr *) &serv_address, sizeof(serv_address)) < 0)
    {
        std::cout << "failed binding" << std::endl;
        return (1);
    }

    listen(server_fd, 1);

    std::cout << "Server listening on port " << 12345 << "..." << std::endl;
    
    
    
    
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket = accept(server_fd, (struct sockaddr *) &client_addr, &client_len);
    std::cout << "===========   client_socket = " << client_socket << "============="<<std::endl;
    if(client_socket < 0)
    {
        std::cerr << "accept failed" << std::endl;
        return 1;
    }
    std::cerr << "connection established with client" << std::endl;


    char buffer[1024];
    // ssize_t recv(int sockfd, void *buf, size_t len, int flags);
/*
    flags:

    These are optional flags that modify the behavior of recv(). Some common flags include:

    MSG_WAITALL: This flag tells recv() to wait until the specified number of bytes (given by len) are received.

    MSG_PEEK: This flag allows you to read the data from the socket without actually removing it from the socket buffer (i.e., the data can be read again in the next recv() call).

    MSG_DONTWAIT: This flag makes recv() non-blocking, so it returns immediately if no data is available.

    MSG_OOB: This flag tells recv() to read out-of-band data (for protocols like TCP/IP with urgent data).

    If no flags are needed, you can pass 0.
*/

    int bytes_recieved = recv(client_socket, buffer, sizeof(buffer), 0);
    if(bytes_recieved < 0)
    {
        std::cerr << "error reading from socket" << std::endl;
        return 1;
    }
    buffer[bytes_recieved] = 0;
    std::cout << "recieved message = " << buffer << std::endl;

    const char *response = "Hello from the server!";
    send(client_socket, response, strlen(response), 0);

    close(client_socket);
    close(server_fd);
    return (0);
}