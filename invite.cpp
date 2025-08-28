#include "Server.hpp"

int    Server::userFound(std::vector<Client> &clients, std::string nickname)
{
    for (size_t i = 0; i < clients.size(); i ++)
        if (clients[i].nickName == nickname)
            return i;
    return -1;
}


int    Server::channelFound(std::vector<Channel> &channels, std::string channel_name)
{
    for (size_t i = 0; i < channels.size(); i ++)
        if (channels[i].name == channel_name)
            return i;
    return -1;
}


void    Server::invite(int index_client, std::vector <std::string> cmd_args)
{
    std::string nickname;
    std::string channel_name;
    int         index_channel = -1;
    int         index_invited = -1;

    if (cmd_args.size() != 3)
    {
        send_log(index_client, "INVITE : Invalid parameters\r\n");
        server_log(index_client, "INVITE : Invalid parameters.");
        return;
    }
    nickname = cmd_args[1];
    channel_name = cmd_args[2];
    index_channel = channelFound(channels, channel_name);
    index_invited = userFound(clients, nickname);
    if (index_invited == -1 || index_channel == -1)
    {
        send_log(index_client, "INVITE : No Such user or channel\r\n");
        server_log(index_client, "INVITE : No Such user or channe");
        return;
    }
    if (!isExistInChannel(clients[index_client].fd_client, channels[index_channel].clients, channels[index_channel].admins))
    {
        send_log(index_client, "INVITE : You are not on that channel\r\n");
        server_log(index_client, "INVITE : You are not on that channel");
        return;
    }
    if (isExistInChannel(clients[index_invited].fd_client, channels[index_channel].clients, channels[index_channel].admins))
    {
        send_log(index_client, "INVITE : Already in this channel\r\n");
        server_log(index_client, "INVITE : Already in this channel");
        return;
    }
    if (channels[index_channel].invit_only)
        channels[index_channel].invited.push_back(clients[index_invited].fd_client);
    sendToAll(clients[index_client].fd_client, channels[index_channel].clients, channels[index_channel].admins,
        "INVITE : @" + clients[index_client].nickName + " invite @" + nickname + " to " + channel_name + "\r\n");
    send_log(index_client, "INVITE : you invite @" + nickname + " to " + channel_name +"\r\n");
    send_log(index_invited, " @" + clients[index_client].nickName + " invite you to " + channel_name + "\r\n");
    server_log(index_client, "invite @" + nickname + " to " + channel_name);
}
