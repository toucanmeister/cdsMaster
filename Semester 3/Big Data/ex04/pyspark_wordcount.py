from pyspark import SparkContext, SparkConf

confCluster = SparkConf().setAppName('pyspark_intro')
sc = SparkContext(conf=confCluster)
lines = sc.textFile('./gutenberg') # reads all text files in the folder gutenberg into lines
words = lines.flatMap(lambda line: line.split(' ')) # split each line into words, flatMap means we get a collection of words
words = words.filter(lambda word: word.isalpha())\
             .map(lambda word: word.lower())
counts = words.map(lambda word: (word, 1))\
              .reduceByKey(lambda count1, count2: count1 + count2)\
              .sortBy(lambda pair: pair[1], ascending=False)\
              .collect()
with open('wordcount_output.txt', 'w+') as outfile:
  for (word, count) in counts:
    outfile.write(f'{word}: {count}\n')
