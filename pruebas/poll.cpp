// poll es una función similar a select que se utiliza para monitorear múltiples descriptores
//  de archivo (como sockets, STDIN, STDOUT, etc.) en busca de eventos de interés, como lectura,
//   escritura o errores. Al igual que select, poll es útil en situaciones donde necesitas manejar
//   múltiples conexiones o entradas/salidas de manera eficiente.

// A diferencia de select, poll no tiene límites en el número de descriptores de archivo que puede manejar y,
// en general, es más eficiente para monitorear grandes conjuntos de descriptores de archivo.

// int poll(struct pollfd *fds, nfds_t nfds, int timeout);

// fds: Un array de estructuras pollfd que especifican los descriptores de archivo que se monitorearán y
// los eventos de interés para cada uno.
// nfds: El número de elementos en el array fds.
// timeout: El tiempo máximo de espera en milisegundos. Un valor negativo indica espera indefinida,
//  un valor de cero indica que poll debe regresar inmediatamente, y un valor positivo especifica el tiempo máximo de espera.

// struct pollfd {
//     int fd;         // El descriptor de archivo que se monitoreará
//     short events;   // Los eventos de interés (por ejemplo, POLLIN para lectura)
//     short revents;  // Los eventos que ocurrieron en el descriptor de archivo (llenado por poll)
// };

// poll modificará el campo revents de cada estructura pollfd para indicar los eventos que ocurrieron en los descriptores de archivo monitoreados.

#include <iostream>
#include <poll.h>
#include <unistd.h>

int main()
{
	struct pollfd fds[1];
	int ret;

	// Configurar la estructura pollfd para monitorear la entrada estándar (STDIN_FILENO) para eventos de lectura
	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN;

	// Monitorear la entrada estándar
	ret = poll(fds, 1, 5000); // Esperar hasta 5 segundos

	if (ret == -1)
	{
		// Error al llamar a poll
		perror("poll");
	}
	else if (ret == 0)
	{
		// Tiempo de espera agotado
		std::cout << "Tiempo de espera agotado. No hay datos disponibles para leer en la entrada estándar." << std::endl;
	}
	else
	{
		// Eventos ocurrieron en la entrada estándar
		if (fds[0].revents & POLLIN)
		{
			// La entrada estándar está lista para ser leída
			std::cout << "Datos disponibles para leer en la entrada estándar." << std::endl;
		}
	}

	return 0;
}

// En este ejemplo, poll monitorea la entrada estándar (STDIN_FILENO) durante un tiempo de espera de 5 segundos. Si hay datos disponibles para leer en la entrada estándar dentro de este tiempo, poll retornará un valor mayor que cero, y el campo revents de la estructura pollfd indicará el evento que ocurrió (en este caso, POLLIN).
