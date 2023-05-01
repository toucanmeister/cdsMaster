import torch
import torchvision
import matplotlib.pyplot as plt
import matplotlib.backends.backend_pdf
import eml.mlp.model
import eml.mlp.trainer
import eml.mlp.tester
import eml.mlp.vis.fashion_mnist

### 4.1 Datasets and Data Loaders

# 1.
l_data_train = torchvision.datasets.FashionMNIST(
    root="data",
    train=True,
    download=True,
    transform=torchvision.transforms.ToTensor()
)

l_data_test = torchvision.datasets.FashionMNIST(
    root="data",
    train=False,
    download=True,
    transform=torchvision.transforms.ToTensor()
)

# 2. 
with matplotlib.backends.backend_pdf.PdfPages('visualization.pdf') as pdf:
    for i in range(4):
        plt.figure(figsize=(3, 3))
        l_img, l_label = l_data_train[i]
        plt.imshow(l_img.squeeze(), cmap='gray')
        plt.title(f'Label: {l_label}')
        pdf.savefig()
        plt.close()

# 3.
l_loader_train = torch.utils.data.DataLoader(l_data_train, batch_size=10000)
l_loader_test = torch.utils.data.DataLoader(l_data_test, batch_size=10000)

### 4.2 Training and Validation

# 1. See eml/mlp/model.py

# 2. and 3. see eml/mlp/trainer.py
l_model = eml.mlp.model.Model()
for l_epoch in range(1):
    l_loss = eml.mlp.trainer.train(torch.nn.CrossEntropyLoss(), l_loader_train, l_model, torch.optim.SGD(l_model.parameters(), lr=0.1))
    print(f'training... loss in epoch {l_epoch}: {l_loss}')

# 4. see eml/mlp/tester.py
l_test_loss_total, l_test_n_correct = eml.mlp.tester.test(torch.nn.CrossEntropyLoss(), l_loader_test, l_model)
print(f'Test loss: {l_test_loss_total}')
print(f'Correctly classified {l_test_n_correct} out of {len(l_loader_test)*l_loader_test.batch_size} test samples')
print()
### 4.3 Visualization

# 1. see eml/mlp/vis/fashion_mnist.py
eml.mlp.vis.fashion_mnist.plot(0, 1000, l_loader_test, l_model, i_path_to_pdf='test_vis.pdf')

# 2.
l_model = eml.mlp.model.Model()
for l_epoch in range(250):
    print(f'--- Epoch {l_epoch} ---')
    if l_epoch % 10 == 0:
        eml.mlp.vis.fashion_mnist.plot(0, 1000, l_loader_test, l_model, i_path_to_pdf=f'test_vis_epoch_{l_epoch}.pdf')
    l_loss = eml.mlp.trainer.train(torch.nn.CrossEntropyLoss(), l_loader_train, l_model, torch.optim.SGD(l_model.parameters(), lr=0.1))
    print(f'training loss {l_epoch}: {l_loss}')
    l_test_loss_total, l_test_n_correct = eml.mlp.tester.test(torch.nn.CrossEntropyLoss(), l_loader_test, l_model)
    print(f'correctly classified {l_test_n_correct} out of {len(l_loader_test)*l_loader_test.batch_size} test samples')
