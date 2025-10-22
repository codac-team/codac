#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import sys, os
import unittest
import math
from codac import *

class TestIntervalManual(unittest.TestCase):

  def tests_Interval_manual(test):

    # [interval-class-1-beg]
    x = Interval(3, 5)                          # [3.0, 5.0]
    x = Interval(4.2)                           # [4.2, 4.2]
    x = Interval()                              # [-∞, ∞]
    x = Interval(-oo, -10)                      # [-∞, -10]
    x = Interval([2.0, -3.0])                   # [-3.0, 2.0]
    x = Interval([3.14])                        # [3.14, 3.14]
    # [interval-class-1-end]

    # [interval-class-2-beg]
    x = Interval()                              # [-∞,∞] (default value)
    x = Interval.empty()                        # ∅
    x = Interval.pi()                           # [π]
    x = Interval.two_pi()                       # [2π]
    x = Interval.half_pi()                      # [π/2]
    x = Interval.zero()                         # [0]
    x = Interval.one()                          # [1]
    # [interval-class-2-end]

    # [interval-class-3-beg]
    x = Interval.pi()                           # [π]
    # x = [3.141592653589793, 3.141592653589794]
    # [interval-class-3-end]

    # [interval-class-4-beg]
    x.lb()      # lower bound
    x.ub()      # upper bound
    x.mid()     # midpoint
    x.rad()     # radius
    x.diam()    # diameter
    x.mag()     # magnitude
    x.mig()     # mignitude
    x.smag()    # signed magnitude
    x.smig()    # signed mignitude
    x.size()    # dimension (always 1)
    # [interval-class-4-end]

    # [interval-class-5-beg]
    x = Interval(2,3)
    y = Interval(1,2)
    z = x + y              # [3, 5]
    z = x - 1              # [1, 2]
    z = 2 * x              # [4, 6]
    z = x / y              # [1, 3]
    # [interval-class-5-end]
    test.assertTrue((x + y) == Interval(3,5))
    test.assertTrue((x - 1) == Interval(1,2))
    test.assertTrue((2 * x) == Interval(4,6))
    test.assertTrue((x / y) == Interval(1,3))

    # [interval-class-6-beg]
    x = Interval.half_pi()
    x |= 0               # x = [0, π/2]
    y = sin(x)           # y = [0, 1]
    z = exp(x)           # z = [1, e^(π/2)]
    # [interval-class-6-end]
    test.assertTrue(Approx(x) == Interval(0,PI/2))
    test.assertTrue(Approx(y) == Interval(0,1))
    test.assertTrue(Approx(z) == Interval(1, math.exp(math.pi/2)))

    # [interval-class-7-beg]
    x = prev_float(1.0)
    # x = 0.9999999999999999
    # [interval-class-7-end]
    test.assertTrue(Approx(x) == 0.9999999999999999 and x != 1)

if __name__ ==  '__main__':
  unittest.main()