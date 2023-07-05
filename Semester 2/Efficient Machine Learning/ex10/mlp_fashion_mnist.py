#!/usr/bin/python3
import torch
import torchvision.datasets
import torchvision.transforms
import torch.utils.data

import eml.mlp.model
import eml.mlp.trainer
import eml.mlp.tester
import eml.vis.fashion_mnist

print( "################################" )
print( "# Welcome to EML's MLP example #" )
print( "################################" )


torch.distributed.init_process_group('mpi')

# set up datasets
if torch.distributed.get_rank() == 0:
  print( 'setting up datasets')
l_data_train = torchvision.datasets.FashionMNIST( root      = "data/fashion_mnist",
                                                  train     = True,
                                                  download  = True,
                                                  transform = torchvision.transforms.ToTensor() )

l_data_test = torchvision.datasets.FashionMNIST( root      = "data/fashion_mnist",
                                                 train     = False,
                                                 download  = True,
                                                 transform = torchvision.transforms.ToTensor() )



if torch.distributed.get_rank() == 0:
  print( 'running distributed sampling' )
sampler = torch.utils.data.distributed.DistributedSampler(l_data_train, 
                                                          num_replicas=torch.distributed.get_world_size(),
                                                          rank=torch.distributed.get_rank(),
                                                          shuffle=False, # shuffles order of samples randomly
                                                          drop_last=False) # drops incomplete last batch


# init data loaders
if torch.distributed.get_rank() == 0:
  print( 'initializing data loaders' )
l_data_loader_train = torch.utils.data.DataLoader( sampler,
                                                   batch_size = 64 )
l_data_loader_test  = torch.utils.data.DataLoader( l_data_test,
                                                   batch_size = 64 )

# set up model, loss function and optimizer
if torch.distributed.get_rank() == 0:
  print( 'setting up model, loss function and optimizer' )
l_model = eml.mlp.model.Model()
l_loss_func = torch.nn.CrossEntropyLoss()
l_optimizer = torch.optim.SGD( l_model.parameters(),
                               lr = 1E-3 )
if torch.distributed.get_rank() == 0:
  print( l_model )

# train for the given number of epochs
l_n_epochs = 25
for l_epoch in range( l_n_epochs ):
  if torch.distributed.get_rank() == 0: print( 'training epoch #' + str(l_epoch+1) )
  l_loss_train = eml.mlp.trainer.train( l_loss_func,
                                        l_data_loader_train,
                                        l_model,
                                        l_optimizer )
  if torch.distributed.get_rank() == 0: print( '  training loss:', l_loss_train )

  if torch.distributed.get_rank() == 0:
    l_loss_test, l_n_correct_test = eml.mlp.tester.test( l_loss_func,
                                                        l_data_loader_test,
                                                        l_model )
    l_accuracy_test = l_n_correct_test / len(l_data_loader_test.dataset)
    print( '  test loss:', l_loss_test )
    print( '  test accuracy:', l_accuracy_test )

    # visualize results of intermediate model every 10 epochs
    if( (l_epoch+1) % 10 == 0 ):
      l_file_name =  'test_dataset_epoch_' + str(l_epoch+1) + '.pdf'
      print( '  visualizing intermediate model w.r.t. test dataset: ' + l_file_name )
      eml.vis.fashion_mnist.plot( 0,
                                  250,
                                  l_data_loader_test,
                                  l_model,
                                  l_file_name )

if torch.distributed.get_rank() == 0:
  # visualize results of final model
  l_file_name = 'test_dataset_final.pdf'
  print( 'visualizing final model w.r.t. test dataset:', l_file_name )
  eml.vis.fashion_mnist.plot( 0,
                              250,
                              l_data_loader_test,
                              l_model,
                              l_file_name )

  # save model
  l_file_name = 'model_mlp.pt'
  print( 'serializing model' )
  l_model_serial = torch.jit.script( l_model )
  print( 'saving model to', l_file_name )
  l_model_serial.save( l_file_name )
  torch.save( l_model.state_dict(), 'state_dict_mlp.pt' )

  print( "#############" )
  print( "# Finished! #" )
  print( "#############" )
