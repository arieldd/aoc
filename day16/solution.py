import numpy as np
np.set_printoptions(threshold=np.inf, linewidth=np.nan)

from copy import deepcopy

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

        self.pressure = 0

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
                paths.append(Path(name, time + 1, p))
        
        if full == len(paths):
            break #No more to explore

        time += 1

    best , best_path = 0, None
    start_index = graph.valves[start].index
    for p in paths:
        indices = p.build_path(graph)
        val = graph.eval_path(indices, endTime, start_index)
        if val > best:
            best = val
            best_path = p

    return best, best_path

def most_pressure_2(graph, endTime):

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
                paths.append(Path(name, time + 1, p))
        
        if full == len(paths):
            break #No more to explore

        time += 1

    start_index = graph.valves[start].index

    for p in paths:
        indices = p.build_path(graph)
        val = graph.eval_path(indices, endTime, start_index)
        p.pressure = val

    print('Starting scan ....')

    paths = [p for p in paths if p.pressure > 1000]
    paths.sort(key=lambda x: x.pressure, reverse=True)
    best_sum, best_pair = 0, [None, None]

    total_paths = len(paths)

    print(total_paths)
    for ix, p1 in enumerate(paths):
        
        valve_set = set(list(p1.valves.values())[1:])
        if ix % 1000 == 0:
            print(ix, best_sum)

        possible = [p for p in paths if valve_set.isdisjoint(p.valves.values())]
        for p2 in possible:
            sum = p1.pressure + p2.pressure
            if sum > best_sum:
                best_sum = sum
                best_pair = [p1, p2]

    return best_sum, best_pair

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
    pressure, _ = most_pressure(graph, 30)
    print(pressure)

    # Part 2
    pressure, [p1, p2] = most_pressure_2(graph, 26)
    print(graph.worth)
    print(p1.pressure, p1, p2.pressure, p2)
    print(pressure)

    # p1 = [7, 46, 25, 17, 31, 4, 28]
    # p2 = [47, 29, 38, 22, 41, 39]
    # print(graph.eval_path(p1, 26, 16))
    # print(graph.eval_path(p2, 26, 16))

    # current = 16
    # for ix in p2:
    #     print(graph.adj[current, ix])
    #     current = ix

    # #print(eval('23 * 8 + 20 * 11 + 17 * 23 + 21 * 14 + 8 * 18 + 5 * 12 + 2 * 9'))
    # print(eval('23 * 4 + 20 * 25+ 16 * 20 + 13 * 22 + 6 * 3'))