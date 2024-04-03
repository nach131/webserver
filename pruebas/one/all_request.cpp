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

	std::string htmlContent =
		"<!DOCTYPE html>\n"
		"<html>\n"
		"<head>\n"
		"<link rel=\"stylesheet\" href=\"mystyle.css\">\n"
		"</head>\n"
		"<body>\n"
		"\n"
		"<h1>Toma mi Heading</h1>\n"
		"<p>Toma mas tomate.</p>\n"
		"\n"
		"</body>\n"
		"</html>\n";

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

		// Analizar la solicitud del cliente
		if (strstr(buffer, "GET /mystyle.css") != nullptr)
		{
			std::cout << "CSS" << std::endl;
			// Servir el archivo CSS
			// (Código para servir mystyle.css)
		}
		else if (strstr(buffer, "GET /image.jpg") != nullptr)
		{
			// Servir una imagen
			// (Código para servir image.jpg)
		}
		// Agrega más condiciones aquí para manejar otros tipos de solicitudes, como JavaScript, imágenes, etc.
		else
		{
			// Si no se solicita un recurso específico, enviar la página HTML predeterminada
			// (Código para servir la página HTML)
		}

		// Cerrar el socket de la conexión actual
		close(newsockfd);

		std::cout << "Respuesta enviada al cliente." << std::endl;
	}

	// Cerrar el socket del servidor (esto no se alcanzará)
	close(sockfd);

	return 0;
}
