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

  # not supported (yet) in Python; # Contractors based on functions with two variables
  # not supported (yet) in Python; x = ScalarVar()
  # not supported (yet) in Python; y = ScalarVar()
  # not supported (yet) in Python; f = AnalyticFunction([x,y], x-y)
  # not supported (yet) in Python; c = CtcInverse(f, Interval(0.))

  def tests_CtcInverse_1(self):

    x = VectorVar(2)
    f = AnalyticFunction([x], x[0]-x[1])
    c = CtcInverse(f, 0)
    self.assertTrue(c.function().input_size() == 2)

    b = IntervalVector(2)

    c.contract(b)
    self.assertTrue(b == IntervalVector(2))

    b = IntervalVector([[1,2],[1,2]])
    c.contract(b)
    self.assertTrue(b == IntervalVector([[1,2],[1,2]]))

    b = IntervalVector([[1,oo],[1,oo]])
    c.contract(b)
    self.assertTrue(b == IntervalVector([[1,oo],[1,oo]]))

    b = IntervalVector([[10],[10]])
    c.contract(b)
    self.assertTrue(b == IntervalVector([[10],[10]]))

    b = IntervalVector([[1,5],[8,9]])
    c.contract(b)
    self.assertTrue(b == IntervalVector.empty(2))

  def tests_CtcInverse_2(self):

    x = VectorVar(2)
    f = AnalyticFunction([x], vec(x[0]-x[1]))
    c = CtcInverse(f, [[0]])

    b = IntervalVector(2)

    c.contract(b)
    self.assertTrue(b == IntervalVector(2))

    b = IntervalVector([[1,2],[1,2]])
    c.contract(b)
    self.assertTrue(b == IntervalVector([[1,2],[1,2]]))

    b = IntervalVector([[1,oo],[1,oo]])
    c.contract(b)
    self.assertTrue(b == IntervalVector([[1,oo],[1,oo]]))

    b = IntervalVector([[10],[10]])
    c.contract(b)
    self.assertTrue(b == IntervalVector([[10],[10]]))

    b = IntervalVector([[1,5],[8,9]])
    c.contract(b)
    self.assertTrue(b == IntervalVector.empty(2))
  
  def tests_CtcInverse_3(self):

    x = VectorVar(2)
    f = AnalyticFunction([x], vec(x[0],sqr(x[0]/7.)+sqr(x[1]/3.)))
    c = CtcInverse(f, [[0,oo],[1]])

    b = IntervalVector([[0,0.8],[-2.28,-1.56]])
    c.contract(b)
    self.assertTrue(b == IntervalVector.empty(2))

    b = IntervalVector([[4,5.4],[-0.05,2.45]])
    c.contract(b)
    self.assertTrue(Approx(b,1e-2) == IntervalVector([[4.0397,5.40],[1.9089,2.45]]))

    b = IntervalVector([[6.25,6.7],[0.9,1.85]])
    c.contract(b)
    self.assertTrue(Approx(b,1e-2) == IntervalVector([[6.25,6.67],[0.9,1.351]]))

    b = IntervalVector([[-6,-5],[0,2]])
    c.contract(b)
    self.assertTrue(b == IntervalVector.empty(2))

    b = IntervalVector([[2,3],[-1,1]])
    c.contract(b)
    self.assertTrue(b == IntervalVector.empty(2))
  
  def tests_CtcInverse_4(self):

    x = VectorVar(2)
    f = AnalyticFunction([x], vec(x[0],sqr(x[0]/7.)+sqr(x[1]/3.)))
    c = CtcInverse(f, CtcWrapper([[0,oo],[1]]))

    b = IntervalVector([[0,0.8],[-2.28,-1.56]])
    c.contract(b)
    self.assertTrue(b == IntervalVector.empty(2))

    b = IntervalVector([[4,5.4],[-0.05,2.45]])
    c.contract(b)
    self.assertTrue(Approx(b,1e-2) == IntervalVector([[4.0397,5.40],[1.9089,2.45]]))

    b = IntervalVector([[6.25,6.7],[0.9,1.85]])
    c.contract(b)
    self.assertTrue(Approx(b,1e-2) == IntervalVector([[6.25,6.67],[0.9,1.351]]))

    b = IntervalVector([[-6,-5],[0,2]])
    c.contract(b)
    self.assertTrue(b == IntervalVector.empty(2))

    b = IntervalVector([[2,3],[-1,1]])
    c.contract(b)
    self.assertTrue(b == IntervalVector.empty(2))
  
  def tests_ParabolasExample(self):

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
    #draw_while_paving(x0, ctc, 0.001)
    p = pave(x0, ctc, 0.01)
    cs = p.connected_subsets()
    self.assertTrue(len(cs) == 1)
    self.assertTrue(Approx(cs[0].box(),1e-4) == [[0.149199,0.182388],[0.148306,0.1826],[0.148054,0.18],[0.148732,0.18]])

if __name__ ==  '__main__':
  unittest.main()