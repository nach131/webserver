// listen() se utiliza en el lado del servidor de una aplicación de red para convertir un socket en un socket de escucha,
//  capaz de aceptar conexiones entrantes. Básicamente, listen() le dice al sistema operativo que el socket está
//   preparado para aceptar conexiones entrantes y especifica el tamaño máximo de la cola de conexiones pendientes
//    que el sistema operativo puede mantener.

// int listen(int sockfd, int backlog);

//     sockfd: El descriptor de socket que se quiere convertir en un socket de escucha.
//     backlog: El tamaño máximo de la cola de conexiones pendientes. Este parámetro
//      indica cuántas conexiones entrantes pueden estar en cola esperando ser aceptadas por el servidor. Si se supera este límite, las conexiones adicionales serán rechazadas.

// Es importante tener en cuenta que después de llamar a listen(), aún se necesita llamar
//  a accept() para realmente aceptar las conexiones entrantes.

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
    int sockfd;
    struct sockaddr_in serverAddr;

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

    // Convertir el socket en un socket de escucha
    if (listen(sockfd, 5) < 0)
    {
        std::cerr << "Error al convertir socket en socket de escucha: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Servidor escuchando en el puerto 8080..." << std::endl;

    // Cerrar socket
    close(sockfd);

    return 0;
}

// listen() se utiliza después de que el socket sockfd ha sido creado y enlazado a una dirección.
// Esto convierte sockfd en un socket de escucha capaz de aceptar conexiones entrantes.
//  Se especifica un tamaño máximo de la cola de conexiones pendientes de 5 mediante el segundo parámetro de listen().
//   Esto significa que el servidor podrá mantener hasta 5 conexiones pendientes antes de empezar a
//   rechazar conexiones adicionales.
