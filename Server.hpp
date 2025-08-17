#pragma once
#include <iostream>
#include <vector>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <netinet/in.h>
#include "Client.hpp"
#include "Channel.hpp"
#define MAX_BUFF 1024

class Server
{
public:
	int						fd_server;
	struct sockaddr_in		addr_server;
	std::string				passwd;
	char					buffer[MAX_BUFF];
	ssize_t					buff_readed;
	std::vector <Client>	clients;
	std::vector <Channel>	channels;
	Server(int ac, char **av);
	~Server();
	void			acceptConnection();

};
