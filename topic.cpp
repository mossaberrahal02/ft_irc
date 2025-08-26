#include "Server.hpp"

void Server::show_topic(int index_client, const std::string &channel_name)
{
    int index_channel = channelFound(channels, channel_name);
    if (index_channel == -1)
    {
        send_log(index_client, "TOPIC : " + channel_name + " :No such channel\r\n");
        server_log(index_client, "TOPIC : No such channel");
        return;
    }

    if (!isExistInChannel(clients[index_client].fd_client, channels[index_channel].clients, channels[index_channel].admins))
    {
        send_log(index_client, "TOPIC : " + channel_name + " :You're not on that channel\r\n");
        server_log(index_client, "TOPIC : You're not on that channel");
        return;
    }

    if (channels[index_channel].topic_name.empty())
    {
        send_log(index_client,"TOPIC : " +  channel_name + " :No topic is set\r\n");
        server_log(index_client, "TOPIC :No topic is set");
    }
    else
    {
        send_log(index_client,"TOPIC : " +  channel_name + " :" + channels[index_channel].topic_name + "\r\n");
        server_log(index_client, "TOPIC : " +  channel_name + " :" + channels[index_channel].topic_name);
    }
}


bool    Server::isOperator(int fd_client, std::vector<Client> &admins)
{
    for (size_t i = 0; i < admins.size(); i++)
	{
		if (fd_client == admins[i].fd_client)
			return true;
	}
    return false;
}

void Server::edit_topic(int index_client, const std::string &channel_name, const std::string &new_topic)
{
    int index_channel = channelFound(channels, channel_name);
    if (index_channel == -1)
    {
        send_log(index_client, "TOPIC :" + channel_name + " : No such channel\r\n");
        server_log(index_client, "TOPIC : No such channel");
        return;
    }

    if (!isExistInChannel(clients[index_client].fd_client, channels[index_channel].clients, channels[index_channel].admins))
    {
        send_log(index_client, "TOPIC :" + channel_name + " :You're not on that channel\r\n");
        server_log(index_client, "TOPIC : You're not on that channel");
        return;
    }
    if(channels[index_channel].topic_restriction && !isOperator(clients[index_client].fd_client, channels[index_channel].admins))
    {
        send_log(index_client, "TOPIC : You're not operator\r\n");
        server_log(index_client, "TOPIC : You're not operator");
        return;
    }
    channels[index_channel].topic_name = new_topic;

    sendToAll(clients[index_client].fd_client, channels[index_channel].clients, channels[index_channel].admins,
        channels[index_channel].name + " : @" + clients[index_client].nickName + " set topic to :" + new_topic + "\r\n");
    send_log(index_client, "TOPIC : " + channels[index_channel].name + " : you set topic to :" + new_topic + "\r\n");
    server_log(index_client, "TOPIC : " + channels[index_channel].name + " : set topic to :" + new_topic);
}

void Server::topic(int index_client, std::vector<std::string> cmd_args)
{
    if (cmd_args.size() == 2)
        show_topic(index_client, cmd_args[1]);
    else if (cmd_args.size() >= 3)
    {
        std::string new_topic;
        for (size_t i = 2; i < cmd_args.size(); i++)
        {
            if (i > 2) new_topic += " ";
            new_topic += cmd_args[i];
        }

        if (!new_topic.empty() && new_topic[0] == ':')
            new_topic = new_topic.substr(1);

        edit_topic(index_client, cmd_args[1], new_topic);
    }
    else
    {
        send_log(index_client, "TOPIC :Invalid parameters\r\n");
        server_log(index_client, "TOPIC :Invalid parameters.");
    }
}
