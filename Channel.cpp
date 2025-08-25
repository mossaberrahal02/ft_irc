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
