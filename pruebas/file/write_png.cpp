#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void writePNG(const std::string &content)
{
    std::ofstream outputFile("received.png", std::ios::binary);
    if (!outputFile.is_open())
    {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return;
    }
    outputFile.write(content.c_str(), content.size());
    outputFile.close();
    std::cout << "File received and saved as 'received.png'." << std::endl;
}

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    long total_bytes_received = 0;
    std::stringstream headerStream;
    bool header_done = false;
    while (!header_done)
    {
        int bytes_received = recv(new_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received < 0)
        {
            perror("recv failed");
            exit(EXIT_FAILURE);
        }
        total_bytes_received += bytes_received;
        headerStream << std::string(buffer, buffer + bytes_received);
        if (headerStream.str().find("\r\n\r\n") != std::string::npos)
        {
            header_done = true;
        }
    }
    std::string header = headerStream.str();
    size_t content_length_start = header.find("Content-Length:");
    size_t content_length_end = header.find("\r\n", content_length_start);
    size_t content_length = std::atoi(header.substr(content_length_start + 15, content_length_end - content_length_start - 15).c_str());
    std::string content;
    while (total_bytes_received < content_length)
    {
        int bytes_received = recv(new_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received < 0)
        {
            perror("recv failed");
            exit(EXIT_FAILURE);
        }
        total_bytes_received += bytes_received;
        content += std::string(buffer, buffer + bytes_received);
    }
    writePNG(content);
    close(new_socket);
    close(server_fd);
    return 0;
}
