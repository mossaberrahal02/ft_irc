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


bool Channel::removeClient(int fd_client)
{
    bool removed = false;

    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].fd_client == fd_client)
        {
            clients.erase(clients.begin() + i);
            removed = true;
            break;
        }
    }

    for (size_t i = 0; i < admins.size(); i++)
    {
        if (admins[i].fd_client == fd_client)
        {
            admins.erase(admins.begin() + i);
            removed = true;
            break;
        }
    }

    for (size_t i = 0; i < invited.size(); i++)
    {
        if (invited[i] == fd_client)
        {
            invited.erase(invited.begin() + i);
            break;
        }
    }

    return removed;
}
