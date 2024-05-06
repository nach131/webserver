#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cerrno>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/event.h>
#include <sys/time.h>

#define MAX_EVENTS 10

int main()
{
    int sockfd, newsockfd;
    socklen_t clilen;
    char buffer[1024];
    struct sockaddr_in serverAddr, clientAddr;
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
    serverAddr.sin_port = htons(8082);               // Puerto del servidor
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

    // Crear kqueue
    int kq = kqueue();
    if (kq < 0)
    {
        std::cerr << "Error al crear kqueue: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    struct kevent change;
    EV_SET(&change, sockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);

    // Registro de sockfd para eventos de lectura
    if (kevent(kq, &change, 1, NULL, 0, NULL) == -1)
    {
        std::cerr << "Error en kevent: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Array para eventos
    struct kevent events[MAX_EVENTS];

    while (true)
    {
        // Esperar eventos en kqueue
        int nev = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);
        if (nev < 0)
        {
            std::cerr << "Error en kevent: " << strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < nev; ++i)
        {
            if (events[i].ident == sockfd)
            {
                // Aceptar la conexión entrante
                newsockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clilen);
                if (newsockfd < 0)
                {
                    std::cerr << "Error al aceptar conexión: " << strerror(errno) << std::endl;
                    continue; // Continuar con el siguiente intento de aceptar conexiones
                }

                std::cout << "Conexión aceptada. Esperando mensaje del cliente..." << std::endl;

                // Registro de newsockfd para eventos de lectura
                EV_SET(&change, newsockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
                if (kevent(kq, &change, 1, NULL, 0, NULL) == -1)
                {
                    std::cerr << "Error en kevent: " << strerror(errno) << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                // Recibir datos del cliente
                memset(buffer, 0, sizeof(buffer));
                n = recv(events[i].ident, buffer, sizeof(buffer), 0);
                if (n < 0)
                {
                    std::cerr << "Error al recibir datos: " << strerror(errno) << std::endl;
                    close(events[i].ident);
                    continue; // Continuar con el siguiente intento de aceptar conexiones
                }

                std::cout << "Mensaje del cliente: " << buffer << std::endl;

                // Analizar la solicitud del cliente
                // (Código para servir la página HTML o recursos adicionales)

                // Cerrar el socket de la conexión actual
                close(events[i].ident);
                std::cout << "Respuesta enviada al cliente." << std::endl;
            }
        }
    }

    // Cerrar el socket del servidor (esto no se alcanzará)
    close(sockfd);

    return 0;
}


// falta salir del loop principal con la liberacion de los recursos...?