import matplotlib.pyplot as plt
import numpy as np 


img = np.genfromtxt('u_exact.csv', delimiter=' ')
plt.imshow(img, cmap='magma')
plt.savefig('u_exact.png')

img = np.genfromtxt('u.csv', delimiter=' ')
plt.imshow(img, cmap='magma')
plt.savefig('u.png')