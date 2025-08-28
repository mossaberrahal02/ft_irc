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
#include "file.hpp"

#include <map>
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
	std::map<std::string, File>	files;
	std::string					serverName;
	Server(int ac, char **av);
	~Server();
	void		    clear_disconnected();
	void    		server_log(int index_client, std::string log);
	void		    send_log(int index_client, std::string log);
	void			sendToAll(int fd_client, std::vector<Client> &clients, std::vector<Client> &admins, std::string log);

	void    		removeFromChannel(const std::string &channel_name, int fd_client, std::vector<Channel> &channels);
	void    		removeFromChannels(int index_client, std::vector<Channel> &channels);
	void    		addChannelOperator(int index_target, Channel &channel, Client client);
	void    		removeChannelOperator(int index_target, Channel &channel, Client client);
	bool    		isOperator(int fd_client, std::vector<Client> &admins);
	int    			userFound(std::vector<Client> &clients, std::string nickname);
	int				channelFound(std::vector<Channel> &channels, std::string channel_name);
	bool			setKey(int index_client,int index_channel, std::vector<std::string> cmd_args, int adding);
	bool			setOperator(int index_client,int index_channel, std::vector<std::string> cmd_args, int adding);
	bool			setLimit(int index_client,int index_channel, std::vector<std::string> cmd_args, int adding);
	bool			checkModeArgs(int index_client, std::vector<std::string> cmd_args);
	void			showModes(int index_client, int index_channel, std::vector<std::string> cmd_args);
	void			show_topic(int index_client, const std::string &channel_name);
	void 			edit_topic(int index_client, const std::string &channel_name, const std::string &new_topic);
	bool			isExistInChannel(int fd_client, std::vector<Client> &channel_clients, std::vector<Client> &admins);
	void			join_channel(int index_client, int index_channel, std::string passkey);
	void			create_channel(int index_client, std::string new_channel, std::string passkey);
	int 			get_client_index_by_nick(const std::string &nick);
	Client*			get_client_by_fd(int fd);
	void    		send_to_client(int fd, std::string log);
	void 			send_to_channel(int index_client, std::string &channel, const std::string &message);









	void    sendFile(int index_client, std::vector <std::string> cmd_args);
	void    getFile(int index_client, std::vector<std::string> cmd_args);
	void    bot(int index_client, std::vector<std::string> cmd_args);
	void    Help(int index_client);
	void	serverInfo(int index_client) ;
	void	channelInfo(int index_client, std::string ChannelName);
	void	listAllChannels(int index_client);
	void 	getUserInfo(int index_client);
	void 	onlineUsers(int index_client);
	void 	getUserInfo(int target, int index_client);














	void    		quit(int index_client);
	void    		pass(int index_client, std::vector <std::string> cmd_args);
	void    		nick(int index_client, std::vector <std::string> cmd_args);
	void    		user(int index_client, std::vector <std::string> cmd_args);
	void    		privmsg(int index_client, std::vector <std::string> cmd_args);
	void    		join(int index_client, std::vector <std::string> cmd_args);
	void			part(int index_client, std::vector<std::string> cmd_args);
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
