import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# compute a 2D function f(x,y) = sin(x^2 + y^2)
x = y = np.linspace(-2, 2, 200)
X, Y = np.meshgrid(x, y)
Z = np.sin(X**2 + Y**2)

# Note that the function can be expressed as sin(x^2 + y^2) - z = 0.
# This allows to derive the normal and the Hessian.

####################
# Task 1a          #
####################
#
# Hessian: [ df/dxx  df/dxy df/dxz
#            df/dyx  df/dyy df/dyz
#            df/dzx  df/dyz df/dzz ]
#
# sin(x^2 + y^2) - z = 0
# df/dx = cos(x^2 + y^2) * 2x
# df/dy = cos(x^2 + y^2) * 2y
# df/dz = 1
# df/dxx = -sin(x^2 + y^2)*4x^2 + cos(x^2 + y^2)*2
# df/dxy = -sin(x^2 + y^2) * 2y * 2x
# df/dxz = 0
# df/dyx = -sin(x^2 + y^2) * 2y * 2x
# df/dyy = -sin(x^2 + y^2)*4y^2 + cos(x^2 + y^2)*2
# df/dyz = 0
# df/dzx = 0
# df/dzy = 0
# df/dzz = 0

def hessian(x, y, z):
    hess = np.empty((3,3))
    hess[0,0] = -np.sin(x**2 + y**2)*4*x**2 + np.cos(x**2 + y**2) * 2
    hess[0,1] = -np.sin(x**2 + y**2) * 2*y * 2*x
    hess[0,2] = 0
    hess[1,0] = -np.sin(x**2 + y**2) * 2*y * 2*x
    hess[1,1] = -np.sin(x**2 + y**2)*2*y**2 + np.cos(x**2 + y**2) * 2
    hess[1,2] = 0
    hess[2,0] = 0
    hess[2,1] = 0
    hess[2,2] = 0
    return hess


####################
# Task 1b          #
####################
#
# Gradient: [ df/dx  df/dy  df/dz ]
#
# df/dx = cos(x^2 + y^2) * 2x
# df/dy = cos(x^2 + y^2) * 2y
# df/dz = 1

def get_P(x, y, z):
    grad_x = np.cos(x**2 + y**2) * 2*x
    grad_y = np.cos(x**2 + y**2) * 2*y
    grad_z = 1
    grad = np.array([grad_x, grad_y, grad_z])
    n = - grad / np.linalg.norm(grad)
    return np.eye(3,3) - np.outer(n,n)

####################
# Task 1c          #
####################
def get_abs_nabla(x, y, z):
    grad_x = np.cos(x**2 + y**2) * 2*x
    grad_y = np.cos(x**2 + y**2) * 2*y
    grad_z = 1
    grad = np.array([grad_x, grad_y, grad_z])
    return np.linalg.norm(grad)

def get_G(P, H, abs_nabla):
    return - (P @ H @ P) / abs_nabla


####################
# Task 1d          #
####################
def get_T(G):
    return np.trace(G)


####################
# Task 1e          #
####################
def get_F(G):
    return np.linalg.norm(G, ord='fro')


####################
# Task 1f          #
####################
def get_kappa(T,F):
    root = np.sqrt(2*F**2 - T**2)
    kappa_1 = (T + root) / 2
    kappa_2 = (T - root) / 2
    return kappa_1, kappa_2

def mean_kappa(i,j):
    # get sample values
    x = X[i,j]
    y = Y[i,j]
    z = Z[i,j]

    P = get_P(x, y, z)
    H = hessian(x, y, z)
    abs_nabla = get_abs_nabla(x, y, z)
    G = get_G(P, H, abs_nabla)
    T = get_T(G)
    F = get_F(G)
    kappa_1, kappa_2 = get_kappa(T,F)
    return (1/2) * (kappa_1 + kappa_2)  # ist dasselbe wie (1/2)*T


####################
# Display Result   #
####################
fig = plt.figure(figsize=(9,7))
ax = fig.add_subplot(projection='3d')

# matrix that will hold mean kappa values for each point
mean_kappa_matrix = np.zeros(np.shape(X))

# probe each sample point of the function
# calculate mean kappa value and save
for i in range(np.shape(X)[0]):
    for j in range(np.shape(X)[1]):
        mean_kappa_matrix[i,j] = mean_kappa(i,j)

# normalize mean kappa_matrix from [minval,maxval] to [0,1]
minval = -1
maxval = 1
kappa_normalized = (mean_kappa_matrix - minval) / (maxval - minval)

# plot result
surface = ax.plot_surface(X,Y,Z,
    rcount=100,
    ccount=100,
    facecolors=mpl.cm.cool(kappa_normalized))

# add a colorbar
cmap = mpl.cm.cool
norm = mpl.colors.Normalize(vmin=minval, vmax=maxval)
fig.colorbar(mpl.cm.ScalarMappable(norm=norm, cmap=cmap),
    ax=ax,
    orientation='horizontal',
    label=r'$(\kappa_1 + \kappa_2)/2$',
    extend='both')

plt.show()