#include "Channel.hpp"


Channel::Channel():
invit_only(0),
topic(0),
limit(MAX_CLIENTS),
topic_restriction(false)
{
	
}

Channel::~Channel()
{
}
