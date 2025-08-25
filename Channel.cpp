#include "Channel.hpp"


Channel::Channel():
invit_only(0),
limit(MAX_CLIENTS),
topic_restriction(false)
{
	
}

Channel::~Channel()
{
}

Channel::Channel(const Channel& obj)
{
	invit_only			= obj.invit_only;
	limit				= obj.limit;
	topic_restriction	= obj.topic_restriction;
	name				= obj.name;
	password			= obj.password;
	topic_name			= obj.topic_name;
	clients				= obj.clients;
	admins				= obj.admins;
	invited				= obj.invited;
}

Channel&	Channel::operator=(const Channel& obj)
{
	if (&obj == this)
		return *this;
	invit_only			= obj.invit_only;
	limit				= obj.limit;
	topic_restriction	= obj.topic_restriction;
	name				= obj.name;
	password			= obj.password;
	topic_name			= obj.topic_name;
	clients				= obj.clients;
	admins				= obj.admins;
	invited				= obj.invited;
	return *this;
}