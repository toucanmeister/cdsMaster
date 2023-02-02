# python3 sol1.py m n p
import sys
import time
import numpy as np

m = int(sys.argv[1])
n = int(sys.argv[2]) 
p = int(sys.argv[3]) 
np.random.seed(27)

X = np.random.randint(1, 4, size = (m,n))
Y = np.random.randint(1, 4, size = (n,p))
Z = np.random.randint(1, 4, size = (m,p))

start = time.time()

for i in range(m):
    for j in range(p):
        for k in range(n):
            Z[i,j] = Z[i,j] + X[i,k]*Y[k,j]
end = time.time()

for r in Z:
    print(r)

print("Python Version:      ", format((end - start)*1000, '.6f'), "ms")
