import sys
import numpy as np
np.set_printoptions(threshold=np.inf, linewidth=np.nan)

dy = [0, 1, 0, -1]
dx = [1, 0, -1, 0]


def valid_pos(i, j, n):
    return i >= 0 and j >= 0 and i < n and j < n


def print_steps(garden, cycle):
    n = len(garden)

    for i in range(n):
        for j in range(n):
            if ((i, j) in cycle):
                print('O', sep='', end='')
            else:
                # print('.', sep='', end='')
                print(garden[i][j], sep='', end='')
        print()
    print()


def part1(garden, start, steps):
    current = {start: 1}

    for _ in range(steps):
        next = {}
        while (current):
            tile = current.pop()
            for d in range(4):
                move = (tile[0] + dy[d], tile[1] + dx[d])
                if (valid_pos(move[0], move[1], len(garden)) and garden[move[0]][move[1]] != '#'):
                    next[move] = 1

        print_steps(garden, next)
        current = next

    return len(current)


def part2(garden, start, steps):
    current = {start: 1}
    n = len(garden)

    for i in range(1, steps + 1):
        max = (i+1) * (i+1)
        next = {}
        for tile in current:
            for d in range(4):
                move = (tile[0] + dy[d], tile[1] + dx[d])
                if (garden[move[0] % n][move[1] % n] != '#'):
                    next[move] = 1

        current = next

        if i == 65 or i % 2 == 1 and i > steps - 10:
            print(i, len(current))

    print_steps(garden, current)

    return len(current)


with open(sys.argv[1], "r") as file:

    steps = int(sys.argv[2])
    data = [line.strip() for line in file.readlines()]

    garden = np.array([[c for c in line] for line in data])

    expanded = np.tile(garden, [4, 4])
    n = len(expanded)

    start = (int(n/2), int(n/2))

    # print(part1(garden, start, steps))

    part2(expanded, start, steps)

    # for l in expanded:
    #     for k in zip(*l):
    #         print(*k)
