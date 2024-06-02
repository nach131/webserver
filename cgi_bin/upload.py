import sys
import shutil

def main():
    if len(sys.argv) != 2:
        print("Nombre de fichero")
        return 1

    file_name = sys.argv[1]

    # Crear el archivo (si no existe ya)
    open(file_name, 'a').close()

    try:
        # with open('/Users/nmota-bu/Desktop/webserver/.tmpdir/.bin', 'rb') as in_file:
        with open('./.tmpdir/.bin', 'rb') as in_file:
            with open(file_name, 'wb') as out_file:
                shutil.copyfileobj(in_file, out_file)
    except FileNotFoundError:
        return 1
    except IOError:
        return 1
    
    return 0

if __name__ == "__main__":
    print(main())