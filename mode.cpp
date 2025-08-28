#include "Server.hpp"



bool    isValidFlag(std::string &flag)
{
    if (flag.size() != 2)
        return false;
    if (flag[0] != '+' && flag[0] != '-')
        return false;
    return true;
}


void    Server::addChannelOperator(int index_target, Channel &channel, Client client)
{
    channel.admins.push_back(client);
    for(size_t i = 0; i < channel.clients.size(); i++)
    {
        if (channel.clients[i].fd_client == client.fd_client)
        {
            channel.clients.erase(channel.clients.begin() + i);
            return;
        }
    }
    send_log(index_target, channel.name + " : You are channel operator\r\n");
}

void    Server::removeChannelOperator(int index_target, Channel &channel, Client client)
{
    channel.clients.push_back(client);
    for(size_t i = 0; i < channel.admins.size(); i++)
    {
        if (channel.admins[i].fd_client == client.fd_client)
        {
            channel.admins.erase(channel.admins.begin() + i);
            return ;
        }
    }
    send_log(index_target, channel.name + " : You're not longer channel operator\r\n");
}

bool	Server::setKey(int index_client,int index_channel, std::vector<std::string> cmd_args, int adding)
{
	if (adding)
	{
		if (cmd_args.size() <= 3)
		{
			send_log(index_client, "MODE +k : Need more parameters\r\n");
			server_log(index_client, "MODE +k : Need more parameters");
			return false;
		}
		channels[index_channel].password = cmd_args[3];
	}
	else
		channels[index_channel].password.clear();
	return true;
}
	
bool	Server::setOperator(int index_client,int index_channel, std::vector<std::string> cmd_args, int adding)
{
	std::string     targetNick;
    int             index_target;

	if (cmd_args.size() <= 3)
	{
		send_log(index_client, "MODE +/-o : Need more parameters\r\n");
		server_log(index_client, "MODE +/-o : Need more parameters");
		return false;
	}
	targetNick = cmd_args[3];
	index_target = userFound(clients, targetNick);
	if(index_client == index_target)
	{
		send_log(index_client, "MODE : Can't edit yourself\r\n");
		server_log(index_client, "MODE : Can't edit yourself");
		return false;
	}
	if (index_target == -1 
		|| !isExistInChannel(clients[index_target].fd_client, channels[index_channel].clients, channels[index_channel].admins))
	{
		send_log(index_client, "MODE : No such user\r\n");
		server_log(index_client, "MODE : No such user");
		return false;
	}
	if (adding && !isOperator(clients[index_target].fd_client, channels[index_channel].admins))
		addChannelOperator(index_target, channels[index_channel], clients[index_target]);
	else if(isOperator(clients[index_target].fd_client, channels[index_channel].admins))
		removeChannelOperator(index_target, channels[index_channel], clients[index_target]);
	return true;
}

bool	Server::setLimit(int index_client,int index_channel, std::vector<std::string> cmd_args, int adding)
{
	if (adding)
	{
		if (cmd_args.size() <= 3)
		{
			send_log(index_client, "MODE +l : Need more parameters\r\n");
			server_log(index_client, "MODE +l : Need more parameters");
			return false;
		}
		channels[index_channel].limit = atoi(cmd_args[3].c_str());
	}
	else
		channels[index_channel].limit = 0;
	return true;
}

bool	Server::checkModeArgs(int index_client, std::vector<std::string> cmd_args)
{
    std::string     channelName;
    int             index_channel;

	if (cmd_args.size() < 2 || cmd_args.size() > 4)
	{
		send_log(index_client, "MODE : Invalid parameters : <MODE #chan flag [...]>\r\n");
		server_log(index_client, "MODE : Invalid parameters");
		return false;
	}

	channelName = cmd_args[1];
	index_channel = channelFound(channels ,channelName);
	if (index_channel == -1)
	{
		send_log(index_client, "MODE : No such channel\r\n");
		server_log(index_client, "MODE : No such channel");
		return false;
	}
	if (!isExistInChannel(clients[index_client].fd_client, channels[index_channel].clients, channels[index_channel].admins))
	{
		send_log(index_client, "MODE : You're not on that channel\r\n");
		server_log(index_client, "MODE : You're not on that channel");
		return false;
	}
	
	if (!isOperator(clients[index_client].fd_client, channels[index_channel].admins))
	{
		send_log(index_client, "MODE : You're not a channel operator\r\n");
		server_log(index_client, "MODE : Not a channel operator");
		return false;
	}
	return true;
}

void	Server::showModes(int index_client, int index_channel, std::vector<std::string> cmd_args)
{
	if (cmd_args.size() == 2)
	{
		std::string modes = "+";
		if (channels[index_channel].invit_only) modes += "i";
		if (channels[index_channel].topic_restriction) modes += "t";
		if (!channels[index_channel].password.empty()) modes += "k";
		if (channels[index_channel].limit > 0) modes += "l";
		if (modes.size() == 1)
			send_log(index_client, "MODE : Channel modes: No modes is set\r\n");
		else 
			send_log(index_client, "MODE : Channel modes: " + modes + "\r\n");
		server_log(index_client, "MODE : channel modes show up");
		return;
	}
}

void    Server::mode(int index_client, std::vector<std::string> cmd_args)
{
    std::string     channelName;
    int             index_channel;
    std::string     flag;
    bool            adding;
	bool			d = 0;

	if (!checkModeArgs(index_client, cmd_args))
		return;
	channelName 	= cmd_args[1];
	index_channel 	= channelFound(channels ,channelName);
	if (cmd_args.size() == 2)
	{
		showModes(index_client, index_channel, cmd_args);
		return;
	}
    flag     = cmd_args[2];
    if (!isValidFlag(flag))
    { 
        send_log(index_client, "MODE : Invalid Flag (+/-flag)\r\n");
        server_log(index_client, "MODE : Invalide Flag");
        return;
    }
    if (flag[0] == '+')
		adding = true;
    else adding = false;
	if (flag[1] ==  'i') 
		channels[index_channel].invit_only = adding, d = 1;
    else if (flag[1] ==  't')
		channels[index_channel].topic_restriction = adding, d = 1;
    else if (flag[1] == 'k')
		d = setKey(index_client, index_channel, cmd_args, adding);
    else if (flag[1] == 'o')
		d = setOperator(index_client, index_channel, cmd_args, adding);
    else if (flag[1] == 'l')
		d = setLimit(index_client, index_channel, cmd_args, adding);
    else
	{
		send_log(index_client, "MODE : Unknown mode flag\r\n");
		server_log(index_client, "MODE : Unknown mode flag");
		return;
	}
	if (d)
	{
		send_log(index_client, "MODE : command applied on channel " + channelName + "\r\n");
		server_log(index_client, "MODE : command applied on channel " + channelName);
	}
}
