import torch
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap

## Plots the given points and colors them by the predicted labels.
#  It is assumed that a prediction larger than 0.5 corresponds to a red point.
#  All other points are black.
#  @param i_points points in R^3.
#  @param io_model model which is applied to derive the predictions.

def plot( i_points,
          io_model ):
  # switch to evaluation mode
  io_model.eval()

  with torch.no_grad():
    l_predictions = io_model.forward(i_points)
    l_labels = [1 if x > 0.5 else 0 for x in l_predictions]
    l_fig = plt.figure()
    l_ax = l_fig.add_subplot(projection='3d')
    l_ax.scatter(i_points[:,0], i_points[:,1], i_points[:,2], c=l_labels, cmap=ListedColormap(['black', 'red']))
    plt.show()