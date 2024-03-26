// se utiliza para enviar datos a través de un socket conectado. Esta función es comúnmente utilizada en el lado
// del cliente de una aplicación de red para enviar datos al servidor, o en el lado del servidor para enviar datos al cliente.

// int send(int sockfd, const void *buf, size_t len, int flags);

// sockfd: El descriptor de socket a través del cual se enviarán los datos.
// buf: Un puntero al búfer que contiene los datos que se enviarán.
// len: La longitud de los datos que se enviarán, en bytes.
// flags: Marcadores que especifican opciones para el envío. En general, se puede establecer en 0 para
// la mayoría de los casos de uso.

// La función send() intentará enviar los datos almacenados en el búfer apuntado por buf a través del
//  socket especificado por sockfd. Devolverá el número de bytes enviados si se realizó con éxito, o -1 si ocurrió un error.

// Es importante tener en cuenta que send() puede no enviar todos los bytes especificados en len de una sola vez.
//  En este caso, la función deberá llamarse nuevamente con el búfer actualizado para enviar los bytes restantes.

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cerrno>

// Cabeceras específicas de sockets en sistemas tipo Unix
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[1024];

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
    serverAddr.sin_port = htons(8080);                   // Puerto del servidor
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Dirección IP del servidor

    // Conectar al servidor
    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Error al conectar al servidor: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Enviar datos al servidor
    strcpy(buffer, "Hello, server!");
    int bytes_sent = send(sockfd, buffer, strlen(buffer), 0);
    if (bytes_sent < 0)
    {
        std::cerr << "Error al enviar datos al servidor: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Cerrar socket
    close(sockfd);

    return 0;
}
