import sys

def main():
    filename = sys.argv[1]
    x, y = "", ""
    with open(filename, 'r') as file:
        x, y = file.readline().split(' ')
    instance = filename.split('/')[-1]
    print(instance, x, y, sep=',',end="")

if __name__ == '__main__':
    main()
