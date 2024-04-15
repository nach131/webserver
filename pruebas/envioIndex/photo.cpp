#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cerrno>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main()
{
    int sockfd, newsockfd;
    socklen_t clilen;
    char buffer[1024];
    struct sockaddr_in serverAddr, clientAddr;
    int n;

    // Abrir el archivo photo.png en modo binario
    std::ifstream file("photo.png", std::ios::binary);
    if (!file)
    {
        std::cerr << "Error al abrir el archivo photo.png" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ostringstream oss;
    oss << file.rdbuf();               // Leer el contenido del archivo en un stringstream
    std::string imageData = oss.str(); // Obtener el contenido como string

    // Crear socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Error al crear socket: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Configurar la dirección del servidor
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);              // Puerto del servidor
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Escuchar en todas las interfaces de red

    // Enlazar el socket a la dirección del servidor
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Error al enlazar socket: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Escuchar por conexiones entrantes
    listen(sockfd, 5);

    std::cout << "Servidor esperando conexiones..." << std::endl;

    while (42)
    {
        clilen = sizeof(clientAddr);

        // Aceptar la conexión entrante
        newsockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clilen);
        if (newsockfd < 0)
        {
            std::cerr << "Error al aceptar conexión: " << strerror(errno) << std::endl;
            continue; // Continuar con el siguiente intento de aceptar conexiones
        }

        std::cout << "Conexión aceptada. Esperando mensaje del cliente..." << std::endl;

        // Recibir datos del cliente
        memset(buffer, 0, sizeof(buffer));
        n = recv(newsockfd, buffer, sizeof(buffer), 0);
        if (n < 0)
        {
            std::cerr << "Error al recibir datos: " << strerror(errno) << std::endl;
            close(newsockfd);
            continue; // Continuar con el siguiente intento de aceptar conexiones
        }

        std::cout << "Mensaje del cliente: " << buffer << std::endl;

        std::string header = "HTTP/1.1 200 OK\r\n";
        header += "Content-Type: image/png\r\n";
        header += "Content-Length: " + std::to_string(imageData.length()) + "\r\n\r\n";

        n = write(newsockfd, header.c_str(), header.length());
        if (n < 0)
        {
            std::cerr << "Error al enviar encabezado HTTP: " << strerror(errno) << std::endl;
            close(newsockfd);
            continue; // Continuar con el siguiente intento de aceptar conexiones
        }

        // // Enviar contenido de la imagen
        // n = write(newsockfd, imageData.c_str(), imageData.length());
        // if (n < 0)
        // {
        //     std::cerr << "Error al enviar contenido de la imagen: " << strerror(errno) << std::endl;
        //     close(newsockfd);
        //     continue; // Continuar con el siguiente intento de aceptar conexiones
        // }

        // Enviar contenido de la imagen
        n = write(newsockfd, imageData.c_str(), imageData.length());
        if (n < 0)
        {
            std::cerr << "Error al enviar contenido de la imagen: " << strerror(errno) << std::endl;
            close(newsockfd);
            continue; // Continuar con el siguiente intento de aceptar conexiones
        }
        else if (n != imageData.length())
        {
            std::cerr << "No se pudo enviar todo el contenido de la imagen" << std::endl;
            close(newsockfd);
            continue; // Continuar con el siguiente intento de aceptar conexiones
        }

        // Cerrar el socket de la conexión actual
        close(newsockfd);

        std::cout << "Respuesta enviada al cliente." << std::endl;
    }

    // Cerrar el socket del servidor (esto no se alcanzará)
    close(sockfd);

    return 0;
}
