from collections import deque
import numpy as np
from copy import deepcopy

class Valve:
    def __init__(self, name, flow, tunnels) -> None:
        self.name = name
        self.flow = flow
        self.open = False
        self.conn = tunnels
        self.distance = 0

    def contribution(self, minutes):
        return self.flow * minutes

    def __str__(self) -> str:
        return self.name + ', ' + str(self.flow) + ', ' + str(self.open) + ', ' + str(self.conn)

    def worth(self):
        return self.flow > 0 and not self.open

def open_along_path(valves, path):
    if not len(path):
        return

    for (v, should_open) in path:
        if should_open:
            valves[v.name].open = True

def find_best_path(valves, dst, current, path, time, end, visited):
    if current.name == dst.name:
        return deepcopy(path), eval_path(path, time, end)

    if time >= end:
        return [], -1

    best_path = []
    best_value = -1
    for adj in current.conn:
        if adj in visited:
            continue
        
        v = valves[adj]
        visited.append(adj)

        for should_open in reversed(range(2)):
            if should_open and not v.worth(): #don't consider opening opened or 0 valve
                continue

            path.append((v, should_open))
            p, val = find_best_path(valves, dst, v, path, time + 1 + should_open, end, visited)
            
            #print(current.name, '->', v.name, val)

            if val > best_value:
                best_path = p
                best_value = val

            path.pop()

            #Don't consider not turning on last valve
            if adj == dst.name:
                break

        visited.pop()

    return best_path, best_value

def print_path(path):
    print('[', end='')
    for (v, open) in path:
        text = v.name if open else str.lower(v.name)
        print(text, end=',')
    print(']')   

def eval_path(path, start, end):
    eval = 0
    time = end - start

    if not len(path):
        return -1

    for (valve, should_open) in path:
        if time < 0:
            return -1
        time -= 1

        if should_open:
            time -= 1

            if time < 0:
                return -1
            eval += valve.flow * time
            
    return eval

def time_consumed(path):
    time = 0
    for (_, open) in path:
        time += 2 if open else 1
    return time

def optimize_pressure(valves, endTime):
   
    current = valves['AA']
    pressure = 0
    time = 1
    while time <= endTime:
        bst_path = []
        bst_val = -1
        bst_fitness = -1000
        for node in valves.values():
            if node.worth():
                print(current.name, '->', node.name, time)
                p, val = find_best_path(valves, node, current, [], time, endTime, [current.name])
                #Scale path fitness by time consumed
                fitness = val - sum(range(endTime - time_consumed(p), endTime))
                if fitness > bst_fitness:
                    bst_path = p
                    bst_val = val
                    bst_fitness = fitness

        if not len(bst_path):
            break

        timeConsumed = time_consumed(bst_path)
        print(current.name, '->', bst_path[-1][0].name, timeConsumed)
        print_path(bst_path)

        open_along_path(valves, bst_path)

        pressure += bst_val
        time += timeConsumed

        current = bst_path[-1][0]

    return pressure

with open("test.txt", "r") as file:
    
    data = [line.strip() for line in file.readlines()]

    valves = {}
    for line in data:
        parts = line.split(';')
        name = parts[0].split()[1]
        flow = int(parts[0].split('=')[1])
        tunnels_part = parts[1].split()
        tunnels = []
        for v in tunnels_part[4:]:
            tunnels.append(v.strip()[:2])

        valves[name] = Valve(name, flow, tunnels)

    best = optimize_pressure(valves, 30)
    print(best)