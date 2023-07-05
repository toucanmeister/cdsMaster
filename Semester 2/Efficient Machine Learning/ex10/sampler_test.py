import torch


class SimpleDataSet( torch.utils.data.Dataset ):
  def __init__( self,
                i_length ):
    self.m_length = i_length

  def __len__( self ):
    return self.m_length

  def __getitem__( self,
                   i_idx ):
    return i_idx*10

torch.distributed.init_process_group('mpi')

dataset = SimpleDataSet(200)
sampler = torch.utils.data.distributed.DistributedSampler(dataset, 
                                                          num_replicas=torch.distributed.get_world_size(),
                                                          rank=torch.distributed.get_rank(),
                                                          shuffle=False, # shuffles order of samples randomly
                                                          drop_last=False) # drops incomplete last batch
loader = torch.utils.data.DataLoader(dataset, sampler=sampler)

dataset = SimpleDataSet(200)
sampler = torch.utils.data.distributed.DistributedSampler(dataset, 
                                                          num_replicas=torch.distributed.get_world_size(),
                                                          rank=torch.distributed.get_rank(),
                                                          shuffle=False, # shuffles order of samples randomly
                                                          drop_last=False) # drops incomplete last batch
batchsampler = torch.utils.data.BatchSampler(sampler, 3, False)

for sample in batchsampler:
  print(f'{torch.distributed.get_rank()} {sample}')

