class Elf:
    def __init__(self, pos) -> None:
        (self.i, self.j) = pos
        self.proposed = None


def elf_rounds(elves, total):
    round = 0 # 0 : N, 1 : S, 2: W, 3 E
    covered_ground(elves)

    while round < total:
        moves = {}
        for elf_pos in elves.keys():
            m = propose_move(elves, round, elf_pos)
            if m not in moves:
                moves[m] = []

            moves[m].append(elf_pos)
        
        for m, p in moves.items():
            if len(p) > 1:
                continue
            proposed = p[0]
            if proposed == m:
                continue
            elves[m] = 1
            del elves[p[0]]

        covered_ground(elves)

        round+=1

def no_move_round(elves):
    round = 0 # 0 : N, 1 : S, 2: W, 3 E
    covered_ground(elves)

    while True:
        moves = {}
        for elf_pos in elves.keys():
            m = propose_move(elves, round, elf_pos)
            if m in elves:
                continue

            if m not in moves:
                moves[m] = []

            moves[m].append(elf_pos)

        #print(round)
        #covered_ground(elves)        
        if not moves:
            return round + 1

        for m, p in moves.items():
            if len(p) > 1:
                continue
            
            proposed = p[0]
            if proposed == m:
                continue

            elves[m] = 1
            del elves[p[0]]

        round+=1


def propose_move(elves, round, pos):
    (i,j) = pos

    adj = {
        'NW' : (i-1,j-1), 'N' : (i-1, j), 'NE': (i-1, j+1),
        'SW' : (i+1, j-1), 'S': (i+1, j), 'SE': (i+1, j+1),
        'W' : (i,j-1), 'E':(i, j+1)
    }

    found = False

    for move in adj.values():
        if move in elves:
            found = True
            break

    if not found:
        return pos


    to_check = [
        [adj['N'], adj['NE'], adj['NW']],
        [adj['S'], adj['SE'], adj['SW']],
        [adj['W'], adj['NW'], adj['SW']],
        [adj['E'], adj['NE'], adj['SE']],
    ]

    start = round % 4
    for i in range(4):
        k = (start + i) % 4
        dirs = to_check[k]
        occupied = False
        for move in dirs:
            if move in elves:
                occupied = True
                break
        if not occupied:
            return dirs[0]

    return pos


def covered_ground(elves):
    [min_i, max_i, min_j, max_j] = find_limits(elves)

    free_space = 0
    for i in range(min_i, max_i + 1):
        for j in range(min_j, max_j + 1):
            text = '#'
            if (i,j) not in elves:
                free_space += 1
                text = '.'
            print(text, end='')
        print()

    print()
    return free_space


def find_limits(elves):
    limits = [None] * 4
    for (i,j) in elves.keys():
        if limits[0] is None or limits[0] > i:
            limits[0] = i
        if limits[1] is None or limits[1] < i:
            limits[1] = i
        if limits[2] is None or limits[2] > j:
            limits[2] = j
        if limits[3] is None or limits[3] < j:
            limits[3] = j

    return limits

with open("input.txt", "r") as file:
    
    data = [line.strip() for line in file.readlines()]

    i =0

    elves = {}
    for line in data:
        j = 0
        for char in line:
            if char == '#':
                elves[(i,j)] = 1
            j+=1
        i+=1


    # elf_rounds(elves, 10)
    # free = covered_ground(elves)

    # print(free)

    finish = no_move_round(elves)
    
    print(finish)


