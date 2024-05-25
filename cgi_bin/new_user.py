#!/usr/bin/env python3

import cgi
import cgitb
from pymongo import MongoClient

cgitb.enable()  # Para la depuración

# Conexión a MongoDB
client = MongoClient("mongodb://root:klingon@192.168.1.20:27017/")
db = client['mi_basededatos']
collection = db['mi_coleccion']

def guardar_datos_en_mongo(datos):
    """Función para guardar los datos en MongoDB"""
    collection.insert_one(datos)

# Procesamiento del formulario
form = cgi.FieldStorage()

nombre = form.getvalue('nombre')
email = form.getvalue('email')

datos = {
    'nombre': nombre,
    'email': email
}

# Guardar los datos en MongoDB
guardar_datos_en_mongo(datos)

# Responder al navegador
print("Content-Type: text/html")
print()
print("<html><body>")
print("<h2>Datos recibidos y almacenados en MongoDB</h2>")
print("<p>Nombre: {}</p>".format(nombre))
print("<p>Email: {}</p>".format(email))
print("</body></html>")
