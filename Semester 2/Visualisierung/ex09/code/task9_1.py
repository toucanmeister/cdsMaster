import numpy as np
import matplotlib.pyplot as plt

# Given is a vector field v(x,y) = (-y, x)^T.
# Utility to sample a given position [x,y]:
def v(pos):
    return np.array([-pos[1], pos[0]])

# Show the vector field using a quiver plot
X, Y = np.meshgrid(np.arange(-8, 8), np.arange(-8, 8))
U = -Y
V = X

fig = plt.figure(figsize=(6,6))
ax = fig.add_subplot()
ax.set_title(r'$v(x,y) =  (-y \quad x)^T$')
ax.quiver(X, Y, U, V)


####################
# Task 1           #
####################

start = np.array([1.0, 0.0])
dt = 0.7
tmax = 2*np.pi

## Euler
t = 0
point = start
while t <= tmax:
    lastpoint = point
    point = point + dt * v(point)
    t = t + dt
    ax.plot([lastpoint[0], point[0]], [lastpoint[1], point[1]], color='blue')

## Runge-Kutta 2. Ordnung
t = 0
point = start
while t <= tmax:
    lastpoint = point
    dv = dt*v(point)
    vmid = v(point + dv/2)
    point = point + dt * vmid
    t = t + dt
    ax.plot([lastpoint[0], point[0]], [lastpoint[1], point[1]], color='orange')

## Runge-Kutta 4. Ordnung
t = 0
point = start
while t <= tmax:
    lastpoint = point
    k1 = dt*v(point)
    k2 = dt*v(point + k1/2)
    k3 = dt*v(point + k2/2)
    k4 = dt*v(point + k3)
    point = point + k1/6 + k2/3 + k3/3 + k4/6
    t = t + dt
    ax.plot([lastpoint[0], point[0]], [lastpoint[1], point[1]], color='green')

plt.show()