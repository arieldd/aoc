import sys

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

def are_adjacent(c1, c2):
    diff = [abs(a_i - b_i) for a_i, b_i in zip(c1,c2)]
    return sum(diff) == 1

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

with open("input.txt", "r") as file:
    
    data = [line.strip() for line in file.readlines()]

    cubes = []
    for line in data:
        cubes.append(tuple([int(coord) for coord in line.split(',')]))

    #Part 1
    count =count_adjacents(cubes)
    print(count)

    #Part 2
    sys.setrecursionlimit(1500)
    count =count_adjacents_2(cubes)
    print(count)

    
            
               