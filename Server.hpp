#pragma once
#include <iostream>
#include <netinet/in.h>
#define MAX_BUFF 1024

class Server
{	
public:
	int					fd_server;
	struct sockaddr_in	addr_server;
	socklen_t			socklen_server;
	char*				buff[MAX_BUFF];
	Server();
	~Server();
};
