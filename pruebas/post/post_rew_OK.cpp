#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cerrno>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\x1B[38;2;47;82;191m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define ORANGE "\033[1;31m"
#define WHITE "\033[0;37m"
#define BLACK "\x1B[30m"
#define RESET "\x1B[0m"

int main()
{
    int sockfd, newsockfd;
    socklen_t clilen;
    char buffer[1024];
    struct sockaddr_in serverAddr, clientAddr;
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Error al crear socket: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Error al enlazar socket: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    listen(sockfd, 5);

    std::cout << "Servidor esperando conexiones..." << std::endl;

    while (true)
    {
        clilen = sizeof(clientAddr);

        newsockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clilen);
        if (newsockfd < 0)
        {
            std::cerr << "Error al aceptar conexión: " << strerror(errno) << std::endl;
            continue;
        }

        std::cout << "Conexión aceptada. Esperando mensaje del cliente..." << std::endl;

        memset(buffer, 0, sizeof(buffer));
        n = recv(newsockfd, buffer, sizeof(buffer), 0);
        if (n < 0)
        {
            std::cerr << "Error al recibir datos: " << strerror(errno) << std::endl;
            close(newsockfd);
            continue;
        }

        std::cout << "Mensaje del cliente: " << buffer << std::endl;

        //=========================================================================
        std::string request_to_forward = "POST /api/users HTTP/1.1\r\n";
        request_to_forward += "Host: 192.168.1.46:3000\r\n";
        request_to_forward += "Content-Type: application/json\r\n";

        std::string content = "{\"full_name\": \"Bilma Picapiedra\", \"username\": \"Bilma\"}";

        request_to_forward += "Content-Length: " + std::to_string(content.length()) + "\r\n\r\n";
        request_to_forward += content;

        std::cout << CYAN << buffer << RESET << std::endl;
        std::cout << "====================\n";
        std::cout << RED << request_to_forward << RESET << std::endl;

        int forward_sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (forward_sockfd < 0)
        {
            std::cerr << "Error al crear socket para la conexión al otro servidor: " << strerror(errno) << std::endl;
            close(newsockfd);
            continue;
        }

        struct sockaddr_in forward_serverAddr;
        memset(&forward_serverAddr, 0, sizeof(forward_serverAddr));
        forward_serverAddr.sin_family = AF_INET;
        forward_serverAddr.sin_port = htons(3000);
        forward_serverAddr.sin_addr.s_addr = inet_addr("192.168.1.46");

        if (connect(forward_sockfd, (struct sockaddr *)&forward_serverAddr, sizeof(forward_serverAddr)) < 0)
        {
            std::cerr << "Error al conectar al otro servidor: " << strerror(errno) << std::endl;
            close(forward_sockfd);
            close(newsockfd);
            continue;
        }

        n = send(forward_sockfd, request_to_forward.c_str(), request_to_forward.size(), 0);
        if (n < 0)
        {
            std::cerr << "Error al enviar datos al otro servidor: " << strerror(errno) << std::endl;
            close(forward_sockfd);
            close(newsockfd);
            continue;
        }

        std::cout << "Solicitud enviada al otro servidor." << std::endl;

        std::string response_from_server;
        while ((n = recv(forward_sockfd, buffer, sizeof(buffer), 0)) > 0)
        {
            response_from_server.append(buffer, n);
        }

        if (n < 0)
        {
            std::cerr << "Error al recibir datos del otro servidor: " << strerror(errno) << std::endl;
            close(forward_sockfd);
            close(newsockfd);
            continue;
        }

        std::cout << "Respuesta del otro servidor recibida." << std::endl;

        send(newsockfd, response_from_server.c_str(), response_from_server.size(), 0);

        std::cout << "Respuesta enviada al cliente." << std::endl;

        close(forward_sockfd);
        close(newsockfd);
    }

    close(sockfd);

    return 0;
}
