import torch
import linear_function_cpp

class Linear( torch.nn.Module ):
    def __init__( self,
                  i_n_features_input,
                  i_n_features_output,
                  bias=True ):
        super().__init__()
        self.has_bias = bias
        self.weights = torch.nn.Parameter( torch.rand( i_n_features_output,
                                                       i_n_features_input,
                                                       dtype=torch.float32) )
        if bias:
            self.bias = torch.nn.Parameter( torch.rand(1, i_n_features_output) )

    def forward( self,
                 i_input ):
        if self.has_bias:
            return Function.apply(i_input, self.weights, self.bias)
        else:
            return Function.apply(i_input, self.weights)


class Function( torch.autograd.Function ):
    
    @staticmethod
    def forward( io_ctx,
                 i_input, 
                 i_weights,
                 i_bias=None):
        io_ctx.save_for_backward( i_input,
                                    i_weights,
                                    i_bias )
        if i_bias:
            return linear_function_cpp.forward_bias(i_input, i_weights, i_bias)
        else:
            return linear_function_cpp.forward(i_input, i_weights)

    @staticmethod
    def backward( i_ctx,
                  i_grad ):
        l_input, l_weights, l_bias = i_ctx.saved_tensors
        #l_grad_input = i_grad @ l_weights        # this was the python version
        #l_grad_weights = i_grad.t() @ l_input
        l_grad_input, l_grad_weights = linear_function_cpp.backward(i_grad, l_input, l_weights)
        if l_bias:
            l_grad_bias = i_grad
            return l_grad_input, l_grad_weights, l_grad_bias
        else:
            return l_grad_input, l_grad_weights
        