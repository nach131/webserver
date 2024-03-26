//  se utiliza para aceptar una conexión entrante en un socket que ha sido configurado para escuchar conexiones entrantes.
//   Esta función se utiliza típicamente en el lado del servidor de una aplicación de red.

// int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

//     sockfd: Es el descriptor de socket que ha sido previamente configurado y está escuchando conexiones entrantes.
//     addr: Un puntero a una estructura sockaddr donde se almacenará la dirección del cliente que se conecta.
//     addrlen: Un puntero a un entero que almacena la longitud de la estructura sockaddr.

// La función accept() bloquea la ejecución del programa hasta que se recibe una conexión entrante en el
// socket especificado. Una vez que se recibe la conexión, accept() devuelve un nuevo descriptor de socket
// que representa la conexión establecida con el cliente. El descriptor de socket devuelto se puede
// utilizar para comunicarse con el cliente.

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

int main()
{
    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[1024];
    int n;

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

    clilen = sizeof(clientAddr);

    // Aceptar la conexión entrante
    newsockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clilen);
    if (newsockfd < 0)
    {
        std::cerr << "Error al aceptar conexión: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // En este punto, newsockfd es un nuevo descriptor de socket que representa la conexión establecida con el cliente

    // Cerrar sockets
    close(newsockfd);
    close(sockfd);

    return 0;
}

//  accept() se utiliza para aceptar una conexión entrante en el socket sockfd.
//   Una vez que se recibe la conexión, accept() devuelve un nuevo descriptor de socket newsockfd que
//   representa la conexión establecida con el cliente. Este nuevo descriptor de socket se puede
//    utilizar para enviar y recibir datos con el cliente.