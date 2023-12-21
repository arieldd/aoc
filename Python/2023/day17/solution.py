import networkx as nx
import sys

dy = [0, 1, 0, -1]
dx = [1, 0, -1, 0]


def valid_pos(i, j, n):
    return i >= 0 and j >= 0 and i < n and j < n


def build_graph_part1(heat_loss, src, n):
    G = nx.DiGraph()

    n = len(heat_loss)
    max_steps = 3

    G.add_edge(src, (0, 1, 0, 1), weight=heat_loss[0][1])
    G.add_edge(src, (1, 0, 1, 1), weight=heat_loss[1][0])

    for i in range(n):
        for j in range(n):
            for s in range(1, max_steps + 1):
                for d in range(4):
                    # A node must exist in the opposite direction to reach here with steps s
                    d_opposite = (d + 2) % 4
                    if valid_pos(i + dy[d_opposite] * s, j + dx[d_opposite] * s, n):
                        ni = i + dy[d]
                        nj = j + dx[d]
                        if valid_pos(ni, nj, n) and s < max_steps:
                            G.add_edge(
                                (i, j, d, s), (ni, nj, d, s+1), weight=heat_loss[ni][nj])
                        for nd in [(d+1) % 4, (d-1) % 4]:
                            ni = i + dy[nd]
                            nj = j + dx[nd]
                            if valid_pos(ni, nj, n):
                                G.add_edge(
                                    (i, j, d, s), (ni, nj, nd, 1), weight=heat_loss[ni][nj])

    return G


def build_graph_part2(heat_loss, src, n):
    G = nx.DiGraph()

    n = len(heat_loss)
    max_steps = 10

    G.add_edge(src, (0, 4, 0, 4), weight=sum(
        [heat_loss[dy[0] * k][dx[0] * k] for k in range(1, 5)]))
    G.add_edge(src, (4, 0, 1, 4), weight=sum(
        [heat_loss[dy[1] * k][dx[1] * k] for k in range(1, 5)]))

    for i in range(n):
        for j in range(n):
            for s in range(4, max_steps + 1):
                for d in range(4):
                    # A node must exist in the opposite direction to reach here with steps s
                    d_opposite = (d + 2) % 4
                    if valid_pos(i + dy[d_opposite] * s, j + dx[d_opposite] * s, n):
                        ni = i + dy[d]
                        nj = j + dx[d]
                        if valid_pos(ni, nj, n) and s < max_steps:
                            G.add_edge(
                                (i, j, d, s), (ni, nj, d, s+1), weight=heat_loss[ni][nj])
                        for nd in [(d+1) % 4, (d-1) % 4]:
                            ni = i + dy[nd] * 4
                            nj = j + dx[nd] * 4
                            if valid_pos(ni, nj, n):
                                total_loss = sum(
                                    [heat_loss[i + dy[nd] * k][j + dx[nd] * k] for k in range(1, 5)])

                                G.add_edge(
                                    (i, j, d, s), (ni, nj, nd, 4), weight=total_loss)
    return G


with open(sys.argv[1], "r") as file:

    data = [line.strip() for line in file.readlines()]

    heat_loss = [[int(c) for c in line] for line in data]
    n = len(heat_loss)
    src = (0, 0, 0, 0)

    G1 = build_graph_part1(heat_loss, src, n)
    G2 = build_graph_part2(heat_loss, src, n)

    paths = []
    ultrapaths = []

    for d in [0, 1]:
        for s1 in range(1, 4):
            end = (n-1, n-1, d, s1)
            if nx.has_path(G1, src, end):
                paths.append(nx.single_source_dijkstra(G1, src, end)[0])

        for s2 in range(4, 11):
            end = (n-1, n-1, d, s2)
            if nx.has_path(G2, src, end):
                ultrapaths.append(nx.single_source_dijkstra(G2, src, end)[0])

    print(paths, min(paths))
    print(ultrapaths, min(ultrapaths))
