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
    c.contract(x)
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
  
  def tests_CtcSegment_fromCodac1(self):

    # Test_CtcSegment01

    c = CtcSegment([[0],[0]], [[10],[20]])
    x = IntervalVector([[-5,50],[-5, 50]])
    c.contract(x)
    self.assertTrue(x == IntervalVector([[0,10],[0,20]]))

    # Test_CtcSegment02

    c = CtcSegment([[0],[0]], [[10],[20]])
    x = IntervalVector([[10,50],[20,50]])
    c.contract(x)
    self.assertTrue(x == IntervalVector([[10,10],[20,20]]))

    # Test_contract_degenerate

    c = CtcSegment([[5],[5]], [[5],[5]])
    x = IntervalVector([[-5,50],[-5,50]])
    c.contract(x)
    self.assertTrue(x == IntervalVector([[5,5],[5,5]]))

    # Test_contract_degenerate_x

    c = CtcSegment([[5],[5]], [[10],[5]])
    x = IntervalVector([[-50,50],[-50,50]])
    c.contract(x)
    self.assertTrue(x == IntervalVector([[5,10],[5,5]]))

    # Test_contract_degenerate_y

    c = CtcSegment([[-5],[-5]], [[-5],[15]])
    x = IntervalVector([Interval(-50, 50), Interval(-50, 50)])
    c.contract(x)
    self.assertTrue(x == IntervalVector([[-5,-5],[-5,15]]))

    # Test_contract_empty

    c = CtcSegment([[0],[0]], [[10],[20]])
    x = IntervalVector([[-5,-2],[-5,50]])
    c.contract(x)
    self.assertTrue(x.is_empty())

    # Test_call_with_empty_x

    c = CtcSegment([[0],[0]], [[10],[20]])
    x = IntervalVector(2)
    c.contract(x)
    self.assertTrue(x == IntervalVector([[0,10],[0,20]]))


if __name__ ==  '__main__':
  unittest.main()