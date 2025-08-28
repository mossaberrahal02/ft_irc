#include "Server.hpp"

void valid_args(char **av)
{
    for (int i = 0; av[1][i]; i++)
        if (std::isdigit(av[1][i]) == 0)
        {
            std::cerr << "Please Enter a Valid Port or default <6667>" << std::endl;
            exit(EXIT_FAILURE);
        }
    if (std::atoi(av[1]) < 1024 || std::atoi(av[1]) > 65535)
    {
        std::cerr << "Please Enter a Valid Port or default <6667>" << std::endl;
        exit(EXIT_FAILURE);
    } 
    if (std::strlen(av[2]) < 4)
    {
        std::cerr << "Please Enter a Valid password, more than 4 characters" << std::endl;
        exit(EXIT_FAILURE);
    }
}

Server::Server(int ac, char **av)
{
    int en = 1;

	if (ac != 3)
    {
        std::cerr << "usage : ./ircserv <port> <password>" << std::endl;
        exit(EXIT_FAILURE);
    }
    valid_args(av);
	memset(&addr_server, 0, sizeof(addr_server));
    fd_server = socket(AF_INET, SOCK_STREAM | O_NONBLOCK, 0);
    if (fd_server < 0)
    {
        std::cerr << "can't create socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
    {
        std::cerr << "setsockopt error" << std::endl;
        exit(EXIT_FAILURE);
    }
    addr_server.sin_addr.s_addr = INADDR_ANY;
    addr_server.sin_family = AF_INET;
    addr_server.sin_port = htons(std::atoi(av[1]));
    passwd = std::string(av[2]);
    if (bind(fd_server,
        (sockaddr *)&addr_server,
        sizeof(addr_server)) < 0)
    {
        std::cerr << "can't bind connection" << std::endl;
        close(fd_server);
        exit(EXIT_FAILURE);
    }
    if (listen(fd_server, SOMAXCONN) < 0)
    {
        std::cerr << "listen Error" << std::endl;
        close(fd_server);
        exit(EXIT_FAILURE);
    }
    serverName = "ft_irc_server";
    new_cli.fd = fd_server;
    new_cli.events = POLLIN;
    new_cli.revents = 0;
    fds.push_back(new_cli);
    std::cout << "Server started on port            : " << ntohs(addr_server.sin_port) << std::endl;
    std::cout << "Server started with the password  : " << passwd << std::endl;
}

Server::~Server()
{
	close(fd_server);
}

void Server::clear_disconnected()
{
    for (size_t i = 0; i < clients.size();) 
    {
        if (clients[i].disconnected) {
            int fd = clients[i].fd_client;
            removeFromChannels(i, channels);
            for (size_t j = 0; j < fds.size(); j++) {
                if (fds[j].fd == fd) {
                    fds.erase(fds.begin() + j);
                    break;
                }
            }
            clients.erase(clients.begin() + i);
        } 
        else {
            ++i;
        }
    }
}

void        Server::loop()
{
    int pollValue;
    std::cout << "Waiting to accept a connection...\n";
    while(true)
    {
        pollValue = poll(&fds[0], fds.size(), -1);
        if ( pollValue < 0)
        {
            std::cerr << "poll() Error" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (pollValue == 0)
            continue;
        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].revents & POLLIN)
            {
                if (fds[i].fd == fd_server)
                    new_connection();
                else
                    process_client_data(fds[i].fd);
            }
        }
        clear_disconnected();
    }
}

void        Server::new_connection()
{
    Client client(-1);
    client.fd_client = accept(fd_server,
        reinterpret_cast<struct sockaddr *>(&client.client_addr),
        &client.client_addr_len);
    if (client.fd_client < 0)
    {
        std::cerr << "can't accept new client" << std::endl;
        return;
    }
    new_cli.fd = client.fd_client;
    new_cli.events = POLLIN;
    new_cli.revents = 0;
    client.ipAddr = std::string(inet_ntoa(client.client_addr.sin_addr));
    clients.push_back(client);
    fds.push_back(new_cli);
    std::cout << "Client <" << client.fd_client << "> : hostname <" << client.ipAddr << "> Connected" << std::endl;
    std::string welcome = "Welcome to the server!\nPlease Register yourself on our server!\r\n";
    send(client.fd_client, welcome.c_str(), welcome.size(), 0);
}


void        Server::process_client_data(int fd)
{
    int         cli_indx = getClient(fd);
    size_t      pos = 0;

    memset(buffer, 0, sizeof(buffer));
    buff_readed = recv(fd, buffer, MAX_BUFF - 1, 0);
    if (buff_readed <= 0)
    {
        quit(cli_indx);
        return;
    }
    clients[cli_indx].buffer.append(buffer, buff_readed);
    while ((pos = clients[cli_indx].buffer.find_first_of("\r\n")) != std::string::npos)
    {
        process_command(cli_indx, clients[cli_indx].buffer.substr(0, pos));
        clients[cli_indx].buffer.erase(0, pos + 1);
    }
}

std::vector<std::string>     get_args(std::string& line)
{
	std::vector<std::string> vec;
	std::istringstream stm(line);
	std::string word;
	while(stm >> word)
		vec.push_back(word);
	return vec;
}

void    Server::server_log(int index_client, std::string log)
{
    std::cout << "client <" << clients[index_client].fd_client << "> : " << log << std::endl;
}

void    Server::send_log(int index_client, std::string log)
{
	if (send(clients[index_client].fd_client, log.c_str(), log.size(), 0) == -1)
    {
        std::cerr << "send error" << std::endl;
        exit(EXIT_FAILURE);
    }
}


void    Server::normal_commands(int index_client, std::vector <std::string> cmd_args)
{
    if (cmd_args[0] == "QUIT")
        return quit(index_client);
    else if (cmd_args[0] == "PRIVMSG")
        return privmsg(index_client, cmd_args);
    else if (cmd_args[0] == "JOIN")
        return join(index_client, cmd_args);
    else if (cmd_args[0] == "PART")
        return part(index_client, cmd_args);
    else if (cmd_args[0] == "INVITE")
        return invite(index_client, cmd_args);
    else if (cmd_args[0] == "KICK")
        return kick(index_client, cmd_args);
    else if (cmd_args[0] == "TOPIC")
        return topic(index_client, cmd_args);
    else if (cmd_args[0] == "MODE")
        return mode(index_client, cmd_args);
    else if (cmd_args[0] == "SENDFILE")
        sendFile(index_client, cmd_args);
    else if (cmd_args[0] == "GETFILEEE")  
        getFile(index_client, cmd_args);
    else if (cmd_args[0] == "BOT")
        bot(index_client, cmd_args);
    else
    {
        send_log(index_client, "Error : Unknown command\r\n");
        server_log(index_client, "Error : Unknown command");
    }
}

void    Server::process_command(int index_client, std::string line)
{
    std::vector<std::string> cmd_args = get_args(line);

    if (line.empty() || cmd_args[0] == "CAP")
    return;
    if (clients[index_client].authenticated == false)
        authenticate(index_client, cmd_args);
    else if (cmd_args[0] == "PASS" || cmd_args[0] == "NICK" || cmd_args[0] == "USER")
    {
        send_log(index_client, "NOTE : you already authentificated\r\n");
        server_log(index_client, "NOTE : you already authentificated");
    }
    else
        normal_commands(index_client, cmd_args);
}

int     Server::getClient(int fd)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].fd_client == fd)
        return i;
    }
    return -1;
}
