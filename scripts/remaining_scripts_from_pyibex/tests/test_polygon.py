#!/usr/bin/env python
#============================================================================
#                                P Y I B E X
# File        : test_polygon.py
# Author      : Benoit Desrochers
# Copyright   : Benoit Desrochers
# License     : See the LICENSE file
# Created     : Dec 28, 2014
#============================================================================
from pyibex import Interval, IntervalVector, Function, SepInverse, pySIVIA

from pyibex.geometry import *

import unittest

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
    # l = [[-1, 1, 0], [-1, -2, 1]]
    l = [[-1, -1], [1, -2], [0, 1]]


    sep = SepPolygon(l)
    f = Function('x', 'y', '(2*x-y, 2*y - x)')
    finv = Function('x', 'y', '(1/3.0*(2*x+y),1/3.0*(2*y+x))')
    # sepInv = SepInverse(sep, f)
    # sepTrans = SepTransform(sep, f, finv)

    Xin = IntervalVector(2, Interval(-10, 10))
    Xout = IntervalVector(2, Interval(-10, 10))

    eps = 0.05
    # sep.separate(Xin, Xout)
    pySIVIA(Xin, sep, eps, figureName='Sep', draw_boxes=False, )
    # SIVIA(Xin, sepInv, figureName='SepInv',  eps=eps)
    # SIVIA(Xin, sepTrans, eps=eps, figureName='sepTransfrom')
if __name__ == '__main__':

	unittest.main()
