#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestCtcInverse(unittest.TestCase):

  # not supported (yet) in Python # Contractors based on functions with two variables
  # not supported (yet) in Python x = ScalarVar()
  # not supported (yet) in Python y = ScalarVar()
  # not supported (yet) in Python f = AnalyticFunction([x,y], x-y)
  # not supported (yet) in Python c = CtcInverse(f, Interval(0.))

  def test_CtcInverse_1(self):

    x = VectorVar(1)
    f = AnalyticFunction([x], x[0]-42)
    c = CtcInverse(f, Interval(0.))
    a = IntervalVector(1)
    a = c.contract(a)
    self.assertTrue(a == IntervalVector([[42]]))

    x = VectorVar(2)
    f = AnalyticFunction([x], x[0]-x[1])
    c = CtcInverse(f, 0)
    self.assertTrue(c.fnc().input_size() == 2)

    b = IntervalVector(2)

    b = c.contract(b)
    self.assertTrue(b == IntervalVector(2))

    b = IntervalVector([[1,2],[1,2]])
    b = c.contract(b)
    self.assertTrue(b == IntervalVector([[1,2],[1,2]]))

    b = IntervalVector([[1,oo],[1,oo]])
    b = c.contract(b)
    self.assertTrue(b == IntervalVector([[1,oo],[1,oo]]))

    b = IntervalVector([[10],[10]])
    b = c.contract(b)
    self.assertTrue(b == IntervalVector([[10],[10]]))

    b = IntervalVector([[1,5],[8,9]])
    b = c.contract(b)
    self.assertTrue(b == IntervalVector.empty(2))

  def test_CtcInverse_2(self):

    x = VectorVar(2)
    f = AnalyticFunction([x], vec(x[0]-x[1]))
    c = CtcInverse(f, [[0]])

    b = IntervalVector(2)

    b = c.contract(b)
    self.assertTrue(b == IntervalVector(2))

    b = IntervalVector([[1,2],[1,2]])
    b = c.contract(b)
    self.assertTrue(b == IntervalVector([[1,2],[1,2]]))

    b = IntervalVector([[1,oo],[1,oo]])
    b = c.contract(b)
    self.assertTrue(b == IntervalVector([[1,oo],[1,oo]]))

    b = IntervalVector([[10],[10]])
    b = c.contract(b)
    self.assertTrue(b == IntervalVector([[10],[10]]))

    b = IntervalVector([[1,5],[8,9]])
    b = c.contract(b)
    self.assertTrue(b == IntervalVector.empty(2))
  
  def test_CtcInverse_3(self):

    x = VectorVar(2)
    f = AnalyticFunction([x], vec(x[0],sqr(x[0]/7.)+sqr(x[1]/3.)))
    c = CtcInverse(f, [[0,oo],[1]])

    b = IntervalVector([[0,0.8],[-2.28,-1.56]])
    b = c.contract(b)
    self.assertTrue(b == IntervalVector.empty(2))

    b = IntervalVector([[4,5.4],[-0.05,2.45]])
    b = c.contract(b)
    self.assertTrue(Approx(b,1e-2) == IntervalVector([[4.0397,5.40],[1.9089,2.45]]))

    b = IntervalVector([[6.25,6.7],[0.9,1.85]])
    b = c.contract(b)
    self.assertTrue(Approx(b,1e-2) == IntervalVector([[6.25,6.67],[0.9,1.351]]))

    b = IntervalVector([[-6,-5],[0,2]])
    b = c.contract(b)
    self.assertTrue(b == IntervalVector.empty(2))

    b = IntervalVector([[2,3],[-1,1]])
    b = c.contract(b)
    self.assertTrue(b == IntervalVector.empty(2))
  
  def test_CtcInverse_4(self):

    x = VectorVar(2)
    f = AnalyticFunction([x], vec(x[0],sqr(x[0]/7.)+sqr(x[1]/3.)))
    c = CtcInverse(f, CtcWrapper([[0,oo],[1]]))

    b = IntervalVector([[0,0.8],[-2.28,-1.56]])
    b = c.contract(b)
    self.assertTrue(b == IntervalVector.empty(2))

    b = IntervalVector([[4,5.4],[-0.05,2.45]])
    b = c.contract(b)
    self.assertTrue(Approx(b,1e-2) == IntervalVector([[4.0397,5.40],[1.9089,2.45]]))

    b = IntervalVector([[6.25,6.7],[0.9,1.85]])
    b = c.contract(b)
    self.assertTrue(Approx(b,1e-2) == IntervalVector([[6.25,6.67],[0.9,1.351]]))

    b = IntervalVector([[-6,-5],[0,2]])
    b = c.contract(b)
    self.assertTrue(b == IntervalVector.empty(2))

    b = IntervalVector([[2,3],[-1,1]])
    b = c.contract(b)
    self.assertTrue(b == IntervalVector.empty(2))
  
  def test_centered_form_scalar(self):

    x = ScalarVar()
    f = AnalyticFunction([x], x+x-42)

    c = CtcInverse(f, Interval(0.))
    a = Interval()
    c.contract(a)
    self.assertTrue(a == Interval(21))

    c = CtcInverse(f, Interval(0.), False)
    a = Interval()
    c.contract(a)
    self.assertTrue(a == Interval())

  def test_centered_form_multiple_scalars(self):

    x, y, z = ScalarVar(), ScalarVar(), ScalarVar()
    f = AnalyticFunction([x,y,z], vec(2*x-y, 2*y-z, 2*z-x))
    c = CtcInverse(f, IntervalVector([0.5,0.5,0.5]))
    a, b, d = Interval(-1,1), Interval(-1,1), Interval(-1,1)
    c.contract(a,b,d)
    self.assertTrue(a == Interval(0.5))
    self.assertTrue(b == Interval(0.5))
    self.assertTrue(d == Interval(0.5))

  def test_centered_form_multiple_vectors(self):

    x, y = VectorVar(2), VectorVar(2)
    f = AnalyticFunction([x,y], vec(x[0]*x[1]+y[0]*y[1], x[0]*y[1]-y[0]*x[1]))
    c = CtcInverse(f, IntervalVector([1.,0.5]))
    a = IntervalVector([[0,1],[1,1]])
    b = IntervalVector([[0,1],[1,1]])
    c.contract(a,b)
    self.assertTrue(a == IntervalVector([0.75,1.]))
    self.assertTrue(b == IntervalVector([0.25,1.]))

  def test_matrix_input_scalar_output(self):

    m = MatrixVar(2,3)
    f = AnalyticFunction([m], m(0,1))
    c = CtcInverse(f, Interval(2))
    x = IntervalMatrix([IntervalVector([[-10,10]]*3) for _ in range(2)])
    c.contract(x)
    self.assertTrue(x[0,1] == Interval(2))
    self.assertTrue(x[1,2] == Interval(-10,10))

  def test_matrix_input_vector_output(self):

    m = MatrixVar(2,3)
    f = AnalyticFunction([m], vec(m(0,0),m(1,2)))
    c = CtcInverse(f, IntervalVector([1,2]))
    x = IntervalMatrix([IntervalVector([[-10,10]]*3) for _ in range(2)])
    c.contract(x)
    self.assertTrue(x[0,0] == Interval(1))
    self.assertTrue(x[1,2] == Interval(2))

  def test_matrix_input_matrix_output(self):

    m = MatrixVar(2,3)
    f = AnalyticFunction([m], m)
    y = IntervalMatrix([IntervalVector([1,2,3]),IntervalVector([4,5,6])])
    c = CtcInverse(f, y)
    x = IntervalMatrix([IntervalVector([[-10,10]]*3) for _ in range(2)])
    c.contract(x)
    self.assertTrue(x == y)

  def test_vector_input_matrix_output(self):

    v = VectorVar(2)
    f = AnalyticFunction([v], mat(vec(v[0],v[1]),vec(v[1],v[0])))
    y = IntervalMatrix([IntervalVector([1,2]),IntervalVector([2,1])])
    c = CtcInverse(f, y)
    x = IntervalVector([[-10,10],[-10,10]])
    c.contract(x)
    self.assertTrue(x == IntervalVector([1,2]))

  def test_matrix_input_shape_mismatch(self):

    m = MatrixVar(2,3)
    c = CtcInverse(AnalyticFunction([m], m(0,0)), Interval(1))
    wrong_shape = IntervalMatrix([IntervalVector([[-10,10]]*2) for _ in range(3)])
    with self.assertRaises(ValueError):
      c.contract(wrong_shape)

  def test_matrix_mixed_inputs_not_supported(self):

    m = MatrixVar(2,3)
    s = ScalarVar()
    f = AnalyticFunction([m,s], m(0,0)+s)
    with self.assertRaises(ValueError):
      CtcInverse(f, Interval(1))

  def test_matrix_output_shape_mismatch(self):

    m = MatrixVar(2,3)
    wrong_shape = IntervalMatrix([IntervalVector([[-10,10]]*2) for _ in range(3)])
    with self.assertRaises(ValueError):
      CtcInverse(AnalyticFunction([m], m), wrong_shape)

  def test_ParabolasExample(self):

    u = ScalarVar()
    v = ScalarVar()
    a = VectorVar(4)
    a0 = VectorVar(3); a1 = VectorVar(3); a2 = VectorVar(3)
    b0 = VectorVar(3); b1 = VectorVar(3); b2 = VectorVar(3)

    b = AnalyticFunction([u,a0,a1,a2], sqr(1-u)*a0+2*u*(1-u)*a1+sqr(u)*a2)
    f = AnalyticFunction([u,v,a0,a1,a2,b0,b1,b2], (1-v)*b(u,a0,a1,a2)+v*b(u,b0,b1,b2))

    h = AnalyticFunction([a],
       f(a[0],a[1],
         IntervalVector([[0],[0],[0]]),IntervalVector([[1],[0],[1]]),IntervalVector([[2],[0],[0]]),
         IntervalVector([[0],[2],[0]]),IntervalVector([[1],[2],[1]]),IntervalVector([[2],[2],[0]]))
      -f(a[2],a[3],
         IntervalVector([[0],[0],[0.55]]),IntervalVector([[0],[1],[-0.45]]),IntervalVector([[0],[2],[0.55]]),
         IntervalVector([[2],[0],[0.55]]),IntervalVector([[2],[1],[-0.45]]),IntervalVector([[2],[2],[0.55]]))
    )

    ctc = CtcInverse(h, [0,0,0])
    x0 = [[0,1],[0,1],[0.05,0.18],[0.05,0.18]]
    #DefaultFigure.pave(x0, ctc, 0.001)
    p = pave(x0, ctc, 0.01)
    cs = p.connected_subsets()
    self.assertTrue(len(cs) == 1)
    hull = IntervalVector([[0.149199,0.182388],[0.148306,0.1826],[0.148054,0.18],[0.148732,0.18]])
    hull.inflate(1e-4)
    self.assertTrue(hull.is_superset(cs[0].box()))
  
  def test_automatic_deduction_issue245(self):

    s = VectorVar(2)
    f = AnalyticFunction([s],[sqr(s[0])+sqr(s[1])-1.0])

    # Compilation checks:

    s1 = SepInverse(f,IntervalVector([0.0])*0.)
    s2 = SepInverse(f,IntervalVector([0.0]))
    s3 = SepInverse(f,Vector([0.0]))
    s4 = SepInverse(f,[[0.0,0.0]])
    s5 = SepInverse(f,[0.0])
    s6 = SepInverse(f,[0])

    c1 = CtcInverse(f,IntervalVector([0.0])*0.)
    c2 = CtcInverse(f,IntervalVector([0.0]))
    c3 = CtcInverse(f,Vector([0.0]))
    c4 = CtcInverse(f,[[0.0,0.0]])
    c5 = CtcInverse(f,[0.0])
    c6 = CtcInverse(f,[0])

    inner,outer = s1.separate(IntervalVector(2))
    self.assertTrue(outer == IntervalVector([[-1,1],[-1,1]]))
    inner,outer = s2.separate(IntervalVector(2))
    self.assertTrue(outer == IntervalVector([[-1,1],[-1,1]]))
    inner,outer = s3.separate(IntervalVector(2))
    self.assertTrue(outer == IntervalVector([[-1,1],[-1,1]]))
    inner,outer = s4.separate(IntervalVector(2))
    self.assertTrue(outer == IntervalVector([[-1,1],[-1,1]]))
    inner,outer = s5.separate(IntervalVector(2))
    self.assertTrue(outer == IntervalVector([[-1,1],[-1,1]]))
    inner,outer = s6.separate(IntervalVector(2))
    self.assertTrue(outer == IntervalVector([[-1,1],[-1,1]]))

    x = IntervalVector(2); x = c1.contract(x)
    self.assertTrue(x == IntervalVector([[-1,1],[-1,1]]))
    x = IntervalVector(2); x = c2.contract(x)
    self.assertTrue(x == IntervalVector([[-1,1],[-1,1]]))
    x = IntervalVector(2); x = c3.contract(x)
    self.assertTrue(x == IntervalVector([[-1,1],[-1,1]]))
    x = IntervalVector(2); x = c4.contract(x)
    self.assertTrue(x == IntervalVector([[-1,1],[-1,1]]))
    x = IntervalVector(2); x = c5.contract(x)
    self.assertTrue(x == IntervalVector([[-1,1],[-1,1]]))
    x = IntervalVector(2); x = c6.contract(x)
    self.assertTrue(x == IntervalVector([[-1,1],[-1,1]]))

if __name__ ==  '__main__':
  unittest.main()
