import torch
import torchvision
import eml.mlp.model
import eml.mlp.tester
import eml.mlp.vis.fashion_mnist
import aimet_common.defs
import aimet_torch.quantsim
from aimet_torch.model_preparer import prepare_model

l_data_test = torchvision.datasets.FashionMNIST(
    root="data",
    train=False,
    download=True,
    transform=torchvision.transforms.ToTensor()
)

l_loader_test = torch.utils.data.DataLoader(l_data_test, batch_size=10000)

def calibrate( io_model, foo=None):
    for (l_data, _) in l_loader_test:
            io_model.forward(l_data)

l_dummy,_ = l_data_test[0]

l_model = torch.load('other_model.pkl')
l_prepared_model = prepare_model(l_model)
l_quantsim_model = aimet_torch.quantsim.QuantizationSimModel(l_prepared_model, 
                                                               dummy_input=l_dummy, 
                                                               quant_scheme=aimet_common.defs.QuantScheme.post_training_tf,
                                                               default_output_bw=8,
                                                               default_param_bw=8)
l_quantsim_model.compute_encodings(calibrate, forward_pass_callback_args=None)
l_quantsim_model.export('./other_quantized_model', 'qmod', l_dummy)
l_quantized_model = torch.load('./other_quantized_model/qmod.pth')

l_test_loss_total, l_test_n_correct = eml.mlp.tester.test(torch.nn.CrossEntropyLoss(), l_loader_test, l_model)
print(f'FP32 Accuracy: {l_test_n_correct / (len(l_loader_test)*l_loader_test.batch_size)}')

l_test_loss_total, l_test_n_correct = eml.mlp.tester.test(torch.nn.CrossEntropyLoss(), l_loader_test, l_quantized_model)
print(f'Int8 Accuracy: {l_test_n_correct / (len(l_loader_test)*l_loader_test.batch_size)}')