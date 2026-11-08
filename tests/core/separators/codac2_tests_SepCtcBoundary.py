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

class TestSepCtcBoundary(unittest.TestCase):

  def test_SepCtcBoundary(self):

    ctc_bound_diamond = CtcSegment([[-1],[0]], [[0],[-1]]) | CtcSegment([[0],[-1]], [[1],[0]]) \
      | CtcSegment([[1],[0]], [[0],[1]]) | CtcSegment([[0],[1]], [[-1],[0]])

    sep_diamond = SepCtcBoundary(ctc_bound_diamond,test_inside_diamond)

    x0 = IntervalVector([[-2,2],[-2,2]])
    #DefaultFigure.pave(x0, sep_diamond, 0.1)

    s = PavingStyle()
    s.boundary = [ Color.black(),Color.black() ]
    s.outside = [ Color.none(),Color.none() ]
    s.inside = [ Color.none(),Color.none() ]

    #DefaultFigure.pave(x0, ctc_bound_diamond, 0.01, s)

    x = IntervalVector(2)
    inner,outer = sep_diamond.separate(IntervalVector(2))
    self.assertTrue(inner == IntervalVector(2))
    self.assertTrue(outer == IntervalVector([[-1,1],[-1,1]]))

    x = IntervalVector([[0,10],[0,10]])
    inner,outer = sep_diamond.separate(x)
    self.assertTrue(inner == x)
    self.assertTrue(outer == IntervalVector([[0,1],[0,1]]))

    x = IntervalVector([[0.2,10],[0.2,10]])
    inner,outer = sep_diamond.separate(x)
    self.assertTrue(inner == x)
    self.assertTrue(outer == IntervalVector([[0.2,0.8],[0.2,0.8]]))

    x = IntervalVector([[0.5,10],[0.5,10]])
    inner,outer = sep_diamond.separate(x)
    self.assertTrue(inner == x)
    self.assertTrue(outer == IntervalVector([[0.5],[0.5]]))

if __name__ ==  '__main__':
  unittest.main()