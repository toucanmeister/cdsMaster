import torch

def test( i_loss_func,
          io_data_loader,
          io_model ):
    io_model.eval()

    l_loss_total = 0
    l_n_correct = 0

    with torch.no_grad():
        for (l_data, l_labels) in io_data_loader:
            l_predictions = io_model.forward(l_data)
            l_loss_total += i_loss_func(l_predictions, l_labels)
            l_compare_vector = l_predictions.argmax(dim=1) == l_labels
            l_n_correct += l_compare_vector.type(torch.float).sum().item()
    return l_loss_total, l_n_correct
