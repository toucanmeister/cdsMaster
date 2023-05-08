import numpy as np # math functionality
import matplotlib.pyplot as plt # plotting
import matplotlib.image as mpimg # loading images

fig = plt.figure(figsize=(8,7))

# show the first chart
ax1 = fig.add_subplot(1, 2, 1)
ax1.set_title("Skalarfeld kontinuierlich\n" + r'$f(x,y) = 3x^2 - 4y^2$')

# build and show the function f(x,y) = 3x^2 - 4y^2
delta = 0.025
x = y = np.arange(-3.0, 3.0, delta)
X, Y = np.meshgrid(x, y)
Z = 3 * np.power(X,2) - 4 * np.power(Y,2)
img1 = ax1.imshow(Z, extent=[-3,3,-3,3], vmin=-30, vmax=30, cmap='coolwarm')

# show the second chart
ax2 = fig.add_subplot(1, 2, 2)
ax2.set_title("Skalarfeld diskret")

# load the test image
circle_png = mpimg.imread('circle.png')
circle_bw = circle_png[:, :, 0]
img2 = ax2.imshow(circle_bw, cmap="gray", vmin="0", vmax="1")

# add colorbars
fig.colorbar(img1, ax=ax1, orientation='horizontal', pad=0.06)
fig.colorbar(img2, ax=ax2, orientation='horizontal', pad=0.06)


## 1a)
# df/dx = 6x
# df/dy = -8y
# grad f = [6x, -8y]

x = y = np.arange(-3.0, 3.01, 0.5)
X, Y = np.meshgrid(x, y)
dfdx = 6*X + 0*Y
dfdy = 0*X - 8*Y
ax1.quiver(X, Y, dfdx, dfdy)

## 1b)
# Kritischer Punkt heißt Gradient = 0
# grad f = [0, 0]    <==>   x = 0 und y = 0
ax1.scatter(0, 0, s=5, color='green')

## 1c)
# forward derivative:
# df/dx = f(x, y) - f(x-1, y)
# df/dy = f(x, y) - f(x, y-1)
x = y = np.arange(0, 100, 10)
X, Y = np.meshgrid(x, y)
dfdx = circle_bw[Y, X] - circle_bw[Y, X-1]
dfdy = circle_bw[Y, X] - circle_bw[Y+1, X] # plus hier weil Achse falschrum läuft
ax2.quiver(X, Y, dfdx, dfdy, scale=0.5)

# Always run show, to make sure everything is displayed.
plt.show()