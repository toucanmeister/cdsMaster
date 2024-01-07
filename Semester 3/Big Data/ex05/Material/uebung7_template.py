#!/usr/bin/python
# -*- coding: utf-8 -*-
import time
import sys
import re

from pyspark.sql import functions as F
from pyspark.sql.types import DoubleType
from pyspark.sql.functions import lit
from pyspark.sql.functions import levenshtein  
from pyspark.sql.functions import col
from pyspark.sql.functions import udf
from pyspark import SparkContext, SparkConf
from pyspark.sql import SQLContext, Row

#Used for preprocessing
def replace(string, substitutions):
    substrings = sorted(substitutions, key=len, reverse=True)
    regex = re.compile('|'.join(map(re.escape, substrings)))
    return regex.sub(lambda match: substitutions[match.group(0)], string)

#Cluster Configuration
confCluster = SparkConf().setAppName("MusicSimilarity Cluster")
confCluster.set("spark.executor.memory", "8g")
confCluster.set("spark.executor.cores", "4")
repartition_count = 32
sc = SparkContext(conf=confCluster)
sqlContext = SQLContext(sc)

#Input Arguments
if len (sys.argv) < 2:
    song = "music/Let_It_Be/beatles+Let_It_Be+06-Let_It_Be.mp3"
else: 
    song = sys.argv[1]

#Start time measurements for performance testing
time_dict = {}
tic1 = int(round(time.time() * 1000))
notes = sc.textFile("./musicfiles/out[0-9]*.notes", minPartitions=repartition_count)
notes = notes.map(lambda x: x.split(';'))
substitutions = {"0": "A", "1": "B", "2": "C", "3": "D", "4": "E", "5": "F", "6": "G", "7": "H", "8": "I", "9": "J", "10": "K", "11": "L", " ": "", ",": "", "[": "", "]": ""}
notes = notes.map(lambda x: (x[0], x[1], x[2], replace(x[3],substitutions))).persist()

df = sqlContext.createDataFrame(notes, ["id", "key", "scale", "notes"])
notes_comp = df.filter(df.id == song).collect()[0][3]
df_merged = df.withColumn("compare", lit(notes_comp))
resultDF = df_merged.withColumn("distances_levenshtein", levenshtein(col("notes"), col("compare")))

#Stop time measurement 
tic1 = int(round(time.time() * 1000))
resultDF.toPandas().to_csv("SONG1.csv", encoding='utf-8')
resultDF.unpersist()
tac1 = int(round(time.time() * 1000))
time_dict['CSV1: ']= tac1 - tic1

#Print results and free stuff
print(time_dict)
notes.unpersist()

