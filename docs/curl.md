ESTA MAL EL PARSING
curl -X POST -d "nombre=Juan&edad=30" http://localhost:8080

curl -X DELETE http://localhost:8080/123

AQUI ESTA MAL EL PARSING
CGI
curl "http://localhost:8080/cgi-bin/script.cgi?nombre=Juan&edad=30"


//=========================================================================

AQUI ESTA MAL EL PARSING

# Enviando datos en la URL:

curl -X GET \
     -H "Content-Type: application/json" \
     -H "Authorization: Bearer <token>" \
     --data-urlencode "query=SELECT * FROM tabla WHERE columna='valor'" \
     "http://localhost:8080/resource?parametro1=valor1&parametro2=valor2&parametro3=valor3&parametro4=valor4"

# Enviando datos en el cuerpo de la solicitud:

curl -X GET \
     -H "Content-Type: application/json" \
     -H "Authorization: Bearer <token>" \
     --data-urlencode "query=SELECT * FROM tabla WHERE columna='valor'" \
     -d '{"parametro1": "valor1", "parametro2": "valor2"}' \
     "http://localhost:8080/resource"

# POST

curl -X POST \
     -H "Content-Type: application/json" \
     -H "Authorization: Bearer <token>" \
     --data-urlencode "query=SELECT * FROM tabla WHERE columna='valor'" \
     -d '{"parametro1": "valor1", "parametro2": "valor2"}' \
     "http://localhost:8080/resource?parametro3=valor3&parametro4=valor4"

curl -X POST \
     -H "Content-Type: application/json" \
     -H "Authorization: Bearer <token>" \
     --data-urlencode "query=SELECT * FROM tabla WHERE columna='valor'" \
     -d '{"parametro1": "valor1", "parametro2": "valor2"}' \
     --compressed \
     "http://localhost:8080/api/resource?parametro3=valor3&parametro4=valor4"

# DELETE

curl -X DELETE \
     -H "Content-Type: application/json" \
     -H "Authorization: Bearer <token>" \
     --data-urlencode "parametro1=valor1" \
     --data-urlencode "parametro2=valor2" \
     "http://localhost:8080/api/resource?parametro3=valor3&parametro4=valor4"


//=========================================================================


- `-X GET`: Especifica el método HTTP GET explícitamente. Aunque en `curl`, `GET` es el método predeterminado, incluir esto puede ser útil para claridad.
- `-H "Content-Type: application/json"`: Encabezado que especifica el tipo de contenido que se envía en el cuerpo de la solicitud. Aquí se usa JSON.
- `-H "Authorization: Bearer <token>"`: Encabezado de autorización con un token de autenticación. Reemplaza `<token>` con tu token real.
- `-d '{"parametro1": "valor1", "parametro2": "valor2"}'`: Datos que se enviarán en el cuerpo de la solicitud en formato JSON.
- `-G`: Convierte la solicitud en una solicitud GET. Esto convierte cualquier dato enviado con `-d` en parámetros de la URL.
- `--data-urlencode "query=SELECT * FROM tabla WHERE columna='valor'"`: Codifica los datos en formato de URL. Útil para enviar datos que pueden contener caracteres especiales.
- `"http://ejemplo.com/api/resource?parametro3=valor3&parametro4=valor4"`: La URL a la que se enviará la solicitud, con parámetros adicionales adjuntos.



## Metodo no valido
curl --location --request PATCH 'http://127.0.0.1:8080'