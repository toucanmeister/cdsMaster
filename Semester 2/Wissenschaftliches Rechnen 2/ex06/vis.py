import matplotlib.pyplot as plt
import numpy as np 


img = np.genfromtxt('vis/u_exact.csv', delimiter=' ')
plt.imshow(img, cmap='magma')
plt.savefig('vis/u_exact.png')

img = np.genfromtxt('vis/u.csv', delimiter=' ')
plt.imshow(img, cmap='magma')
plt.savefig('vis/u.png')