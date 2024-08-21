#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestCtcCartProd(unittest.TestCase):
  
  def tests_CtcCartProd(self):
    
    v = VectorVar(1)
    c1 = CtcInverse(AnalyticFunction([v], v), IntervalVector([[-1,1]]))
    c2 = CtcInverse(AnalyticFunction([v], v), IntervalVector([[5,6]]))

    a = IntervalVector(1)
    b = IntervalVector(1)
    c1.contract(a)
    self.assertTrue(a == IntervalVector([[-1,1]]))
    c2.contract(b)
    self.assertTrue(b == IntervalVector([[5,6]]))

    x = IntervalVector(2)
    c3 = CtcCartProd(c1,c2)
    c3.contract(x)
    self.assertTrue(x == IntervalVector([[-1,1],[5,6]]))

    y = IntervalVector(2)
    c4 = cart_prod(c1,c2)
    c4.contract(y)
    self.assertTrue(y == IntervalVector([[-1,1],[5,6]]))

if __name__ ==  '__main__':
  unittest.main()