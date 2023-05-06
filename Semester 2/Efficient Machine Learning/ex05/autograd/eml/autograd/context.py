## Context object used to pass information between the forward and backward pass.
class Context:
  ## Saves the given data for the backward pass.
  # @param i_data data which is saved.
  def save_for_backward( self,
                         *i_data ):
    self.m_saved_data = i_data