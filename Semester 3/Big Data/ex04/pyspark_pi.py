from pyspark import SparkContext, SparkConf
from random import random


confCluster = SparkConf().setAppName('pyspark_intro')
sc = SparkContext(conf=confCluster)

n = 10000000
count = 0
def sample(_):
  x = random() * 2 - 1
  y = random() * 2 - 1
  return 1 if x ** 2 + y ** 2 <= 1 else 0


indices = sc.parallelize(range(n))
count = indices.map(sample)\
               .sum()
pi_approx = 4.0 * count / n
with open('pi_output.txt', 'w+') as outfile:
  outfile.write(f'PI ~ {pi_approx}')
