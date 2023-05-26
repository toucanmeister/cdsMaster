import numpy as np
import matplotlib.pyplot as plt
import vtk
from vtk.util.numpy_support import vtk_to_numpy

# read a volume image
reader = vtk.vtkNrrdReader()
reader.SetFileName("MRHead.nrrd")
reader.Update()
imageData = reader.GetOutput()

# convert from vtkImageData to numpy array
x_dim, y_dim, z_dim = imageData.GetDimensions()
sc = imageData.GetPointData().GetScalars()
image = vtk_to_numpy(sc)
image = image.reshape(x_dim, y_dim, z_dim, order='F')
image = np.rot90(np.flip(image, axis=1))

# normalize image values
image = np.divide(image, float(np.max(image)))

# create a figure
fig = plt.figure(figsize=(16,5))


####################
# Task 1a 
####################
maximum_projection = np.max(image, axis=2)
ax = fig.add_subplot(1, 3, 1)
ax.imshow(maximum_projection, cmap='gray')
ax.set_title('Maximum Intensity Projection')



####################
# Task 1b 
####################
# I(s) = I(s0) * e^(-tau(s0, s))

tau = np.sum(image, 2) / image.shape[2]  # approximate integral
volume_rendering_equation = 1 * np.exp(-tau)
volume_rendering_equation = 1 - volume_rendering_equation
ax = fig.add_subplot(1, 3, 2)
ax.imshow(volume_rendering_equation, cmap='gray')
ax.set_title('Projection using Volume Rendering Equation')

####################
# Task 1c 
####################
gamma = 0
alpha = 0.06

C = np.zeros(image[:,:,0].shape)
a = np.zeros(image[:,:,0].shape)
current_max = np.zeros(image[:,:,0].shape)

for i in range(image.shape[2]):
    current_max = np.maximum(current_max, image[:,:,i]) # update current maximum along ray
    delta = image[:,:,i] - current_max
    delta[delta < 0] = 0
    if gamma <= 0:
        beta = 1 - delta*(1+gamma)
    else:
        beta = 1 - delta
    C = beta*C + (1 - beta*a)*image[:,:,i]
    a = beta*a + (1 - beta*a)*alpha

ax = fig.add_subplot(1, 3, 3)
ax.imshow(C, cmap='gray')
ax.set_title('Projection using MIDA')

# Always run show, to make sure everything is displayed.
plt.show()