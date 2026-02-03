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

  def tests_BoolInterval_manual(test):

    # [boolinterval-class-1-beg]
    BoolInterval.FALSE   # certainly false
    BoolInterval.TRUE    # certainly true
    BoolInterval.UNKNOWN # undetermined
    BoolInterval.EMPTY   # inconsistent / impossible
    # [boolinterval-class-1-end]

    # [boolinterval-class-2-beg]
    BoolInterval.UNKNOWN == BoolInterval.TRUE | BoolInterval.FALSE
    BoolInterval.EMPTY   == BoolInterval.TRUE & BoolInterval.FALSE
    # [boolinterval-class-2-end]


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
    w = y & z            # w = [1, 1]
    # [interval-class-6-end]
    test.assertTrue(Approx(x) == Interval(0,PI/2))
    test.assertTrue(Approx(y) == Interval(0,1))
    test.assertTrue(Approx(z) == Interval(1, math.exp(math.pi/2)))

    # [interval-class-7-beg]
    x = prev_float(1.0)
    # x = 0.9999999999999999
    # [interval-class-7-end]
    test.assertTrue(Approx(x) == 0.9999999999999999 and x != 1)


  def tests_IntervalVector_manual(test):

    # [intervalvector-class-1-beg]
    # Default box: [-oo,oo]^n
    x = IntervalVector(3)

    # Cube [-1,3]^2
    y = IntervalVector.constant(2,[-1,3])

    # From a list of bounds (each entry is [lb,ub])
    z = IntervalVector([[3,4],[4,6]]) # [3,4]×[4,6]

    # From a list of components (Intervals and/or bounds pairs)
    q = IntervalVector([y[1], z[0], [0,oo]]) # [-1,3]×[3,4]×[0,oo]

    # From a point (degenerate intervals)
    p = Vector([0.42,0.42,0.42])
    bp = IntervalVector(p) # [0.42,0.42]^3
    # [intervalvector-class-1-end]


    # [intervalvector-class-2-beg]
    x = IntervalVector.constant(2,[-1,3]) # [-1,3]^2
    x[1] = Interval(0,10) # [-1,3]×[0,10]

    # Iterating/accessing over components
    y = IntervalVector(2)
    for i, xi in enumerate(x):
      y[i] = xi

    # Unpacking (Python convenience)
    a,b = x
    assert a == x[0] and b == x[1]

    # Building a new box from existing components
    v = IntervalVector([*x, [3,6]]) # concatenation in Python
    # v == [[-1,3]×[0,10]×[3,6]]

    # Resize: new components are default-initialized ([-oo,oo])
    v.resize(4) # v == [[-1,3]×[0,10]×[3,6]×[-oo,oo]]
    s = v.subvector(1,2) # [0,10]×[3,6]
    # [intervalvector-class-2-end]


    # [intervalvector-class-3-beg]
    x = IntervalVector([[0,2],[-1,3]])

    n = x.size()        # dimension
    # Common box information (component-wise):
    lo = x.lb()         # Vector of lower bounds
    hi = x.ub()         # Vector of upper bounds
    m  = x.mid()        # Vector of midpoints
    d  = x.diam()       # Vector of diameters
    # [intervalvector-class-3-end]


    # [intervalvector-class-4-beg]
    x = IntervalVector([[0,1],[2,3]])
    y = IntervalVector([[-0.5,2],[1,4]])

    assert x.intersects(y)
    assert x.is_subset(y)
    # [intervalvector-class-4-end]


    # [intervalvector-class-5-beg]
    x = IntervalVector([[0,1],[2,3]])
    y = IntervalVector([[1,2],[0,1]])

    z1 = x+y
    z2 = 2*x
    z3 = x/2
    # [intervalvector-class-5-end]

if __name__ ==  '__main__':
  unittest.main()