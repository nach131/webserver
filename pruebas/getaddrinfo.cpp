//  se utiliza para resolver nombres de host y servicios en direcciones de red utilizable.
//   Esta función es especialmente útil en situaciones donde necesitas manejar conexiones de
//   red y necesitas convertir nombres de host y números de puerto en estructuras de direcciones
//    que puedan ser utilizadas por funciones de sockets, como connect(), bind(), sendto(), recvfrom(), entre otras.

// int getaddrinfo(const char *node, const char *service,
//                 const struct addrinfo *hints, struct addrinfo **res);

// node: Una cadena que especifica el nombre del host o la dirección IP en formato textual que deseas resolver.
// service: Una cadena que especifica el número de puerto o el nombre del servicio que deseas resolver.
// hints: Un puntero a una estructura addrinfo que proporciona sugerencias o restricciones sobre cómo
// se debe realizar la resolución de direcciones. Puede ser NULL si no hay restricciones.
// res: Un puntero a un puntero de estructura addrinfo. Esta estructura contendrá los resultados de
// la resolución de direcciones.

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

int main()
{
    const char *host = "www.example.com";
    const char *port = "80";
    struct addrinfo hints, *result, *rp;
    int status;

    // Llenar la estructura hints con ceros y especificar restricciones
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;     // IPv4 o IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP

    // Obtener la información de la dirección
    status = getaddrinfo(host, port, &hints, &result);
    if (status != 0)
    {
        std::cerr << "Error en getaddrinfo: " << gai_strerror(status) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Iterar sobre los resultados y mostrar las direcciones encontradas
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        char addr_str[INET6_ADDRSTRLEN];
        void *addr;

        if (rp->ai_family == AF_INET)
        { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)rp->ai_addr;
            addr = &(ipv4->sin_addr);
        }
        else
        { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)rp->ai_addr;
            addr = &(ipv6->sin6_addr);
        }

        // Convertir la dirección a una cadena legible
        inet_ntop(rp->ai_family, addr, addr_str, sizeof(addr_str));
        std::cout << "Dirección: " << addr_str << std::endl;
    }

    // Liberar memoria utilizada por la lista de resultados
    freeaddrinfo(result);

    return 0;
}
