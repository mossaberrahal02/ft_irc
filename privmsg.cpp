#include "Server.hpp"
#include <iostream>

int Server::get_client_index_by_nick(const std::string &nick)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].nickName == nick)
            return i;
    }
    return -1;
}

Client *get_client_by_fd()
{
    
}

void    Server::send_to_client(int fd, std::string log)
{
    std::cout << "sending to fd = <" << send_to_client << "> : " << " nickname : " << clients[index_client].nickName << " " << std::endl;
	int ret = send(clients[index_client].fd_client, log.c_str(), log.size(), 0);
    if (ret == -1)
    {
        std::cout << "i could'nt send to <" << clients[index_client].fd_client << "> : " << " nickname : " << clients[index_client].nickName << " " << std::endl;
        std::cerr << "send error" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Server::send_to_channel(int index_client, std::string &channel, const std::string &message)
{
    // std::string chan_name = channel.substr(1); // remove # or &
    bool found = false;
    for (size_t i = 0; i < channels.size(); i++)
    {
        std::cout << "Checking channel: " << channels[i].name  << " received: " << channel << std::endl;
        if (channels[i].name == channel)
        {
            std::cout << "Channel name : " << channels[i].name << std::endl;
            found = true;
            for (size_t j = 0; j < channels[i].clients.size(); j++)
            {
                std::cout << "      clients in this channel : " << channels[i].clients[j].nickName << std::endl;
                std::cout << "        file descriptors " << channels[i].clients[j].fd_client << std::endl;
                std::cout <<"          " << clients[index_client].nickName <<" Sending to client: " << channels[i].clients[j].nickName << " message: " << message << std::endl;
                send_to_client(channels[i].clients[j].fd_client, message);
            }
            break;
        }
    }
    std::cout << "channel found? " << (found ? "yes" : "no") << std::endl;
    if (!found)
    {
        send_log(index_client, "PRIVMSG : No such channel\r\n");
        server_log(index_client, "PRIVMSG : No such channel");
    }
}

void Server::privmsg(int index_client, std::vector<std::string> cmd_args)
{
	if (cmd_args.size() < 3)
    {
        send_log(index_client, "PRIVMSG : invalid args : <PRIVMSG> <recipient> <message>\r\n");
        server_log(index_client, "PRIVMSG : invalid args");
        return;
    }
    std::string reciepient = cmd_args[1];
    std::string message = "";
    for(unsigned int i = 2; i < cmd_args.size(); i++)
    {
        if (i > 2)
            message += " ";
        message += cmd_args[i];
    }
    int reciepient_index = get_client_index_by_nick(reciepient);
    if(reciepient_index == index_client)
    {
        send_log(index_client, "You cannot send a message to yourself\r\n");
        server_log(index_client, "You cannot send a message to yourself");
        return;
    }
    if (!message.empty() && reciepient[0] != '#' && reciepient[0] != '&')
    {
        if (reciepient_index != -1)
        {
            send_log(reciepient_index, "@" + clients[index_client].nickName + ": " + message + "\r\n");
            server_log(index_client, "Sent private message to " + reciepient);
        }
        else
        {
            send_log(index_client, "404 PRIVMSG : No such nick\r\n");
            server_log(index_client, "404 PRIVMSG : No such nick");
        }
    }
    else if (reciepient[0] == '#' || reciepient[0] == '&')
        send_to_channel(index_client, reciepient, "@" + clients[index_client].nickName + ": " + message + "\r\n");
}