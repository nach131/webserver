import os
from datetime import datetime

def generate_directory_listing(directory_path):
    # Obtener lista de archivos y directorios
    items = os.listdir(directory_path)
    items.sort()  # Ordenar alfabéticamente

    # Generar HTML
    html = """<html>
<head>
    <title>Index of {}</title>
</head>
<body>
    <h1>Index of {}</h1>
    <hr>
    <pre><a href="../">../</a>
""".format(directory_path, directory_path)

    for item in items:
        item_path = os.path.join(directory_path, item)
        if os.path.isdir(item_path):
            item += '/'
        modification_time = datetime.fromtimestamp(os.path.getmtime(item_path)).strftime('%d-%b-%Y %H:%M')
        html += '<a href="{}">{}</a>    {}\n'.format(item, item, modification_time)

    html += """</pre>
    <hr>
</body>
</html>"""
    return html

if __name__ == "__main__":
    directory_path = "."  # Directorio actual
    print(generate_directory_listing(directory_path))
