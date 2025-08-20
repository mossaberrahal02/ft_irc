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
	std::string 		nickName;
	std::string 		userName;
	std::string 		hostName;
	std::string 		realName;
	bool 				authenticated;
	std::string 		buffer;
	std::string 		password;
	Client(int fd);
	~Client();
};

