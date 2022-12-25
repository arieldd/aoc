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

    def __hash__(self) -> int:
        return self.name.__hash__()

    def worth(self):
        return self.flow > 0 and not self.open

class Tunnels:
    def __init__(self, valves) -> None:
        self.valves = valves
        self.size = len(valves)
        self.adj = np.zeros([self.size, self.size], dtype=int)
        self.__fill_adj__()
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

    def get_max_contributions(self, current_name, time, endTime):
        worth = [v for v in graph.valves.values() if v.flow > 0 and not v.open]
       
        time_left = endTime -  time

        current = graph.valves[current_name]
        result = [None, 0, 0]
        for valve in worth:
            d = self.adj[current.index][valve.index]
            
            time = time_left - d - 1

            if time < 0:
                continue

            contribution = valve.flow * time
            if contribution > result[1]:
                result = [valve, contribution, d]

        return result

    def eval_path(self, path, endTime, start = 0):
        if not path:
            return 0

        current = start
        time_left = endTime

        contribution = 0
        for index in path:
            d = self.adj[current][index]
            time_left -= d + 1
            contribution += self.flows[index] * time_left
            current = index

        return contribution

class Path:

    def __init__(self, next, time, path = None) -> None:
        self.valves = {}
        if path:
            self.valves = deepcopy(path.valves)

        self.valves.update({ time : next })

    def __len__(self):
        return len(self.valves)

    def __str__(self) -> str:
        return self.valves.__str__()

    def last_opened(self):
        time = 0
        for t in self.valves:
            if t > time:
                time = t

        return time, self.valves[time]

    def build_path(self, graph):
        result = []
        times = sorted(list(self.valves.keys()))
        for t in times[1:]:
            name = self.valves[t]
            valve = graph.valves[name]
            result.append(valve.index)

        return result
class DoublePath:

    def __init__(self, time, for_me, for_elephant, path = None) -> None:
        self.mine = {}
        self.elephant = {}
        if path:
            self.mine = deepcopy(path.mine)
            self.elephant = deepcopy(path.elephant)

        if for_me:
            self.mine.update( {time : for_me} )
        if for_elephant:
            self.elephant.update( {time : for_elephant} )

    def __len__(self):
        return len(self.mine) + len(self.elephant)

    def __str__(self) -> str:
        return self.mine.__str__() + '\n' + self.elephant.__str__() + '\n'

    def last_opened(self, for_elephant = False):
        dict = self.mine if not for_elephant else self.elephant

        time = 0
        for t in dict:
            if t > time:
                time = t

        return time, dict[time]
    
    def build_paths(self, graph):
        result = [[], []]
        dicts = [self.mine, self.elephant]
        for ix, d in enumerate(dicts):
            times = sorted(list(d.keys()))
            for t in times[1:]:
                name = d[t]
                valve = graph.valves[name]
                result[ix].append(valve.index)

        return result

def most_pressure(graph, endTime):

    start = 'AA'

    time = 1
    paths = [Path(start, time)]

    total_valves_to_open = len(graph.worth) + 1

    while time <= endTime:

        current_paths = list(paths)
        full = 0
        for p in current_paths:
            if len(p) == total_valves_to_open:
                full += 1
                continue

            t, last = p.last_opened()
            valve = graph.valves[last]
            openable = find_openable(graph, t, time, valve)
            for name in openable:
                if name in p.valves.values():
                    continue
                paths.append(Path(name, time, p))
        
        if full == len(paths):
            break #No more to explore

        time += 1

    best = 0
    start_index = graph.valves[start].index
    for p in paths:
        indices = p.build_path(graph)
        val = graph.eval_path(indices, endTime, start_index)
        if val > best:
            best = val

    return best

def most_double_pressure(graph, endTime):

    start = 'AA'

    time = 1
    paths = [DoublePath(time, start, start)]

    total_valves_to_open = len(graph.worth) + 2

    while time <= endTime:

        current_paths = list(paths)
        full = 0
        for p in current_paths:
            if len(p) >= total_valves_to_open:
                full += 1
                continue

            t_mine, last_mine = p.last_opened()
            t_elephant, last_elephant = p.last_opened(for_elephant=True)
            valve_mine = graph.valves[last_mine]
            valve_elephant = graph.valves[last_elephant]
            
            openable_mine = find_openable(graph, t_mine, time, valve_mine)
            openable_mine = [name for name in openable_mine if name not in p.mine.values() and name not in p.elephant.values()]
            
            openable_elephant = find_openable(graph, t_elephant, time, valve_elephant)
            openable_elephant = [name for name in openable_elephant if name not in p.mine.values() and name not in p.elephant.values()]

            for name_mine in openable_mine:
                paths.append(DoublePath(time, name_mine, None, p))
                        
            for name_elephant in openable_elephant:
                paths.append(DoublePath(time, None, name_elephant, p))

            for name_mine in openable_mine:
                for name_elephant in openable_elephant:
                    if name_elephant == name_mine:
                        continue
                    paths.append(DoublePath(time, name_mine, name_elephant, p))

            
        if full == len(paths):
            break #No more to explore

        time += 1

    best = 0
    start_index = graph.valves[start].index
    for p in paths:
        [ix_mine, ix_elephant ]= p.build_paths(graph)
        val_mine = graph.eval_path(ix_mine, endTime, start_index)
        val_elephant = graph.eval_path(ix_elephant, endTime, start_index)
        val = val_mine + val_elephant

        if val > best:
            #print(p, val_mine, val_elephant)
            best = val

    return best

def greedy_double_pressure(graph, endTime):

    total_valves_to_open = len(graph.worth)

    pos_mine = 'AA'
    pos_eleph = 'AA'

    time_mine = time_eleph = 0
    opened = 0
    pressure = 0
    while opened < total_valves_to_open:
        [vm,cm,dm] = graph.get_max_contributions(pos_mine, time_mine, endTime)
        [ve,ce,de] = graph.get_max_contributions(pos_eleph, time_eleph, endTime)

        if ce <= cm:
            print(vm.name, cm, dm)
            vm.open = True
            opened +=1 
            pos_mine = vm.name
            time_mine += (dm + 1)
            pressure += cm
    
            if opened == total_valves_to_open:
                break

            [ve,ce,de] = graph.get_max_contributions(pos_eleph, time_eleph, endTime)

            print(ve.name, ce, de)
            ve.open = True
            opened +=1  
            pos_eleph = ve.name
            time_eleph += (de + 1)
            pressure += ce
        
        else:
            print(ve.name, ce, de)
            ve.open = True
            opened +=1 
            pos_eleph = ve.name
            time_eleph += (de + 1)
            pressure += ce
        
            if opened == total_valves_to_open:
                break
            
            [vm,cm,dm] = graph.get_max_contributions(pos_mine, time_mine, endTime)

            print(vm.name, cm, dm)
            vm.open = True
            opened +=1 
            pos_mine = vm.name
            time_mine += (dm + 1)
            pressure += cm
    
    return pressure

def find_openable(graph, t1, t2, valve):
    d = t2 - t1

    result = []
    for v in graph.valves.values():
        if  v == valve:
            continue
        if v.worth() and graph.adj[valve.index, v.index] ==  d:
            result.append(v.name)

    return result

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

    # Part 1
    #pressure = most_pressure(graph, 30)
    pressure = most_double_pressure(graph, 26)
    #pressure = greedy_double_pressure(graph, 26)
    print(pressure)