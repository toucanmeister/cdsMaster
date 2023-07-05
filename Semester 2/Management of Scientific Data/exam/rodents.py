import numpy as np
import pandas as pd
import matplotlib as plt


## Loading
df = pd.read_csv('rodents.csv', usecols=['Created Date', 'City'])


## Cleaning
print(df.info())
print()
df = df.dropna() # drop any rows where there are non-specified entries
print(df.info())
print()

print(df['City'].value_counts())
print()
df['City'] = df['City'].apply(lambda x: x.lower().strip()) # make all cities lowercase and remove leading and trailing whitespace

## Filtering
df = df[df['City'] == 'brooklyn']
