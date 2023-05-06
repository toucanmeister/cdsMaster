import math

## No operation
class Nop:
  ## Backward method which simply forwards the gradient.
  # @param i_ctx context object.
  # @param i_grad gradient w.r.t. to output of forward method.
  # @return gradient w.r.t. to input of forward method.
  def backward( i_ctx,
                i_grad ):
    return i_grad

## Addition
class Add:
  ## Forward method which compute a+b.
  # @param i_ctx context object.
  # @param i_a node a.
  # @param i_b node b.
  # @return result a+b.
  def forward( io_ctx,
               i_a,
               i_b ):
    l_result = i_a + i_b
    return l_result

  ## Backward method.
  # @param i_ctx context object.
  # @param i_grad gradient w.r.t. to output of forward method.
  # @return gradient w.r.t. to input of forward method.
  def backward( i_ctx,
                i_grad ):
    l_grad_a = i_grad
    l_grad_b = i_grad
    return [ l_grad_a, l_grad_b ]

## Multiplication
class Mul:
  ## Forward method which compute a*b.
  # @param i_ctx context object.
  # @param i_a node a.
  # @param i_b node b.
  # @return result a*b.
  def forward( io_ctx,
               i_a,
               i_b ):
    io_ctx.save_for_backward( i_a,
                              i_b )
    l_result = i_a * i_b
    return l_result

  ## Backward method.
  # @param i_ctx context object.
  # @param i_grad gradient w.r.t. to output of forward method.
  # @return gradient w.r.t. to input of forward method.
  def backward( i_ctx,
                i_grad ):    
    l_a, l_b = i_ctx.m_saved_data
    l_grad_a = l_b * i_grad
    l_grad_b = l_a * i_grad
    return [ l_grad_a, l_grad_b ]

class Reciprocal:
  def forward( io_ctx,
               i_x ):
    io_ctx.save_for_backward( i_x )
    l_result = 1.0 / i_x
    return l_result
  
  def backward( i_ctx, 
                i_grad ):
    l_x = i_ctx.m_saved_data[0] # [0] is necessary because it doesnt unpack the one-element tuple
    l_grad_x = (-(1.0 / (l_x*l_x))) * i_grad
    return [ l_grad_x ]

class Exp:
  def forward( io_ctx,
               i_x ):
    io_ctx.save_for_backward( i_x )
    l_result = math.exp( i_x )
    return l_result
  
  def backward( i_ctx,
                i_grad ):
    l_x = i_ctx.m_saved_data[0] # [0] is necessary because it doesnt unpack the one-element tuple
    l_grad_x = math.exp( l_x ) * i_grad
    return [ l_grad_x ]
  
class Diff:
  def forward( io_ctx,
               i_x,
               i_y ):
    l_result = i_x - i_y
    return l_result
  
  def backward( i_ctx,
                i_grad ):
    l_grad_x = 1.0 * i_grad
    l_grad_y = (-1.0) * i_grad
    return [ l_grad_x, l_grad_y ]
  
class Neg:
  def forward( io_ctx,
               i_x ):
    l_result = -i_x
    return l_result

  def backward( i_ctx,
                i_grad ):
    l_grad_x = (-1.0) * i_grad
    return [ l_grad_x ]

class Sin:
  def forward( io_ctx, 
               i_x ):
    io_ctx.save_for_backward( i_x )
    l_result = math.sin( i_x )
    return l_result

  def backward( i_ctx,
                i_grad ):
    l_x = i_ctx.m_saved_data[0] # [0] is necessary because it doesnt unpack the one-element tuple
    l_grad_x = math.cos( l_x ) * i_grad
    return [ l_grad_x ]

class Cos:
  def forward( io_ctx, 
               i_x ):
    io_ctx.save_for_backward( i_x )
    l_result = math.cos( i_x )
    return l_result

  def backward( i_ctx,
                i_grad ):
    l_x = i_ctx.m_saved_data[0] # [0] is necessary because it doesnt unpack the one-element tuple
    l_grad_x = -math.sin( l_x ) * i_grad
    return [ l_grad_x ]