#include "Server.hpp"



void	Server::part(int index_client, std::vector<std::string> cmd_args)
{
	int index_channel;
	if (cmd_args.size() < 2 || cmd_args.size() > 3 )
	{
		send_log(index_client, "PART : Invalid parameters\r\n");
		server_log(index_client, "PART : Invalid parameters");
		return;
	}
	index_channel = channelFound(channels, cmd_args[1]);
	if (index_channel == -1)
	{
		send_log(index_client, "PART : No such channel\r\n");
		server_log(index_client, "PART : No such channel");
		return;
	}
	if (!isExistInChannel(clients[index_client].fd_client, channels[index_channel].clients, channels[index_channel].admins))
	{
		send_log(index_client, "PART : Not on channel\r\n");
		server_log(index_client, "PART : Not on channel");
		return;
	}
	send_log(index_client, "PART: You part " + channels[index_channel].name + "\r\n");
	std::string msg = channels[index_channel].name + " : @" + clients[index_client].nickName + " part.\r\n";
	sendToAll(clients[index_client].fd_client, channels[index_channel].clients, channels[index_channel].admins, msg);
	removeFromChannel(channels[index_channel].name, clients[index_client].fd_client, channels);
	server_log(index_client, "PART : " + channels[index_channel].name);
}