#include "Bot.hpp"
#include "Server.hpp"
#include <iostream>


void Server::Help()
{
    std::string Greeting = "";
    Greeting.append("                                                                                \n");
    Greeting.append("\t Usage: Bot [nickName] [option]\r\t\t\t\t\t\t\t\t\t\t \n");
    Greeting.append("\t How Can I Help You: (You can use The following list of commands)\r\t\t\t\t\t\t\t\t\t\t \n");
    Greeting.append("\t [0]: to List all Your stats\r\t\t\t\t\t\t\t\t\t\t \n");
    Greeting.append("\t [1]: to List all Your Joined Channels\r\t\t\t\t\t\t\t\t\t\t \n");
    Greeting.append("\t [2]: to see How many user online\r\t\t\t\t\t\t\t\t\t\t \n");
    Greeting.append("\t [3]: to List all Channels in Server\r\t\t\t\t\t\t\t\t\t\t \n");
    Greeting.append("\t [4]: to List stats of specific Channel\r\t\t\t\t\t\t\t\t\t\t \n");
    Greeting.append("\t [5]: to List Infos about the Server\r\t\t\t\t\t\t\t\t\t\t \n");
    
    std::cout << Greeting;
}

void Server::bot(int index_client, std::vector<std::string> cmd_args)
{
    //BOT <nickName> <option>
    if (cmd_args.size() == 1)
        Help();
	else if (cmd_args.size() == 3)
	{
        int client = get_client_index_by_nick(cmd_args[1]);
        if(client == -1)
        {
            send_to_client(index_client, "No such client\r\n");
            send_to_client(index_client, "BOT <nickName> <option>\r\n");
            server_log(index_client, "No such client\r\n");
            return ;
        }
		if (cmd_args[2] == "0")
			return (getUserInfo(client));
		// else if (cmd_args[2] == "1")
		// 	return ("Online Users: " + std::to_string(this->_online_c - 1) + "\n");
		else if (cmd_args[2] == "2")
			return (listAllChannels());
		else if (cmd_args[2] == "4")
		{
			if (cmd_args.size() == 3)
				return(_channelInfo(cmd_args[1], index_client));
			else
				return ("Usage of this Command: DEEZNUTS 4 [CHANNEL NAME]\n");
		}
		else if (cmd_args[2] == "5")
			return (_serverInfo());
	}
}

std::string Server::_serverInfo() const
{
	std::string server("Server Name: " + this->_name + "\n");
	server.append("Online Users: " + std::to_string(this->_online_c - 1) + "\n");
	server.append("Max Online Users: " + std::to_string(this->_max_online_c) + "\n");
	server.append("Number of Channels in the Server: " + std::to_string(this->_allChannels.size()) + "\n");
	return (server);
}

std::string Server::_channelInfo(std::string ChannelName, int i)
{
	std::map<std::string, Channel *>::const_iterator it = this->_allChannels.find(ChannelName);
	if (it != this->_allChannels.end())
	{
		if (this->clients[i]->isJoined(ChannelName))
		{
			std::string Info;
			Info.append("Channel Name: " + it->second->getName() + "\n");
			Info.append("Channel Creator: " + it->second->getCreator()->getFullName() + "\n");
			Info.append("Online Users: " + std::to_string(it->second->getOnlineUsers()) + "\n");
			Info.append("Channel Topic: " + it->second->getTopic() + "\n");
			return (Info);
		}
		else
			return ("You Need To Join th channel first\n");
	}
	return ("There's No Channel Named " + ChannelName + "!\n");
}

// void	Server::listAllChannels() const
// {
// 	for (int i = )
// 	{
// 		"Channel Name: " << channel.second->getName() << "\n";
// 		"Channel Creator: " << channel.second->getCreator()->getFullName() << "\n";
// 		"Online Users: " << channel.second->getOnlineUsers() << "\n";
// 		"Channel Topic: " << channel.second->getTopic() << "\n";
// 	}
// };
