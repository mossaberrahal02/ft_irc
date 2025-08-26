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
	std::string 		userName;
	std::string			hostname;
	std::string			servername;
	std::string			realname;
	bool 				authenticated;
	std::string 		password;
	std::string 		nickName;
	std::string			ipAddr;
	int					max_channel;
	bool				disconnected;
	Client(int fd);
	~Client();
};

