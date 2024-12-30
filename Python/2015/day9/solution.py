from collections import defaultdict, deque
from sys import maxsize

with open("input.txt") as f:
    s = f.read().strip()

# s = """
# London to Dublin = 464
# London to Belfast = 518
# Dublin to Belfast = 141
# """

lines = s.split("\n")

G = defaultdict(list)
for line in lines:
    if line == "":
        continue
    parts = line.split(" = ")
    dist = int(parts[-1])
    nodes = parts[0].split(" to ")
    G[nodes[0]].append((nodes[1], dist))
    G[nodes[1]].append((nodes[0], dist))


def explore(graph, path, dist, current, use_max=False):
    if len(path) == len(graph):
        return dist

    result = maxsize
    if use_max:
        result = 0
    if current in graph:
        for node, d in graph.get(current):
            if node in path:
                continue
            dn = explore(graph, path + [node], dist + d, node, use_max)
            if use_max:
                result = max(dn, result)
            else:
                result = min(dn, result)

    return result


part1 = maxsize
part2 = 0
for node in G:
    path = [node]
    dmin = explore(G, path, 0, node)
    dmax = explore(G, path, 0, node, True)
    part1 = min(part1, dmin)
    part2 = max(part2, dmax)

print(part1)
print(part2)
