Las descripciones detalladas de los códigos de estado HTTP, incluidos los errores del servidor como 400, 403, 404, 405, 409, 413, 414, 500 y 501, se encuentran en la especificación oficial de HTTP/1.1 del Grupo de Trabajo de Ingeniería de Internet (IETF). Puedes consultar el RFC 7231, que es la especificación actualizada de los códigos de estado HTTP, para obtener información detallada sobre cada código de estado.


- **400 Bad Request**: La solicitud del cliente es incorrecta o está malformada, y el servidor no puede o no quiere procesarla debido a un error del cliente.
- **403 Forbidden**: El servidor comprende la solicitud del cliente, pero se niega a cumplirla. Este código de estado se utiliza cuando el servidor no desea proporcionar acceso al recurso solicitado.
- **404 Not Found**: El servidor no pudo encontrar el recurso solicitado. Es el código de error más comúnmente conocido y se utiliza cuando la URL solicitada no se encuentra en el servidor.
- **405 Method Not Allowed**: El método de solicitud (por ejemplo, GET, POST) no está permitido para el recurso solicitado. Este código de estado se utiliza cuando el servidor no admite el método utilizado en la solicitud.
- **409 Conflict**: Indica que la solicitud no se pudo completar debido a un conflicto con el estado actual del recurso. Este código de estado se utiliza principalmente en aplicaciones que manejan operaciones de escritura concurrentes.
- **413 Payload Too Large**: La entidad solicitante es más grande de lo que el servidor está dispuesto o capaz de procesar. Este código de estado se utiliza cuando el servidor se niega a procesar una solicitud porque la carga útil (por ejemplo, el cuerpo del mensaje) es demasiado grande.
- **414 URI Too Long**: La URI (Identificador de Recurso Uniforme) proporcionada en la solicitud es demasiado larga para que el servidor la procese. Este código de estado se utiliza cuando la URI de la solicitud es más larga de lo que el servidor está dispuesto a procesar.
- **500 Internal Server Error**: Indica un error genérico en el servidor que impide que la solicitud del cliente sea completada. Este código de estado se utiliza cuando el servidor encuentra una condición inesperada que le impide cumplir con la solicitud.
- **501 Not Implemented**: El servidor no admite la funcionalidad requerida para completar la solicitud del cliente. Este código de estado se utiliza cuando el servidor no reconoce o no admite el método de solicitud utilizado en la solicitud.

