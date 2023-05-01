
def train( i_loss_func,
           io_data_loader,
           io_model,
           io_optimizer ):
    io_model.train()

    l_loss_total = 0
    i = True
    for (l_data, l_labels) in io_data_loader:
        l_predictions = io_model.forward(l_data)
        l_loss = i_loss_func(l_predictions, l_labels)
        io_optimizer.zero_grad()    
        l_loss.backward()
        l_loss_total += l_loss
        io_optimizer.step()
    return l_loss_total