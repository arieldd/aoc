import numpy as np

def count_adjacents(cubes):
    adj = {}
    size = len(cubes)

    for i in range(size):
        adj[i] = 0

    for ix, cube in enumerate(cubes):
        for j in range(ix + 1, size):
            if are_adjacent(cube, cubes[j]):
                adj[ix] += 1
                adj[j] += 1


    exposed = 0
    for k,v in adj.items():
        exposed += 6 - v

    return exposed

def count_adjacents_2(cubes):
    outside = {(0,0,0): 1}
    inside = {}

    exposed = 0 
    for cube in cubes:
        sides = []
        [x, y, z] = cube
        sides.append((x+1, y, z))
        sides.append((x-1, y, z))
        sides.append((x, y+1, z))
        sides.append((x, y-1, z))
        sides.append((x, y, z+1))
        sides.append((x, y, z-1))
        for s in sides:
            if s in cubes:
                continue

            explored = {}
            r = is_exterior_surface(s, cubes, outside, inside, explored)
            if r == 1:
                outside.update(explored) 
                exposed += 1
            
            elif r <= 0:
                inside.update(explored)

    #print(outside)
    #print(inside)

    return exposed

def can_escape(coord, cube, max, min):
    [x,y,z] = coord

    if coord in cubes:
        return False

    #Escape on X
    blocked = False
    for i in range(x+1, max[0] + 1):
        if [i, y, z] in cube:
            blocked = True
            break

    if not blocked:
        return True

    blocked = False
    for i in reversed(range(min[0], x)):
        if [i, y, z] in cube:
            blocked = True
            break
    
    if not blocked:
        return True
        
    #Escape on Y
    blocked = False
    for j in range(y+1, max[1] + 1):
        if [x, j, z] in cube:
            blocked = True
            break

    if not blocked:
        return True

    blocked = False
    for j in reversed(range(min[1], y)):
        if [x, j, z] in cube:
            blocked = True
            break
    
    if not blocked:
        return True

    #Escape on Z
    blocked = False
    for k in range(z+1, max[2] + 1):
        if [x, y, k] in cube:
            blocked = True
            break

    if not blocked:
        return True

    blocked = False
    for k in reversed(range(min[2], z)):
        if [x, y, k] in cube:
            blocked = True
            break
    
    return not blocked

def are_adjacent(c1, c2):
    diff = [abs(a_i - b_i) for a_i, b_i in zip(c1,c2)]
    return sum(diff) == 1

def count_air_pockets(adj):
    pockets = 0

    free_pos = {}
    coord_list = [item for sublist in adj.values() for item in sublist]

    for [x,y,z] in coord_list:
        coord = (x,y,z)
        if coord not in free_pos:
            free_pos[coord] = 0
        free_pos[coord] += 1

    for v in free_pos.values():
        if v ==6:
            pockets +=1

    return pockets

def is_exterior_surface(current, cubes, outside, inside, explored):
    
    explored[current] = 0
    
    (x,y,z) = current

    if current in outside or x < 0 or y < 0 or z < 0:
        return 1
    
    if current in inside:
        return -1

    pos = [(x - 1, y, z), (x, y-1, z), (x, y, z-1), (x+1, y, z), (x, y+1, z), (x, y, z+1)]
    for next in pos:

        if next in explored or next in cubes:
            continue

        is_knwon = is_exterior_surface(next, cubes, outside, inside, explored)
        
        if is_knwon == 1:
            outside[current] = 1
            return 1

        if is_knwon == -1:
            inside[current] = 1
            return -1

    return 0

def build_droplet(cubes):

    max = [0, 0, 0]
    for [x, y, z] in cubes:
        if x > max[0]:
            max[0] = x
        if y > max[1]:
            max[1] = y
        if z > max[2]:
            max[2] = z

    grid = np.zeros([c + 1 for c in max])

    for [x, y, z] in cubes:
        grid[x][y][z] = 1

    print(grid)


import sys
with open("input.txt", "r") as file:
    
    data = [line.strip() for line in file.readlines()]

    cubes = []
    for line in data:
        cubes.append(tuple([int(coord) for coord in line.split(',')]))

    #Part 1
    count =count_adjacents(cubes)
    print(count)

    #Part 2
    # airp = count_air_pockets(adj)
    # print(airp)

    # print(count - (6 * airp))
    sys.setrecursionlimit(1500)
    count =count_adjacents_2(cubes)
    print(count)

    
            
               