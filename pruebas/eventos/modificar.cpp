#include <iostream>
#include <sys/event.h> // Incluye esta librería para usar EV_SET y las constantes

// Definir tus banderas
#define EV_FLAG0 0x01
#define EV_FLAG1 0x02

void setupEvent(struct kevent &evSet, int fd)
{
    // Añadir EV_ADD, EV_FLAG0 y EV_FLAG1
    EV_SET(&evSet, fd, EVFILT_READ, EV_ADD | EV_FLAG0 | EV_FLAG1, 0, 0, NULL);
}

void modifyEvent(struct kevent &evSet, uintptr_t fd)
{
    // Inicialización de banderas
    int flags = EV_ADD | EV_FLAG0 | EV_FLAG1;
    // Eliminar EV_FLAG0 pero mantener los demás
    flags &= ~EV_FLAG0;

    // Reemplaza la línea anterior con las nuevas banderas
    EV_SET(&evSet, fd, EVFILT_READ, flags, 0, 0, NULL);
}

int main()
{
    struct kevent evSet;
    int fd = 0; // Suponiendo un file descriptor de ejemplo

    setupEvent(evSet, fd);
    std::cout << "Flags after setup: " << evSet.flags << std::endl;

    modifyEvent(evSet, fd);
    std::cout << "Flags after modification: " << evSet.flags << std::endl;

    return 0;
}
