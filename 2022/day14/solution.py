import numpy as np

class Line:
    def __init__(self, p1, p2) -> None:
        self.p1 = p1
        self.p2 = p2

    def is_vertical(self):
        return self.p1[0] == self.p2[0]

    def __str__(self) -> str:
        return str(self.p1) +  ':' + str(self.p2)

def draw_cave(map):
    for row in map:
        for value in row:
            text = '.' if value == 0 else '#'
            if value == 2:
                text ='o'
            print(text, end=' ')
        print()
    print('\n')

def fill_map(map, lines, minX):
    for line in lines:
        if line.is_vertical():
            j = line.p1[0] - minX
            r1 = line.p2[1]
            r2 = line.p1[1]
            pos = range(min(r1, r2), max(r1,r2)+1)
            for i in pos:
                map[i][j] = 1
        else:
            i = line.p1[1]
            c1 = line.p2[0] - minX
            c2 = line.p1[0] - minX
            pos = range(min(c1, c2), max(c1,c2)+1)
            for j in pos:
                map[i][j] = 1
    pass

def simulate_sand(map, lines, minX):
    fill_map(map, lines, minX)
    count = 0
    while True:
        sand = [500 - minX, 0]
        map[sand[1]][sand[0]] = 2
        while True:
            rest = move_sand(map, sand)
            if rest == 1:
                count += 1
                break
            if rest == -1:
                return count
        if count % 400 == 0:
            draw_cave(map)

def move_sand(map, sand):
    x, y = sand

    if y + 1 >= len(map):
        return -1
    
    if map[y+1][x] == 0:
        sand[1] = y+1
        map[y+1][x] = 2
        map[y][x] = 0
        return 0
    
    if x - 1 < 0:
        return -1
    
    if map[y+1][x-1] == 0:
        sand[1] = y+1
        sand[0] = x-1
        map[y+1][x-1] = 2
        map[y][x] = 0
        return 0

    if x + 1 > len(map[0]):
        return -1
    
    if map[y+1][x+1] == 0:
        sand[1] = y+1
        sand[0] = x+1
        map[y+1][x+1] = 2
        map[y][x] = 0
        return 0
    
    return 1


def simulate_sand_floor(map, lines, minX):
    fill_map(map, lines, minX)
    count = 0
    j, i = 500 - minX, 0
    while True:
        sand = [j, i]
        map[i][j] = 2
        while True:
            rest = move_sand(map, sand)
            if map[i][j] == 2 or rest == -1:
                draw_cave(map)
                return count + 1

            if rest == 1:
                count += 1
                break

def move_sand_floor(map, sand):
    x, y = sand

    if y + 1 == len(map) - 1:
        return 1
    
    if map[y+1][x] == 0:
        sand[1] = y+1
        map[y+1][x] = 2
        map[y][x] = 0
        return 0
    
    if x - 1 < 0:
        return -1

    if map[y+1][x-1] == 0:
        sand[1] = y+1
        sand[0] = x-1
        map[y+1][x-1] = 2
        map[y][x] = 0
        return 0

    if x + 1 > len(map[0]):
        return -1
    
    if map[y+1][x+1] == 0:
        sand[1] = y+1
        sand[0] = x+1
        map[y+1][x+1] = 2
        map[y][x] = 0
        return 0
    
    return 1

with open("input.txt", "r") as file:
    
    data = [line.strip() for line in file.readlines()]

    list =[]

    for line in data:
        rocks = line.split(' -> ')
        for i in range(len(rocks) - 1):
            v1 = rocks[i].split(',')
            v2 = rocks[i+1].split(',')
            p1 = (int(v1[0]), int(v1[1]))
            p2 = (int(v2[0]), int(v2[1]))
            list.append(Line(p1, p2))


    minX, maxX, maxY = 500, 0, 0
    for line in list:
        minX = min(minX, line.p1[0])
        minX = min(minX, line.p2[0])
        maxX = max(maxX, line.p1[0])
        maxX = max(maxX, line.p2[0])
        maxY = max(maxY, line.p1[1])
        maxY = max(maxY, line.p2[1])

    # #Part 1
    # width = maxX - minX + 1
    # height = maxY + 1 

    # map = np.zeros((height, width))
    # draw_cave(map)
    # count = simulate_sand(map, list, minX)
    
    # print(count)

    #Part 2
    width = maxX - minX + 1
    minX -= width *4
    maxX += width *4
    height = maxY + 3 

    map = np.zeros((height, width * 9))
    list.append(Line((minX, height - 1), (maxX, height - 1)))
    count = simulate_sand_floor(map, list, minX)
    
    print(count)
