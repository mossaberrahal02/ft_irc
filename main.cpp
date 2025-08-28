#include "Server.hpp"



int main(int ac, char** av)
{
    Server  server(ac, av);

    server.loop();
    return 0;
}