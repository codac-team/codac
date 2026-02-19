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


    # # Generates the documentation figure:

    # sty = PavingStyle(
    #   [Color.black(),Color.black()],
    #   [Color.none(),Color.none()],
    #   [Color.none(),Color.none()])

    # l = [
    #   IntervalVector([[0,3],[2,4]]),
    #   IntervalVector([[0,3.5],[0,1]]),
    #   IntervalVector([[-2,-1],[0,3]]),
    #   IntervalVector([[0,2],[-3,-2]]),
    #   IntervalVector([[-5,-1],[-4.5,-1]]),
    #   IntervalVector([[3,4],[-3.5,-0.5]])
    # ]

    # fig = Figure2D("My figure", GraphicOutput.VIBES | GraphicOutput.IPE)
    # fig.set_axes(axis(0,[-6,6]), axis(1,[-6,6]))

    # for xi in l:
    #   fig.draw_box(xi, [Color.blue(),Color.blue(0.1)])
    #   c1 = CtcFixpoint(c)
    #   xi = c.contract(xi)
    #   fig.draw_box(xi, [Color.blue(),Color.white()])
    #   c1 = CtcFixpoint(c,0)
    #   xi = c1.contract(xi)
    #   fig.draw_box(xi, [Color.red(),Color.none()])

    # fig.pave([[-6,6],[-6,6]], c, 5e-3, sty)


    # [ctcinv-2-beg]
    z = IntervalVector([[0,3.5],[0,1]])
    DefaultFigure.draw_box(z, [Color.blue(),Color.blue(.1)]) # prior to contraction
    z = c.contract(z)
    DefaultFigure.draw_box(z, [Color.blue(),Color.white()]) # after one CtcInverse contraction
    # z == [ [1.84, 3.5] ; [0, 1] ]

    # Combining CtcInverse with a CtcFixpoint:
    cfix = CtcFixpoint(c)
    z = c.contract(z)
    DefaultFigure.draw_box(z, Color.red()) # after a fixed point contraction
    # z == [ [1.84, 2.483] ; [0, 1] ]
    # [ctcinv-2-end]

    # [ctcinv-3-beg]
    DefaultFigure.pave([[-6,6],[-6,6]], c, 1e-2)
    # [ctcinv-3-end]

    # [ctcinv-4-beg]
    s = SepInverse(f, [0,50])
    DefaultFigure.pave([[-6,6],[-6,6]], s, 1e-2)
    # [ctcinv-4-end]

    # [ctcinv-5-beg]
    cu = CtcInverse(f,50) | CtcInverse(f,150) | CtcInverse(f,250)
    DefaultFigure.pave([[-6,6],[-6,6]], cu, 1e-2)
    # [ctcinv-5-end]

    # [ctcinv-6-beg]
    x = VectorVar(2)
    f = AnalyticFunction([x], x[0])

    # Enforce the first component not in [0,1]
    c = CtcInverseNotIn(f, [0,1])

    y = IntervalVector([[0.5,3],[-1,1]])
    c.contract(y) # [[1,3],[-1,1]]
    # Only the first component is constrained by the not-in condition
    # [ctcinv-6-end]

    test.assertTrue(y == IntervalVector([[1,3],[-1,1]]))

    # [ctcinv-7-beg]
    x = VectorVar(2)
    f = AnalyticFunction([x], x[0]-x[1])
    c = CtcInverse(f, 0)
    assert c.fnc().input_size() == 2
    assert c.fnc().output_size() == 1
    # [ctcinv-7-end]

if __name__ ==  '__main__':
  unittest.main()