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
	void		    clear_disconnected();
	void    		server_log(int index_client, std::string log);
	void		    send_log(int index_client, std::string log);
	void    		removeFromChannel(const std::string &channel_name, int fd_client, std::vector<Channel> &channels);
	void    		removeFromChannels(int fd_client, std::vector<Channel> &channels);
	bool    		isOperator(int fd_client, std::vector<Client> &admins);
	int				channelFound(std::vector<Channel> &channels, std::string channel_name);
	void			show_topic(int index_client, const std::string &channel_name);
	void 			edit_topic(int index_client, const std::string &channel_name, const std::string &new_topic);
	bool			isExistInChannel(int fd_client, std::vector<Client> &channel_clients, std::vector<Client> &admins);
	void			sendToAll(int fd_client, std::vector<Client> &clients, std::vector<Client> &admins, std::string log);
	void			join_channel(int index_client, int index_channel, std::string passkey);
	void			create_channel(int index_client, std::string new_channel, std::string passkey);
	void    		quit(int index_client);
	void    		pass(int index_client, std::vector <std::string> cmd_args);
	void    		nick(int index_client, std::vector <std::string> cmd_args);
	void    		user(int index_client, std::vector <std::string> cmd_args);
	// void    		privmsg(int index_client, std::vector <std::string> cmd_args);
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
};
