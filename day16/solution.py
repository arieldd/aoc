import numpy as np
np.set_printoptions(threshold=np.inf, linewidth=np.nan)

from copy import deepcopy
from itertools import permutations 
class Valve:
    def __init__(self, name, flow, edges, index) -> None:
        self.name = name
        self.flow = flow
        self.open = False
        self.conn = edges
        self.index = index

    def contribution(self, minutes):
        return self.flow * minutes

    def __str__(self) -> str:
        return f'[{self.index}: {self.name}]  {self.flow} {self.open} {self.conn}'

    def __eq__(self, other) -> bool:
        if isinstance(other, Valve):
            return self.name == other.name
        return False

    def worth(self):
        return self.flow > 0 and not self.open

class Tunnels:
    def __init__(self, valves) -> None:
        self.valves = valves
        self.size = len(valves)
        self.adj = np.zeros([self.size, self.size], dtype=int)
        self.__fill_adj__()
        self.current = 'AA'
        self.flows = np.zeros(self.size, dtype=int)
        for _, v in self.valves.items():
            self.flows[v.index] = v.flow
        self.worth = [i for i, flow in enumerate(self.flows) if flow > 0]

    def __str__(self) -> str:
        return self.adj.__str__()

    def __fill_adj__(self):
        for _, v in self.valves.items():
            self.adj[v.index][v.index] = 0
            for name in v.conn:
                next = self.valves[name].index
                self.adj[v.index][next] = self.adj[next][v.index] = 1

        for i in range(self.size):
            for j in range(i, self.size):
                self.adj[i][j] = self.adj[j][i] = self.__min_dist__(i, j, [])

    def __min_dist__(self, i, j, visited):
        if i == j:
            return 0

        if self.adj[i][j]:
            return self.adj[i][j]
        
        neighbors = [ix for ix, d in enumerate(self.adj[i]) if d == 1]

        visited.append(i)

        dist = self.size
        for ix in neighbors:
            if ix in visited:
                continue
            d = self.__min_dist__(ix, j, visited)
            if d + 1 < dist:
                dist = d + 1

        return dist

    def find_best_path(self):
        possible_paths = permutations(self.worth)
        #self.generate_paths([], combinations)

        best = 0 
        best_path = []
        for p in possible_paths:
            val = self.eval_path(p)
            if val > best:
                best_path = p
                best = val

        return best, best_path        

    def __worth__(self):
        return  [v for v in valves.values() if v.worth()]

    def optimize_pressure(self):
        time_left = 30
        current = self.valves['AA']

        total_pressure = 0
        while time_left:
            
            best = None
            worth = self.__worth__()
            for valve in worth:
                d = self.adj[current.index][valve.index]

                if time_left - d - 1 < 0:
                    continue

                if not best:
                    best = valve
                    continue

                bd= self.adj[current.index][best.index]
                if (d >= bd and valve.flow > best.flow * (d - bd + 1)) \
                or (d < bd and valve.flow * (bd - d + 1) > best.flow):
                    best = valve

            if not best:
                return total_pressure

            valve = best
            print(valve.name)
            d = self.adj[current.index][valve.index]
            total_pressure += valve.flow * (time_left - d - 1)

            valve.open = True
            current = valve

            time_left -= (d + 1)
        
        return total_pressure

    def get_contributions(self, current, time_left):
        worth = self.__worth__()
       
        result = {}
        for valve in worth:
            d = self.adj[current.index][valve.index]
            
            time = time_left - d - 1

            if time < 0:
                continue

            contribution = valve.flow * time
            result[valve.name] = (contribution, d)

        return result

    def generate_paths(self, path, combinations):
        if len(path) == len(self.worth):
            combinations.append(deepcopy(path))
            return
        for i in self.worth:
            if i not in path:
                path.append(i)
                self.generate_paths(path, combinations)
                path.pop()

    def eval_path(self, path, start = 0):
        current = start
        time_left = 30

        contribution = 0
        for index in path:
            d = self.adj[current][index]
            time_left -= d + 1
            contribution += self.flows[index] * time_left
            current = index

        return contribution


with open("input.txt", "r") as file:
    
    data = [line.strip() for line in file.readlines()]

    valves = {}
    i = 0
    for line in data:
        parts = line.split(';')
        name = parts[0].split()[1]
        flow = int(parts[0].split('=')[1])
        tunnels_part = parts[1].split()
        tunnels = []
        for v in tunnels_part[4:]:
            tunnels.append(v.strip()[:2])

        valves[name] = Valve(name, flow, tunnels, i)
        i += 1

    #best = optimize_pressure_2(valves, 30)
    graph = Tunnels(valves)

    p = graph.optimize_pressure()
    print(p)

    print(graph.eval_path([7, 31, 46, 25, 17, 28, 4], 16))
    
    #best, path = graph.find_best_path()
    # print(path, best)

    #print(graph.adj[0])

#    class Path:
#     def __init__(self) -> None:
#         self.nodes = []
#         self.val = -1
        
#     def length(self):
#         return len(self.nodes)

#     def time_consumed(self):
#         l = 0
#         for (_, open) in self.nodes: 
#             l += 2 if open else 1
#         return l

#     def add(self, valve, will_open):
#         self.nodes.append((valve, will_open))

#     def pop(self):
#         self.nodes.pop()

#     def contains(self, name):
#         return any([x[0].name == name for x in self.nodes])

#     def released_pressure(self):
#         time = self.time_consumed()
#         l = self.length()
#         pressures = [0]
#         i = 0
#         p = 0
#         for _ in range(1, time):
#             if i < l:
#                 (v, o) = self.nodes[l - i - 1]
#                 if o:
#                     pressures.append(v.flow)
#                     time +=1
#             p += sum(pressures)

#         return p

#     def eval(self, time, end):
#         if not len(self.nodes):
#             return -1

#         time = time + self.time_consumed()
#         eval = 0
#         time -= 1

#         for (valve, should_open) in self.nodes:

#             mult = end - time
#             if should_open:
#                 eval += valve.flow * mult

#                 time -= 1

#             time -= 1

#         self.val = eval
#         return eval

#     def clone(self):
#         result = Path()
#         result.val = self.val
#         result.nodes = deepcopy(self.nodes)
#         return result

#     def print_path(self):
#         print('[', end='')
#         for (v, open) in reversed(self.nodes):
#             text = v.name if open else str.lower(v.name)
#             print(text, end=',')
#         print(f']') 

# def best_path(src, dst, current, valves, time, endTime, path):
#     if time + path.time_consumed() > endTime:
#         return None

#     bst_path = None
#     bst_val = -1
#     for adj in current.conn:    
#         if path.contains(adj):
#             continue

#         if adj == src.name:
#             v1 = path.eval(time, endTime)
            
#             if v1 > bst_val:
#                 bst_val = v1
#                 bst_path = path.clone()
#             continue

#         valve = valves[adj]
#         will_open = valve.worth() and valve.flow * path.length() > dst.flow

#         path.add(valve, will_open)
#         p = best_path(src, dst, valve, valves, time, endTime, path)
#         if p:
#             v2 = p.eval(time, endTime)

#             if v2 > bst_val:
#                 bst_val = v2
#                 bst_path = p.clone()    

#         path.pop()

#     return bst_path

# def open_along_path(valves, path):
#     if not len(path):
#         return

#     for (v, should_open) in path:
#         if should_open:
#             valves[v.name].open = True

# def find_best_path(valves, dst, current, path, time, end, visited):
#     if current.name == dst.name:
#         return deepcopy(path), eval_path(path, time, end)

#     if time >= end:
#         return [], -1

#     best_path = []
#     best_value = -1
#     for adj in current.conn:
#         if adj in visited:
#             continue
        
#         v = valves[adj]
#         visited.append(adj)

#         for should_open in reversed(range(2)):
#             if should_open and not v.worth(): #don't consider opening opened or 0 valve
#                 continue

#             path.append((v, should_open))
#             p, val = find_best_path(valves, dst, v, path, time + 1 + should_open, end, visited)
            
#             #print(current.name, '->', v.name, val)

#             if val > best_value or len(p) < len():
#                 best_path = p
#                 best_value = val

#             path.pop()

#             #Don't consider not turning on last valve
#             if adj == dst.name:
#                 break

#         visited.pop()

#     return best_path, best_value

# def print_path(path):
#     print('[', end='')
#     for (v, open) in path:
#         text = v.name if open else str.lower(v.name)
#         print(text, end=',')
#     print(']')   

# def eval_path(path, time, end):

#     if not len(path):
#         return -1

#     eval = 0
#     time -= 1
#     for (valve, should_open) in reversed(path):
        
#         mult = end - time
#         if should_open:
#             eval += valve.flow * mult

#             time -= 1

#         time -= 1

#     return eval

# def time_consumed(path):
#     time = 0
#     for (_, open) in path:
#         time += 2 if open else 1
#     return time

# def optimize_pressure(valves, endTime):
   
#     current = valves['AA']
#     pressure = 0
#     time = 1
#     while time <= endTime:
#         bst_path = []
#         bst_val = -1
#         bst_fitness = -1000
#         print(f'Next at time {time}')
#         for node in valves.values():
#             if node.worth():
#                 p, val = find_best_path(valves, node, current, [], time, endTime, [current.name])
#                 #Scale path fitness by time consumed
#                 fitness = val - sum(range(endTime - time - time_consumed(p), endTime - time + 1))
#                 if fitness > bst_fitness:
#                     bst_path = p
#                     bst_val = val
#                     bst_fitness = fitness

#                 print(val, fitness)
#                 print_path(p)

#         if not len(bst_path):
#             break

#         timeConsumed = time_consumed(bst_path)
#         print(current.name, '->', bst_path[-1][0].name, bst_val)
#         print_path(bst_path)

#         open_along_path(valves, bst_path)

#         pressure += bst_val
#         time += timeConsumed

#         current = bst_path[-1][0]

#     return pressure