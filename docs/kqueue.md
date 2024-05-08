
	EV_SET(&change, sockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);


La función `EV_SET` se utiliza para inicializar o modificar una estructura de evento (`struct kevent`). Esta estructura se utiliza en el sistema de eventos `kqueue` para representar un evento que el programa está interesado en monitorear o manipular.


- **change**: Es un puntero a la estructura `kevent` que se inicializará o modificará.
- **ident**: Es el identificador del objeto para el cual se desea recibir eventos. En este caso, `newsockfd` es el descriptor de archivo del socket que se está añadiendo al `kqueue`.
- **filter**: Es el tipo de filtro del evento. En este caso, `EVFILT_READ` indica que estamos interesados en eventos de lectura en el socket.
- **flags**: Especifica cómo se manejará el evento. `EV_ADD` indica que se está agregando el evento al conjunto de eventos que se están monitoreando.
- **fflags**: Son los filtros específicos del evento. En este caso, `0` indica que no hay filtros específicos adicionales.
- **data**: Es un dato adicional asociado con el evento. `0` en este caso indica que no se necesita ningún dato adicional.
- **udata**: Es un puntero a datos de usuario opcionales asociados con el evento. `NULL` indica que no se están utilizando datos de usuario en este caso.

## udata

El campo `udata` de la estructura `struct kevent` en el sistema de eventos `kqueue` es un puntero a datos de usuario opcionales asociados con el evento.

Aqui podemos almacenar cualquier tipo de datos que desees en este campo para que estén disponibles cuando se maneje el evento.

1. **Punteros a estructuras de datos**: Puedes almacenar un puntero a una estructura de datos que contenga información relevante para el evento. Por ejemplo, podrías tener una estructura que almacene detalles sobre la conexión asociada con el evento.

2. **Punteros a datos dinámicos**: Puedes asignar memoria dinámicamente para almacenar datos relevantes para el evento y luego almacenar un puntero a estos datos en el campo `udata`. Esto es útil cuando necesitas almacenar una cantidad variable de información para cada evento.

3. **Identificadores de objetos**: En algunos casos, el campo `udata` puede utilizarse simplemente para almacenar un identificador único asociado con el evento. Esto puede ser útil cuando necesitas asociar un evento con un objeto específico en tu programa.
