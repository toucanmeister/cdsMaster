import torch

class Model(torch.nn.Module):
    def __init__(self):
        super().__init__()
        self.linear = torch.nn.Linear(3,1)
        self.sigmoid = torch.nn.Sigmoid()
    
    def forward(self, i):
        l = self.linear(i)
        return self.sigmoid(l)