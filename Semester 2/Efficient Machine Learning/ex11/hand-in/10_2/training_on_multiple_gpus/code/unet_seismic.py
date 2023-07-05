#!/usr/bin/python3
import torch
import torchvision.datasets
import torchvision.transforms
import torch.utils.data
import time

import numpy as np

import eml.data.seismic
import eml.unet.model
import eml.unet.trainer
import eml.unet.tester

# config
l_config = { 'unet': { 'n_init_channels':   64,
                       'kernel_size':        3,
                       'n_layers_per_block': 2,
                       'n_levels':           4 },
             'train': { 'data':           { 'seismic':      'data/data_train.npz',
                                            'labels':       'data/labels_train.npz',
                                            # data size is reduced for three levels in unet
                                            'sample_shape': (1004, 1, 588),
                                            'subset':       ( (0, 1004),
                                                              (0,  750),
                                                              (0,  588) )  },
                        'n_epochs':       100,
                        'n_epochs_print': 5,
                        'n_batch_abort':  5000,
                        'batch_size':     8 },
             'test':  { 'data':           { 'seismic':      'data/data_train.npz',
                                            'labels':       'data/labels_train.npz',
                                             # data size is reduced for three levels in unet
                                            'sample_shape': (1004, 1, 588),
                                            'subset':       ( (  0, 1004),
                                                              ( 751, 782),
                                                              (  0,  588) ) },
                        'batch_size':      1} }

print( "##############################################" )
print( "# Welcome to EML's U-Net for seismic example #" )
print( "##############################################" )
if( torch.cuda.is_available() ):
  l_n_cuda_devices = torch.cuda.device_count();
  print( 'CUDA devices:', l_n_cuda_devices )
  for l_de in range(l_n_cuda_devices):
    print( '  ', torch.cuda.get_device_name(l_de) )
else:
  print( 'could not find a CUDA device' )

print( 'printing configuration:' )
try:
  import json
  print( json.dumps( l_config,
                     indent = 2 ) )
except:
  print( '  json module missing, continuing' )

print( '********************')
print( '* assembling U-Net *')
print( '********************')
# construct U-Net and print info
l_unet2d = eml.unet.model.Unet2d( i_n_init_channels    = l_config['unet']['n_init_channels'],
                                  i_kernel_size        = l_config['unet']['kernel_size'],
                                  i_n_layers_per_block = l_config['unet']['n_layers_per_block'],
                                  i_n_levels           = l_config['unet']['n_levels'] )

if( torch.cuda.is_available() ):
  l_unet2d = torch.nn.DataParallel(l_unet2d, device_ids=[0,1,2,3])
  l_unet2d = l_unet2d.to( torch.device('cuda') )
print( l_unet2d )

# set U-Net to training mode
l_unet2d.train()

# loss function and optimizer
l_loss_func = torch.nn.CrossEntropyLoss()
l_optimizer = torch.optim.Adam( l_unet2d.parameters(),
                                lr=1E-4 )

print( '*****************')
print( '* prepping data *')
print( '*****************')
# training dataset
print( 'loading training dataset' )
l_data_set_train = eml.data.seismic.SeismicDataSet( l_config['train']['data']['seismic'],
                                                    l_config['train']['data']['labels'],
                                                    i_item_shape = l_config['train']['data']['sample_shape'],
                                                    i_subset     = l_config['train']['data']['subset'] )

print( 'loading test data set' )
l_data_set_test = eml.data.seismic.SeismicDataSet( l_config['test']['data']['seismic'],
                                                   l_config['test']['data']['labels'],
                                                   i_item_shape = l_config['test']['data']['sample_shape'],
                                                   i_subset     = l_config['test']['data']['subset'] )

print( 'deriving mean and standard deviation of training data' )
l_mean_train = l_data_set_train.getMean()
l_std_dev_train = l_data_set_train.getStdDev()
print( '  mean:', l_mean_train    )
print( '  std:',  l_std_dev_train )

print( 'normalizing training and test data' )
l_data_set_train.normalize( l_mean_train,
                            l_std_dev_train )
l_data_set_test.normalize( l_mean_train,
                           l_std_dev_train )

# training dataloader
print( 'initializing data loaders' )
l_data_loader_train = torch.utils.data.DataLoader( l_data_set_train,
                                                   batch_size = l_config['train']['batch_size'],
                                                   shuffle    = True )

l_data_loader_test = torch.utils.data.DataLoader( l_data_set_test,
                                                  batch_size = l_config['test']['batch_size'],
                                                  shuffle    = False )

print( '************')
print( '* training *')
print( '************')
start = time.time()
# train for the given number of epochs
for l_epoch in range( l_config['train']['n_epochs'] ):
  print( 'training epoch', l_epoch+1 )
  l_loss_train = eml.unet.trainer.train( l_loss_func,
                                         l_data_loader_train,
                                         l_unet2d,
                                         l_optimizer,
                                         i_n_batches_abort = l_config['train']['n_batch_abort'] )
  print( '  training loss:', l_loss_train )

  print( 'applying net to test data' )
  l_loss_test, l_n_correct_test, l_n_total_test = eml.unet.tester.test( l_loss_func,
                                                                        l_data_loader_test,
                                                                        l_unet2d )
  l_accuracy_test = l_n_correct_test / l_n_total_test
  print( '  test loss:', l_loss_test )
  print( '  test accuracy:', l_accuracy_test )

  # do an intermediate evaluation on the test data
  if( (l_epoch+1) % l_config['train']['n_epochs_print'] == 0 ):
    l_unet2d.eval()
    with torch.no_grad():
      l_data_raw = l_data_set_test.m_data[:,0,:].squeeze().reshape( 1, 1, 1004, 588 )
      l_data_raw = torch.Tensor( l_data_raw )
      if( torch.cuda.is_available() ):
        l_data_raw = l_data_raw.to( torch.device('cuda') )
      l_prediction = l_unet2d.forward( l_data_raw )
      l_prediction = l_prediction.argmax(1).squeeze()
      l_prediction = l_prediction.to('cpu')

      # pad invalid values, i.e., -1, to match input data
      l_pad = l_unet2d.module.m_padding
      l_prediction = torch.nn.functional.pad( l_prediction,
                                              (l_pad, l_pad, l_pad, l_pad),
                                              value = -1 )

    l_unet2d.train()

end = time.time()
print()
print(f'Took {end-start} seconds.')
