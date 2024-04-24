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

std::string readHTMLFile(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error al abrir el archivo: " << filename << std::endl;
		return "";
	}

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return content;
}

void saveFile(const std::string &filename, const char *data, size_t size)
{
	std::ofstream file(filename.c_str(), std::ios::out | std::ios::binary);
	if (file.is_open())
	{
		file.write(data, size);
		file.close();
		std::cout << "Archivo guardado como: " << filename << std::endl;
	}
	else
	{
		std::cerr << "Error al guardar el archivo: " << filename << std::endl;
	}
}

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

		// Analizar la solicitud del cliente
		if (strstr(buffer, "POST /submit") != nullptr)
		{
			// Obtener el nombre del archivo
			char *filenameStart = strstr(buffer, "filename=\"");
			if (filenameStart != nullptr)
			{
				filenameStart += strlen("filename=\"");
				char *filenameEnd = strstr(filenameStart, "\"");
				if (filenameEnd != nullptr)
				{
					*filenameEnd = '\0'; // Terminar la cadena en el primer carácter después del nombre del archivo
					std::string filename = filenameStart;

					// Encontrar el comienzo de los datos del archivo
					char *fileDataStart = strstr(filenameEnd, "\r\n\r\n");
					if (fileDataStart != nullptr)
					{
						fileDataStart += strlen("\r\n\r\n");
						size_t dataSize = buffer + n - fileDataStart;

						// Guardar los datos del archivo
						saveFile(filename, fileDataStart, dataSize);

						// Respuesta al cliente
						std::string response = "HTTP/1.1 200 OK\r\n"
											   "Content-Type: text/html\r\n"
											   "\r\n"
											   "<!DOCTYPE html>\n"
											   "<html>\n"
											   "<head>\n"
											   "<title>Archivo Subido</title>\n"
											   "</head>\n"
											   "<body>\n"
											   "<h1>Archivo Subido Exitosamente</h1>\n"
											   "</body>\n"
											   "</html>\n";

						// Enviar la respuesta al cliente
						n = send(newsockfd, response.c_str(), response.length(), 0);
						if (n < 0)
						{
							std::cerr << "Error al enviar respuesta al cliente: " << strerror(errno) << std::endl;
						}
						else
						{
							std::cout << "Respuesta enviada al cliente." << std::endl;
						}
					}
				}
			}
		}
		else
		{
			// Si no se solicita un recurso específico, enviar la página HTML predeterminada
			std::string response = "HTTP/1.1 200 OK\r\n"
								   "Content-Type: text/html\r\n"
								   "\r\n" +
								   htmlContent;

			// Enviar la respuesta al cliente
			n = send(newsockfd, response.c_str(), response.length(), 0);
			if (n < 0)
			{
				std::cerr << "Error al enviar respuesta al cliente: " << strerror(errno) << std::endl;
			}
			else
			{
				std::cout << "Respuesta enviada al cliente." << std::endl;
			}
		}

		// Cerrar el socket de la conexión actual
		close(newsockfd);
	}

	// Cerrar el socket del servidor (esto no se alcanzará)
	close(sockfd);

	return 0;
}
