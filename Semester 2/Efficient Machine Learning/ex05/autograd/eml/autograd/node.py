import context
import function

class Node:
  ## Initializes a node of the computation graph.
  # @param optional value used for leaf nodes.
  def __init__( self,
                i_value = None ):
    self.m_value = i_value
    self.m_grad = 0
    self.m_grad_fn = function.Nop.backward
    self.m_children = []
    self.m_ctx = context.Context()

  ## String representation of a node.
  # @return newline separated string with value, grad and grad_fn.
  def __str__( self ):
    l_string  = "node:\n"
    l_string += "  value: "   + str( self.m_value ) + "\n"
    l_string += "  grad: "    + str( self.m_grad ) + "\n"
    l_string += "  grad_fn: " + str( self.m_grad_fn )
    return l_string

  ## Backward pass of the node.
  # @param i_grad  grad w.r.t. to output of forward pass.
  def backward( self,
                i_grad ):
    self.m_grad += i_grad
    l_grad_children = self.m_grad_fn( self.m_ctx,
                                      i_grad )

    for l_ch in range( len(self.m_children) ):
      self.m_children[l_ch].backward( l_grad_children[l_ch] )

  ## Zeroes the grad of the node and all children.
  def zero_grad( self ):
    self.m_grad = 0
    for l_ch in self.m_children:
      l_ch.zero_grad()

  ## Returns a new node which represents the addition of the two input nodes.
  # @param self first input node.
  # @param i_other second input node.
  # @return node representing the addition.
  def __add__( self,
               i_other ):
    l_node = Node()
    l_node.m_grad_fn = function.Add.backward
    l_node.m_children = [self, i_other]
    l_node.m_value = function.Add.forward( l_node.m_ctx,
                                           self.m_value,
                                           i_other.m_value )
    return l_node

  ## Returns a new node which represents the multiplication of the two input nodes.
  # @param self first input node.
  # @param i_other second input node.
  # @return node representing the multiplication.
  def __mul__( self,
               i_other ):
    l_node = Node()
    l_node.m_grad_fn = function.Mul.backward
    l_node.m_children = [self, i_other]
    l_node.m_value = function.Mul.forward( l_node.m_ctx,
                                           self.m_value,
                                           i_other.m_value )
    return l_node

  def __truediv__( self, 
                   i_other ):
    # Node for 1 / i_other
    l_rec_node = Node()
    l_rec_node.m_grad_fn = function.Reciprocal.backward
    l_rec_node.m_children = [i_other]
    l_rec_node.m_value = function.Reciprocal.forward( l_rec_node.m_ctx,
                                                      i_other.m_value )
    # Node for self / i_other
    return self * l_rec_node
  
  def __sub__( self,
               i_other ):
    l_node = Node()
    l_node.m_grad_fn = function.Diff.backward
    l_node.m_children = [self, i_other]
    l_node.m_value = function.Diff.forward( l_node.m_ctx,
                                            self.m_value,
                                            i_other.m_value )
    return l_node

  def __neg__( self ):
    l_node = Node()
    l_node.m_grad_fn = function.Neg.backward
    l_node.m_children = [self]
    l_node.m_value = function.Neg.forward( l_node.m_ctx,
                                           self.m_value )
    return l_node


def exp( i_x ):
  l_node = Node()
  l_node.m_grad_fn = function.Exp.backward
  l_node.m_children = [i_x]
  l_node.m_value = function.Exp.forward( l_node.m_ctx, 
                                         i_x.m_value )
  return l_node

def sin( i_x ):
  l_node = Node()
  l_node.m_grad_fn = function.Sin.backward
  l_node.m_children = [i_x]
  l_node.m_value = function.Sin.forward( l_node.m_ctx, 
                                         i_x.m_value )
  return l_node

def cos( i_x ):
  l_node = Node()
  l_node.m_grad_fn = function.Cos.backward
  l_node.m_children = [i_x]
  l_node.m_value = function.Cos.forward( l_node.m_ctx, 
                                         i_x.m_value )
  return l_node