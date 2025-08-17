#include "main.hpp"



int main(int ac, char** av)
{
    Server  server(ac, av);

    server.buff_readed = read(server.clients[0].fd_client, server.buffer, MAX_BUFF);
    if (server.buff_readed < 0)
    {
        perror("read");
        close(server.fd_server);
        close(server.clients[0].fd_client);
        return 1;
    }
    server.buffer[server.buff_readed] = '\0';
    std::cout << "client send : " << server.buffer << std::endl;
    close(server.fd_server);
    close(server.clients[0].fd_client);
    return 0;
}