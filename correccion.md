
## Comprobación del código y preguntas

- Iniciar la instalación de siege con homebrew.

- Preguntar explicaciones sobre los conceptos básicos de un servidor HTTP.

- Preguntar qué función utiliza el grupo para la multiplexación de E/S.

- Preguntar por una explicación de cómo funciona select() (o equivalente).

- Preguntar si solo utilizan un select() (o equivalente) y cómo han gestionado el servidor para que acepte y el cliente para leer/escribir.

- El select() (o equivalente) debe estar en el bucle principal y debe comprobar los descriptores de archivo para leer y escribir AL MISMO TIEMPO. Si no es así, la calificación es O y el proceso de evaluación termina ahora.

- Debe haber solo una lectura o una escritura por cliente por select() (o equivalente). Pida al grupo que le muestre el código desde el select() (o equivalente) hasta la lectura y escritura de un cliente.

- Busque todas las lecturas/recibos/escrituras/envíos en un socket y compruebe que, si se devuelve un error, el cliente se elimina.

- Busque todas las lecturas/recibos/escrituras/envíos y compruebe si el valor devuelto se comprueba correctamente (no es suficiente comprobar solo los valores -1 o 0, se deben comprobar ambos).

- Si se comprueba errno después de read/recv/write/send, la calificación es O y el proceso de evaluación termina ahora.

- Está estrictamente PROHIBIDO escribir o leer CUALQUIER descriptor de archivo sin pasar por el select() (o equivalente).

- El proyecto debe compilarse sin ningún problema de relinking. Si no es así, utilice la bandera "Invalid compilation".

- Si algún punto no está claro o no es correcto, la evaluación se detiene.

## Configuración

En el archivo de configuración, compruebe si puede hacer lo siguiente y pruebe el resultado:

- Buscar en Internet la lista de códigos de estado de respuesta HTTP. Durante esta evaluación, si algún código de estado es incorrecto, no otorgue ningún punto relacionado.

- Configurar varios servidores con diferentes puertos.

- Configurar varios servidores con diferentes nombres de host (use algo como: curl --resolve example.com:80:127.0.0.1 [http://example.com/](http://example.com/)).

- Configurar una página de error predeterminada (intente cambiar el error 404).

- Limitar el cuerpo del cliente (use: curl -X POST -H "Content-Type: plain/text" --data "BODY IS HERE write something shorter or longer than body limit").

- Configurar rutas en un servidor a diferentes directorios.

- Configurar un archivo predeterminado para buscar si se solicita un directorio.

- Configurar una lista de métodos aceptados para una ruta determinada (por ejemplo, intentar eliminar algo con y sin permiso).

# Comprobaciones básicas

Utilizando telnet, curl y archivos preparados, demuestre que las siguientes funciones funcionan correctamente:

- Las solicitudes GET, POST y DELETE deberían funcionar.

- Las solicitudes DESCONOCIDAS no deberían provocar un bloqueo.

- Para cada prueba, debería recibir el código de estado correspondiente.

- Suba un archivo al servidor y recupérelo.

# Comprobar CGI

- El servidor funciona correctamente utilizando un CGI.

- El CGI se debe ejecutar en el directorio correcto para el acceso a archivos de ruta relativa.

- Con la ayuda de los estudiantes, debe comprobar que todo funcione correctamente. Debe probar el CGI con los métodos "GET" y "POST".

- Debe probar con archivos que contienen errores para ver si el manejo de errores funciona correctamente. Puede utilizar un script que contenga un bucle infinito o un error; puede realizar las pruebas que desee dentro de los límites de la aceptabilidad que quedan a su discreción. El grupo que se evalúa debe ayudarlo con esto.

- El servidor nunca debe fallar y un error debe ser visible en caso de un problema.


# Comprobar con un navegador

- Utilice el navegador de referencia del equipo. Abra la parte de red del mismo y trate de conectarse al servidor usándolo.

- Observe el encabezado de solicitud y el encabezado de respuesta.

- Debe ser compatible para servir un sitio web completamente estático.

- Pruebe una URL incorrecta en el servidor.

- Intente enumerar un directorio.

- Pruebe una URL redirigida.

- Pruebe todo lo que quiera.


# Problemas de puertos

- En el archivo de configuración, configure varios puertos y utilice diferentes sitios web. Utilice el navegador para asegurarse de que la configuración funciona como se esperaba y muestra el sitio web correcto.

- En la configuración, intente configurar el mismo puerto varias veces. No debería funcionar.

- Inicie varios servidores al mismo tiempo con diferentes configuraciones pero con puertos comunes. ¿Funciona? Si es así, pregunte por qué el servidor debería funcionar si una de las configuraciones no es funcional. Siga adelante.


# Siege y prueba de estrés

- Utilice Siege para ejecutar algunas pruebas de estrés.

- La disponibilidad debe ser superior al 99,5% para un GET simple en una página vacía con un siege -b en esa página.

- Verifique que no haya fugas de memoria (monitoree el uso de memoria del proceso. No debería aumentar indefinidamente).

- Compruebe si no hay conexiones colgadas.

- Debería poder utilizar siege indefinidamente sin tener que reiniciar el servidor (consulte siege -b).

- Al realizar pruebas de carga utilizando el comando siege (depende de su sistema operativo), es crucial limitar el número de conexiones por segundo especificando opciones como -c (número de clientes), -d (tiempo máximo de espera antes de que un cliente se reconecte) y -r (número de intentos). La elección de estos parámetros queda a discreción del evaluador, sin embargo, es imperativo llegar a un acuerdo con la persona que se evalúa para garantizar una evaluación justa y transparente del rendimiento del servidor web.


siege -c 5 -r 5 http://localhost:8080/form

siege -b 5 -r 5 http://localhost:8080/form

siege -b -t 3600 -c 100 http://localhost:8080

siege -b -c 10 http://localhost:8080

- b indica el modo de benchmarking (sin salida de pantalla).
- t <tiempo> especifica la duración de la prueba en segundos. Por ejemplo, -t 3600 para una hora.
- c <clientes> especifica el número de clientes concurrentes. Por ejemplo, -c 100 para simular 100 usuarios concurrentes.