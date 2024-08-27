#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestSepInverse(unittest.TestCase):

  def tests_SepInverse(self):

    x = VectorVar(2)
    f = AnalyticFunction([x], vec(x[0],sqr(x[0]/7.)+sqr(x[1]/3.)))
    s = SepInverse(f, [[0,oo],[-oo,1]])

    b = IntervalVector([[0,0.8],[-2.28,-1.56]])
    xs = s.separate(b)
    self.assertTrue(xs.inner == IntervalVector([[0],[-2.28,-1.56]]))
    self.assertTrue(xs.outer == b)

    b = IntervalVector([[4,5.4],[-0.05,2.45]])
    xs = s.separate(b)
    self.assertTrue(Approx(xs.inner,1e-2) == IntervalVector([[4.039,5.40],[1.908,2.45]]))
    self.assertTrue(xs.outer == b)

    b = IntervalVector([[6.25,6.7],[0.9,1.85]])
    xs = s.separate(b)
    self.assertTrue(Approx(xs.inner,1e-2) == IntervalVector([[6.25,6.70],[0.9, 1.85]]))
    self.assertTrue(Approx(xs.inner,1e-2) == IntervalVector([[6.25,6.70],[0.9, 1.85]]))

    b = IntervalVector([[-6,-5],[0,2]])
    xs = s.separate(b)
    self.assertTrue(xs.inner == b)
    self.assertTrue(xs.outer == IntervalVector.empty(2))

    b = IntervalVector([[2,3],[-1,1]])
    xs = s.separate(b)
    self.assertTrue(xs.inner == IntervalVector.empty(2))
    self.assertTrue(xs.outer == b)

  def tests_SepInverse_other_test(self):

    x = VectorVar(2)
    f = AnalyticFunction([x], vec(sqr(x[0])+sqr(x[1])))
    s = SepInverse(f, [[0,1]])

    #pave([[-5,5],[-5,5]], s, 0.01);

    xs = s.separate(IntervalVector([[0.1,0.2],[0.1,0.2]])) # fully inside
    self.assertTrue(xs.inner.is_empty())
    self.assertTrue(xs.outer == IntervalVector([[0.1,0.2],[0.1,0.2]]))

    xs = s.separate(IntervalVector([[-0.2,0.2],[-0.2,0.2]])) # fully inside
    self.assertTrue(xs.inner.is_empty())
    self.assertTrue(xs.outer == IntervalVector([[-0.2,0.2],[-0.2,0.2]]))

    xs = s.separate(IntervalVector(2))
    self.assertTrue(xs.inner == IntervalVector(2))
    self.assertTrue(xs.outer == IntervalVector([[-1,1],[-1,1]]))

if __name__ ==  '__main__':
  unittest.main()