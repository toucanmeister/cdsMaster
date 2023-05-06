import unittest
import math
import torch
import node

class ExampleTest( unittest.TestCase ):
    def test_f( self ):
        x = node.Node(1.0)
        y = node.Node(2.0)
        z = node.Node(3.0)

        f = x * (y + z)
        self.assertAlmostEqual(f.m_value, 5.0)

        f.backward(1.0)
        self.assertAlmostEqual(x.m_grad, 5.0)
        self.assertAlmostEqual(y.m_grad, 1.0)
        self.assertAlmostEqual(z.m_grad, 1.0)

    def test_g( self ):
        w0 = node.Node( 1.0 )
        w1 = node.Node( 2.0 )
        w2 = node.Node( 3.0 )
        x0 = node.Node( 4.0 )
        x1 = node.Node( 5.0 )
        f = node.Node(1.0) / (node.Node(1.0) + node.exp(-(w0*x0 + w1*x1 + w2)))
        self.assertAlmostEqual( f.m_value, 1.0 / (1.0 + math.exp(-(w0.m_value*x0.m_value + w1.m_value*x1.m_value + w2.m_value))) )
        f.backward( 1.0 )
        
        tw0 = torch.tensor(w0.m_value, requires_grad=True)
        tw1 = torch.tensor(w1.m_value, requires_grad=True)
        tw2 = torch.tensor(w2.m_value, requires_grad=True)
        tx0 = torch.tensor(x0.m_value, requires_grad=True)
        tx1 = torch.tensor(x1.m_value, requires_grad=True)
        result = 1.0 / (1.0 + torch.exp(-(tw0*tx0 + tw1*tx1 + tw2)))
        result.backward()

        self.assertAlmostEqual(w0.m_grad, tw0.grad.item())
        self.assertAlmostEqual(w1.m_grad, tw1.grad.item())
        self.assertAlmostEqual(w2.m_grad, tw2.grad.item())
        self.assertAlmostEqual(x0.m_grad, tx0.grad.item())
        self.assertAlmostEqual(x1.m_grad, tx1.grad.item())
    
    def test_h( self ):
        xv = 1.5
        yv = 0.75
        x = node.Node(xv)
        y = node.Node(yv)
        f = (node.sin(x*y) + node.cos(x+y)) / node.exp(x-y)
        self.assertAlmostEqual(f.m_value, (math.sin(xv*yv) + math.cos(xv + yv)) / math.exp(xv - yv))
        f.backward(1.0)

        tx = torch.tensor(xv, requires_grad=True)
        ty = torch.tensor(yv, requires_grad=True)
        result = (torch.sin(tx*ty) + torch.cos(tx + ty)) / torch.exp(tx - ty)
        result.backward()

        self.assertAlmostEqual(x.m_grad, tx.grad.item())
        self.assertAlmostEqual(y.m_grad, ty.grad.item())


if __name__ == '__main__':
    unittest.main()