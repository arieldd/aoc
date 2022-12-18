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

class Path:
    def __init__(self) -> None:
        self.nodes = []
        self.val = -1
        
    def length(self):
        return len(self.nodes)

    def time_consumed(self):
        l = 0
        for (_, open) in self.nodes: 
            l += 2 if open else 1
        return l

    def add(self, valve, will_open):
        self.nodes.append((valve, will_open))

    def pop(self):
        self.nodes.pop()

    def contains(self, name):
        return any([x[0].name == name for x in self.nodes])

    def released_pressure(self):
        time = self.time_consumed()
        l = self.length()
        pressures = [0]
        i = 0
        p = 0
        for _ in range(1, time):
            if i < l:
                (v, o) = self.nodes[l - i - 1]
                if o:
                    pressures.append(v.flow)
                    time +=1
            p += sum(pressures)

        return p

    def eval(self, time, end):
        if not len(self.nodes):
            return -1

        time = time + self.time_consumed()
        eval = 0
        time -= 1

        for (valve, should_open) in self.nodes:

            mult = end - time
            if should_open:
                eval += valve.flow * mult

                time -= 1

            time -= 1

        self.val = eval
        return eval

    def clone(self):
        result = Path()
        result.val = self.val
        result.nodes = deepcopy(self.nodes)
        return result

    def print_path(self):
        print('[', end='')
        for (v, open) in reversed(self.nodes):
            text = v.name if open else str.lower(v.name)
            print(text, end=',')
        print(f']') 

def best_path(src, dst, current, valves, time, endTime, path):
    if time + path.time_consumed() > endTime:
        return None

    bst_path = None
    bst_val = -1
    for adj in current.conn:    
        if path.contains(adj):
            continue

        if adj == src.name:
            v1 = path.eval(time, endTime)
            
            if v1 > bst_val:
                bst_val = v1
                bst_path = path.clone()
            continue

        valve = valves[adj]
        will_open = valve.worth() and valve.flow * path.length() > dst.flow

        path.add(valve, will_open)
        p = best_path(src, dst, valve, valves, time, endTime, path)
        if p:
            v2 = p.eval(time, endTime)

            if v2 > bst_val:
                bst_val = v2
                bst_path = p.clone()    

        path.pop()

    return bst_path

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

            if val > best_value or len(p) < len():
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

def eval_path(path, time, end):

    if not len(path):
        return -1

    eval = 0
    time -= 1
    for (valve, should_open) in reversed(path):
        
        mult = end - time
        if should_open:
            eval += valve.flow * mult

            time -= 1

        time -= 1

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
        print(f'Next at time {time}')
        for node in valves.values():
            if node.worth():
                p, val = find_best_path(valves, node, current, [], time, endTime, [current.name])
                #Scale path fitness by time consumed
                fitness = val - sum(range(endTime - time - time_consumed(p), endTime - time + 1))
                if fitness > bst_fitness:
                    bst_path = p
                    bst_val = val
                    bst_fitness = fitness

                print(val, fitness)
                print_path(p)

        if not len(bst_path):
            break

        timeConsumed = time_consumed(bst_path)
        print(current.name, '->', bst_path[-1][0].name, bst_val)
        print_path(bst_path)

        open_along_path(valves, bst_path)

        pressure += bst_val
        time += timeConsumed

        current = bst_path[-1][0]

    return pressure

def optimize_pressure_2(valves, endTime):
   
    visited = list(valves.values())

    current = valves['AA']
    pressure = 0
    time = 1
    while time <= endTime:
        
        if node.worth():
            path = Path()
            path.add(node, 1)
            print(f'to {node.name}: ')
            p = best_path(current, node, node, valves, time, endTime, path)


            if not p:
                break

            timeConsumed = p.time_consumed()
            print(current.name, '->', p.nodes[0][0].name)
            p.print_path()

            open_along_path(valves, p.nodes)

            pressure += p.val
            time += timeConsumed

            current = p.nodes[0][0]

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

    best = optimize_pressure_2(valves, 30)
    print(best)