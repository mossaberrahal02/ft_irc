#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <netinet/in.h>
#include <poll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "Client.hpp"
#include "Channel.hpp"
#define MAX_BUFF 1024


#include <cerrno>

class Server
{
public:
	int							fd_server;
	struct sockaddr_in			addr_server;
	std::string					passwd;
	char						buffer[MAX_BUFF];
	ssize_t						buff_readed;
	std::vector <Client>		clients;
	std::vector <Channel>		channels;
	std::vector <struct pollfd>	fds;
	struct pollfd				new_cli;
	Server(int ac, char **av);
	~Server();
	void    		server_log(int index_client, std::string log);
	void		    send_log(int index_client, std::string log);
	void    		quit(int index_client);
	void    		pass(int index_client, std::vector <std::string> cmd_args);
	void    		nick(int index_client, std::vector <std::string> cmd_args);
	void    		user(int index_client, std::vector <std::string> cmd_args);
	void    		privmsg(int index_client, std::vector <std::string> cmd_args);
	void    		join(int index_client, std::vector <std::string> cmd_args);
	void    		invite(int index_client, std::vector <std::string> cmd_args);
	void    		kick(int index_client, std::vector <std::string> cmd_args);
	void    		topic(int index_client, std::vector <std::string> cmd_args);
	void    		mode(int index_client, std::vector <std::string> cmd_args);
	void    		normal_commands(int index_client, std::vector <std::string> cmd_args);
	void    		authenticate(int index_client, std::vector <std::string> cmd_args);
	int 			getClient(int fd);
	void			new_connection();
	void			process_command(int index_client,std::string line);
	void			process_client_data(int fd);
	void			loop();
	int 			get_client_index_by_nick(const std::string &nick);
	void 			send_to_channel(int index_client, std::string &channel, const std::string &message);
};
