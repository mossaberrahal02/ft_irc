#include "Server.hpp"
#include <cstddef>
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

Client *Server::get_client_by_fd(int fd)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].fd_client == fd)
            return &clients[i];
    }
    return NULL;
}

void    Server::send_to_client(int fd, std::string log)
{
    if(get_client_by_fd(fd) == NULL) 
        return;
    if (send(get_client_by_fd(fd)->fd_client, log.c_str(), log.size(), 0) <= -1)
    {
        std::cout << "i could'nt send to <" << get_client_by_fd(fd)->fd_client << "> : " << " nickname : " << get_client_by_fd(fd)->nickName << " " << std::endl;
        std::cerr << "send error" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Server::send_to_channel(int index_client, std::string &channel, const std::string &message)
{
    for (size_t i = 0; i < channels.size(); i++)
    {
        if (channels[i].name == channel)
        {
            if (!isExistInChannel(clients[index_client].fd_client, channels[i].clients, channels[i].admins))
            {
                send_to_client(clients[index_client].fd_client, "PRIVMSG : You're not on that channel\r\n");
                server_log(index_client, "PRIVMSG : Not on that channel");
                return;
            }
            for (size_t j = 0; j < channels[i].clients.size(); j++)
            {
                if (channels[i].clients[j].fd_client != clients[index_client].fd_client)
                {
                    send_to_client(channels[i].clients[j].fd_client, message);
                    server_log(index_client, message.substr(0, message.size() - 2));
                }
            }
            for (size_t j = 0; j < channels[i].admins.size(); j++)
            {
                if (channels[i].admins[j].fd_client != clients[index_client].fd_client)
                {
                    send_to_client(channels[i].admins[j].fd_client, message);
                    server_log(index_client, message.substr(0, message.size() - 2));
                }
            }
            return;
        }
    }
    send_to_client(clients[index_client].fd_client, "PRIVMSG : No such channel\r\n");
    server_log(index_client, "PRIVMSG : No such channel");
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
    if (!message.empty() && reciepient[0] != '#')
    {
        if (reciepient_index != -1 && clients[reciepient_index].authenticated)
        {
            send_log(reciepient_index, " @" + clients[index_client].nickName + ": " + message + "\r\n");
            server_log(index_client, "PRIVMSG : to @" + reciepient + " : " + message);
        }
        else
        {
            send_log(index_client, "PRIVMSG : No such user\r\n");
            server_log(index_client, "PRIVMSG : No such user");
        }
    }
    else if (reciepient[0] == '#')
        send_to_channel(index_client, reciepient, reciepient + ": @" + clients[index_client].nickName + ": " + message + "\r\n");
}