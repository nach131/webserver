import os
from datetime import datetime

def generate_directory_listing(directory_path, root_path):
    # Sustituir %20 por un espacio en blanco y eliminar todas las instancias de "./quitar"
    directory_path = directory_path.replace('%20', ' ').replace('./quitar', '')
    
    # Obtener lista de archivos y directorios
    items = os.listdir(directory_path)
    items.sort()  # Ordenar alfabéticamente

    # Generar HTML
    html = """<html>
<head>
    <title>Index of {}</title>
    <style>
        body {
            font-family: Arial, sans-serif;
        }
        pre {
            font-family: monospace;
        }
        .directory-listing {
            display: table;
            width: 100%;
        }
        .directory-item {
            display: table-row;
        }
        .directory-item a {
            display: table-cell;
            text-decoration: none;
            color: #0000FF;
            padding-right: 20px;
        }
        .directory-item span {
            display: table-cell;
            padding-right: 20px;
        }
        .directory-item .size {
            text-align: right;
        }
        hr {
            margin: 20px 0;
        }
    </style>
</head>
<body>
    <h1>Index of {}</h1>
    <hr>
    <pre class="directory-listing">
        <div class="directory-item"><a href="../">../</a><span></span><span></span></div>
""".format(directory_path, directory_path)

    for item in items:
        item_path = os.path.join(directory_path, item)
        is_directory = os.path.isdir(item_path)
        if is_directory:
            size = '-'
            item_link = '<a href="{}/{}">{}</a>'.format(root_path, item, item)
        else:
            size = os.path.getsize(item_path)
            item_link = '<a href="{}/{}">{}</a>'.format(root_path, item, item)

        modification_time = datetime.fromtimestamp(os.path.getmtime(item_path)).strftime('%d-%b-%Y %H:%M')
        html += '<div class="directory-item">{:<50} <span>{:>20}</span> <span class="size">{:>10}</span></div>\n'.format(item_link, modification_time, size)

    html += """</pre>
    <hr>
</body>
</html>"""
    return html

if __name__ == "__main__":
    import sys
    if len(sys.argv) != 3:
        print("Usage: python script.py directory_path root_path")
    else:
        directory_path = sys.argv[1]
        root_path = sys.argv[2]
        print(generate_directory_listing(directory_path, root_path))
