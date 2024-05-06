#include <sys/event.h> // Para kqueue

void AdminServer::run()
{
	int sockfd, newsockfd;
	socklen_t client;
	struct sockaddr_in serverAddr, clientAddr;
	int n;

	char buffer[1024];

	// Crear socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		std::string errorMsg = RED "Error creating socket:\n";
		errorMsg += CYAN;
		errorMsg += strerror(errno);
		throw std::runtime_error(errorMsg);
	}

	// Configurar la dirección del servidor
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8080);				// Puerto del servidor
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Escuchar en todas las interfaces de red

	// Enlazar el socket a la dirección del servidor
	if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		std::string errorMsg = RED "Socket binding error:\n";
		errorMsg += CYAN + std::to_string(ntohs(serverAddr.sin_port)) + " ";
		errorMsg += strerror(errno);
		throw std::runtime_error(errorMsg);
	}

	// Escuchar por conexiones entrantes
	listen(sockfd, 5);

	std::cout << "Servidor esperando conexiones..." << std::endl;

	// Crear kqueue
	int kq = kqueue();
	if (kq == -1)
	{
		std::cerr << "Error creating kqueue: " << strerror(errno) << std::endl;
		close(sockfd);
		return;
	}

	// Configurar evento para el socket de escucha
	struct kevent event;
	EV_SET(&event, sockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);

	// Monitorear eventos con kqueue
	while (true)
	{
		struct kevent events[32];
		int nev = kevent(kq, NULL, 0, events, 32, NULL);
		if (nev == -1)
		{
			std::cerr << "Error in kevent: " << strerror(errno) << std::endl;
			close(sockfd);
			return;
		}

		for (int i = 0; i < nev; ++i)
		{
			if (events[i].ident == sockfd)
			{
				client = sizeof(clientAddr);
				newsockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &client);
				if (newsockfd < 0)
				{
					std::cerr << "Error accepting connection: " << strerror(errno) << std::endl;
					continue;
				}
				std::cout << MAGENTA << "Connection accepted. Client socket: " << newsockfd << RESET << std::endl;

				// Recibir datos del cliente
				memset(buffer, 0, sizeof(buffer));
				n = recv(newsockfd, buffer, sizeof(buffer), 0);
				if (n < 0)
				{
					std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
					close(newsockfd);
					continue;
				}

				// Manejar la petición recibida
				handleRequest(buffer, newsockfd);

				// Cerrar el socket de la conexión actual
				close(newsockfd);
			}
		}
	}

	// Cerrar el socket del servidor
	close(sockfd);
}
