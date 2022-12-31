import numpy as np

dirs = {
    '>' : (0, 1), 'v' : (1, 0), '':(0,0), '^':(-1, 0), '<': (0, -1)
}

class Blizzard:
    def __init__(self, pos, dir) -> None:
        self.i = pos[0]
        self.j = pos[1]
        self.symbol = dir
        self.dir = dirs[dir]

    def __str__(self) -> str:
        return f' ({self.i}, {self.j}) : {self.symbol} {self.dir}'

    def next(self, map):
        [h, w] = map.shape

        ni, nj = (self.i + self.dir[0]) % h, (self.j + self.dir[1]) %  w

        if self.symbol in ['>', '<'] :
            while map[ni % h, nj % w] == -1:
                nj = (nj + self.dir[1]) % w
        else:
            while map[ni % h, nj % w] == -1:
                ni = (ni + self.dir[0]) % h

        return (ni, nj)


def avoid_blizzards(map, blizzards, current, end, steps, best):
    [h, w] = map.shape

    if best and steps > best:
        return None

    if current == end:
        return steps

    (i, j) = current

    next_blizzards = [b.next(map) for b in blizzards]
    
    for symbol in dirs.keys():
        (di, dj) = dirs[symbol]
        next = (i + di, j + dj)

        if next == end:
            return steps + 1

        if map[next[0] % h, next[1] % w] == -1:
            continue
        
        if next in next_blizzards:
            continue
        
        new_blizzards = [Blizzard(pos, blizzards[ix].symbol) for ix, pos in enumerate(next_blizzards)] 
        c = avoid_blizzards(map, new_blizzards, next, end, steps + 1, best)
        if c and (not best or c < best):
            print(c)
            best = c

    return best

def shortest_path(map, blizzards, start, end):
    [h, w] = map.shape


    minutes = 0
    positions = {start : 1} 
    while True:

        next_blizzards = []
        for b in blizzards:
            nb = b.next(map)
            (b.i, b.j) = nb
            next_blizzards.append(nb)

        next_blizzards = set(next_blizzards)
        should_check = list(positions.keys())
        for p in should_check:
            (i,j) = p
            for symbol in dirs.keys():
                (di, dj) = dirs[symbol]
                next = (i + di, j + dj)

                if next == end:
                    return minutes + 1

                if map[next[0] % h, next[1] % w] == -1:
                    continue
                
                if next in next_blizzards:
                    if next in positions:
                        del positions[next]
                    continue 

                positions[next] = 1  
        
        minutes += 1

    return minutes
    
def print_valley(map, blizzrds):
    [h,w] = map.shape 

    for i in range(h):
        for j in range(w):
            text = '.'
    pass


with open("input.txt", "r") as file:

    data = [line.strip() for line in file.readlines()]

    h, w = len(data), len(data[0])
    
    map = np.zeros([h,w])
    blizzards = []
    i = 0
    for row in data:
        j = 0
        for char in row:
            if char == '#':
                map[i,j] = -1
            elif char != '.':
                blizzards.append(Blizzard((i, j), char))

            j += 1
        i += 1

    start = (0,1)
    end = (h -1, w -2)
    first = shortest_path(map, blizzards, start, end)
    print(first)

    back = shortest_path(map, blizzards, end, start)

    again = shortest_path(map, blizzards, start, end)

    print(first + back + again)

