import numpy as np

if __name__ == '__main__':
	A = np.loadtxt('data.txt', delimiter=',').T
	print(np.median(A, 1))
