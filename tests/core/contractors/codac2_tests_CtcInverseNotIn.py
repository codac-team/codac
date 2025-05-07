#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestCtcInverseNotIn(unittest.TestCase):

  # Multivariable functions not supported (yet) for CtcInverseNotInNotIn: ScalarVar x,y
  # Multivariable functions not supported (yet) for CtcInverseNotInNotIn: AnalyticFunction f { {x,y}, x-y }
  # Multivariable functions not supported (yet) for CtcInverseNotInNotIn: CtcInverseNotInNotIn<Interval> c(f, Interval(0.))

  def tests_CtcInverseNotIn_2(self):

    # Test not really relevant, but copied from CtcInverse:

    x = VectorVar(2)
    f = AnalyticFunction([x], vec(x[0]-x[1]))
    c = CtcInverseNotIn(f, [[0]])

    #pave(IntervalVector([[-10,10],[-10,10]]), c, 0.1)

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
    self.assertTrue(b == IntervalVector([[1,5],[8,9]]))
  
  def tests_CtcInverseNotIn_3(self):

    x = VectorVar(2)
    f = AnalyticFunction([x], vec(x[0],sqr(x[0]/7.)+sqr(x[1]/3.)))
    c = CtcInverseNotIn(f, [[0,oo],[1]])

    b = IntervalVector([[0,0.8],[-2.28,-1.56]])
    c.contract(b)
    self.assertTrue(b == IntervalVector([[0,0.8],[-2.28,-1.56]]))

    b = IntervalVector([[4,5.4],[-0.05,2.45]])
    c.contract(b)
    self.assertTrue(Approx(b,1e-2) == IntervalVector([[4,5.4],[-0.05,2.45]]))

    b = IntervalVector([[6.25,6.7],[0.9,1.85]])
    c.contract(b)
    self.assertTrue(Approx(b,1e-2) == IntervalVector([[6.25,6.70],[0.9,1.85]]))

    b = IntervalVector([[-6,-5],[0,2]])
    c.contract(b)
    self.assertTrue(b == IntervalVector([[-6,-5],[0,2]]))

    b = IntervalVector([[2,3],[-1,1]])
    c.contract(b)
    self.assertTrue(b == IntervalVector([[2,3],[-1,1]]))
  
  # disabled: def tests_CtcInverseNotIn_3_CtcWrapper(self):
  # disabled:   
  # disabled:   x = VectorVar(2)
  # disabled:   f = AnalyticFunction([x], vec(x[0],sqr(x[0]/7.)+sqr(x[1]/3.)))
  # disabled:   c = CtcInverseNotIn(f, CtcWrapper([[0,oo],[1]]))
  # disabled:   
  # disabled:   b = IntervalVector([[0,0.8],[-2.28,-1.56]])
  # disabled:   c.contract(b)
  # disabled:   self.assertTrue(b == IntervalVector([[0,0.8],[-2.28,-1.56]]))
  # disabled:   
  # disabled:   b = IntervalVector([[4,5.4],[-0.05,2.45]])
  # disabled:   c.contract(b)
  # disabled:   self.assertTrue(Approx(b,1e-2) == IntervalVector([[4,5.4],[-0.05,2.45]]))
  # disabled:   
  # disabled:   b = IntervalVector([[6.25,6.7],[0.9,1.85]])
  # disabled:   c.contract(b)
  # disabled:   self.assertTrue(Approx(b,1e-2) == IntervalVector([[6.25,6.70],[0.9,1.85]]))
  # disabled:   
  # disabled:   b = IntervalVector([[-6,-5],[0,2]])
  # disabled:   c.contract(b)
  # disabled:   self.assertTrue(b == IntervalVector([[-6,-5],[0,2]]))
  # disabled:   
  # disabled:   b = IntervalVector([[2,3],[-1,1]])
  # disabled:   c.contract(b)
  # disabled:   self.assertTrue(b == IntervalVector([[2,3],[-1,1]]))
  
  def tests_CtcInverseNotIn_4(self):

    v = VectorVar(1)
    c = CtcInverseNotIn(AnalyticFunction([v], v), [[-1,1]])
    x = IntervalVector(1)

    c.contract(x)
    self.assertTrue(x == IntervalVector(1))

    x = IntervalVector([[0,8]])
    c.contract(x)
    self.assertTrue(x == IntervalVector([[1,8]]))
  
  def tests_CtcInverseNotIn_5(self):

    v = VectorVar(2)
    c = CtcInverseNotIn(AnalyticFunction([v], v), IntervalVector([[-1,1],[5,6]]))
    x = IntervalVector(2)

    c.contract(x)
    self.assertTrue(x == IntervalVector(2))

    x = IntervalVector([[0,8],[5.5,oo]])
    c.contract(x)
    self.assertTrue(x == IntervalVector([[0,8],[5.5,oo]])) # unchanged

if __name__ ==  '__main__':
  unittest.main()