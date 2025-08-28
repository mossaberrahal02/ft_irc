#include "file.hpp"
#include "Server.hpp"
#include <fstream>
#include <iostream>

void Server::sendFile(int index_client, std::vector<std::string> cmd_args)
{
    if (clients[index_client].nickName.empty())
    {
        send_to_client(clients[index_client].fd_client, ":You must set a nickname before sending files\r\n");
        server_log(index_client, ":You must set a nickname before sending files\r\n");
        return;
    }
    if (cmd_args.size() < 3)
        return send_to_client(clients[index_client].fd_client, ":Not enough parameters\r\n"), server_log(index_client, ":Not enough parameters\r\n");
    int receiver_index = get_client_index_by_nick(cmd_args[1]);
    if (receiver_index == -1)
        return send_to_client(clients[index_client].fd_client, cmd_args[1] + " :No such nick\r\n"), server_log(index_client, cmd_args[1] +":No such nick\r\n");
    std::cout << "Sending file from " << clients[index_client].nickName << " to " << cmd_args[1] << " file path "  << cmd_args[2] << std::endl;
    std::fstream ifs(cmd_args[2].c_str(), std::fstream::in); // cmd_args[2] is the file path
    if (ifs.fail())
    {
        send_to_client(clients[index_client].fd_client, ":Invalid file path\r\n");
        server_log(index_client, ":Invalid file path\r\n");
        ifs.close();
        return;
    }
    size_t pos = cmd_args[2].find_last_of('/');
    std::string filename;
    if (pos == std::string::npos)
        filename = cmd_args[2];
    else
        filename = cmd_args[2].substr(pos + 1);
    File file(filename, cmd_args[2], clients[index_client].nickName, cmd_args[1]);
    if (files.find(filename) != files.end())
    {
        send_to_client(clients[index_client].fd_client, ":File by this name already exists\r\n");
        send_log(index_client, ":File by this name already exists\r\n");
        ifs.close();
        return;
    }
    files.insert(std::pair<std::string, File>(filename, file));
    ifs.close();
    std::string notice = ":" + clients[index_client].nickName + " NOTICE " + cmd_args[1] + " :" +
                        clients[index_client].nickName + " wants to send you a file called " + filename + ".\r\n";
    send_to_client(clients[receiver_index].fd_client, notice);
    send_to_client(clients[index_client].fd_client, ":File " + filename + " queued for " + cmd_args[1] + "\r\n");
    return;
}




void Server::getFile(int index_client, std::vector<std::string> cmd_args)
{
    if (cmd_args.size() < 3) // Need GETFILEEE <filename> <destination_path>
        return send_to_client(clients[index_client].fd_client, ":Not enough parameters\r\n");
    if (files.find(cmd_args[1]) == files.end())
        return send_to_client(clients[index_client].fd_client, ":No such file on the server\r\n");
    File file = files.find(cmd_args[1])->second;
    if (file.receiver != clients[index_client].nickName)
        return send_to_client(clients[index_client].fd_client, ":Permission Denied\r\n");
    std::string output_path = cmd_args[2];
    if (!output_path.empty() && output_path[output_path.size() - 1] != '/')
        output_path += "/";
    output_path += cmd_args[1];
    std::fstream ofs(output_path.c_str(), std::fstream::out);
    if (!ofs.is_open())
        return send_to_client(clients[index_client].fd_client, ":Cannot create output file\r\n");
    std::fstream ifs(file.Path.c_str(), std::fstream::in);
    if (!ifs.is_open())
    {
        send_to_client(clients[index_client].fd_client, ":Cannot read source file\r\n");
        ofs.close();
        return;
    }
    ofs << ifs.rdbuf();
    ofs.close();
    ifs.close();
    files.erase(file.Name);
    send_to_client(clients[index_client].fd_client, ":File " + cmd_args[1] + " downloaded successfully to " + output_path + "\r\n");
    return;
}