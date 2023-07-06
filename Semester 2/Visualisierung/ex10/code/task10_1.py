import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

dataframe = pd.read_csv("co2_data.csv")

####################
# Task 1a          #
####################
fig, ax1 = plt.subplots()
plt.xticks(rotation=45, ha='right')
plt.grid(axis='y')
plt.ylim(0,500)

dataframe = dataframe.sort_values('Emissions', ascending=False)

countries = dataframe['Country'].to_numpy()
emissions = dataframe['Emissions'].to_numpy() * 1E-9

ax1.bar(countries[:20], emissions[:20])

####################
# Task 1b          #
####################
ax2 = ax1.twinx()
plt.ylim(0,1)

cum_emissions = np.cumsum(emissions) / np.sum(emissions) # got to love variable names that have to do with anything cumulative...
ax2.plot(countries[:20], cum_emissions[:20], '-ro')


# Show the result
plt.title('Total CO2 Emissions 1751-2017')
plt.tight_layout()
plt.show()