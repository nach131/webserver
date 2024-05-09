void AdminServer::run()
{
    int sockfd, newsockfd;
    struct sockaddr_in clientAddr;
    socklen_t client;
    char buffer[1024];
    int n;

    sockfd = createSocket();

    //=========================================================================

    // Crear kqueue
    int kq = kqueue();
    if (kq == 0)
    {
        std::cerr << "Error creating kqueue: " << strerror(errno) << std::endl;
        close(sockfd);
        return;
    }

    // Configurar evento para el socket de escucha
    struct kevent event;
    EV_SET(&event, sockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);

    // Registro de sockfd para eventos de lectura
    if (kevent(kq, &event, 1, NULL, 0, NULL) == -1)
    {
        std::cerr << "Error en kevent: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Array para eventos
    struct kevent events[MAX_EVENTS];

    //=========================================================================

    // Inicializar el conjunto de descriptores de archivo, para CGI
    // fd_set fdSet;
    // FD_ZERO(&fdSet);

    //=========================================================================

    while (42)
    {

        // Esperar eventos en kqueue
        int nev = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL); // miarar kqueue.cpp para timeout
        if (nev < 0)
        {
            std::cerr << "Error en kevent: " << strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < nev; ++i)
        {
            std::cout << "i: " << i << ", nev: " << nev << std::endl;

            if (events[i].ident == (unsigned long)sockfd)
            {
                // Verifica si el evento es para lectura o escritura

                std::cout << CYAN << "toma: " << events[i].filter << std::endl;

                if (events[i].filter == EVFILT_READ)
                {
                    std::cout << CYAN << "EVENTO lectura" << RESET << std::endl;
                }
                else if (events[i].filter == EVFILT_WRITE)
                {
                    std::cout << CYAN << "EVENTO escritura" << RESET << std::endl;
                }
            }

            if (events[i].ident == (unsigned long)sockfd)
            {
                // Aceptar la conexión entrante
                newsockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &client);
                // newsockfd = -1; // error
                if (newsockfd < 0)
                {
                    std::cerr << "Error al aceptar conexión: " << strerror(errno) << std::endl;
                    continue; // Continuar con el siguiente intento de aceptar conexiones
                }

                std::cout << MAGENTA << "Conexión aceptada. Socket del cliente: " << newsockfd << RESET << std::endl;

                // Registro de newsockfd para eventos de lectura
                struct kevent eventRead;
                EV_SET(&eventRead, newsockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);

                if (kevent(kq, &eventRead, 1, NULL, 0, NULL) == -1)
                {
                    std::cerr << "Error en kevent: " << strerror(errno) << std::endl;
                    exit(EXIT_FAILURE);
                }
                else
                {
                    // Recibir datos del cliente
                    memset(buffer, 0, sizeof(buffer));
                    n = recv(newsockfd, buffer, sizeof(buffer), 0);
                    if (n < 0)
                    {
                        std::cerr << "Error al recibir datos: " << strerror(errno) << std::endl;
                        close(newsockfd);
                        continue; // Continuar con el siguiente intento de aceptar conexiones
                    }

                    //===================PETICION==============================================
                    // TODO
                    printPeticion(buffer);
                    //===================PARSING==============================================

                    HTTPRequest request(buffer);
                    // TODO
                    request.print();

                    //=========================================================================

                    // HTTPBody body(request);
                    HTTPRes response(request, &_config);
                    // TODO
                    printResponse(response.getHeader(), response.getContent());

                    //=========================================================================

                    // Enviar la respuesta al cliente utilizando la función creada
                    if (request.getHeader("Method") == "GET")
                    {
                        sendResGet(newsockfd, response.getHeader(), response.getContent());
                    }
                    else if (request.getHeader("Method") == "POST")
                    {
                        std::cout << "[ POST ]" << std::endl;
                        if (request.getHeader("Path") == "/submit")
                        {
                            std::cout << "FILE" << std::endl;
                            uploadFile(newsockfd);
                        }
                        else
                            sendResPost(newsockfd, response.getHeader().c_str(), response.getContent(), buffer);
                    }

                    else if (request.getHeader("Method") == "DELETE")
                    {
                        std::cout << "ES DELETE\n";
                    }

                    //=========================================================================
                    // Cerrar el socket de la conexión actual
                    close(newsockfd);

                    // TODO
                    // [ Server Configuration ]
                    _config.print();
                }
            }
            else
            {
                // Client disconnected
                if (events[i].flags & EV_EOF)
                {
                    int fd = events[i].ident;
                    std::cout << "client #" << get_conn(fd) << " disconnected." << std::endl;
                    struct kevent eventDelete;
                    EV_SET(&eventDelete, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                    kevent(kq, &eventDelete, 1, NULL, 0, NULL);
                    conn_del(fd);
                }
                // Read message from client
                else if (events[i].filter == EVFILT_READ)
                {
                    recv_msg(events[i].ident);
                }
            }
        }
    }

    // Cerrar el socket del servidor (esto no se alcanzará)
    close(sockfd);
}
