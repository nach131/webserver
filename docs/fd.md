# FD_SET FD_CLR FD_ISSET FD_ZERO
Los que se pueden usar

## FD_SET

`FD_SET` es una macro utilizada en programación de sockets en sistemas basados en Unix para agregar un descriptor de archivo a un conjunto de descriptores de archivo (`fd_set`). Este conjunto se utiliza comúnmente con funciones como `select()`, `pselect()`, `FD_ISSET()`, entre otras, para verificar la disponibilidad de operaciones de lectura, escritura o excepcionales en múltiples descriptores de archivo de manera eficiente.

La macro `FD_SET(int fd, fd_set *set)` agrega el descriptor de archivo `fd` al conjunto `set`. Después de llamar a `FD_SET`, `fd` estará marcado como presente en el conjunto, lo que significa que será monitoreado por la función de multiplexación (por ejemplo, `select()`).

Es importante tener en cuenta que `FD_SET` puede resultar en modificaciones directas en la estructura `fd_set`, por lo que se debe tener cuidado para evitar condiciones de carrera si se están utilizando múltiples subprocesos.

Aquí hay un ejemplo de cómo se puede usar `FD_SET` para agregar un descriptor de archivo al conjunto:

```cpp
#include <sys/select.h> // Para fd_set y FD_SET

// Definición de un conjunto de descriptores de archivo
fd_set fds;

// Descriptor de archivo que se desea agregar al conjunto
int sockfd = ...;

// Inicializar el conjunto a un conjunto vacío
FD_ZERO(&fds);

// Agregar el descriptor de archivo al conjunto
FD_SET(sockfd, &fds);
```

Después de esta secuencia de código, el descriptor de archivo `sockfd` estará presente en el conjunto `fds` y puede ser utilizado con funciones de multiplexación como `select()`.


## select()

`select()` es una función del sistema operativo utilizada en sistemas basados en Unix y Windows para monitorear múltiples descriptores de archivo (sockets, archivos, etc.) para determinar si alguno de ellos está listo para realizar operaciones de lectura, escritura o excepcionales, sin bloquear el proceso. Es útil en situaciones donde un programa necesita manejar múltiples conexiones de forma simultánea y eficiente.

La función `select()` toma como argumentos tres conjuntos de descriptores de archivo: uno para los descriptores que se deben monitorear para operaciones de lectura, otro para operaciones de escritura y otro para operaciones excepcionales. La función modificará estos conjuntos para indicar qué descriptores están listos para cada tipo de operación.

La función `select()` también toma como argumento un cuarto conjunto de tiempo de espera (`struct timeval *timeout`). Este argumento especifica cuánto tiempo `select()` debe esperar antes de regresar. Si `timeout` es `NULL`, `select()` se bloqueará indefinidamente hasta que ocurra una actividad en uno de los descriptores. Si `timeout` es un puntero a una estructura `timeval`, `select()` regresará después de el tiempo especificado, incluso si no ha ocurrido ninguna actividad en los descriptores.

Aquí hay un ejemplo básico de cómo se puede usar `select()`:

```cpp
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h> // Para close()

int main() {
    fd_set read_fds;
    FD_ZERO(&read_fds);

    int sockfd = ...; // descriptor de archivo del socket

    // Agregar el descriptor de archivo del socket al conjunto de lectura
    FD_SET(sockfd, &read_fds);

    // Configurar el tiempo de espera
    struct timeval timeout;
    timeout.tv_sec = 5; // 5 segundos
    timeout.tv_usec = 0;

    // Llamar a select() para esperar eventos en el descriptor de archivo del socket
    int ready = select(sockfd + 1, &read_fds, NULL, NULL, &timeout);
    if (ready == -1) {
        // Error en select()
    } else if (ready == 0) {
        // Se alcanzó el tiempo de espera sin que ocurriera ninguna actividad
    } else {
        // El descriptor de archivo del socket está listo para lectura
        if (FD_ISSET(sockfd, &read_fds)) {
            // Leer del socket
        }
    }

    // Cerrar el descriptor de archivo cuando ya no se necesite
    close(sockfd);

    return 0;
}
```

En este ejemplo, `select()` espera hasta que haya actividad en el descriptor de archivo del socket (`sockfd`) o hasta que se alcance el tiempo de espera especificado. Después de `select()` retorna, el programa puede verificar si el socket está listo para lectura utilizando `FD_ISSET()`.


## FD_CLR

`FD_CLR` es una macro en C que se utiliza para borrar un descriptor de archivo específico de un conjunto de descriptores de archivo (`fd_set`). Esta macro se utiliza junto con `FD_ZERO`, `FD_SET` y `FD_ISSET` para manipular conjuntos de descriptores de archivo en el contexto de la función `select()`.

La macro `FD_CLR` toma dos argumentos: el descriptor de archivo que se desea borrar y el conjunto de descriptores de archivo del cual se desea borrar. Al llamar a `FD_CLR`, se elimina el descriptor de archivo especificado del conjunto, lo que significa que ya no se monitoreará ese descriptor de archivo cuando se llame a `select()`.

Aquí hay un ejemplo de cómo se usa `FD_CLR`:

```c
#include <sys/select.h>
#include <unistd.h> // Para close()

int main() {
    fd_set read_fds;
    FD_ZERO(&read_fds);

    int sockfd = ...; // descriptor de archivo del socket

    // Agregar el descriptor de archivo del socket al conjunto de lectura
    FD_SET(sockfd, &read_fds);

    // Algo de código...

    // Borrar el descriptor de archivo del socket del conjunto de lectura
    FD_CLR(sockfd, &read_fds);

    // Ahora el descriptor de archivo del socket ya no está en el conjunto de lectura

    // Cerrar el descriptor de archivo cuando ya no se necesite
    close(sockfd);

    return 0;
}
```

En este ejemplo, `FD_CLR(sockfd, &read_fds)` elimina el descriptor de archivo del socket (`sockfd`) del conjunto de descriptores de archivo `read_fds`. Después de esto, `sockfd` ya no será monitoreado para operaciones de lectura cuando se llame a `select()`.

## FD_ISSET

`FD_ISSET` es una macro en C que se utiliza para verificar si un descriptor de archivo específico está presente y listo para una operación en un conjunto de descriptores de archivo (`fd_set`). Esta macro se utiliza junto con `FD_ZERO`, `FD_SET` y `FD_CLR` en el contexto de la función `select()`.

La macro `FD_ISSET` toma dos argumentos: el descriptor de archivo que se desea verificar y el conjunto de descriptores de archivo en el que se desea verificar la presencia del descriptor de archivo. Al llamar a `FD_ISSET`, se comprueba si el descriptor de archivo especificado está presente en el conjunto y si está listo para una operación, como lectura o escritura.


```c
#include <sys/select.h>

int main() {
    fd_set read_fds;
    FD_ZERO(&read_fds);

    int sockfd = ...; // descriptor de archivo del socket

    // Agregar el descriptor de archivo del socket al conjunto de lectura
    FD_SET(sockfd, &read_fds);

    // Algo de código...

    // Verificar si el descriptor de archivo del socket está listo para leer
    if (FD_ISSET(sockfd, &read_fds)) {
        // El descriptor de archivo del socket está listo para leer
        // Realizar operaciones de lectura...
    } else {
        // El descriptor de archivo del socket no está listo para leer
        // Esperar o realizar otras operaciones...
    }

    return 0;
}
```

En este ejemplo, `FD_ISSET(sockfd, &read_fds)` verifica si el descriptor de archivo del socket (`sockfd`) está presente y listo para una operación de lectura en el conjunto de descriptores de archivo `read_fds`. Si el descriptor de archivo está presente y listo, se realiza la operación de lectura correspondiente. De lo contrario, se pueden realizar otras acciones, como esperar o realizar otras operaciones.