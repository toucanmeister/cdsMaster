import numpy as np

file = np.load('data/data_train.npz')
data_train = file['data']
print(f'data_train | shape: {data_train.shape} | dtype: {data_train.dtype}')

file = np.load('data/labels_train.npz')
labels_train = file['labels']
print(f'labels_train | shape: {labels_train.shape} | dtype: {labels_train.dtype}')

file = np.load('data/data_test_1.npz')
data_test_1 = file['data']
print(f'data_test_1 | shape: {data_test_1.shape} | dtype: {data_test_1.dtype}')

file = np.load('data/data_test_2.npz')
data_test_2 = file['data']
print(f'data_test_2 | shape: {data_test_2.shape} | dtype: {data_test_2.dtype}')

""" table filled with program outputs
File            |  nx  |  ny  |  nz  |  dtype  | size(GB) 
---------------------------------------------------------
data_train.npz  | 1006 | 782  | 590  | float32 | 1.59
labels_train.npz| 1006 | 782  | 590  | int8    | 0.007
data_test_1.npz | 1006 | 782  | 251  | float32 | 0.70
data_test_2.npz | 1006 | 334  | 841  | float32 | 1.00
"""