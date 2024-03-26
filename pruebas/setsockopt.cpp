
// se utiliza para establecer opciones en un socket. Estas opciones controlan varios aspectos del
// comportamiento del socket, como el tiempo de espera, el tamaño del búfer, el reenvío de paquetes, entre otros.

// int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);

//     sockfd: El descriptor de socket al cual se aplicará la opción.
//     level: El nivel en el que se encuentra la opción. Normalmente, se establece en SOL_SOCKET
//     para opciones relacionadas con el socket en sí, o en IPPROTO_TCP o IPPROTO_UDP para opciones específicas del protocolo.
//     optname: El nombre de la opción que se desea establecer.
//     optval: Un puntero al valor que se desea establecer para la opción.
//     optlen: La longitud del valor de la opción en bytes.

// La función setsockopt() devuelve 0 si la operación tiene éxito o -1 si hay un error.

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>

int main()
{
    int sockfd;
    int option_value = 1; // Habilitar la opción de reenvío de paquetes

    // Crear socket TCP
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Error al crear socket: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Habilitar reenvío de paquetes
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(option_value)) < 0)
    {
        std::cerr << "Error al establecer la opción de socket: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Resto del código...

    return 0;
}

// En este ejemplo, se crea un socket TCP con socket() y luego se habilita la opción de reenvío de paquetes
// utilizando setsockopt(). La opción de reenvío de paquetes se establece en SO_REUSEADDR,
// lo que permite reutilizar direcciones locales en el servidor incluso si aún están en uso.
//  La opción_value se establece en 1 para habilitar la opción. Si setsockopt() tiene éxito,
//   el socket estará configurado para reenviar paquetes.