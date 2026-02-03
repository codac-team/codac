#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2026
#  \author     Simon Rohou
#  \copyright  Copyright 2026 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import sys, os
import unittest
import math
from codac import *

class TestCtcAnalyticManual(unittest.TestCase):

  def tests_CtcInverse_manual(test):

    # [ctcinv-1-beg]
    # Example of Himmelblau's function
    a = 11; b = 7
    x = VectorVar(2)
    f = AnalyticFunction([x], sqr(sqr(x[0])+x[1]-a)+sqr(x[0]+sqr(x[1])-b))
    c = CtcInverse(f, 50)
    # [ctcinv-1-end]

    # [ctcinv-2-beg]
    DefaultFigure.pave([[-6,6],[-6,6]], c, 1e-2)
    # [ctcinv-2-end]

    # [ctcinv-3-beg]
    s = SepInverse(f, [0,50])
    DefaultFigure.pave([[-6,6],[-6,6]], s, 1e-2)
    # [ctcinv-3-end]

    # [ctcinv-4-beg]
    cu = CtcInverse(f,50) | CtcInverse(f,150) | CtcInverse(f,250)
    DefaultFigure.pave([[-6,6],[-6,6]], cu, 1e-2)
    # [ctcinv-4-end]

    # [ctcinv-5-beg]
    x = VectorVar(2)
    f = AnalyticFunction([x], x[0])

    # Enforce the first component not in [0,1]
    c = CtcInverseNotIn(f, [0,1])

    y = IntervalVector([[0.5,3],[-1,1]])
    c.contract(y) # [[1,3],[-1,1]]
    # Only the first component is constrained by the not-in condition
    # [ctcinv-5-end]

    test.assertTrue(y == IntervalVector([[1,3],[-1,1]]))

    # [ctcinv-6-beg]
    x = VectorVar(2)
    f = AnalyticFunction([x], x[0]-x[1])
    c = CtcInverse(f, 0)
    assert c.fnc().input_size() == 2
    assert c.fnc().output_size() == 1
    # [ctcinv-6-end]

if __name__ ==  '__main__':
  unittest.main()