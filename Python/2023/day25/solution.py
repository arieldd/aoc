import networkx as nx
import matplotlib.pyplot as plt
import sys

with open(sys.argv[1], "r") as file:

    data = [line.strip() for line in file.readlines()]

    G = nx.Graph()
    for node_str in data:
        sides = node_str.split(':')
        label = sides[0]
        edges = sides[1].split(' ')
        for e in edges:
            if e == "":
                continue
            G.add_edge(label, e)

    #G.remove_edges_from([("xhg","ljl"), ("ffj","lkm"), ("xjb", "vgs")])
    
    # parts = nx.connected_components(G)
    # print([len(c) for c in parts])

    # xhg-ljl ffj-lkm xjb-vgs
    nx.draw_networkx(G)
    plt.show()
