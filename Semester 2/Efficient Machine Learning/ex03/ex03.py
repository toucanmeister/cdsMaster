import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
import torch
import numpy as np

import eml.perceptron.model
import eml.perceptron.trainer
import eml.vis.points

l_points = np.loadtxt('data_points.csv', delimiter=',')
l_labels = np.loadtxt('data_labels.csv')

print('### 3.1 Data Exploration ###')
l_fig = plt.figure()
l_ax = l_fig.add_subplot(projection='3d')
l_ax.scatter(l_points[:,0], l_points[:,1], l_points[:,2], c=l_labels, cmap=ListedColormap(['black', 'red']))
plt.show()
print()

print('### 3.2 Datasets and Data Loaders ###')
l_points = torch.Tensor(l_points)
l_labels = torch.Tensor(l_labels).unsqueeze(-1)
l_dataset = torch.utils.data.TensorDataset(l_points, l_labels)
l_data_loader = torch.utils.data.DataLoader(l_dataset, batch_size=50)
print('The parameter batch_size signifies how many points are in one batch.')
print(f'Number of batches with batch_size 50: {len(l_data_loader)}')
l_data_loader = torch.utils.data.DataLoader(l_dataset, batch_size=100)
print(f'Number of batches with batch_size 100: {len(l_data_loader)}')
print()

print('### 3.3 Training and 3.4 Visualization ###')
l_model = eml.perceptron.model.Model()
for l_epoch in range(10):
    l_loss = eml.perceptron.trainer.train(torch.nn.BCELoss(), l_data_loader, l_model, torch.optim.SGD(l_model.parameters(), lr=0.1))
    print(f'training... loss in epoch {l_epoch}: {l_loss}')
    if l_epoch % 2 == 0:
        eml.vis.points.plot(l_points, l_model)