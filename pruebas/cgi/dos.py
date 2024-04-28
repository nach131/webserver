#!/usr/bin/env python3

import sys

# Leer la entrada estándar (solicitud CGI)
request_body = sys.stdin.read()

# Procesar la solicitud PHP (opcional)
# Aquí podrías analizar la solicitud para extraer datos relevantes

# Enviar la solicitud al servidor C++ (simulado)
# Aquí podrías usar sockets, HTTP, u otro mecanismo de comunicación

# Simular respuesta del servidor C++
response_body = "Respuesta del servidor C++"

# Imprimir la respuesta CGI válida
print("Content-Type: text/plain")
print("")
print(response_body)