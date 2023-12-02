import numpy as np

def rope_motions(data):
    
    h = (0,0)
    t = (0,0)
    positions = [t]
    for movement in data:
        dir = delta(movement[0])
        steps = int(movement[1])

        while steps:
            h = update_pos(h, dir)
            d = distance(h, t)
            if abs(d[0]) > 1 or abs(d[1]) > 1: #need to follow
                t = (t[0] + 1 * np.sign(d[0]), t[1] + 1* np.sign(d[1]))
            if t not in positions:
                positions.append(t)
            steps-=1

    return positions

def longer_rope_motions(data):
    
    rope = [(0,0) for _ in range(10)]
    t = rope[-1]
    positions = [t]

    for movement in data:
        dir = delta(movement[0])
        steps = int(movement[1])

        while steps:
            rope[0] = update_pos(rope[0], dir)

            for i in range(1,10):
                rope[i] = updated_tail(rope[i-1], rope[i])
                
            t = rope[-1]
            if t not in positions:
                positions.append(t)
            steps-=1

    return positions

def updated_tail(h, t):
    d = distance(h, t)
    if abs(d[0]) > 1 or abs(d[1]) > 1: #need to follow
        return (t[0] + 1 * np.sign(d[0]), t[1] + 1* np.sign(d[1]))

    return t


def distance(h, t) -> int:
    return (h[0] - t[0], h[1] - t[1])

def update_pos(old, delta) -> int:
    return (old[0]+ delta[0], old[1] + delta[1])

def delta(dir) -> int:
    if dir == 'R':
         return (0,1)
    if dir == 'L':
         return (0,-1)
    if dir == 'U':
        return (-1, 0)
    
    return (1,0)


with open("input.txt", "r") as file:

    data = [line.strip().split() for line in file.readlines()]

    #positions = rope_motions(data)

    #print(len(positions))

    long_positions = longer_rope_motions(data)
    print(len(long_positions))
