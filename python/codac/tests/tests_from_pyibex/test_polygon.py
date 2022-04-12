#!/usr/bin/env python

# Codac tests
# Originated from the former pyIbex library (Benoît Desrochers)
# ---------------------------------------------------------------------------
# \date      2022
# \author    Benoît Desrochers
# \copyright Copyright 2022 Codac Team
# \license   This program is distributed under the terms of
#            the GNU Lesser General Public License (LGPL).

import unittest
from codac import *
from codac.unsupported import *

class TestPolygon(unittest.TestCase):

  def test_CtcSegment01(self):
    c = CtcSegment(0,0,10,20);
    box = IntervalVector([[-5,50],  [-5,50]]);
    resbox = IntervalVector([[0,10], [0, 20]]);
    c.contract(box);
    self.assertEqual(box , resbox)

  def test_CtcSegment02(self):
    c = CtcSegment(0,0,10,20);
    box = IntervalVector([[10,50],  [20,50]]);
    resbox = IntervalVector([[10,10], [20, 20]]);
    c.contract(box);
    self.assertEqual(box , resbox)

  def test_contract_degenerate(self):
    c = CtcSegment(5, 5, 5, 5);
    box = IntervalVector([[-5,50],  [-5,50]]);
    resbox = IntervalVector([[5, 5], [5, 5]]);
    c.contract(box);
    self.assertEqual(box , resbox)

  def test_contract_empty(self):
    c = CtcSegment(0,0,10,20);
    box = IntervalVector([[-5,-2],  [-5,50]]);
    resbox = IntervalVector.empty(2);
    c.contract(box);
    self.assertEqual(box , resbox)

  def test_call_with_empty_box(self):
    c = CtcSegment(0,0,10,20)
    box = IntervalVector(2, Interval.ALL_REALS)
    resbox = IntervalVector([[0,10], [0, 20]]);
    c.contract(box);
    self.assertEqual(box , resbox)

class TestSepPolygon(unittest.TestCase):
  def test_SIVIA(self):
    l = [[-1, -1], [1, -2], [0, 1]]
    sep = SepPolygon(l)
    f = Function('x', 'y', '(2*x-y, 2*y - x)')
    finv = Function('x', 'y', '(1/3.0*(2*x+y),1/3.0*(2*y+x))')
    Xin = IntervalVector(2, Interval(-10, 10))
    Xout = IntervalVector(2, Interval(-10, 10))

    eps = 0.05
    # todo: pySIVIA(Xin, sep, eps, figureName='Sep', draw_boxes=False)

if __name__ == '__main__':

	unittest.main()