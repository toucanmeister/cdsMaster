import numpy as np # math functionality
import matplotlib.pyplot as plt # plotting


## 1a)
def phi(r):
    return np.exp( -(r*r) )

points = np.array([[-2,-2], [2,0], [0,-1], [-1,2]])
values = np.array( [0.2,     0.6,   0.3,    0.5])

fig = plt.figure(figsize=(8,7))

## 1b)
ax = fig.add_subplot(projection='3d')
ax.scatter(points[:,0], points[:,1], values, color='red')

## 1c)
def f(x, w):
    result = 0
    for idx, point in enumerate(points):
        result += w[idx] * phi(np.linalg.norm(x - point))
    return result

n = values.shape[0]
coefficients = np.array( 
                [
                  [phi(np.linalg.norm(p_i - p_j)) for p_j in points] 
                  for p_i in points
                ] )
weights = np.linalg.solve(coefficients, values)

x = y = np.arange(-4, 4, 0.1)
X, Y = np.meshgrid(x, y)

interpolated_values = np.empty((len(x), len(y)))
for i,x_i in enumerate(x):
    for j,y_i in enumerate(y):
        interpolated_values[i,j] = f(np.array([x_i, y_i]), weights)

ax.plot_surface(Y, X, interpolated_values, cmap='viridis')

# Always run show, to make sure everything is displayed.
plt.show()