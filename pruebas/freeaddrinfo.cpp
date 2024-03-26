//  se utiliza para liberar la memoria asignada por la función getaddrinfo()
//  cuando se resuelven nombres de host y servicios en direcciones de red utilizable.

// void freeaddrinfo(struct addrinfo *res);

// res: Un puntero a la estructura de datos addrinfo que se desea liberar.

// La función freeaddrinfo() libera la memoria asignada a la lista de estructuras
// addrinfo que se generó previamente utilizando getaddrinfo().

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

// En este ejemplo, después de llamar a getaddrinfo() y obtener la lista de resultados en result,
//  utilizamos un bucle for para iterar sobre los resultados y mostrar las direcciones IP
//  asociadas con el host. Finalmente, llamamos a freeaddrinfo() para liberar la memoria
//  asignada a result una vez que hayamos terminado de usarla.