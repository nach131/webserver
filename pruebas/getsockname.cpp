// se utiliza para obtener la dirección local asociada con un socket.
// Esta función es útil cuando necesitas saber la dirección local a la que está enlazado
// un socket, por ejemplo, después de llamar a bind() en un servidor.

// int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

//     sockfd: El descriptor de socket del cual se quiere obtener la dirección local.
//     addr: Un puntero a una estructura sockaddr donde se almacenará la dirección local.
//     addrlen: Un puntero a un entero que almacena la longitud de la estructura sockaddr.

// La función getsockname() devuelve 0 si tiene éxito y -1 si hay un error.

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int sockfd;
    struct sockaddr_in serverAddr;
    socklen_t len = sizeof(serverAddr);

    // Crear socket TCP
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

    // Obtener la dirección local asociada con el socket
    if (getsockname(sockfd, (struct sockaddr *)&serverAddr, &len) < 0)
    {
        std::cerr << "Error al obtener dirección local: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Convertir la dirección a una cadena legible
    char addr_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(serverAddr.sin_addr), addr_str, sizeof(addr_str));
    std::cout << "Dirección local: " << addr_str << ", Puerto local: " << ntohs(serverAddr.sin_port) << std::endl;

    // Resto del código...

    return 0;
}

// En este ejemplo, después de enlazar el socket a la dirección del servidor con bind(),
//  utilizamos getsockname() para obtener la dirección local asociada con el socket y el
//   puerto local en el que está enlazado. Luego, convertimos esta dirección a una cadena
//   legible usando inet_ntop() y la imprimimos en la consola.

// Dirección local: 0.0.0.0, Puerto local: 8080