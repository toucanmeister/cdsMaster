import unittest
import examples
import torch
import math

class ExamplesTest(unittest.TestCase):
    def test_example1_forward(self):
        f = examples.forward1(1.0, 2.0, 3.0)
        self.assertAlmostEqual(f, 5.0)
    
    def test_example1_backward(self):
        a, b, c = examples.backward1(1.0, 2.0, 3.0)
        self.assertAlmostEqual(a, 5.0)
        self.assertAlmostEqual(b, 1.0)
        self.assertAlmostEqual(c, 1.0)

    def test_example2_forward(self):
        w0 = 1.0
        w1 = 2.0
        w2 = 3.0
        x0 = 4.0
        x1 = 5.0
        f = examples.forward2(w0, w1, w2, x0, x1)
        self.assertAlmostEqual(f, 1.0 / (1.0 + math.exp(-(w0*x0 + w1*x1 + w2))))

    def test_example2_backward(self):
        w0 = torch.tensor(1.0, requires_grad=True)
        w1 = torch.tensor(2.0, requires_grad=True)
        w2 = torch.tensor(3.0, requires_grad=True)
        x0 = torch.tensor(4.0, requires_grad=True)
        x1 = torch.tensor(5.0, requires_grad=True)
        result = 1.0 / (1.0 + torch.exp(-(w0*x0 + w1*x1 + w2)))
        result.backward()

        dw0, dw1, dw2, dx0, dx1 = examples.backward2(1.0, 2.0, 3.0, 4.0, 5.0)

        self.assertAlmostEqual(dw0, w0.grad.item())
        self.assertAlmostEqual(dw1, w1.grad.item())
        self.assertAlmostEqual(dw2, w2.grad.item())
        self.assertAlmostEqual(dx0, x0.grad.item())
        self.assertAlmostEqual(dx1, x1.grad.item())
    
    def test_example3_forward(self):
        x = 1.5
        y = 0.75
        f = examples.forward3(x, y)
        self.assertAlmostEqual(f, (math.sin(x*y) + math.cos(x + y)) / math.exp(x - y))

    def test_example3_backward(self):
        x = torch.tensor(1.5, requires_grad=True)
        y = torch.tensor(0.75, requires_grad=True)
        result = (torch.sin(x*y) + torch.cos(x + y)) / torch.exp(x - y)
        result.backward()

        dx, dy = examples.backward3(1.5, 0.75)

        self.assertAlmostEqual(dx, x.grad.item())
        self.assertAlmostEqual(dy, y.grad.item())


if __name__ == '__main__':
    unittest.main() 