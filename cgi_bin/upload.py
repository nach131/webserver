#!/usr/bin/env python

import os
import cgi
import cgitb

cgitb.enable()

# Configuración de la carpeta donde se guardarán los archivos subidos
UPLOAD_DIR = "/path/to/upload/folder"

print("Content-Type: text/html")
print()

# Asegurarse de que la carpeta de subida existe
if not os.path.exists(UPLOAD_DIR):
    os.makedirs(UPLOAD_DIR)

form = cgi.FieldStorage()

# Comprobar si se ha enviado el archivo
if "file" not in form:
    print("<html><body>No file was uploaded</body></html>")
    exit()

fileitem = form["file"]

# Comprobar si el archivo fue subido
if fileitem.filename:
    # Sanitizar el nombre del archivo
    filename = os.path.basename(fileitem.filename)
    filepath = os.path.join(UPLOAD_DIR, filename)

    # Guardar el archivo en la carpeta de subida
    with open(filepath, 'wb') as f:
        f.write(fileitem.file.read())

    print(f"<html><body>File '{filename}' uploaded successfully</body></html>")
else:
    print("<html><body>No file was uploaded</body></html>")
