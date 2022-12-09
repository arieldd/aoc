from copy import deepcopy

class Score:
    def __init__(self):
        self.left = 0
        self.up = 0
        self.right = 0
        self.down = 0

    def eval(self):
        return self.left * self.up * self.right * self.down

    def __str__(self) -> str:
        return str([self.left, self.up, self.right, self.down])

def find_visible(grid):
    rows = len(grid)
    columns = len(grid[0])

    left = [[0] * columns for _ in range(rows)]

    right = deepcopy(left)
    up = deepcopy(left)
    down = deepcopy(left)

    visible = deepcopy(left)

    for i in range(rows):
        left[i][0] = grid[i][0]
        right[i][columns -1] = grid[i][columns-1]
        visible[i][0] = 1
        visible[i][columns -1]= 1


    for j in range(columns):
        up[0][j] = grid[0][j]
        down[rows -1][j] = grid[rows -1][j]
        visible[0][j] = 1
        visible[rows -1][j] = 1

    for i in range(1, rows- 1):
        for j in range(1, columns -1):
            tree = grid[i][j]
            
            if left[i][j-1] < tree:
                left[i][j] = tree
                visible[i][j] = 1
            
            else:
                left[i][j] =left[i][j-1]

            if up[i-1][j] < tree:
                up[i][j] = tree
                visible[i][j] = 1
            
            else:
                up[i][j] =up[i-1][j]
    
    for i in reversed(range(1, rows-1)):
        for j in reversed(range(1, columns-1)):
            tree = grid[i][j]
            
            if right[i][j+1] < tree:
                right[i][j] = tree
                visible[i][j] = 1
            
            else:
                right[i][j] =right[i][j+1]

            if down[i+1][j] < tree:
                down[i][j] = tree
                visible[i][j] = 1
            
            else:
                down[i][j] = down[i+1][j]

    return visible


def find_score(grid):
    rows = len(grid)
    columns = len(grid[0])

    score = []

    for i in range(rows):
        score.append([])
        for j in range(columns):
            score[i].append(Score())

    for i in range(rows):
        for j in range(columns):
            tree = grid[i][j]

            if j > 0:
                for k in reversed(range(j)): 
                    score[i][j].left += 1 
                    if grid[i][k] >= tree:
                        break
            if i > 0:
                for k in reversed(range(i)): 
                    score[i][j].up += 1 
                    if grid[k][j] >= tree:
                        break
            
            if j < columns - 1:
                for k in range(j+1, columns): 
                    score[i][j].right += 1 
                    if grid[i][k] >= tree:
                        break
            if i < rows - 1:
                for k in range(i+1, rows): 
                    score[i][j].down += 1 
                    if grid[k][j] >= tree:
                        break
    
    
    return score  

def print_matrix(m):
    for row in m:
        print(row)
    print("\n")
    

with open("input.txt", "r") as file:

    data = [line.strip() for line in file.readlines()]

    grid = []
    for row in data:
        newRow = [int(tree) for tree in row]
        grid.append(newRow)

    #visible = find_visible(grid)

    #total = [sum(row) for row in visible]

    #print_matrix(visible)

    #print(total, sum(total))

    score = find_score(grid)

    values = []

    for row in score:
        for val in row:
            values.append(val.eval())
            print(val, end = ' ')
        print('\n')

    values.sort()

    print(values)

