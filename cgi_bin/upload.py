import sys

def write_to_file(filename, data):
    try:
        with open(filename, 'wb') as file:
            file.write(data)
            print(f"File saved: {filename}")
            return 0
    except IOError:
        print("Error: Unable to open file for writing.")
        return 1

if __name__ == "__main__":
    filname = sys.argv[1]
    print(sys.argv[2])
    data = sys.argv[2].encode()  # Convert data to bytes
    result = write_to_file(filname, data)
    print("tomate file write",result)


# Datos binarios de ejemplo
# data = 'datos binarios.'
# Nombre del archivo
# filename = 'example.bin'