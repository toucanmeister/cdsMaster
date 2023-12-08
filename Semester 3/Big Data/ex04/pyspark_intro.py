from pyspark import SparkContext, SparkConf

confCluster = SparkConf().setAppName('pyspark_intro')
sc = SparkContext(conf=confCluster)
lines = sc.textFile('./gutenberg/pg20417.txt')
filtered = lines.filter(lambda x: "the" in x)
counted_lines = filtered.count()
with open('output.txt', 'w+') as outfile:
  outfile.write(f'Counted {counted_lines} lines with the word "the"')
