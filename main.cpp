#include "main.hpp"


void    data_init()
{
    
}

int main(int ac, char** av)
{
    if (ac != 3)
    {
        std::cout << "usage : ./ircserv IPaddr port" << std::endl;
        return 1;
    }
    data_init();
    return 0;
}