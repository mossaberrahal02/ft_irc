#include "main.hpp"



int main(int ac, char** av)
{
    Server  server(ac, av);

    try
    {
        server.loop();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}