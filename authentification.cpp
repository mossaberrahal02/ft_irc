#include "Server.hpp"


void    Server::pass(int index_client, std::vector <std::string> cmd_args)
{
    if (!clients[index_client].password.empty())
    {
        send_log(index_client, "PASS : you already enter the password\r\n");
        server_log(index_client, "PASS : you already enter the password");
        return;
    }
    if (cmd_args.size() != 2)
    {
        send_log(index_client, "PASS : invalide args : <PASS> <password>\r\n");
        server_log(index_client, "PASS : invalide args");
        return;
    }
    if (passwd != cmd_args[1])
    {
        send_log(index_client, "PASS : incorrect password\r\n");
        server_log(index_client, "PASS : incorrect password");
        return;
    }
    clients[index_client].password = cmd_args[1];
    send_log(index_client, "PASS : password success\r\n");
    server_log(index_client, "PASS : password success");
}


bool isValidNick(const std::string &nick)
{
    if (nick.empty())
        return false;

    if (nick.size() > 9)
        return false;

    for (size_t i = 0; i < nick.size(); i++)
    {
        char c = nick[i];
        if (!std::isalnum(c) && c != '_')
            return false;
    }

    return true;
}


void Server::nick(int index_client, std::vector<std::string> cmd_args)
{
    if (cmd_args.size() != 2)
    {
        send_log(index_client, "NICK : invalid args : <NICK> <nickname>\r\n");
        server_log(index_client, "NICK : invalid args");
        return;
    }

    std::string newNick = cmd_args[1];

    if (!isValidNick(newNick))
    {
        send_log(index_client, "NICK : nickname must be alphanumeric or '_'\r\n");
        server_log(index_client, "NICK : invalid nickname format");
        return;
    }

    for (size_t i = 0; i < clients.size(); i++)
    {
        if ((int)i != index_client && clients[i].nickName == newNick)
        {
            send_log(index_client, "NICK : nickname already in use\r\n");
            server_log(index_client, "NICK : nickname already in use");
            return;
        }
    }

    clients[index_client].nickName = newNick;
    send_log(index_client, "NICK : nickname set to " + newNick + "\r\n");
    server_log(index_client, "NICK set to " + newNick);
}


void    Server::user(int index_client, std::vector <std::string> cmd_args)
{
    if (!clients[index_client].userName.empty())
    {
        send_log(index_client, "USER : Already set\r\n");
        server_log(index_client, "USER : Already set");
        return;
    }
    if (cmd_args.size() < 5) 
    {
        send_log(index_client, "USER : Not enough parameters\r\n");
        server_log(index_client, "USER : Not enough parameters");
        return;
    }
    if (cmd_args[1].empty())
    {
        send_log(index_client, "USER : username should not be empty\r\n");
        server_log(index_client, "USER : username should not be empty");
        return;
    }
    clients[index_client].userName   = cmd_args[1];
    clients[index_client].hostname   = cmd_args[2];
    clients[index_client].servername = cmd_args[3];
    clients[index_client].realname   = cmd_args[4];
    send_log(index_client, "USER : username set to " + cmd_args[1] + "\r\n");
    server_log(index_client, "USER set to " + cmd_args[1]);
}

void    Server::quit(int index_client)
{
    server_log(index_client, " disconnect.");
    close(clients[index_client].fd_client);
	clients[index_client].disconnected = true;
}

void    Server::authenticate(int index_client, std::vector <std::string> cmd_args)
{
    if (cmd_args[0] == "QUIT")
    {
        quit(index_client);
        return;
    }
    else if (clients[index_client].password.empty() && cmd_args[0] != "PASS")
    {
        send_log(index_client, "Please Enter the server Password First : PASS <passwd>\r\n");
        server_log(index_client, "Please Enter the server Password First");
        return;
    }
    else if (cmd_args[0] == "PASS")
        pass(index_client, cmd_args);
    else if (cmd_args[0] == "NICK")
        nick(index_client, cmd_args);
    else if (cmd_args[0] == "USER")
        user(index_client, cmd_args);
    else
    {
        send_log(index_client, "Unknown command during authentication.Please complete your Registration\r\n");
        server_log(index_client, "Unknown command during authentication.");
        return;
    }
    if (!clients[index_client].password.empty() 
        && !clients[index_client].nickName.empty()
        && !clients[index_client].userName.empty()
        && !clients[index_client].authenticated)
    {
        clients[index_client].authenticated = 1;
        send_log(index_client, "Registration successful. You are now authenticated!\r\n");
        server_log(index_client, "Registration successful.");
    }
}
