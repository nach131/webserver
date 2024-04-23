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
	serverAddr.sin_port = htons(8080);				// Puerto del servidor
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

	while (true)
	{
		clilen = sizeof(clientAddr);

		// Aceptar la conexión entrante
		newsockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clilen);
		if (newsockfd < 0)
		{
			std::cerr << "Error al aceptar conexión: " << strerror(errno) << std::endl;
			continue; // Continuar con el siguiente intento de aceptar conexiones
		}

		std::cout << "Conexión aceptada. Esperando mensaje del cliente..." << std::endl;

		// Recibir datos del cliente
		memset(buffer, 0, sizeof(buffer));
		n = recv(newsockfd, buffer, sizeof(buffer), 0);
		if (n < 0)
		{
			std::cerr << "Error al recibir datos: " << strerror(errno) << std::endl;
			close(newsockfd);
			continue; // Continuar con el siguiente intento de aceptar conexiones
		}

		std::cout << "Mensaje del cliente: " << buffer << std::endl;

		// Construir la solicitud para enviar al otro servidor
		std::string request_to_forward = "POST / HTTP/1.1\r\n";
		request_to_forward += "Host: localhost:3000/api/users\r\n";
		request_to_forward += "Content-Type: application/json\r\n";
		request_to_forward += "Content-Length: " + std::to_string(strlen(buffer)) + "\r\n\r\n";
		request_to_forward += buffer;

		// Crear un socket para la conexión al otro servidor
		int forward_sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (forward_sockfd < 0)
		{
			std::cerr << "Error al crear socket para la conexión al otro servidor: " << strerror(errno) << std::endl;
			close(newsockfd);
			continue; // Continuar con el siguiente intento de aceptar conexiones
		}

		// Conectar al otro servidor
		if (connect(forward_sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
		{
			std::cerr << "Error al conectar al otro servidor: " << strerror(errno) << std::endl;
			close(forward_sockfd);
			close(newsockfd);
			continue; // Continuar con el siguiente intento de aceptar conexiones
		}

		// Enviar la solicitud al otro servidor
		n = send(forward_sockfd, request_to_forward.c_str(), request_to_forward.size(), 0);
		if (n < 0)
		{
			std::cerr << "Error al enviar datos al otro servidor: " << strerror(errno) << std::endl;
			close(forward_sockfd);
			close(newsockfd);
			continue; // Continuar con el siguiente intento de aceptar conexiones
		}

		std::cout << "Solicitud enviada al otro servidor." << std::endl;

		// Recibir la respuesta del otro servidor
		std::string response_from_server;
		while ((n = recv(forward_sockfd, buffer, sizeof(buffer), 0)) > 0)
		{
			response_from_server.append(buffer, n);
		}

		if (n < 0)
		{
			std::cerr << "Error al recibir datos del otro servidor: " << strerror(errno) << std::endl;
			close(forward_sockfd);
			close(newsockfd);
			continue; // Continuar con el siguiente intento de aceptar conexiones
		}

		std::cout << "Respuesta del otro servidor recibida." << std::endl;

		// Enviar la respuesta al cliente
		send(newsockfd, response_from_server.c_str(), response_from_server.size(), 0);

		std::cout << "Respuesta enviada al cliente." << std::endl;

		// Cerrar los sockets
		close(forward_sockfd);
		close(newsockfd);
	}

	// Cerrar el socket del servidor (esto no se alcanzará)
	close(sockfd);

	return 0;
}
