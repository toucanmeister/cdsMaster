import unittest
import math
import function
import context

class TestNop( unittest.TestCase ):
  def test_backward( self ):
    l_grad = function.Nop.backward( None, 5.0 )
    self.assertEqual( l_grad, 5.0 )

class TestAdd( unittest.TestCase ):
  def test_forward( self ):
    l_result = function.Add.forward( None, 3.0, 4.0 )
    self.assertAlmostEqual( l_result, 7.0 )
  
  def test_backward( self ):
    l_grad_a, l_grad_b = function.Add.backward( None, 5.0 )
    self.assertEqual( l_grad_a, 5.0 )
    self.assertEqual( l_grad_b, 5.0 )

class TestMul( unittest.TestCase ):
  def test_forward( self ):
    ctx = context.Context()
    l_result = function.Mul.forward( ctx, 2.0, 7.0 )
    self.assertEqual( l_result, 2.0*7.0 )
  
  def test_backward( self ):
    ctx = context.Context()
    l_result = function.Mul.forward( ctx, 2.0, 7.0 )
    l_dx, l_dy = function.Mul.backward( ctx, 1.0 )
    self.assertEqual( l_dx, 7.0 )
    self.assertEqual( l_dy, 2.0 )

class TestReciprocal( unittest.TestCase ):
  def test_forward( self ):
    ctx = context.Context()
    l_result = function.Reciprocal.forward( ctx, 4.0 )
    self.assertAlmostEqual( l_result, 0.25 )
  
  def test_backward( self ):
    ctx = context.Context()
    l_result = function.Reciprocal.forward( ctx, 4.0 )
    l_dx = function.Reciprocal.backward( ctx, 1.0 )[0]
    self.assertAlmostEqual(l_dx, -(1.0 / 16.0))

class TestExp( unittest.TestCase ):
  def test_forward( self ):
    ctx = context.Context()
    l_result = function.Exp.forward( ctx, 2.0 )
    self.assertAlmostEqual( l_result, math.exp( 2.0 ) )
  
  def test_backward( self ):
    ctx = context.Context()
    l_result = function.Exp.forward( ctx, 2.0 )
    l_dx = function.Exp.backward( ctx, 1.0 )[0]
    self.assertAlmostEqual(l_dx, math.exp( 2.0 ))

class TestDiff( unittest.TestCase ):
  def test_forward( self ):
    ctx = context.Context()
    l_result = function.Diff.forward( ctx, 5.0, 3.0)
    self.assertAlmostEqual( l_result, 2.0 )
  
  def test_backward( self ):
    ctx = context.Context()
    l_result = function.Diff.forward( ctx, 5.0, 3.0 )
    l_dx, l_dy = function.Diff.backward( ctx, 1.0 )
    self.assertAlmostEqual(l_dx, 1.0)
    self.assertAlmostEqual(l_dy, -1.0)
  
class TestSin( unittest.TestCase ):
  def test_forward( self ):
    ctx = context.Context()
    l_result = function.Sin.forward( ctx, 2.0 )
    self.assertAlmostEqual( l_result, math.sin( 2.0 ) )
  
  def test_backward( self ):
    ctx = context.Context()
    l_result = function.Sin.forward( ctx, 2.0 )
    l_dx = function.Sin.backward( ctx, 1.0 )[0]
    self.assertAlmostEqual(l_dx, math.cos( 2.0 ))
  
class TestCos( unittest.TestCase ):
  def test_forward( self ):
    ctx = context.Context()
    l_result = function.Cos.forward( ctx, 2.0 )
    self.assertAlmostEqual( l_result, math.cos( 2.0 ) )
  
  def test_backward( self ):
    ctx = context.Context()
    l_result = function.Cos.forward( ctx, 2.0 )
    l_dx = function.Cos.backward( ctx, 1.0 )[0]
    self.assertAlmostEqual(l_dx, -math.sin( 2.0 ))

if __name__ == '__main__':
    unittest.main() 