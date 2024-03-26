// socketpair es una función que crea un par de sockets conectados entre sí, útiles para la
// comunicación entre procesos en un mismo sistema. Estos sockets son similares a los pipes,
//  pero ofrecen una interfaz de sockets completa, lo que significa que se pueden utilizar
//   con funciones como send, recv, read y write.

// int socketpair(int domain, int type, int protocol, int sv[2]);

//     domain: El dominio del socket, como AF_UNIX para sockets Unix o AF_INET para sockets de red.
//     type: El tipo de socket, como SOCK_STREAM para un socket de flujo o SOCK_DGRAM para un socket de datagrama.
//     protocol: El protocolo a utilizar, generalmente 0 para que el sistema elija automáticamente el protocolo adecuado.
//     sv: Un array de dos enteros donde se almacenarán los descriptores de archivo de los sockets creados.

// La función socketpair devuelve -1 en caso de error, y establece la variable errno para indicar la causa del error.

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
	int sv[2];		   // Array para almacenar los descriptores de archivo de los sockets
	char buffer[1024]; // Buffer para almacenar los datos leídos del socket

	// Crear el par de sockets
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1)
	{
		perror("Error al crear el par de sockets");
		return 1;
	}

	// Escribir en el primer socket
	const char *message = "Hola desde el primer proceso.";
	if (write(sv[0], message, strlen(message)) == -1)
	{
		perror("Error al escribir en el socket");
		return 1;
	}

	// Leer del segundo socket
	int bytesRead = read(sv[1], buffer, sizeof(buffer));
	if (bytesRead == -1)
	{
		perror("Error al leer del socket");
		return 1;
	}

	// Mostrar el mensaje recibido
	std::cout << "Mensaje recibido en el segundo socket: " << buffer << std::endl;

	// Cerrar los sockets
	close(sv[0]);
	close(sv[1]);

	return 0;
}
