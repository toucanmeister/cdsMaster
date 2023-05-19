import torch
import eml.ext.linear_python
import eml.linear_cpp.linear_cpp
import eml.vis.points
import numpy as np

## Testing my implementations using the example problem from exercise 3

# Model to test my linear layer
class MyModel( torch.nn.Module ):
    def __init__(self):
        super().__init__()
        # self.linear = eml.ext.linear_python.Linear( 3, 1, bias=True )   # pure python version
        self.linear = eml.linear_cpp.linear_cpp.Linear( 3, 1, bias=True )
        self.sigmoid = torch.nn.Sigmoid()
    
    def forward(self, i):
        l = self.linear(i)
        return self.sigmoid(l)

# Import data
l_points = np.loadtxt('data/data_points.csv', delimiter=',')
l_labels = np.loadtxt('data/data_labels.csv')
l_points = torch.Tensor(l_points)
l_labels = torch.Tensor(l_labels).unsqueeze(-1)
l_dataset = torch.utils.data.TensorDataset(l_points, l_labels)
l_data_loader = torch.utils.data.DataLoader(l_dataset, batch_size=1)


l_model = MyModel()
l_loss_func = torch.nn.BCELoss()
l_optimizer = torch.optim.SGD(l_model.parameters(), lr=0.1)

for l_epoch in range(51):
    l_loss_total = 0
    i = True
    for (l_data, l_labels) in l_data_loader:
        l_predictions = l_model.forward(l_data)
        l_loss = l_loss_func(l_predictions, l_labels)
        l_optimizer.zero_grad()    
        l_loss.backward()
        l_loss_total += l_loss
        l_optimizer.step()
    print(f'training... loss in epoch {l_epoch}: {l_loss}')
    #if l_epoch % 10 == 0:
        #eml.vis.points.plot(l_points, l_model)