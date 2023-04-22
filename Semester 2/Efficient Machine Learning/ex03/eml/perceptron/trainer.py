import torch
## Trains the given linear perceptron.
#  @param i_loss_func used loss function.
#  @param io_data_loader data loader which provides the training data.
#  @param io_model model which is trained.
#  @param io_optimizer used optimizer.
#  @return loss.

def train(i_loss_func,
          io_data_loader,
          io_model,
          io_optimizer ):
    # switch model to training mode
    io_model.train()

    l_loss_total = 0
    for (l_points, l_labels) in io_data_loader:
        l_predictions = io_model.forward(l_points)
        l_loss = i_loss_func(l_predictions, l_labels)    
        l_loss.backward()
        l_loss_total += l_loss
        io_optimizer.step()
    return l_loss_total