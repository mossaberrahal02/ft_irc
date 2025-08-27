#pragma once
#include <string>
#include <vector>

class Bot
{
public:
    void    send_message(int index_client, const std::string &message);
    void    receive_message(int index_client);
};
