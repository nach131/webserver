import os
from datetime import datetime

def generate_directory_listing(directory_path):
        # Sustituir %20 por un espacio en blanco
    directory_path = directory_path.replace('%20', ' ')

    # res_path = directory_path.rstrip('/') # quitar ultimpo '/'
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
            size = '-'
        else:
            size = os.path.getsize(item_path)

        modification_time = datetime.fromtimestamp(os.path.getmtime(item_path)).strftime('%d-%b-%Y %H:%M')
        html += '{:<50} {:>20} {:>10}\n'.format('<a href="{}">{}</a>'.format(item, item), modification_time, size)
        # html += '{:<50} {:>20} {:>10}\n'.format('<a href="{}/{}">{}</a>'.format(directory_path, item, item), modification_time, size)

    html += """</pre>
    <hr>
</body>
</html>"""
    return html

if __name__ == "__main__":
    import sys
    if len(sys.argv) != 2:
        print("Usage: python script.py directory_path")
    else:
        directory_path = sys.argv[1]
        print(generate_directory_listing(directory_path))