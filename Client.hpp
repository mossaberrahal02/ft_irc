#pragma once
#include <iostream>
#include <netinet/in.h>


class Client
{
public:
	int					fd_client;
	struct sockaddr_in	client_addr;
	socklen_t			client_addr_len;
	Client();
	~Client();
};

