// se utiliza para recibir datos de un socket conectado. Esta función es comúnmente utilizada en el lado del cliente
//  de una aplicación de red para recibir datos del servidor, o en el lado del servidor para recibir datos del cliente.

// int recv(int sockfd, void *buf, size_t len, int flags);

// sockfd: El descriptor de socket desde el cual se recibirán los datos.
// buf: Un puntero al búfer donde se almacenarán los datos recibidos.
// len: La longitud máxima del búfer, en bytes.
// flags: Marcadores que especifican opciones para la recepción. En general,
// se puede establecer en 0 para la mayoría de los casos de uso.

// La función recv() intentará leer datos del socket especificado por sockfd y almacenarlos
//  en el búfer apuntado por buf. Devolverá el número de bytes recibidos si se realizó con éxito,
//  o -1 si ocurrió un error. Si recv() devuelve 0, significa que la conexión se ha cerrado por el otro extremo.

// Es importante tener en cuenta que recv() puede no llenar completamente el búfer especificado en buf.
//  En este caso, la función devolverá el número de bytes recibidos y el búfer contendrá esos datos.
//   Además, recv() puede bloquear la ejecución del programa si no hay datos disponibles para leer.
//    Para evitar esto, se puede utilizar el indicador MSG_DONTWAIT en el parámetro flags para hacer
//    que la función sea no bloqueante.

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

	// Recibir datos del servidor
	memset(buffer, 0, sizeof(buffer));
	int bytes_received = recv(sockfd, buffer, sizeof(buffer), 0);
	if (bytes_received < 0)
	{
		std::cerr << "Error al recibir datos del servidor: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	else if (bytes_received == 0)
	{
		std::cerr << "La conexión se ha cerrado por el servidor." << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "Mensaje del servidor: " << buffer << std::endl;

	// Cerrar socket
	close(sockfd);

	return 0;
}
