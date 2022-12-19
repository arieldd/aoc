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
    cycle_heights = [[0, 0]]
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


        if rocks_fallen % 5 ==0:
            cycle_heights.append( [highest_rock, highest_rock - cycle_heights[-1][0]])

    #print_rocks(full, highest_rock + 4)
    print([delta[1] for delta in cycle_heights])
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
    i = len(map) - 1
    for row in reversed(map):
        print(i, end='\t')
        i -= 1
        for val in row:
            text = '#' if val else '.'
            if val == 2:
                text ='@'
            print(text, end=' ')
        print()

with open("input.txt", "r") as file:
    
    rock1 = [[1,1,1,1]]
    rock2 = [[0, 1, 0], [1, 1, 1], [0, 1, 0]]
    rock3 = [[0, 0, 1], [0, 0, 1], [1, 1, 1]]
    rock4 = [[1],[1],[1],[1]]
    rock5 = [[1,1], [1,1]]

    data = [dir for dir in file.read()]
    

    pushes = parse_dirs(data)
    #end = 100910
    end = 1000000000000
    #height = simulate_rocks(pushes, end)
    #print(height)

    # Test data 
    # rocks_remaining = end - 15
    # repeat = [11, 7, 8, 9, 6, 6, 6]
    # times = rocks_remaining // (len(repeat) * 5)
    # remainder = rocks_remaining % (len(repeat) * 5)
    # print(remainder, repeat[:remainder//5], sum(repeat[:remainder//5]))

    # print(25 + times * sum(repeat) )

    # Input data 
    prelim = [8, 4, 7, 9, 8, 8, 9, 4, 7, 12, 1, 7, 10, 12, 8, 10, 11, 9, 7, 7, 8, 7, 8, 3, 8, 11, 7, 7, 6, 5, 10, 7, 6, 7, 8, 8, 8, 10, 9, 9, 11, 6, 5, 7, 5, 6, 9, 7, 6, 7, 5, 10, 9, 6, 8, 3, 7]
    rocks_remaining = end - 5 * len(prelim)
    repeat = [7, 9, 6, 11, 11, 8, 9, 4, 10, 3, 8, 9, 6, 10, 9, 8, 9, 9, 9, 9, 6, 7, 8, 7, 6, 8, 10, 9, 8, 6, 7, 12, 11, 7, 7, 13, 9, 6, 12, 11, 6, 8, 9, 8, 8, 6, 6, 4, 9, 6, 10, 9, 11, 7, 8, 11, 7, 9, 5, 10, 3, 7, 6, 5, 8, 5, 6, 9, 7, 6, 10, 9, 7, 5, 10, 10, 8, 9, 5, 5, 9, 10, 7, 6, 9, 10, 8, 7, 7, 8, 12, 11, 7, 10, 6, 7, 4, 10, 10, 6, 11, 7, 7, 8, 8, 7, 7, 9, 7, 9, 6, 4, 9, 7, 7, 6, 10, 6, 8, 9, 11, 8, 10, 8, 11, 5, 9, 11, 8, 9, 7, 6, 9, 7, 6, 8, 6, 5, 9, 10, 6, 13, 8, 3, 9, 8, 6, 11, 10, 3, 6, 11, 11, 7, 9, 7, 6, 7, 7, 6, 7, 10, 11, 7, 9, 8, 5, 9, 11, 9, 9, 7, 11, 8, 9, 11, 8, 10, 11, 8, 9, 9, 7, 10, 9, 7, 7, 7, 7, 9, 8, 7, 11, 7, 5, 6, 6, 5, 10, 7, 11, 9, 7, 8, 10, 7, 9, 7, 6, 7, 6, 6, 7, 5, 9, 8, 9, 6, 8, 4, 7, 9, 11, 6, 11, 10, 9, 7, 7, 5, 11, 8, 7, 11, 10, 7, 6, 6, 6, 8, 13, 11, 5, 7, 10, 5, 7, 7, 8, 9, 7, 8, 13, 13, 8, 7, 7, 6, 5, 13, 5, 10, 8, 9, 5, 8, 6, 11, 7, 8, 7, 8, 6, 6, 6, 9, 5, 10, 6, 8, 6, 9, 6, 13, 3, 5, 7, 12, 8, 8, 9, 7, 8, 7, 6, 6, 5, 10, 8, 6, 13, 8, 5, 6, 6, 7, 7, 6, 7, 9, 8, 6, 7, 8, 8, 9, 7, 7, 13, 7, 8, 7, 7, 5, 7, 6, 9, 8, 8, 7, 11, 8, 6, 8, 6, 8, 3, 11, 7, 6, 11, 7, 8, 7, 13, 7, 5, 11, 9]
    times = rocks_remaining // (len(repeat) * 5)
    remainder = rocks_remaining % (len(repeat) * 5)

    print(sum(prelim) + times * sum(repeat) +  sum(repeat[:remainder //5]))