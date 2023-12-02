from collections import deque

class Node:
    def __init__(self, i, j, height, dist) -> None:
        self.i = i
        self.j = j
        self.height = height
        self.dist = dist
        self.visited = False

    def can_climb(self, other):
        return self.height - other.height >= -1

    def could_climb(self, other):
        return self.height - other.height <= 1

    def __str__(self) -> str:
        return str(f'{self.height}')

def least_steps(map):

    rows = len(map)
    columns = len(map[0])

    node_dict = {}

    inf = rows * columns
    for i in range(rows):
        for j in range(columns):
            height = map[i][j]
            node = Node(i, j, height, inf)
            if height == 69: #E
                height = 122
                node.height = height
                end = (i,j)
                node.dist = 0
            if height == 83:
                height = 97
                node.height = height
                #node.dist = 0 Part 1
            
            node.height -= 97
            node_dict[(i,j)] = node

    unvisited = [node for node in node_dict.values()]

    #least_steps_iter(map, node_dict, unvisited) Part 1
    #return node_dict[end].dist

    best_spot_iter(map, node_dict, unvisited)

    best = inf
    for node in node_dict.values():
        if node.height == 0 and node.dist < best:
            best = node.dist

    return best

def least_steps_iter(map, dict, unvisited):
    rows = len(map)
    columns = len(map[0])  

    delta = [(0,1), (1,0), (0, -1), (-1, 0)]

    while len(unvisited) > 0:
    
        unvisited.sort(key = lambda n: n.dist, reverse = True)

        current = unvisited.pop()
        current.visited = True

        for shift in delta:
            newPos = (current.i + shift[0], current.j + shift[1])

            if is_valid(newPos, rows, columns):
                next = dict[newPos]

                if next.visited == False and current.can_climb(next) and next.dist > current.dist + 1:
                    next.dist = current.dist + 1

def best_spot_iter(map, dict, unvisited):
    rows = len(map)
    columns = len(map[0])  

    delta = [(0,1), (1,0), (0, -1), (-1, 0)]

    while len(unvisited) > 0:
    
        unvisited.sort(key = lambda n: n.dist, reverse = True)

        current = unvisited.pop()
        current.visited = True

        for shift in delta:
            newPos = (current.i + shift[0], current.j + shift[1])

            if is_valid(newPos, rows, columns):
                next = dict[newPos]

                if next.visited == False and current.could_climb(next) and next.dist > current.dist + 1:
                    next.dist = current.dist + 1

def print_nodes(r, c, nodes):
    for i in range(r):
        for j in range(c):
            print(nodes[(i,j)], end=' ')
        print()
   
def print_matrix(matrix):
    for row in matrix:
        print(row)
    print()

def is_valid(pos, rows, columns):
    return (0 <= pos[0] < rows) and (0 <= pos[1] < columns) 

with open("input.txt", "r") as file:

    data = [line.strip() for line in file.readlines()]
    
    map = [[ord(c) for c in line] for line in data]

    result = least_steps(map)
    
    print(result)