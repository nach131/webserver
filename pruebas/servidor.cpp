#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cerrno>

// Cabeceras específicas de sockets en sistemas tipo Unix
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int sockfd, newsockfd;
    socklen_t clilen;
    char buffer[1024];
    struct sockaddr_in serverAddr, clientAddr;
    int n;

    // Crear socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error al crear socket: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Configurar la dirección del servidor
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); // Puerto del servidor
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Escuchar en todas las interfaces de red

    // Enlazar el socket a la dirección del servidor
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error al enlazar socket: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Escuchar por conexiones entrantes
    listen(sockfd, 5);

    std::cout << "Servidor esperando conexiones..." << std::endl;

    clilen = sizeof(clientAddr);

    // Aceptar la conexión entrante
    newsockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clilen);
    if (newsockfd < 0) {
        std::cerr << "Error al aceptar conexión: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Recibir datos del cliente
    memset(buffer, 0, sizeof(buffer));
    n = recv(newsockfd, buffer, sizeof(buffer), 0);
    if (n < 0) {
        std::cerr << "Error al recibir datos: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Mensaje del cliente: " << buffer << std::endl;

    // Enviar respuesta al cliente
    n = send(newsockfd, "Hola, cliente!", 13, 0);
    if (n < 0) {
        std::cerr << "Error al enviar respuesta: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Cerrar sockets
    close(newsockfd);
    close(sockfd);

    return 0;
}
