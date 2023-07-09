import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

## Loading
df = pd.read_csv('rodents.csv', usecols=['Created Date', 'City'], parse_dates=['Created Date'])


## Cleaning
print(df.info())
print()
df = df.dropna() # drop any rows where there are non-specified entries
print(df.info())
print()

print(df['City'].value_counts())
print()
df['City'] = df['City'].apply(lambda x: x.lower().strip()) # make all cities lowercase and remove leading and trailing whitespace
 
df = df.assign(week=df['Created Date'].dt.dayofyear // 7) # turn dates into weeks
df = df.drop(columns='Created Date')


## Filtering
df = df[df['City'] == 'brooklyn']


## Plot
month_counts = df.groupby('week').count()
ax = month_counts.plot.line()
ax.set_xlabel('week')
ax.set_ylabel('311 calls about rodents')
ax.set_xticks([327 / 7, 358 / 7])
ax.set_xticklabels(['Thanksgiving', 'Christmas'])
plt.show()