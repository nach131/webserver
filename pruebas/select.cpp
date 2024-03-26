// select se utiliza para monitorear múltiples
// descriptores de archivo (como sockets, STDIN, STDOUT, etc.)
// para ver si están listos para lectura, escritura o si ocurrió un error.
//  Es útil en situaciones donde necesitas manejar múltiples conexiones o
//  entradas/salidas de manera eficiente, sin bloquear el programa mientras espera la disponibilidad de datos.

// int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

//     nfds: El valor más alto de los descriptores de archivo más uno.
//     readfds: Conjunto de descriptores de archivo que se monitorearán para lectura.
//     writefds: Conjunto de descriptores de archivo que se monitorearán para escritura.
//     exceptfds: Conjunto de descriptores de archivo que se monitorearán para excepciones (errores).
//     timeout: Estructura que especifica un límite de tiempo para el tiempo de espera. Puede ser NULL para esperar indefinidamente.

// select modificará los conjuntos readfds, writefds y exceptfds para indicar qué descriptores de archivo están listos para realizar operaciones de lectura, escritura o si ocurrió un error respectivamente.

// Aquí tienes un ejemplo simple de uso de select para monitorear la entrada estándar (STDIN_FILENO) en espera de entrada:

#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	fd_set readfds;
	struct timeval tv;
	int retval;

	// Limpiar el conjunto de descriptores de archivo
	FD_ZERO(&readfds);
	// Agregar la entrada estándar al conjunto de descriptores de archivo
	FD_SET(STDIN_FILENO, &readfds);

	// Configurar el tiempo de espera en 5 segundos
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	// Monitorear la entrada estándar para ver si hay datos disponibles para leer
	retval = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);

	if (retval == -1)
		perror("select()");
	else if (retval)
		std::cout << "Datos disponibles para leer en la entrada estándar." << std::endl;
	else
		std::cout << "Tiempo de espera agotado. No hay datos disponibles para leer en la entrada estándar." << std::endl;

	return 0;
}

// En este ejemplo, select monitorea la entrada estándar (STDIN_FILENO) durante un tiempo de espera de 5 segundos (tv.tv_sec = 5).
//  Si hay datos disponibles para leer en la entrada estándar dentro de este tiempo, select retornará un valor mayor que cero (retval),
//   indicando que la entrada está lista para ser leída. En caso contrario, si no hay datos dentro del tiempo de espera,
//   select retornará cero, indicando que el tiempo de espera ha expirado.