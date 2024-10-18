#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestCtcAction(unittest.TestCase):
  
  def tests_CtcAction(self):
    
    x = VectorVar(2)
    c1 = CtcInverse(AnalyticFunction([x], x-IntervalVector([[1,5],[8,6]])), IntervalVector([[0],[0]]))

    b = IntervalVector(2)
    c1.contract(b)
    self.assertTrue(b == IntervalVector([[1,5],[8,6]]))

    c2 = CtcAction(c1, [2,-1])
    b = IntervalVector(2)
    c2.contract(b)
    self.assertTrue(b == IntervalVector([[8,6],[-5,1]]))

if __name__ ==  '__main__':
  unittest.main()