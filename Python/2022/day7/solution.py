class Folder:
    def __init__(self, name, parent):
        self.name = name
        self.dirs = {}
        self.parent = parent
        self.size = 0

    def find_smaller(self, limit, result):
        if self.size <= limit:
            result.append(self.size)
        
        for _, sub in self.dirs.items():
            sub.find_smaller(limit, result)

    def find_larger(self, limit, result):
        if self.size >= limit:
            result.append(self.size)
        
        for _, sub in self.dirs.items():
            sub.find_larger(limit, result)

    def scan_size(self):
        if len(self.dirs) == 0:
            return self.size

        for _, subfolder in self.dirs.items():
            self.size += subfolder.scan_size()

        return self.size 

def parse_filesystem(data):
    root = Folder('/', None)

    currentDir = root

    for line in data:
        output = line.split()
        
        if output[0] == '$': # command
            if output[1] == 'cd':
                dir = output[2]
                
                if dir == '..':
                    currentDir = currentDir.parent
                    continue

                if dir == '/':
                    currentDir = root
                    continue
     
                currentDir = currentDir.dirs[dir]

            continue

        if output[0] == 'dir':
            if output[1] not in currentDir.dirs:
                currentDir.dirs[output[1]] = Folder(output[1], currentDir)
            continue

        currentDir.size += int(output[0])

    root.scan_size()

    return root
    
with open("input.txt", "r") as file:

    data = [line for line in file.readlines()]

    root = parse_filesystem(data)

    #Part 1
    smaller = []
    size_limit = 100000
    root.find_smaller(size_limit, smaller)

    print(sum(smaller))

    #Part 2
    filesystem_size = 70000000
    update_space = 30000000

    available = filesystem_size - root.size
    needed = update_space - available

    larger = []
    root.find_larger(needed, larger)
    larger.sort()

    print(larger)
