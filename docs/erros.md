Las descripciones detalladas de los códigos de estado HTTP, incluidos los errores del servidor como 400, 403, 404, 405, 409, 413, 414, 500 y 501, se encuentran en la especificación oficial de HTTP/1.1 del Grupo de Trabajo de Ingeniería de Internet (IETF). Puedes consultar el RFC 7231, que es la especificación actualizada de los códigos de estado HTTP, para obtener información detallada sobre cada código de estado.


- **100 Continue**

- **200 OK**

- **400 Bad Request**: La solicitud del cliente es incorrecta o está malformada, y el servidor no puede o no quiere procesarla debido a un error del cliente.
- **403 Forbidden**: El servidor comprende la solicitud del cliente, pero se niega a cumplirla. Este código de estado se utiliza cuando el servidor no desea proporcionar acceso al recurso solicitado.
- **404 Not Found**: El servidor no pudo encontrar el recurso solicitado. Es el código de error más comúnmente conocido y se utiliza cuando la URL solicitada no se encuentra en el servidor.
- **405 Method Not Allowed**: El método de solicitud (por ejemplo, GET, POST) no está permitido para el recurso solicitado. Este código de estado se utiliza cuando el servidor no admite el método utilizado en la solicitud.
- **409 Conflict**: Indica que la solicitud no se pudo completar debido a un conflicto con el estado actual del recurso. Este código de estado se utiliza principalmente en aplicaciones que manejan operaciones de escritura concurrentes.
- **413 Payload Too Large**: La entidad solicitante es más grande de lo que el servidor está dispuesto o capaz de procesar. Este código de estado se utiliza cuando el servidor se niega a procesar una solicitud porque la carga útil (por ejemplo, el cuerpo del mensaje) es demasiado grande.
- **414 URI Too Long**: La URI (Identificador de Recurso Uniforme) proporcionada en la solicitud es demasiado larga para que el servidor la procese. Este código de estado se utiliza cuando la URI de la solicitud es más larga de lo que el servidor está dispuesto a procesar.
- **500 Internal Server Error**: Indica un error genérico en el servidor que impide que la solicitud del cliente sea completada. Este código de estado se utiliza cuando el servidor encuentra una condición inesperada que le impide cumplir con la solicitud.
- **501 Not Implemented**: El servidor no admite la funcionalidad requerida para completar la solicitud del cliente. Este código de estado se utiliza cuando el servidor no reconoce o no admite el método de solicitud utilizado en la solicitud.


A estudiar pero CREO QUE NO PONDREMOS

1. **301 Moved Permanently**: Indica que el recurso solicitado ha sido movido permanentemente a una nueva ubicación. Los clientes deberían actualizar sus URLs para apuntar a la nueva ubicación.

2. **302 Found (o Moved Temporarily)**: Indica que el recurso solicitado ha sido movido temporalmente a una nueva ubicación. Los clientes deberían seguir utilizando la URL original para futuras solicitudes.

3. **401 Unauthorized**: Indica que el cliente no está autorizado a acceder al recurso solicitado. Esto generalmente significa que el cliente necesita autenticarse antes de acceder al recurso.

4. **402 Payment Required**: Este código de estado está reservado para un uso futuro y aún no se utiliza comúnmente en la web.

5. **406 Not Acceptable**: Indica que el servidor no puede generar una respuesta que sea aceptable para el cliente según los encabezados Accept proporcionados en la solicitud.

6. **502 Bad Gateway**: Indica que el servidor, mientras actuaba como proxy o gateway, recibió una respuesta inválida del servidor ascendente al que intentaba acceder.

7. **503 Service Unavailable**: Indica que el servidor no puede manejar la solicitud en este momento debido a una sobrecarga temporal o mantenimiento del servidor. Es comúnmente utilizado para mensajes de mantenimiento o sobrecarga temporal.

8. **505 HTTP Version Not Supported**: Indica que el servidor no soporta la versión del protocolo HTTP utilizada en la solicitud.


https://datatracker.ietf.org/doc/html/rfc7231

