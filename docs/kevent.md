# `kevent`

```c
int kevent(int kq, const struct kevent *changelist, int nchanges,
           struct kevent *eventlist, int nevents,
           const struct timespec *timeout);
```

1. `kq`: Este es el descriptor de archivo devuelto por la función `kqueue`, que representa la instancia de kqueue en la que se están monitoreando los eventos.

2. `changelist`: Este es un puntero a una estructura `kevent` o a un arreglo de estructuras `kevent`. Se utiliza para especificar los cambios que se desean realizar en el conjunto de descriptores de archivo que están siendo monitoreados por `kqueue`. Puede ser `NULL` si no se desean realizar cambios.

3. `nchanges`: Este es el número de cambios que se están especificando en `changelist`. Si `changelist` es un puntero a una única estructura `kevent`, `nchanges` será 1.

4. `eventlist`: Este es un puntero a una estructura `kevent` o a un arreglo de estructuras `kevent`. Se utiliza para recibir eventos que ocurran en los descriptores de archivo monitoreados por `kqueue`.

5. `nevents`: Este es el tamaño del arreglo `eventlist`. Indica cuántos eventos pueden ser almacenados en `eventlist`.

6. `timeout`: Este parámetro es opcional. Es un puntero a una estructura `timespec` que especifica un intervalo de tiempo durante el cual `kevent` esperará eventos. Si `timeout` es `NULL`, `kevent` no esperará eventos y regresará inmediatamente. Si `timeout` apunta a una estructura `timespec` que tiene el campo `tv_sec` y `tv_nsec` establecido en 0, `kevent` tampoco esperará y regresará inmediatamente.

En resumen, `kevent` se utiliza para realizar operaciones como agregar, modificar o eliminar eventos en el conjunto de descriptores de archivo que están siendo monitoreados por `kqueue`, así como para esperar eventos y recibirlos.


# `struct kevent`
es una estructura utilizada en la interfaz `kqueue` de FreeBSD y macOS para representar un evento que ha ocurrido o que se espera que ocurra en un descriptor de archivo.

```cpp
struct kevent {
    uintptr_t ident;     // Identificador del descriptor de archivo (por ejemplo, un socket)
    short filter;        // Tipo de filtro de evento (por ejemplo, EVFILT_READ, EVFILT_WRITE)
    u_short flags;       // Opciones para controlar el comportamiento del evento
    u_int fflags;        // Opciones específicas del filtro de evento (por ejemplo, NOTE_READ, NOTE_WRITE)
    intptr_t data;       // Datos adicionales asociados con el evento
    void *udata;         // Puntero de usuario para datos adicionales (opcionalmente utilizado por el usuario)
};
```

- `ident`: Es un identificador que representa un descriptor de archivo, como un socket, un archivo, un tubo, etc. Cuando se produce un evento en este descriptor, se utiliza este identificador para identificar el descriptor de archivo afectado.

- `filter`: Especifica el tipo de filtro de evento que se está utilizando. Algunos valores comunes para `filter` incluyen `EVFILT_READ` para eventos de lectura y `EVFILT_WRITE` para eventos de escritura.

- `flags`: Son opciones que controlan el comportamiento del evento. Pueden incluir opciones como `EV_ADD` para agregar un nuevo evento, `EV_DELETE` para eliminar un evento existente, `EV_ENABLE` para habilitar un evento, `EV_DISABLE` para deshabilitar un evento, etc.

- `fflags`: Son opciones específicas del filtro de evento que complementan el `filter`. Por ejemplo, `NOTE_READ` y `NOTE_WRITE` son fflags comunes para indicar eventos de lectura y escritura, respectivamente.

- `data`: Proporciona datos adicionales asociados con el evento. Por ejemplo, si se trata de un evento de lectura, `data` podría indicar cuántos bytes están disponibles para leer.

- `udata`: Es un puntero de usuario opcional que se puede utilizar para asociar datos adicionales definidos por el usuario con el evento. Este campo puede ser útil para mantener información adicional relacionada con el evento.

Estos son los elementos principales de la estructura `kevent` que se utilizan para representar eventos en `kqueue`.