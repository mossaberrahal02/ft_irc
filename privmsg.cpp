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
    std::cout << "sending to fd = <" << fd << "> : " << " nickname : " << get_client_by_fd(fd)->nickName << " " << std::endl;
	int ret = send(get_client_by_fd(fd)->fd_client, log.c_str(), log.size(), 0);
    if (ret == -1)
    {
        std::cout << "i could'nt send to <" << get_client_by_fd(fd)->fd_client << "> : " << " nickname : " << get_client_by_fd(fd)->nickName << " " << std::endl;
        std::cerr << "send error" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Server::send_to_channel(int index_client, std::string &channel, const std::string &message)
{
    std::cout << "Sending to channel: " << channel << " message: " << message << std::endl;
    for (size_t i = 0; i < channels.size(); i++)
    {
        if (channels[i].name == channel)
        {
            for (size_t j = 0; j < channels[i].clients.size(); j++)
            {
                if (channels[i].clients[j].fd_client != clients[index_client].fd_client)
                {
                    std::cout << "Sending to channel client: " << channels[i].clients[j].nickName << " fd: " << channels[i].clients[j].fd_client << " message: " << message << std::endl;
                    send_to_client(channels[i].clients[j].fd_client, message);
                }
            }
            for (size_t j = 0; j < channels[i].admins.size(); j++)
            {
                if (channels[i].admins[j].fd_client != clients[index_client].fd_client)
                {
                    std::cout << "Sending to channel admin: " << channels[i].admins[j].nickName << " fd: " << channels[i].admins[j].fd_client << " message: " << message << std::endl;
                    send_to_client(channels[i].admins[j].fd_client, message);
                }
            }
            return;
        }
    }
    send_to_client(index_client, "PRIVMSG : No such channel\r\n");
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