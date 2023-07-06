import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# we only need one subplot
fig, ax = plt.subplots(figsize=(13,8))

# fix axis limits to prevent jumpy animations

# positions for 2 points in time
#               point 1     point 2 
population = pd.read_csv('population_filtered.csv').drop(columns='name').to_numpy()
gdp = pd.read_csv('gdp_filtered.csv').drop(columns='name').to_numpy()
child_mortality = pd.read_csv('child_mortality_filtered.csv').drop(columns='name').to_numpy()
life_expectancy = pd.read_csv('life_expectancy_filtered.csv').drop(columns='name').to_numpy()

num_countries = gdp.shape[0]
num_years = gdp.shape[1]

gdp_life = np.empty((num_years, num_countries, 2))
gdp_life[:,:,0] = gdp.T
gdp_life[:,:,1] = life_expectancy.T

population = population.T / 5E5
child_mortality = child_mortality.T

# create an initial scatterplot of first time point
ax.set(xlim=(gdp.min(), gdp.max()/2), ylim=(life_expectancy.min(), life_expectancy.max()))
scatterplot = ax.scatter(gdp_life[0,:,0], gdp_life[0,:,1], s=population[0], c=child_mortality[0], vmin=0.0, vmax=200.0)

# animation parameters
time_res = 3 # interpolation steps between keyframes
time_speed = 0.01 # seconds between each keyframe 
time_steps = time_res*(num_years-1) # number of global timesteps

def animate(i):
    # linear interpolation parameters
    t = i / time_res  # current point in time (e.g. 2.15)
    t_low = int(t)    # lower discrete time (e.g. 2)
    f = t - t_low     # interpolation factor (e.g. 0.15)

    # set the new positions
    p_interp = (1-f) * gdp_life[t_low] + f * gdp_life[t_low + 1]
    scatterplot.set_offsets(p_interp)

    # set the new sizes
    s_interp = (1-f) * population[t_low] + f * population[t_low + 1]
    scatterplot.set_sizes(s_interp)

    # set the new colors
    c_interp = (1-f) * child_mortality[t_low] + f * child_mortality[t_low + 1]
    scatterplot.set_array(c_interp)
    ax.set_title(1900 + int(t)+1)


# show the animation with a call to FuncAnimation
# this needs to be stored in a variable (here: 'anim') to prevent garbage collection
anim = FuncAnimation(fig, animate, interval=(1000*time_speed)/time_res, frames=time_steps)
plt.show()