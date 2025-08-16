#include "main.hpp"

int valid_args(char **av)
{
    for (int i = 0; av[1][i]; i++)
        if (std::isdigit(av[1][i]) == 0)
        {
            std::cerr << "Please Enter a Valid Port or default <6667>" << std::endl;
            return 1;
        }
    if (std::atoi(av[1]) < 1024 || std::atoi(av[1]) > 60000)
    {
        std::cerr << "Please Enter a Valid Port or default <6667>" << std::endl;
        return 1;
    }
    return 0;
}

bool    init_data(char **av, t_env *env)
{
    if (valid_args(av))
        return 1;
    
}

int main(int ac, char** av)
{
    t_env   env;
    (void)av;

    if (ac != 3)
    {
        std::cout << "usage : ./ircserv <port> <password>" << std::endl;
        return 1;
    }
    if (init_data(av, &env))
        return 1;
    
    return 0;
}