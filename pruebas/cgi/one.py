
#!/usr/bin/env python3

#  procesar una solicitud POST:

import os

# Obtener los datos POST de la entrada estándar
content_length = int(os.environ.get('CONTENT_LENGTH', 0))
post_data = sys.stdin.read(content_length)

# Procesar los datos POST
# Aquí puedes realizar cualquier acción necesaria con los datos recibidos

# Enviar una respuesta al servidor web
print("Content-Type: text/plain")
print("")
print("Respuesta del script CGI en Python")
