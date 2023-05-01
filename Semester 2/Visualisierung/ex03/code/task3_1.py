import numpy as np # math
import matplotlib.pyplot as plt # plotting
import matplotlib.image as mpimg # loading images
import pandas # handling csv data

fig, ax = plt.subplots(1, 2)
for i in range(2):
    ax[i].get_xaxis().set_visible(False)
    ax[i].get_yaxis().set_visible(False)


# loading a table with pandas
data_frame = pandas.read_csv("spread_data.csv")


# a)
print(data_frame)
img = plt.imread('germany.png')
ax[0].imshow(img)
ax[1].imshow(img)


# b)
infected = data_frame['Anzahl Inf. 7T']
xs = data_frame['x_pos']
ys = data_frame['y_pos']
ax[0].scatter(xs, ys, s=(infected*0.05))


# c)
# I = a*r^(1.4)
# (I / a) = r^(1.4)
# (I / a)^(1 / 1.4) = r
# r is the radius, we give scatter the area
# -> need to give pi*r^2
def steven_radius(i: float) -> float:
    return (i / 300)**(1.0 / 1.4)
ax[1].scatter(xs, ys, s=3.14*(steven_radius(infected)**2))


# d)
infected_per_hundred_k = (infected / data_frame['Einwohner']) * 100000
scatterplot = ax[1].scatter(xs, ys, s=3.14*(steven_radius(infected)**2), cmap='magma', c=infected_per_hundred_k, vmin=0, vmax=250)
fig.colorbar(scatterplot, ax=ax[1], orientation='horizontal', pad=0.02)
 

# Run show, to make sure everything is displayed.
plt.show()