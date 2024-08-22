#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestCtcSegment(unittest.TestCase):
  
  def tests_CtcSegment(self):

    x = IntervalVector(2)
    c = CtcSegment([[3],[-1]],[[3],[4]])

    x = IntervalVector(2)
    c.contract(x)
    self.assertTrue(x == IntervalVector([[3,3],[-1,4]]))

    x = IntervalVector([[3.0,3.16],[2.5,3.2]])
    c.contract(x);
    self.assertTrue(x == IntervalVector([[3,3],[2.5,3.2]]))

    x = IntervalVector([[3.02,3.16],[2.5,3.2]])
    c.contract(x)
    self.assertTrue(x.is_empty())

    x = IntervalVector(2)
    c = CtcSegment([[-1],[-1]],[[1],[1]])

    x = IntervalVector(2)
    c.contract(x)
    self.assertTrue(x == IntervalVector([[-1,1],[-1,1]]))

    x = IntervalVector([[-oo,oo],[0]])
    c.contract(x)
    self.assertTrue(x == IntervalVector([[0],[0]]))

    x = IntervalVector([[0,oo],[0,oo]])
    c.contract(x)
    self.assertTrue(x == IntervalVector([[0,1],[0,1]]))

    x = IntervalVector([[2,oo],[2,oo]])
    c.contract(x)
    self.assertTrue(x.is_empty())

if __name__ ==  '__main__':
  unittest.main()