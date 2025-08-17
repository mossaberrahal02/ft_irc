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
    env->server.fd_server = socket(AF_INET, SOCK_STREAM, 0);
    if (env->server.fd_server < 0)
    {
        perror("socket");
        return 1;
    }
    env->server.addr_server.sin_addr.s_addr = INADDR_ANY;
    env->server.addr_server.sin_family = AF_INET;
    env->server.addr_server.sin_port = htons(std::atoi(av[1]));
    env->server.passwd = std::string(av[2]);
    return 0;
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
    if (bind(env.server.fd_server,
        reinterpret_cast <struct sockaddr *>(&env.server.addr_server),
        sizeof(env.server.addr_server)) < 0)
    {
        perror("bind");
        close(env.server.fd_server);
        return 1;
    }
    if (listen(env.server.fd_server, 1) < 0)
    {
        perror("listen");
        close(env.server.fd_server);
        return 1;
    }
    env.clients.push_back(Client());
    env.clients[env.clients.size() - 1].fd_client = accept(
        env.server.fd_server,
        reinterpret_cast <struct sockaddr*>(&env.clients[env.clients.size() - 1].client_addr),
        &env.clients[env.clients.size() - 1].client_addr_len
    );
    if (env.clients[env.clients.size() - 1].fd_client < 0)
    {
        std::cerr << "Can't accepte the client" << std::endl;
        env.clients.pop_back();
    }
    else
        std::cout << "Client connected" << std::endl;
    env.server.buff_readed = read(env.clients[0].fd_client, env.server.buffer, MAX_BUFF);
    if (env.server.buff_readed < 0)
    {
        perror("read");
        close(env.server.fd_server);
        close(env.clients[0].fd_client);
        return 1;
    }
    env.server.buffer[env.server.buff_readed] = '\0';
    std::cout << "client send : " << env.server.buffer << std::endl;
    close(env.server.fd_server);
    close(env.clients[0].fd_client);
    return 0;
}