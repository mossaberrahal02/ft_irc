#pragma once
#include <iostream>



class Channel
{
public:
	int invit_only;
	int topic;
	int key;
	int limit;
	bool topic_restriction;
	std::string name;
	// std::string time_creation;
	std::string password;
	std::string topic_name;
	std::vector<Client> clients;
	std::vector<Client> admins;
	Channel();
	~Channel();
};

