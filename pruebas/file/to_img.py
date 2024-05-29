import sys

def copy_file(input_file, output_file):
    try:
        with open(input_file, 'rb') as infile, open(output_file, 'wb') as outfile:
            outfile.write(infile.read())
        print(f"File copied from {input_file} to {output_file}")
        return 0
    except IOError as e:
        print(f"Error: {e}")
        return 1

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script.py <input_file> <output_file>")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2]

    result = copy_file(input_file, output_file)
    sys.exit(result)
