import numpy as np

def follow_path(map, moves):
    (h, w) = map.shape

    [r, c] =[0, map[0].nonzero()[0][0]]
    
    pos ={(r,c) : 0}
    facing = 0
    for move in moves:
        if move < 0:
            next = 1 if move == -1 else -1
            facing = (facing + next) % 4
            continue
        
        for _ in range(move):
            if facing == 0: #Right
                j = c + 1
                if j >= w or not map[r][j]:
                    j = 0
                    while not map[r][j]:
                        j += 1
                if map[r][j] == 2:
                    break
                c = j

            if facing == 1: #Down
                i = r + 1
                if i >= h or not map[i][c]:
                    i = 0
                    while not map[i][c]:
                        i += 1
                if map[i][c] == 2:
                    break
                r = i

            if facing == 2: #Left
                j = c - 1
                if j < 0 or not map[r][j]:
                    j = w -1
                    while not map[r][j]:
                        j -= 1
                if map[r][j] == 2:
                    break
                c = j

            if facing == 3: # Up
                i = r - 1
                if i < 0 or not map[i][c]:
                    i = h - 1
                    while not map[i][c]:
                        i -= 1
                if map[i][c] == 2:
                    break
                r = i

            pos[(r,c)] = facing
    
    #draw_path(map, pos)
    return [r + 1, c + 1, facing]

def follow_cube(map, moves, sides):
    (h, w) = map.shape

    [r, c] =[0, map[0].nonzero()[0][0]]
    
    pos ={(r,c) : 0}
    facing = 0
    for move in moves:
        if move < 0:
            next = 1 if move == -1 else -1
            facing = (facing + next) % 4
            continue
        
        for _ in range(move):
            if facing == 0: #Right
                i = r
                j = c + 1
                if j >= w or not map[r][j]:
                    [i, j, facing] = next_in_cube(r, c, facing, current_side, sides)
                if map[i][j] == 2:
                    break
                r = i
                c = j

            if facing == 1: #Down
                i = r + 1
                if i >= h or not map[i][c]:
                    i = 0
                    while not map[i][c]:
                        i += 1
                if map[i][c] == 2:
                    break
                r = i

            if facing == 2: #Left
                j = c - 1
                if j < 0 or not map[r][j]:
                    j = w -1
                    while not map[r][j]:
                        j -= 1
                if map[r][j] == 2:
                    break
                c = j

            if facing == 3: # Up
                i = r - 1
                if i < 0 or not map[i][c]:
                    i = h - 1
                    while not map[i][c]:
                        i -= 1
                if map[i][c] == 2:
                    break
                r = i

            pos[(r,c)] = facing


input_transitions = [
        { 0 : [2, 0, 1], 1: [3, 1, 1], 2: [4, 0, -1], 3:[6, 1, -1] }, #Face 1
        { 0 : [5, 1, -1], 1: [3, 2, -1], 2: [1, 1, 1], 3:[6, 3, 1] }, #Face 2
        { 0 : [2, 3, -1], 1: [5, 1, 1], 2: [4, 1, -1], 3:[1, 3, 1] }, #Face 3
        { 0 : [5, 0, 1], 1: [6, 1, 1], 2: [1, 0, -1], 3:[3, 0, -1] }, #Face 4
        { 0 : [2, 2, 1], 1: [6, 2, -1], 2: [4, 2, 1], 3:[3, 3, 1] }, #Face 5
        { 0 : [5, 3, -1], 1: [2, 1, 1], 2: [1, 1, -1], 3:[4, 3, 1] }, #Face 6
    ]

def find_password(pos):
    return 1000 * pos[0] + 4 * pos[1] + pos[2]

def fill_map(text_map, width):
    height = len(text_map)
    map = np.zeros([height, width], dtype=int)

    for i in range(height):
        row = text_map[i]
        for j in range(width):
            if j >= len(row):
                break

            if row[j] == '.':
                map[i][j] = 1
            elif row[j] == '#':
                map[i][j] = 2

    return map

def parse_path(path):
    right = path.split('R')

    result = []
    for turn in right:
        move = turn.split('L')
        
        result.append(int(move[0]))
        for i in range(1, len(move)):
            result.append(-2)
            result.append(int(move[i]))
        result.append(-1)

    result.pop()
    return result

def fold_cube(map, size):
    change = size * size

    [h, w] = map.shape

    cube = np.zeros([h, w], dtype=int)

    count = 0
    faces = np.array([0,0,0,0,0,0])
    for i in range(h):
        face = np.where(faces < change)[0][0] + 1
        for j in range(w):
            if map[i,j]:
                cube[i,j] = face
                faces[face - 1] += 1
                count += 1
                if count % size == 0:
                    face += 1
    
    return cube

def find_sides(cube, size):
    [h, w] = cube.shape
    sides = np.zeros([6, size, size], dtype=tuple)

    faces = [0] * 6
    for i in range(h):
        for j in range(w):
            f = cube[i,j]
            if not f:
                continue

            count = faces[f -1]
            k = count // size
            l = count % size
            sides[f-1, k, l] = (i, j)
            faces[f-1] += 1

    print(sides)
    return sides

def draw_cube(cube):

    [h, w] = cube.shape

    for i in range(h):
        for j in range(w):
            text = ' '
            if cube[i,j]:
                text = str(cube[i,j])
            print(text, end='')
        print()

def next_in_cube(r, c, f, current, sides):
    
    size = sides.shape[1]
    switch = input_transitions[current][f]
    next_face = sides[switch[0] - 1]
    next_direction = switch[1]
    nr = r
    nc = c
    
    if f == next_direction: #Same direction
        if f % 2 == 0:
            nc = size - 1 - c
        else:
            nr = size - 1 - r
    elif abs(next_direction - f) % 2 == 0: #Opposite directions
        if f % 2 == 0:
            nr = size - 1 - r
        else:
            nc = size - 1 - c
    else:
        pass

    return (next_face[nr, nc], next_direction)

def draw_path(map, pos):
    [h,w] = map.shape

    for i in range(h):
        for j in range(w):
            text = ' '
            if (i,j) in pos:
                d = pos[(i,j)]
                if d == 0:
                    text = '>'
                elif d == 1:
                    text = 'v'
                elif d == 2:
                    text = '<'
                else:
                    text = '^'
            elif map[i,j] == 2:#
                text = '#'
            elif map[i,j] == 1:
                text = '.'

            print(text, end='')
        print()

with open("test.txt", "r") as file:
    
    data = [line for line in file.readlines()]

    index = data.index('\n')
    text_map = [row[:-1] for row in data[:index]]

    width = 0
    for row in text_map:
        l = len(row)
        if l > width:
            width = l

    map = fill_map(text_map, width)

    path = data[index +1 : ]
    moves = parse_path(path[0])

    [r, c, f] = follow_path(map, moves)

    print(r, c, f)

    print(find_password([r, c, f]))

    cube = fold_cube(map, 4)
    print(cube)
    sides = find_sides(cube, 4)
    [r, c, f] = follow_cube(map, moves, sides)