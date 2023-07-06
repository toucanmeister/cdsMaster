import torch
import aimet_common.defs
import aimet_torch.quantsim
from aimet_torch.model_preparer import prepare_model

class SimpleLinear( torch.nn.Module ):
  def __init__( self ):
    super( SimpleLinear, self ).__init__()

    self.m_layer = torch.nn.Linear( in_features = 4,
                                    out_features = 4,
                                    bias = False )

  def forward( self,
               i_input ):
    l_result = self.m_layer( i_input )
    return l_result

def calibrate( io_model,
               i_use_cuda = False ):
  l_data = torch.Tensor( [0.61, -0.93, 0.71, 0.19] )
  io_model( l_data )

if __name__ == "__main__":
  print( 'Running simple linear quantization example' )

  l_model = SimpleLinear()
  
  # freeze model
  l_model = l_model.eval()
  for l_pa in l_model.parameters():
    l_pa.requires_grad = False

  l_w = torch.tensor( [ [ 0.25, -0.32,  1.58,  2.10],
                        [-1.45,  1.82, -0.29,  3.78],
                        [-2.72, -0.12,  2.24, -1.84],
                        [ 1.93,  0.49,  0.00, -3.19] ],
                        requires_grad = False )

  l_model.m_layer.weight = torch.nn.Parameter( l_w,
                                               requires_grad = False )
  print( l_model )

  l_x = torch.tensor( [0.25, 0.17, -0.31, 0.55] )

  l_prepaired_model = prepare_model(l_model)
  l_quantsim_model = aimet_torch.quantsim.QuantizationSimModel(l_prepaired_model, 
                                                               dummy_input=l_x, 
                                                               quant_scheme=aimet_common.defs.QuantScheme.post_training_tf,
                                                               default_output_bw=8,
                                                               default_param_bw=8)
  l_quantsim_model.compute_encodings(calibrate, forward_pass_callback_args=None)
  l_quantsim_model.export('./quantized_model', 'qmod', l_x)
  l_quantized_model = torch.load('./quantized_model/qmod.pth')


  print( 'FP32 Result:')
  l_y = l_model( l_x )
  print( l_y )

  print( 'Int8 Result:')
  print(l_quantized_model(l_x))
  print()

  print( l_quantsim_model )
  print('Explanations:')
  print('Min is the smallest representable value, everything below min gets clamped to min.')
  print('Max is the largest representable value, everything above max gets clamped to max.')
  print('Delta is the granularity of our fixed-point representation.')