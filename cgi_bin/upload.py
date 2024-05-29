import sys

def write_file(input_file, output_file):
    try:
        with open(input_file, 'rb') as infile, open(output_file, 'wb') as outfile:
            outfile.write(infile.read())
        return 0
    except IOError as e:
        print(f"Error: {e}")
        return 1

if __name__ == "__main__":
    input_file = sys.argv[1]
    output_file = sys.argv[2]

    result = write_file(input_file, output_file)
    print(result)
