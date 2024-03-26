// se utiliza en el lado del cliente de una aplicación de red para establecer una conexión con un servidor
// remoto a través de un socket. Esta función es esencial para la comunicación en red, ya que permite
// que el cliente se conecte al servidor y envíe y reciba datos.

// int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

//     sockfd: El descriptor de socket que se utilizará para la conexión.
//     addr: Un puntero a una estructura sockaddr que contiene la dirección del servidor al que se desea conectar.
//     addrlen: El tamaño de la estructura sockaddr.

// La función connect() intentará establecer una conexión con el servidor especificado por addr a través
// del socket sockfd. Devolverá 0 si la conexión se establece con éxito, o -1 si ocurre algún error.

// Es importante tener en cuenta que connect() puede bloquear la ejecución del programa hasta que
//  se establezca la conexión o se agote el tiempo de espera, dependiendo de la configuración del
//   socket. Si se establece un tiempo de espera, se puede utilizar la función select() o poll()
//    para esperar a que el socket esté listo para la conexión antes de llamar a connect().

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

    // Resto del código para enviar y recibir datos...

    // Cerrar socket
    close(sockfd);

    return 0;
}
