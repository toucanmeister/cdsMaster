import numpy as np


# indices of the parents of the node
def parents(adj: np.ndarray, node_idx: int):
    return np.argwhere(adj[:,node_idx] == 1).flatten().tolist() 


# indices of children of the node
def children(adj: np.ndarray, node_idx: int):
    return np.argwhere(adj[node_idx,:] == 1).flatten().tolist() 


# checks if there is a path between two nodes
def path_exists(adj: np.ndarray, start_idx: int, end_idx: int) -> bool:
    discovered: [int] = []
    def dfs(node_idx: int) -> bool:
        if node_idx == end_idx: return True
        found = False
        for child in children(adj, node_idx):
            if child not in discovered:
                discovered.append(child)
                found = found or dfs(child)
        return found
    return dfs(start_idx)
        