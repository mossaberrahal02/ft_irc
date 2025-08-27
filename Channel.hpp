#pragma once
#include <iostream>
#include <vector>
#include "Client.hpp"
#define MAX_CLIENTS 100


class Channel
{
public:
	bool invit_only;
	int limit;
	bool topic_restriction;
	std::string name;
	std::string password;
	std::string topic_name;
	std::vector<Client> clients;
	std::vector<Client> admins;
	std::vector<int>	invited;
	Channel();
	~Channel();
	bool	removeClient(int fd_client);
};
