import sys

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script.py filePath  buffer")
    else:
        filePath = sys.argv[1]
        buffer = sys.argv[2]
        print("File Path:", filePath)
        # print("Buffer:", buffer)
