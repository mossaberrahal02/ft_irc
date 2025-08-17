#pragma once
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <netinet/in.h>
#define MAX_BUFF 1024

class Server
{	
public:
	int					fd_server;
	struct sockaddr_in	addr_server;
	std::string			passwd;
	char				buffer[MAX_BUFF];
	ssize_t				buff_readed;
	Server();
	~Server();
};
