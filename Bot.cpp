#include "Bot.hpp"
#include "Server.hpp"
#include <iostream>
#include <string>

Bot::Bot()
{
}

Bot::~Bot()
{
}

void Server::Help(int index_client)
{
    std::string Greeting = "";
    Greeting.append("Usage: BOT <option> [argument]\r\n");
    Greeting.append("[0] <nick>   : Show user info\r\n");
    Greeting.append("[1]          : List online users\r\n");
    Greeting.append("[2]          : List all channels\r\n");
    Greeting.append("[3] <channel>: Show channel info\r\n");
    Greeting.append("[4]          : Show server info\r\n");
    send_to_client(clients[index_client].fd_client, Greeting);
    server_log(index_client, Greeting);
}

void Server::bot(int index_client, std::vector<std::string> cmd_args)
{
    if (cmd_args.size() == 1) 
        return Help(index_client);

    std::string option = cmd_args[1];

    if (option == "0" && cmd_args.size() == 3) 
	{
        int target = get_client_index_by_nick(cmd_args[2]);
        if (target == -1)
            return send_to_client(clients[index_client].fd_client, "No such user\r\n"), server_log(index_client, "No such user\r\n");
        return getUserInfo(target, index_client);
    }
    else if (option == "1")
        return onlineUsers(index_client);
    else if (option == "2")
        return listAllChannels(index_client);
    else if (option == "3" && cmd_args.size() == 3)
        return channelInfo(index_client, cmd_args[2]);
    else if (option == "4")
        return serverInfo(index_client);

    send_to_client(clients[index_client].fd_client, "Invalid BOT usage\r\n");
	server_log(index_client, "Invalid BOT usage\r\n");
}

void Server::getUserInfo(int target, int index_client)
{
	for (unsigned int i = 0; i < this->clients.size(); i++)
	{
		if (this->clients[i].nickName == this->clients[target].nickName)
		{
			std::string userInfo;
			userInfo.append("User Info:\r\n");
			userInfo.append("Nickname: " + this->clients[i].nickName + "\r\n");
			userInfo.append("Username: " + this->clients[i].userName + "\r\n");
			send_to_client(clients[index_client].fd_client, userInfo);
			server_log(index_client, userInfo);
			return ;
		}
	}
	send_to_client(clients[index_client].fd_client, "No such user\r\n");
	server_log(index_client, "No such user\r\n");
	return ;
}

void Server::onlineUsers(int index_client)
{
	std::string onlineUsersList = "Online Users:\r\n";
	for (size_t i = 1; i < this->fds.size(); ++i)
	{
		if(this->fds[i].fd != -1 && get_client_by_fd(this->fds[i].fd) != NULL)
		{
			onlineUsersList += get_client_by_fd(this->fds[i].fd)->nickName + "\r\n";
		}
	}
	send_to_client(clients[index_client].fd_client, onlineUsersList);
	server_log(index_client, onlineUsersList);
}

void Server::serverInfo(int index_client) {
    std::string server("Server Name: " + this->serverName + "\r\n");
    server.append("Online Users:\r\n");
    for (size_t i = 0; i < this->clients.size(); ++i)
        server.append(" - " + this->clients[i].nickName + "\r\n");
    send_to_client(clients[index_client].fd_client, server);
    server_log(index_client, server);
}


void Server::channelInfo(int index_client, std::string ChannelName) {
    for (size_t i = 0; i < this->channels.size(); i++) {
        if (this->channels[i].name == ChannelName) {
            bool isMember = false;
            for (size_t j = 0; j < this->channels[i].clients.size(); j++) {
				if (this->channels[i].clients[j].nickName == clients[index_client].nickName) {
					isMember = true;
                    break;
                }
            }
			for (size_t j = 0; j < this->channels[i].admins.size(); j++) {
				if (this->channels[i].admins[j].nickName == clients[index_client].nickName) {
					isMember = true;
                    break;
                }
            }
            if (!isMember) {
                send_to_client(clients[index_client].fd_client, "You are not a member of this channel\r\n");
                server_log(index_client, "You are not a member of this channel\r\n");
                return;
            }

            std::string Info;
            Info.append("Channel Name: " + this->channels[i].name + "\r\n");

            Info.append("Admins:\r\n");
            for (size_t k = 0; k < this->channels[i].admins.size(); k++)
                Info.append(" - " + this->channels[i].admins[k].nickName + "\r\n");

            Info.append("Members:\r\n");
            for (size_t m = 0; m < this->channels[i].clients.size(); m++)
                Info.append(" - " + this->channels[i].clients[m].nickName + "\r\n");

            if (this->channels[i].topic_name.empty())
                Info.append("Topic: (no topic)\r\n");
            else
                Info.append("Topic: " + this->channels[i].topic_name + "\r\n");

            send_to_client(clients[index_client].fd_client, Info);
            server_log(index_client, Info);
            return;
        }
    }
    send_to_client(clients[index_client].fd_client, "No such channel\r\n");
    server_log(index_client, "No such channel\r\n");
}


void	Server::listAllChannels(int index_client)
{
	std::string allChannels = "List of all Channels:\r\n";
	
	if (this->channels.empty())
	{
	    allChannels.append("		No channels available.\r\n");
		send_to_client(clients[index_client].fd_client, allChannels);
		server_log(index_client, allChannels);
		return;
	}
	for (unsigned int i = 0; i < this->channels.size(); i++)
	{
		allChannels.append("Channel Name: " + this->channels[i].name + "\r\n");
		if (this->channels[i].topic_name.empty())
			allChannels.append("Topic: (no topic)\r\n");
		else
			allChannels.append("Topic: " + this->channels[i].topic_name + "\r\n");
	}
	send_to_client(clients[index_client].fd_client, allChannels);
	server_log(index_client, allChannels);
};