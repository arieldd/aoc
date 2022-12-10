import numpy as np

def parse_signal(data):
    cycle = 1
    regx = 1

    main_cycles = [20, 60, 100, 140, 180, 220]
    signals = []

    for command in data:

        if cycle in main_cycles:
            signals.append(regx * cycle)
        
        if len(command) < 2:
            cycle += 1
            continue

        value = int(command[1])
        
        if cycle + 1 in main_cycles:
            signals.append(regx * (cycle + 1))
        
        regx += value
        cycle += 2

    return signals


def track_signal(data):
    regx = 1

    signals = [0]

    for command in data:

        signals.append(regx)
        
        if len(command) < 2:
            continue

        value = int(command[1])
        
        signals.append(regx)
        
        regx += value

    return signals


def render_image(data):
    screen =  np.zeros((6,40), int)

    regx = track_signal(data)
    print_screen(screen, regx)

def print_screen(screen, regx):

    for i in range(6):
        for j in range(1, 41):
            cycle = j + 40 * i
            pos =  regx[cycle]
            sprite = [pos -1, pos, pos +1]
            #print(sprite, cycle)
            if j-1 in sprite:
                print('#', end ='')
            else:
                print('.', end='')
        print('')


with open("input.txt", "r") as file:

    data = [line.strip().split() for line in file.readlines()]
    
    #signals = parse_signal(data)

    render_image(data)

    #print(sum(signals[:6]), signals)