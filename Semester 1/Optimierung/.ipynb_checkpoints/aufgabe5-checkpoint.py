# Aufgabe 5

# Lösung durch Algorithmus von Ford ausgeführt auf den Graphen mit negativ logarithmierten Gewichten.

import sys
import math


class Graph:
    def __init__(self, vertices, edges):
        self.vertices = vertices
        self.edges = [] # Each edge is of the form (from,to,weight)
        for from_idx, from_v in enumerate(vertices):
            for to_idx, to_v in enumerate(vertices):
                if not from_v == to_v: self.edges.append((from_v, to_v, edges[from_idx][to_idx]))

    def add_edges(self, a, b, c):
        self.edges.append((a, b, c))

    def bellman_ford(self, starting_node):
        potential = {vertex: sys.float_info.max for vertex in self.vertices} # Initialize all potentials as the largest float
        potential[starting_node] = 0
        pred = {} # Dict for storing predecessors
        for _ in range(len(self.vertices) - 1): # Convergence after at most (num_vertices - 1) iterations
            for a, b, c in self.edges:
                if potential[a] + c < potential[b]:
                    potential[b] = potential[a] + c
                    pred[b] = a
        for a, b, c in self.edges:
            if potential[a] + c < potential[b]:
                print(f'Arbitragemöglichkeit:')
                for n in self.vertices:
                    print(f'{pred[n]} -> {n}')
                return

vertices = ["EUR", "USD", "CAD", "GBP", "CHF"]
edges = [[1, 1.350, 1.351, 0.888, 1.433],
         [0.741, 1, 1.004, 0.657, 1.061],
         [0.732, 0.995, 1, 0.650, 1.049],
         [1.126, 1.521, 1.538, 1, 1.614],
         [0.698, 0.943, 0.953, 0.620, 1]]
log_edges = [list(map(lambda x: -math.log(x),line)) for line in edges] # Gewichte logarithmieren und negieren
g = Graph(vertices, log_edges)
g.bellman_ford("EUR")
# Ausgabe: 
## Arbitragemöglichkeit:
## USD -> EUR
## CAD -> USD
## CHF -> CAD
## CHF -> GBP
## GBP -> CHF

# Sehen einen negativen Zyklus CHF -> GBP -> CHF
# Probe: 0.620 * 1.614 = 1.00068 > 1