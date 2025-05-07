#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestCtcInter(unittest.TestCase):
  
  def tests_CtcInter(self):
    
    c1 = CtcWrapper([[-10,10],[-2,2]])
    c2 = CtcWrapper([[-12,2],[0,4]])

    c3 = c1 & c2

    x = IntervalVector([[-oo,oo],[-oo,oo]])
    c3.contract(x)
    self.assertTrue(x == IntervalVector([[-10,2],[0,2]]))

    x = IntervalVector([[-oo,0],[-oo,0]])
    c3.contract(x)
    self.assertTrue(x == IntervalVector([[-10,0],[0,0]]))

    x = IntervalVector([[0,oo],[0,oo]])
    c3.contract(x)
    self.assertTrue(x == IntervalVector([[0,2],[0,2]]))

    x = IntervalVector([[0,0],[0,0]])
    c3.contract(x)
    self.assertTrue(x == IntervalVector.zero(2))

if __name__ ==  '__main__':
  unittest.main()