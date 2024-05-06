# `send()`

```cpp
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
```

Esta función se utiliza para enviar datos a través de un socket.

- `sockfd`: Es el descriptor de archivo del socket por el cual se enviarán los datos.
- `buf`: Es un puntero al búfer que contiene los datos que se enviarán.
- `len`: Es el tamaño en bytes de los datos que se enviarán.
- `flags`: Es un conjunto de indicadores que modifican el comportamiento del envío. Puede ser utilizado para especificar opciones adicionales, como el envío de mensajes fuera de banda o el control del comportamiento en caso de error.

La función `send()` devuelve el número de bytes enviados exitosamente, o -1 si ocurre un error. 

Es importante tener en cuenta que `send()` es específica para operaciones de red y está diseñada para ser utilizada con sockets. Proporciona funcionalidades específicas de red, como el envío de mensajes fuera de banda (`MSG_OOB`), y puede controlar el comportamiento en caso de error mediante los indicadores proporcionados en el parámetro `flags`.


Claro, aquí tienes ejemplos de cómo utilizar los indicadores de `flags` para controlar el comportamiento en caso de error y para enviar mensajes fuera de banda utilizando la función `send()` en C++98:

### Control de Comportamiento en Caso de Error:

Puedes utilizar los indicadores de `flags` para controlar el comportamiento en caso de error, por ejemplo, puedes especificar el comportamiento de bloqueo o no bloqueo del socket:

```cpp
// Establecer socket en modo no bloqueo
int flags = MSG_DONTWAIT;
ssize_t bytes_sent = send(sockfd, buffer, strlen(buffer), flags);
if (bytes_sent == -1) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
        // El socket está en modo no bloqueo y no se puede escribir en este momento
        // Puedes manejar esta situación aquí
    } else {
        // Se produjo otro error
        perror("Error al enviar datos");
    }
} else {
    // Los datos se enviaron correctamente
}
```

### Envío de Mensajes Fuera de Banda:

Puedes utilizar el indicador `MSG_OOB` para enviar mensajes fuera de banda a través del socket:

```cpp
// Enviar un mensaje fuera de banda
int flags = MSG_OOB;
ssize_t bytes_sent = send(sockfd, buffer, strlen(buffer), flags);
if (bytes_sent == -1) {
    perror("Error al enviar mensaje fuera de banda");
} else {
    // El mensaje fuera de banda se envió correctamente
}
```
# flags

1. **MSG_CONFIRM**: Confirma la entrega del mensaje en un protocolo confiable. Esta bandera es específica de algunos protocolos, como SCTP.
   
2. **MSG_DONTROUTE**: Indica que el mensaje no debe ser enrutado y se debe enviar solo al destino local.

3. **MSG_EOR**: Indica que este es el último fragmento de un mensaje.

4. **MSG_MORE**: Indica que hay más datos por enviar después de este mensaje.

5. **MSG_NOSIGNAL**: Indica que el sistema operativo no debe generar señales SIGPIPE si se intenta enviar datos a un socket que ya ha sido cerrado por el otro extremo.

6. **MSG_PEEK**: Permite leer datos del socket sin eliminarlos de la cola de recepción.

7. **MSG_WAITALL**: Especifica que la llamada `send()` debe bloquearse hasta que todos los datos solicitados hayan sido enviados.



PARA USAR

1. **MSG_DONTWAIT**: Si deseas que el socket opere en modo no bloqueo, puedes utilizar esta bandera para evitar que la función `send()` bloquee si no puede enviar los datos de inmediato.


3. **MSG_MORE**: Si estás enviando datos en fragmentos y deseas indicar que hay más datos por enviar después de este fragmento, puedes utilizar esta bandera para indicarlo.


5. **MSG_EOR**: Si estás enviando un mensaje completo y deseas indicar que este es el último fragmento del mensaje, puedes utilizar esta bandera para marcar el final del mensaje.

