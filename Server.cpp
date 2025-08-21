#include "Server.hpp"

void valid_args(char **av)
{
    for (int i = 0; av[1][i]; i++)
        if (std::isdigit(av[1][i]) == 0)
        {
            std::cerr << "Please Enter a Valid Port or default <6667>" << std::endl;
            exit(EXIT_FAILURE);
        }
    if (std::atoi(av[1]) < 1024 || std::atoi(av[1]) > 65535)
    {
        std::cerr << "Please Enter a Valid Port or default <6667>" << std::endl;
        exit(EXIT_FAILURE);
    }
}

Server::Server(int ac, char **av)
{
    int en = 1;

	if (ac != 3)
    {
        std::cout << "usage : ./ircserv <port> <password>" << std::endl;
        exit(EXIT_FAILURE);
    }
    valid_args(av);
	memset(&addr_server, 0, sizeof(addr_server));
    fd_server = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_server < 0)
    {
        std::cerr << "can't create socket" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (fcntl(fd_server, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "can't set server non blocking" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
    {
        std::cerr << "setsockopt error" << std::endl;
        exit(EXIT_FAILURE);
    }
    addr_server.sin_addr.s_addr = INADDR_ANY;
    addr_server.sin_family = AF_INET;
    addr_server.sin_port = htons(std::atoi(av[1]));
    passwd = std::string(av[2]);
    if (bind(fd_server,
        (sockaddr *)&addr_server,
        sizeof(addr_server)) < 0)
    {
        std::cerr << "can't bind connection" << std::endl;
        close(fd_server);
        exit(EXIT_FAILURE);
    }
    if (listen(fd_server, SOMAXCONN) < 0)
    {
        std::cerr << "listen Error" << std::endl;
        close(fd_server);
        exit(EXIT_FAILURE);
    }
    new_cli.fd = fd_server;
    new_cli.events = POLLIN;
    new_cli.revents = 0;
    fds.push_back(new_cli);
    std::cout << "Server started on port            : " << ntohs(addr_server.sin_port) << std::endl;
    std::cout << "Server started with the password  : " << passwd << std::endl;
}

Server::~Server()
{
	close(fd_server);
}



void        Server::loop()
{
    int pollValue;
    std::cout << "Waiting to accept a connection...\n";
    while(true)
    {
        pollValue = poll(&fds[0], fds.size(), -1);
        if ( pollValue < 0)
        {
            std::cerr << "poll() Error" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (pollValue == 0)
            continue;
        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].revents & POLLIN)
            {
                if (fds[i].fd == fd_server)
                    new_connection();
                else
                    process_client_data(fds[i].fd);
            }
        }
    }
}

void        Server::new_connection()
{
    Client client(-1);
    client.fd_client = accept(fd_server,
        reinterpret_cast<struct sockaddr *>(&client.client_addr),
        &client.client_addr_len);
    if (fcntl( client.fd_client, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "can't set server non blocking" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (client.fd_client < 0)
    {
        std::cerr << "can't accept new client" << std::endl;
        return;
    }
    if (fcntl(client.fd_client, F_SETFL, O_NONBLOCK) == -1)
    {
        std::cerr << "fcntl Error" << std::endl;
        return;
    }
    new_cli.fd = client.fd_client;
    new_cli.events = POLLIN;
    new_cli.revents = 0;
    client.ipAddr = std::string(inet_ntoa(client.client_addr.sin_addr));
    clients.push_back(client);
    fds.push_back(new_cli);
    std::cout << "Client <" << client.fd_client << "> Connected" << std::endl;
    std::string welcome = "Welcome to the server!\nPlease Register yourself on our server!\r\n";
    send(client.fd_client, welcome.c_str(), welcome.size(), 0);
}


void        Server::process_client_data(int fd)
{
    Client *client = getClient(fd);
    if (client == NULL)
        return;
    memset(buffer, 0, sizeof(buffer));
    buff_readed = recv(fd, buffer, MAX_BUFF - 1, 0);
    if (buff_readed <= 0) //client disconneced ...
    {
        std::cout << "Client " << client->userName << " disconnected." << std::endl;
        // removeFromChannel(fd);
        // fds.erase(client);
        // clients.erase(client)
        return;
    }
    std::cout << "client " << client->fd_client << " write : " << buffer << std::endl;
}


Client*     Server::getClient(int fd)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].fd_client == fd)
        return &clients[i];
    }
    return NULL;
}
