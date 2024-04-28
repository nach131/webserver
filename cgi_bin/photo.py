#!/usr/bin/env python3

import os

# Imprime las cabeceras HTTP requeridas
print("Content-Type: text/html")  # Indica que se va a enviar HTML
print()  # Imprime una línea en blanco para indicar el final de las cabeceras

# Imprime el cuerpo de la respuesta HTML
print("<html>")
print("    <head>")
print("        <title>Random Image CGI</title>")
print("        <style>")
print("            body {")
print("                  margin: 0;")
print("            }")
print("            .index {")
print("                 text-shadow: 1px 1px 0 rgba(0, 0, 0, 0.4) !important;")
print("            }")
print("            .index {")
print("                 width: 100vw;")
print("                 height: 100vh;")
print("            }")
print("            .masthead {")
print("                color: white;")
print("                min-height: 30rem;")
print("                height: 100%;")
print("                display: flex;")  # Usa un contenedor flexible
print("                justify-content: center;")  # Centra horizontalmente
print("                align-items: center;")  # Centra verticalmente
print("                background: url(\"https://source.unsplash.com/random\");")
print("                background-position: center center;")
print("                background-repeat: no-repeat;")
print("                background-size: cover;")
print("            }")
print("        </style>")
print("    </head>")
print("    <body>")
print("      <main class=\"index\">")
print("        <div class=\"masthead\">")
print("            <h1 class=\"portada\">Random Image CGI</h1>")
print("        </div>")
print("      </\main>")
print("    </body>")
print("</html>")
