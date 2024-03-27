#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cerrno>
#include <fstream>

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

	while (42)
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

		//=========================================================================
		// Servir el archivo CSS si la solicitud lo pide
		if (strstr(buffer, "GET /mystyle.css") != nullptr)
		{
			std::ifstream cssFile("mystyle.css", std::ios::in | std::ios::binary | std::ios::ate);
			if (!cssFile.is_open())
			{
				std::cerr << "Error al abrir el archivo CSS." << std::endl;
				// Envía una respuesta de error 404 si no se puede abrir el archivo
				std::string response404 = "HTTP/1.1 404 Not Found\r\n\r\n";
				write(newsockfd, response404.c_str(), response404.length());
				close(newsockfd);
				continue;
			}
			std::ifstream::pos_type size = cssFile.tellg();
			cssFile.seekg(0, std::ios::beg);
			std::string cssContent(size, ' ');
			cssFile.read(&cssContent[0], size);
			cssFile.close();

			std::string cssHeader = "HTTP/1.1 200 OK\r\n";
			cssHeader += "Content-Type: text/css\r\n";
			cssHeader += "Content-Length: " + std::to_string(cssContent.length()) + "\r\n\r\n";

			write(newsockfd, cssHeader.c_str(), cssHeader.length());
			write(newsockfd, cssContent.c_str(), cssContent.length());
		}
		else
		{
			// Si no se solicita el archivo CSS, enviar la página HTML
			std::string header = "HTTP/1.1 200 OK\r\n";
			header += "Content-Type: text/html\r\n";
			header += "Content-Length: " + std::to_string(htmlContent.length()) + "\r\n\r\n";

			n = write(newsockfd, header.c_str(), header.length());
			if (n < 0)
			{
				std::cerr << "Error al enviar encabezado HTTP: " << strerror(errno) << std::endl;
				close(newsockfd);
				continue; // Continuar con el siguiente intento de aceptar conexiones
			}

			// Enviar contenido HTML
			n = write(newsockfd, htmlContent.c_str(), htmlContent.length());
			if (n < 0)
			{
				std::cerr << "Error al enviar contenido HTML: " << strerror(errno) << std::endl;
				close(newsockfd);
				continue; // Continuar con el siguiente intento de aceptar conexiones
			}
		}

		//=========================================================================

		// Cerrar el socket de la conexión actual
		close(newsockfd);

		std::cout << "Respuesta enviada al cliente." << std::endl;
	}

	// Cerrar el socket del servidor (esto no se alcanzará)
	close(sockfd);

	return 0;
}
