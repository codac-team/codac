#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

def test_inside_diamond(x):
  if (x[1] < x[0]+1) and (x[1] > x[0]-1) and (x[1] < -x[0]+1) and (x[1] > -x[0]-1):
    return BoolInterval.TRUE
  elif not(x[1] < x[0]+1) or not(x[1] > x[0]-1) or not(x[1] < -x[0]+1) or not(x[1] > -x[0]-1):
    return BoolInterval.FALSE
  else:
    return BoolInterval.UNKNOWN

class TestCtcCtcBoundary(unittest.TestCase):

  def tests_CtcCtcBoundary(self):

    ctc_bound_diamond = CtcSegment([[-1],[0]], [[0],[-1]]) | CtcSegment([[0],[-1]], [[1],[0]]) \
      | CtcSegment([[1],[0]], [[0],[1]]) | CtcSegment([[0],[1]], [[-1],[0]])

    ctc_diamond = CtcCtcBoundary(ctc_bound_diamond,test_inside_diamond)
    #draw_while_paving(IntervalVector([[-2,2],[-2,2]]), ctc_diamond, 0.1)

    x = IntervalVector(2)
    ctc_diamond.contract(x)
    #self.assertTrue(x == IntervalVector([[-1,1],[-1,1]]))

    x = IntervalVector([[0,10],[0,10]])
    ctc_diamond.contract(x)
    #self.assertTrue(x == IntervalVector([[0,1],[0,1]]))

    x = IntervalVector([[0.2,10],[0.2,10]])
    ctc_diamond.contract(x)
    #self.assertTrue(x == IntervalVector([[0.2,0.8],[0.2,0.8]]))

    x = IntervalVector([[0.5,10],[0.5,10]])
    ctc_diamond.contract(x)
    #self.assertTrue(x == IntervalVector([[0.5],[0.5]]))

if __name__ ==  '__main__':
  unittest.main()