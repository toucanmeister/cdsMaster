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
fig, ax = plt.subplots(1, 1)
ax.plot(month_counts, linewidth=1.5)
ax.set_xlabel('week')
ax.set_ylabel('311 calls about rodents')
ax.set_xticks([327 / 7])
ax.set_xticklabels(['Thanksgiving',])
ax.axvline(x=327 / 7, color='black', linewidth=0.5)
ax.axvspan(327/7, (327+14)/7, color='black', alpha=0.1, lw=0)
plt.show()
