#include "Client.hpp"
#include <cstring>


Client::Client(int fd):
fd_client(fd),
password("")
{
	memset(&client_addr, 0, sizeof(client_addr));
	client_addr_len = sizeof(client_addr);
}

Client::~Client()
{
	
}
