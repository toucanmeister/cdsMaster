import torch
import torchvision
import eml.mlp.model
import eml.mlp.trainer
import eml.mlp.tester
import eml.mlp.vis.fashion_mnist

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

l_loader_train = torch.utils.data.DataLoader(l_data_train, batch_size=10000)
l_loader_test = torch.utils.data.DataLoader(l_data_test, batch_size=10000)

l_model = eml.mlp.model.Model()
for l_epoch in range(100):
    l_loss = eml.mlp.trainer.train(torch.nn.CrossEntropyLoss(), l_loader_train, l_model, torch.optim.SGD(l_model.parameters(), lr=0.1))
    print(f'training... loss in epoch {l_epoch}: {l_loss}')

l_test_loss_total, l_test_n_correct = eml.mlp.tester.test(torch.nn.CrossEntropyLoss(), l_loader_test, l_model)
print(f'Test loss: {l_test_loss_total}')
print(f'Correctly classified {l_test_n_correct} out of {len(l_loader_test)*l_loader_test.batch_size} test samples')

print('Saving model')
torch.save(l_model, 'other_model.pkl')
l_model = torch.load('other_model.pkl')
l_model.eval()

l_test_loss_total, l_test_n_correct = eml.mlp.tester.test(torch.nn.CrossEntropyLoss(), l_loader_test, l_model)
print(f'Test loss: {l_test_loss_total}')
print(f'Correctly classified {l_test_n_correct} out of {len(l_loader_test)*l_loader_test.batch_size} test samples')
