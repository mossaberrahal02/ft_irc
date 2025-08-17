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
        perror("socket");
        exit(EXIT_FAILURE);
    }
    addr_server.sin_addr.s_addr = INADDR_ANY;
    addr_server.sin_family = AF_INET;
    addr_server.sin_port = htons(std::atoi(av[1]));
    passwd = std::string(av[2]);
    if (bind(fd_server,
        reinterpret_cast <struct sockaddr *>(&addr_server),
        sizeof(addr_server)) < 0)
    {
        perror("bind");
        close(fd_server);
        exit(EXIT_FAILURE);
    }
    if (listen(fd_server, SOMAXCONN) < 0)
    {
        perror("listen");
        close(fd_server);
        exit(EXIT_FAILURE);
    }
    std::cout << "Server started on port            : " << ntohs(addr_server.sin_port) << std::endl;
    std::cout << "Server started with the password  : " << passwd << std::endl;
}

Server::~Server()
{
	close(fd_server);
}

void			Server::acceptConnection()
{
	int		newFdClient;
    
	newFdClient = accept(fd_server, NULL, NULL);
    if (newFdClient < 0)
    {
        std::cerr << "Can't accepte the client" << std::endl;
        return;
    }
    else
        std::cout << "Client connected" << std::endl;
	clients.push_back(Client(newFdClient));
}