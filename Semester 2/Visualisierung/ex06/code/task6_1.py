import numpy as np # math functionality
import matplotlib.pyplot as plt # plotting

fig = plt.figure(figsize=(8,7))

ax = fig.add_subplot()
ax.set_title("Marching Squares")

# G holds some scalar values of a grid
G = np.array([[-1,-3,-3,-6,-9],
              [ 1, 1,-3,-6,-9],
              [ 2, 3, 3,-9,-9],
              [ 2, 3, 3,-9,-9],
              [ 9, 6, 6,-6,-9]])
G_rows = G.shape[0]
G_cols = G.shape[1]

# Build a grid 
x = np.linspace(0, G_cols-1, G_cols)
y = np.linspace(G_rows-1, 0, G_rows)
X, Y = np.meshgrid(x,y)
X /= G_cols-1
Y /= G_rows-1


####################
# Task 1           #
####################

def sign(x):
    return x / abs(x)

def edge_zero(p1, p2):
    v1 = G[p1[0],p1[1]]
    v2 = G[p2[0],p2[1]]
    if sign(v1) == sign(v2):
        return None
    else:
        zero = abs(v1) / (abs(v1) + abs(v2))
        if p1[0]+1 == p2[0]: # vertical edge
            return ((4-p1[0])-zero, p1[1])
        else:                # horizontal edge
            return (4-p2[0], p2[1]+zero-1)
    

for i in range(G.shape[0] - 1):
    for j in range(G.shape[1] - 1):
        edges = [((i  , j  ), (i+1, j  )),
                 ((i  , j  ), (i  , j+1)),
                 ((i+1, j  ), (i+1, j+1)),
                 ((i  , j+1), (i+1, j+1))]
        zeros = []
        for p1,p2 in edges:
            if edge_zero(p1, p2):
                zeros.append(edge_zero(p1, p2))
        if len(zeros) == 2:
            z1_x, z1_y = zeros[0]
            z2_x, z2_y = zeros[1]
            ax.plot([z1_y/4, z2_y/4], [z1_x/4, z2_x/4], color="red")















####################
# Plot the grid    #
####################
# set XY-ticks to resemble grid lines
ax.set_xticks(np.linspace(0, 1, G_cols))
ax.set_yticks(np.linspace(0, 1, G_rows))
ax.grid(True)
ax.set_axisbelow(True)
ax.get_xaxis().set_ticklabels([])
ax.get_yaxis().set_ticklabels([])

# annotate each gridpoint with the scalar value in G
ax.scatter(X, Y, s=200)
for i in range(G_rows):
    for j in range(G_cols):
        ax.annotate(G[i][j], xy=(X[i][j], Y[i][j]), ha='center', va='center', c='white')

# Always run show, to make sure everything is displayed.
plt.show()