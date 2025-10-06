#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestOctaSym(unittest.TestCase):
  
  def test_OctaSym(self):

    a = OctaSym([-2,1])
    self.assertTrue(a.invert() == OctaSym([2,-1]))

    x = IntervalVector([[-1,1],[5,6]])
    self.assertTrue(a(x) == IntervalVector([[-6,-5],[-1,1]]))
    self.assertTrue(a.invert()(a(x)) == x)

    b = OctaSym([2,-1])
    self.assertTrue(b.invert() == OctaSym([-2,1]))
    self.assertTrue(b*b == OctaSym([-1,-2]))

    c = OctaSym([-2,1,3])
    self.assertTrue(c.permutation_matrix() == Matrix([[0,-1,0],[1,0,0],[0,0,1]]))

  def test_OctaSym_as_operator(self):

    a = OctaSym([3,1,-2])
    x = VectorVar(3)
    f = AnalyticFunction([x], a(2*x))
    self.assertTrue(f.eval(IntervalVector([[1],[2],[3]])) == IntervalVector([[6],[2],[-4]]))
    self.assertTrue(f.eval(IntervalVector([[-oo,oo],[-oo,oo],[-oo,oo]])) == IntervalVector(3))
    self.assertTrue(f.eval(IntervalVector.empty(3)) == IntervalVector.empty(3))

if __name__ ==  '__main__':
  unittest.main()