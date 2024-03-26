// kqueue es un mecanismo de E/S en sistemas operativos tipo Unix, específicamente en FreeBSD y macOS
// (donde se llama kqueue). Similar a epoll en Linux, kqueue proporciona una interfaz eficiente para
//  monitorear múltiples descriptores de archivo para eventos de interés como lectura, escritura o errores.

#include <iostream>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>

#define MAX_EVENTS 10

int main()
{
	int kq, nev;
	struct kevent change, event[MAX_EVENTS];
	struct timespec timeout = {5, 0}; // 5 segundos

	// Crear un nuevo objeto kqueue
	kq = kqueue();
	if (kq == -1)
	{
		perror("kqueue");
		return 1;
	}

	// Configurar la estructura kevent para monitorear STDIN_FILENO para eventos de lectura
	EV_SET(&change, STDIN_FILENO, EVFILT_READ, EV_ADD, 0, 0, NULL);

	// Aplicar el cambio a kqueue
	if (kevent(kq, &change, 1, NULL, 0, NULL) == -1)
	{
		perror("kevent");
		return 1;
	}

	// Esperar eventos en kqueue
	nev = kevent(kq, NULL, 0, event, MAX_EVENTS, &timeout);
	if (nev == -1)
	{
		perror("kevent");
		return 1;
	}
	else if (nev == 0)
	{
		std::cout << "Tiempo de espera agotado. No hay eventos disponibles." << std::endl;
	}
	else
	{
		for (int i = 0; i < nev; ++i)
		{
			if (event[i].ident == STDIN_FILENO)
			{
				// Leer de STDIN_FILENO
				char buffer[1024];
				ssize_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
				if (bytes_read == -1)
				{
					perror("read");
					return 1;
				}
				buffer[bytes_read] = '\0'; // Añadir terminador nulo
				std::cout << "Datos leídos de STDIN_FILENO: " << buffer << std::endl;
			}
		}
	}

	// Cerrar el descriptor de kqueue
	close(kq);

	return 0;
}
