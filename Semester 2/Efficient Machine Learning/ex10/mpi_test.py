import torch.distributed as tdst
import torch

## 1.
tdst.init_process_group('mpi')

print(f'rank: {tdst.get_rank()} of {tdst.get_world_size()}')

## 2.
if tdst.get_rank() == 0:
  l_tensor = torch.ones((3,4))
else:
  l_tensor = torch.zeros((3,4))

if tdst.get_rank() == 0:
  tdst.send(l_tensor, 1)
if tdst.get_rank() == 1:
  tdst.recv(l_tensor, 0)

## 3.
if tdst.get_rank() == 0:
  l_tensor = torch.ones((3,4))
else:
  l_tensor = torch.zeros((3,4))

if tdst.get_rank() == 0:
  req = tdst.isend(l_tensor, 1)
  req.wait()
if tdst.get_rank() == 1:
  req = tdst.irecv(l_tensor, 0)
  req.wait()

## 4.
l_tensor = torch.tensor([[1,  2,  3,  4],
                         [5,  6,  7,  8],
                         [9, 10, 11, 12]])

tdst.all_reduce(l_tensor, op=tdst.ReduceOp.SUM)

if tdst.get_rank()==0:
  print(l_tensor)