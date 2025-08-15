#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define PORT 12345
#define SERVER_IP "127.0.0.1"

int main() {
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid server address" << std::endl;
        return 1;
    }

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return 1;
    }
    std::cout << "Connected to server" << std::endl;

    const char *message = "Hello from the client!";
    send(client_fd, message, strlen(message), 0);

    char buffer[1024];
    int n = recv(client_fd, buffer, sizeof(buffer), 0);
    if (n < 0) {
        std::cerr << "Error reading from socket" << std::endl;
        return 1;
    }
    buffer[n] = '\0';
    std::cout << "Server response: " << buffer << std::endl;

    close(client_fd);

    return 0;
}
