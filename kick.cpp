#include "Server.hpp"

void Server::removeFromChannel(const std::string &channel_name, int fd_client, std::vector<Channel> &channels)
{
    for (size_t i = 0; i < channels.size(); i++)
    {
        if (channels[i].name == channel_name)
        {
            if (channels[i].removeClient(fd_client))
            {
                std::cout << "client :<" << fd_client << "> Removed from channel: " + channel_name << std::endl;
                if (channels[i].clients.empty() && channels[i].admins.empty())
                {
                    std::cout << "server : Channel :" << channel_name << " is empty, deleting it." << std::endl;
                    channels.erase(channels.begin() + i);
                }
				else if (channels[i].admins.empty() && !channels[i].clients.empty())
				{
					channels[i].admins.push_back(channels[i].clients[0]);
					channels[i].clients.erase(channels[i].clients.begin());
				}
            }
            return;
        }
    }
}

void    Server::removeFromChannels(int index_client, std::vector<Channel> &channels)
{
    for (size_t i = 0; i < channels.size(); i++)
    {
        if (channels[i].removeClient(clients[index_client].fd_client))
        {
			std::string msg = " @" + clients[index_client].nickName + " disconnect from " + channels[i].name + "\r\n";
			sendToAll(clients[index_client].fd_client, channels[i].clients, channels[i].admins, msg);
            std::cout << "client <" << clients[index_client].fd_client << "> : Removed from channel: " + channels[i].name << std::endl;
            if (channels[i].clients.empty() && channels[i].admins.empty())
            {
                std::cout << "Channel " << channels[i].name << " is empty, deleting it." << std::endl;
                channels.erase(channels.begin() + i);
                i--;
            }
			else if (channels[i].admins.empty() && !channels[i].clients.empty())
			{
				channels[i].admins.push_back(channels[i].clients[0]);
				channels[i].clients.erase(channels[i].clients.begin());
			}
        }
    }
}

void Server::kick(int index_client, std::vector<std::string> cmd_args)
{
    std::string channel_name;
    std::string user_kicked;
    int         index_channel;
    int         index_kicked;

    if (cmd_args.size() < 3 || cmd_args.size() > 4)
    {
        send_log(index_client, "KICK : Invalid parameters\r\n");
        server_log(index_client, "KICK : Invalid parameters");
        return;
    }
    if (cmd_args.size() == 4)
    {
        channel_name  = cmd_args[2];
        user_kicked   = cmd_args[3].substr(1);
    }
    else
    {
        channel_name  = cmd_args[1];
        user_kicked   = cmd_args[2];
    }
    index_channel = channelFound(channels, channel_name);
    index_kicked  = userFound(clients, user_kicked);
    if (index_channel == -1)
    {
        send_log(index_client, "KICK : No such channel\r\n");
        server_log(index_client, "KICK : No such channel");
        return;
    }
    if (!isExistInChannel(clients[index_client].fd_client, channels[index_channel].clients, channels[index_channel].admins))
    {
        send_log(index_client, "KICK : You're not on that channel\r\n");
        server_log(index_client, "KICK : You're not on that channel");
        return;
    }
    if (!isOperator(clients[index_client].fd_client, channels[index_channel].admins))
    {
        send_log(index_client, "KICK  : You are not operator\r\n");
        server_log(index_client, "KICK : You are not operator");
        return; 
    }
    if (index_client == index_kicked)
    {
        send_log(index_client, "KICK  : Can't kick yourself\r\n");
        server_log(index_client, "KICK : Can't kick yourself");
        return;
    }
    if (index_kicked == -1 || !isExistInChannel(clients[index_kicked].fd_client, channels[index_channel].clients, channels[index_channel].admins))
    {
        send_log(index_client, "KICK : No such user\r\n");
        server_log(index_client, "KICK : No such user");
        return;
    }
    removeFromChannel(channel_name, clients[index_kicked].fd_client, channels);
    std::string msg = " @" + clients[index_client].nickName + " Kiked @" + user_kicked + " from " + channel_name + "\r\n";
    sendToAll(clients[index_client].fd_client, channels[index_channel].clients, channels[index_channel].admins, msg);
    send_log(index_kicked, ":You were kicked from " + channel_name + "\r\n");
    send_log(index_client, "Kick : you Kicked @" + user_kicked + " from " + channel_name + "\r\n");
}

