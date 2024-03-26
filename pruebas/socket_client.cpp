// para trabajar con sockets, generalmente se recurre a las API de sockets
// proporcionadas por el sistema operativo en uso. Estas API son funciones y
// estructuras definidas en las bibliotecas del sistema que permiten la comunicación a través de redes.

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cerrno>

// Cabeceras específicas de sockets en sistemas tipo Unix
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
	int sockfd;
	struct sockaddr_in serverAddr;
	char buffer[1024];

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
	serverAddr.sin_port = htons(8080);					 // Puerto del servidor
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Dirección IP del servidor

	// Conectar al servidor
	if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		std::cerr << "Error al conectar al servidor: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	// Enviar datos al servidor
	strcpy(buffer, "Hello, server!");
	send(sockfd, buffer, strlen(buffer), 0);

	// Recibir datos del servidor
	memset(buffer, 0, sizeof(buffer));
	recv(sockfd, buffer, sizeof(buffer), 0);
	std::cout << "Mensaje del servidor: " << buffer << std::endl;

	// Cerrar socket
	close(sockfd);

	return 0;
}
