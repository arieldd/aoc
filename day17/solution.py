import numpy as np   

class Rock():
    def __init__(self, shape, pos) -> None:
        self.shape = shape
        self.height = len(shape)
        self.width = len(shape[0])
        self.origin = pos #upper left corner
    
    def occupied(self):
        occupied = []
        for i in range(self.height):
            for j in range(self.width):
                if self.shape[i][j]:
                    pos = (self.origin[0] - i, self.origin[1] + j)
                    occupied.append(pos)

        return occupied

def get_next_rock(cycle):
    rocks = [rock1, rock2, rock3, rock4, rock5]

    return rocks[cycle % 5]

def move_rock(rock, move, full):
    old = rock.origin
    rock.origin = (old[0] + move[0], old[1] + move[1])

    for pos in rock.occupied():
        if pos in full or pos[0] == 0 or pos[1] >= 7 or pos[1] < 0:
            rock.origin = old
            return move[0]

    return 1

def parse_dirs(symbols):
    dirs = []
    for s in symbols:
        if s == '>':
            val = 1
        elif s == '<':
            val = -1
        else:
            continue
        dirs.append(val)  

    return dirs

def simulate_rocks(pushes, end):
    push_length = len(pushes)

    rocks_fallen = 0
    
    highest_rock = 0
    full = {}

    steps = 0
    while rocks_fallen < end:
        shape = get_next_rock(rocks_fallen)

        rock = Rock(shape, (highest_rock + 3 + len(shape), 2))

        while True:
            move = (-1, 0) if steps % 2 else (0, pushes[steps//2 % push_length])
        
            result = move_rock(rock, move, full)

            steps += 1

            if steps > 2*push_length:
                steps %= 2*push_length
        
            if result < 0:
                break

        rocks_fallen += 1
        for pos in rock.occupied():
            if pos[0] > highest_rock:
                highest_rock = pos[0]
            if pos not in full:
                full[pos] = rocks_fallen

    print_rocks(full, highest_rock + 4)
    return highest_rock

def find_repeat(rocks_fallen, steps, push_length, highest_rock, full):
    if rocks_fallen % 5 != 0:
        return False

    if steps//2 % push_length != 0:
        return False

    occupied = 0
    for k in full.keys():
        if k[0] == highest_rock:
            ocuppied +=1

    print(occupied)
    if occupied > 5:
        return True

    return False

def print_rocks(occupied, height, rock=None):
    map = np.zeros([height,7])


    for k in occupied.keys():
        map[k[0]][k[1]] = 1

    if rock:
        for k in rock.occupied():
            map[k[0]][k[1]] = 2

    print()
    for row in reversed(map):
        for val in row:
            text = '#' if val else '.'
            if val == 2:
                text ='@'
            print(text, end=' ')
        print()

with open("test.txt", "r") as file:
    
    rock1 = [[1,1,1,1]]
    rock2 = [[0, 1, 0], [1, 1, 1], [0, 1, 0]]
    rock3 = [[0, 0, 1], [0, 0, 1], [1, 1, 1]]
    rock4 = [[1],[1],[1],[1]]
    rock5 = [[1,1], [1,1]]

    data = [dir for dir in file.read()]
    

    pushes = parse_dirs(data)
    end = 2050
    #end = 1000000000000
    height = simulate_rocks(pushes, end)
    print(height)

