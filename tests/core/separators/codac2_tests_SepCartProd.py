#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestSepCartProd(unittest.TestCase):
  
  def tests_SepCartProd(self):
    
    v = VectorVar(1)
    s1 = SepInverse(AnalyticFunction([v], v), IntervalVector([[-1,1]]))
    s2 = SepInverse(AnalyticFunction([v], v), IntervalVector([[5,6]]))

    x = IntervalVector([[0,8],[5.5,oo]])
    s3 = SepCartProd(s1,s2)
    inner,outer = s3.separate(x)
    self.assertTrue(inner == IntervalVector([[1,8],[6,oo]]))
    self.assertTrue(outer == IntervalVector([[0,1],[5.5,6]]))

    x = IntervalVector([[0,8],[5.5,oo]])
    s4 = cart_prod(s1,s2)
    inner,outer = s4.separate(x)
    self.assertTrue(inner == IntervalVector([[1,8],[6,oo]]))
    self.assertTrue(outer == IntervalVector([[0,1],[5.5,6]]))

if __name__ ==  '__main__':
  unittest.main()