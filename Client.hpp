#pragma once
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>


class Client
{
public:
	int					fd_client;
	struct sockaddr_in	client_addr;
	socklen_t			client_addr_len;
	std::string 		buffer;
	std::string 		nickName;
	std::string 		userName;
	bool 				authenticated;
	std::string 		password;
	std::string			ipAddr;
	Client(int fd);
	~Client();
};

