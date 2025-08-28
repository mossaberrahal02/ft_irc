#include "Server.hpp"

bool	isInvited(int fd_client, std::vector<int> &invited)
{
	for (size_t i = 0; i < invited.size(); i++)
		if (fd_client == invited[i])
			return true;
	return false;
}

bool	Server::isExistInChannel(int fd_client, std::vector<Client> &channel_clients, std::vector<Client> &admins)
{
	for (size_t i = 0; i < channel_clients.size(); i++)
	{
		if (fd_client == channel_clients[i].fd_client)
			return true;
	}
	for (size_t i = 0; i < admins.size(); i++)
	{
		if (fd_client == admins[i].fd_client)
			return true;
	}
	return false;
}

void	Server::sendToAll(int fd_client, std::vector<Client> &clients, std::vector<Client> &admins, std::string log)
{
	for (size_t i = 0; i < admins.size(); i++)
		if (admins[i].fd_client != fd_client)
			send(admins[i].fd_client, log.c_str(), log.size(), 0);
	for (size_t i = 0; i < clients.size(); i++)
		if (clients[i].fd_client != fd_client)
			send(clients[i].fd_client, log.c_str(), log.size(), 0);
}

void	Server::join_channel(int index_client, int index_channel, std::string passkey)
{
	if (isExistInChannel(clients[index_client].fd_client, channels[index_channel].clients, channels[index_channel].admins))
	{
		send_log(index_client, "JOIN : You Already in this channel\r\n");
		server_log(index_client, "JOIN : Already in this channel");
		return;
	}
	if ((size_t)channels[index_channel].limit <= channels[index_channel].clients.size())
	{
		send_log(index_client, "JOIN : Channel is full\r\n");
		server_log(index_client, "JOIN : Channel is full");
		return;
	}
	if (!channels[index_channel].password.empty() && channels[index_channel].password != passkey)
	{
		send_log(index_client, "JOIN : Channel Invalid key\r\n");
		server_log(index_client, "JOIN : Channel Invalid key");
		return;
	}
	if (channels[index_channel].invit_only && !isInvited(clients[index_client].fd_client, channels[index_channel].invited))
	{
		send_log(index_client, "JOIN : Channel is Invite only\r\n");
		server_log(index_client, "JOIN : Channel is Invite only");
		return;
	}
	sendToAll(clients[index_client].fd_client, channels[index_channel].clients, channels[index_channel].admins,
		" @" + clients[index_client].nickName + " join " + channels[index_channel].name + "\r\n");
	send_log(index_client, "JOIN : you are in channel " + channels[index_channel].name + "\r\n");
	server_log(index_client, "JOIN : to channel " + channels[index_channel].name);
	channels[index_channel].clients.push_back(clients[index_client]);
	clients[index_client].max_channel ++;
}


bool isValidChannelName(const std::string &new_channel)
{
    if (new_channel.size() <= 3 || new_channel[0] != '#')
        return false;

    for (std::size_t i = 1; i < new_channel.size(); ++i) 
	{
        char c = new_channel[i];
        if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_')
            return false;
    }

    if (new_channel.size() > 20)
		return false;

	return true;
}

void	Server::create_channel(int index_client, std::string new_channel, std::string passkey)
{
	Channel n_channel;

	if (!isValidChannelName(new_channel))
	{
		send_log(index_client, "JOIN : Invalid channel name\r\n");
		server_log(index_client, "JOIN : Invalid channel name");
		return;
	}
	n_channel.password = passkey;
	n_channel.name = new_channel;
	n_channel.admins.push_back(clients[index_client]);
	channels.push_back(n_channel);
	clients[index_client].max_channel ++;
	send_log(index_client, "JOIN : Channel " + new_channel + " created\r\n");
	server_log(index_client, "create channel " + new_channel);
}

void    		Server::join(int index_client, std::vector <std::string> cmd_args)
{
	std::string		chan("");
	std::string		passkey("");
	if (cmd_args.size() < 2 || cmd_args.size() > 3)
	{
		send_log(index_client, "JOIN : Invalid parameters\r\n");
		server_log(index_client, "JOIN : Invalid parameters");
		return;
	}
	if (clients[index_client].max_channel >= 9)
	{
		send_log(index_client, "JOIN : Max Channels Reached (10)\r\n");
		server_log(index_client, "JOIN : Max Channels Reached (10)");
		return;
	}
	chan = cmd_args[1];
	if (cmd_args.size() == 3)
		passkey = cmd_args[2];
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i].name == chan)
		{
			join_channel(index_client, i, passkey);
			return;
		}
	}
	create_channel(index_client, chan, passkey);
}
