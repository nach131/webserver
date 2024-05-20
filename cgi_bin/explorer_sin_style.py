import os
from datetime import datetime

def remove_after_last_slash(path):
    pos = path.rfind('/')
    if pos != -1:
        return path[:pos]
    return path

def generate_directory_listing(directory_path, root_location):
   
    # Sustituir %20 por un espacio en blanco
    directory_path = directory_path.replace('%20', ' ')
    path = directory_path.replace(root_location, '')

    pre_path = remove_after_last_slash(path)

    # print("============")
    # print("root_location:", root_location, "   -    ")
    # print("directory_path:", directory_path, "   -  ")
    # print("path:", path, "    -   ")
    # print("pre_path:", pre_path, "    -   ")
    # print("============")

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
    <pre><a href="{}">../</a>
""".format(path, path, pre_path)

    for item in items:
        item_path = os.path.join(directory_path, item)
        is_directory = os.path.isdir(item_path)
        if is_directory:
            size = '-'
            item_link = '<a href="{}/{}">{}</a>'.format(path, item, item)
        else:
            size = os.path.getsize(item_path)
            item_link = '<a href="{}/{}">{}</a>'.format(path, item, item)

        modification_time = datetime.fromtimestamp(os.path.getmtime(item_path)).strftime('%d-%b-%Y %H:%M')
        html += '{:<50} {:>20} {:>10}\n'.format(item_link, modification_time, size)

    html += """</pre>
    <hr>
</body>
</html>"""
    return html

if __name__ == "__main__":
    import sys
    if len(sys.argv) != 3:
        print("Usage: python script.py directory_path root_location")
    else:
        directory_path = sys.argv[1]
        root_location = sys.argv[2]
        print(generate_directory_listing(directory_path, root_location))


