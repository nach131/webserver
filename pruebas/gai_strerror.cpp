// gai_strerror es una función que se utiliza para obtener una cadena de caracteres que describe un
// código de error de una llamada a funciones relacionadas con la resolución de nombres de dominio,
// como getaddrinfo y getnameinfo. Estas funciones se utilizan para convertir entre direcciones
// IP y nombres de host, y viceversa.

// const char *gai_strerror(int errcode);

// errcode: El código de error para el cual se quiere obtener una descripción.

#include <iostream>
#include <netdb.h> // Para gai_strerror

int main()
{
	struct addrinfo hints;
	struct addrinfo *result;
	int status;

	// Configurar hints para una búsqueda de dirección de socket
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;	 // IPv4 o IPv6
	hints.ai_socktype = SOCK_STREAM; // Socket de flujo (TCP)
	hints.ai_flags = AI_PASSIVE;	 // Dirección para enlace

	// Intentar obtener información de la dirección de socket
	status = getaddrinfo("localhost", "http", &hints, &result);
	if (status != 0)
	{
		std::cerr << "Error al obtener información de la dirección de socket: " << gai_strerror(status) << std::endl;
		return 1;
	}

	// Realizar operaciones con la información de la dirección de socket

	freeaddrinfo(result); // Liberar la memoria asignada a la estructura addrinfo

	return 0;
}

// En este ejemplo, utilizamos getaddrinfo para obtener información sobre la dirección de socket para
// el nombre de host "localhost" y el servicio "http". Si getaddrinfo falla (devuelve un valor distinto de
// cero), imprimimos un mensaje de error utilizando gai_strerror(status), donde status es el código de
// error devuelto por getaddrinfo. Esto nos proporciona una descripción legible del error que ocurrió
// durante la resolución de la dirección de socket.