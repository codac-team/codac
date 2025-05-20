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

class TestCtcDistManual(unittest.TestCase):

  def tests_CtcDist_manual(test):

    # [ctcdist-1-beg]
    x = IntervalVector([[2,5],[2,6],[0,0],[0,0],[1,3]])
    c = CtcDist()
    x = c.contract(x)
    # x = [ [2, 2.23607] ; [2, 2.23607] ; <0, 0> ; <0, 0> ; [2.82842, 3] ]
    # [ctcdist-1-end]

    test.assertTrue(Approx(x,1e-5) == IntervalVector([[2,2.23607],[2,2.23607],[0],[0],[2.82842,3]]))


    # [ctcdist-2-beg]
    a1,a2,b1,b2,d = Interval(2,5),Interval(2,6),Interval(0),Interval(0),Interval(1,3)
    c = CtcDist()
    a1,a2,b1,b2,d = c.contract(a1,a2,b1,b2,d)
    # a1 = [2, 2.23607] ; a2 = [2, 2.23607] ; b1 = <0, 0> ; b2 = <0, 0> ; d = [2.82842, 3]
    # [ctcdist-2-end]

    test.assertTrue(Approx(a1,1e-5) == Interval([2,2.23607]))
    test.assertTrue(Approx(a2,1e-5) == Interval([2,2.23607]))
    test.assertTrue(Approx(b1,1e-5) == Interval([0]))
    test.assertTrue(Approx(b2,1e-5) == Interval([0]))
    test.assertTrue(Approx(d,1e-5) == Interval([2.82842,3]))

    # [ctcdist-3-beg]
    b1, b2 = Vector([1,2]), Vector([3.6,2.4])
    y1, y2 = Interval(2,2.4), Interval(4.1,4.5)

    c = CtcDist() # generic distance contractor
    c1 = CtcProj(c, [0,1], cart_prod(b1,y1)) # projection involving data
    c2 = CtcProj(c, [0,1], cart_prod(b2,y2))
    # [ctcdist-3-end]

    if False: # graphic outputs is disabled for tests

      # [ctcdist-4-beg]
      draw_while_paving( # calling a paver algorithm for graphic output
        [[-3,5],[-2.5,5.5]], # initial 2d box
        c1 & c2, # intersection of the two projected contractors
        0.1 # paver precision
      )

      DefaultFigure.draw_ring(b1, y1)
      DefaultFigure.draw_ring(b2, y2)
      DefaultFigure.draw_circle([0,0], 0.1, [Color.red(),Color.red()])
      # [ctcdist-4-end]

    #DefaultFigure.draw_circle(b1, y1.lb()); DefaultFigure.draw_circle(b1, y1.ub())
    #DefaultFigure.draw_circle(b2, y2.lb()); DefaultFigure.draw_circle(b2, y2.ub())

if __name__ ==  '__main__':
  unittest.main()