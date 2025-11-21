#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *
import sys
import math

class TestPolygonSlice(unittest.TestCase):

  def test_polygon_slice_1(self):

    tdomain = create_tdomain([-1,3])
    x = SlicedTube(tdomain, Interval())
    v = SlicedTube(tdomain, Interval(-1,1))

    sx = x.first_slice()
    sv = v.first_slice()

    self.assertTrue(sx.t0_tf() == Interval(-1,3))
    self.assertTrue(sx.codomain() == Interval())
    x.set([-1,2],-1)
    x.set([-2,0],3)

    ctc_deriv = CtcDeriv(TimePropag.FWD_BWD, False)
    ctc_deriv.contract(sx, sv)

    p1 = sx.polygon_slice(sv)
    self.assertTrue(p1.box()[0] == Interval(-1.,3.))
    self.assertTrue(p1.box()[1] == Interval(-3.5,3.))

    p2 = ConvexPolygon([ [-1,-1],[-1,2],[0,3],[3,0],[3,-2],[1.5,-3.5] ])
    self.assertTrue(Approx(p1) == p2)

  def test_polygon_slice_2(self):

    tdomain = create_tdomain([-1,3])
    x = SlicedTube(tdomain, Interval(-5,3))
    v = SlicedTube(tdomain, Interval(-1))

    sx = x.first_slice()
    sv = v.first_slice()

    x.set([-1,3],-1)
    x.set([-5,0.5],3)

    ctc_deriv = CtcDeriv(TimePropag.FWD_BWD, False)
    ctc_deriv.contract(sx, sv)

    p1 = sx.polygon_slice(sv)
    self.assertTrue(p1.box()[0] == Interval(-1.,3.))
    self.assertTrue(p1.box()[1] == Interval(-5,3.))

    p2 = ConvexPolygon([ [-1,-1],[-1,3],[3,-1],[3,-5],[-1,1] ])
    self.assertTrue(Approx(p1) == p2)

  def test_polygon_slice_3(self):

    tdomain = create_tdomain([-1,3])
    x = SlicedTube(tdomain, Interval(-5,3))
    v = SlicedTube(tdomain, Interval(-1,1))

    sx = x.first_slice()
    sv = v.first_slice()

    x.set([1,3],-1)
    x.set([-4,-3],3)

    ctc_deriv = CtcDeriv(TimePropag.FWD_BWD, False)
    ctc_deriv.contract(sx, sv)

    p1 = sx.polygon_slice(sv)
    self.assertTrue(p1.box()[0] == Interval(-1.,3.))
    self.assertTrue(p1.box()[1] == Interval(-3,1.))

    #DefaultFigure::draw_polygon(p1)

    p2 = ConvexPolygon([ [-1,1],[3,-3] ])

    self.assertTrue(Approx(p1) == p2)
    self.assertTrue(p1.size() == 1)

  def test_polygon_slice_4(self):

    tdomain = create_tdomain([0,4])
    x = SlicedTube(tdomain, Interval(-1,7))
    v = SlicedTube(tdomain, Interval(-1.5,4))

    sx = x.first_slice()
    sv = v.first_slice()

    x.set([2,3],0)
    x.set([3,4],4)

    ctc_deriv = CtcDeriv(TimePropag.FWD_BWD, False)
    ctc_deriv.contract(sx, sv)

    p1 = sx.polygon_slice(sv)
    self.assertTrue(p1.box()[0] == Interval(0,4))
    self.assertTrue(p1.box()[1] == Interval(-1,7))

    p2 = ConvexPolygon([ [0,2],[0,3],[1,7],[2,7],[4,4],[4,3],[3,-1],[2,-1] ])
    self.assertTrue(Approx(p1) == p2)

  def test_polygon_slice_4b(self):

    tdomain = create_tdomain([4,8])
    x = SlicedTube(tdomain, Interval(-1,7))
    v = SlicedTube(tdomain, Interval(-0.75,-0.5))

    sx = x.first_slice()
    sv = v.first_slice()

    x.set([3,4],4)
    x.set([1],8)

    ctc_deriv = CtcDeriv(TimePropag.FWD_BWD, False)
    ctc_deriv.contract(sx, sv)

    p1 = sx.polygon_slice(sv)
    self.assertTrue(p1.box()[0] == Interval(4,8))
    self.assertTrue(p1.box()[1] == Interval(1,4))

    #DefaultFigure::draw_polygon(p1)

    p2 = ConvexPolygon([ [4,3],[4,4],[8,1] ])
    self.assertTrue(Approx(p1,1e-10) == p2)

  def test_polygon_slice_5(self):

    tdomain = create_tdomain([8,12])
    x = SlicedTube(tdomain, Interval(-1,7))
    v = SlicedTube(tdomain, Interval(-1./3.,1))

    sx = x.first_slice()
    sv = v.first_slice()

    x.set([1],8)
    x.set([1],12)

    ctc_deriv = CtcDeriv(TimePropag.FWD_BWD, False)
    ctc_deriv.contract(sx, sv)

    p1 = sx.polygon_slice(sv)
    self.assertTrue(p1.box()[0] == Interval(8,12))
    self.assertTrue(Approx(p1.box()[1]) == Interval(0,2))

    #DefaultFigure::draw_polygon(p1)

    p2 = ConvexPolygon([ [8,1],[9,2],[12,1],[11,0] ])
    self.assertTrue(Approx(p1,1e-10) == p2)

  def test_polygon_slice_6(self):

    tdomain = create_tdomain([12,14])
    x = SlicedTube(tdomain, Interval(-1,7))
    v = SlicedTube(tdomain, Interval(4.5)/2)

    sx = x.first_slice()
    sv = v.first_slice()

    x.set([1],12)
    x.set([5.5],14)

    ctc_deriv = CtcDeriv(TimePropag.FWD_BWD, False)
    ctc_deriv.contract(sx, sv)

    p1 = sx.polygon_slice(sv)
    self.assertTrue(p1.box()[0] == Interval(12,14))
    self.assertTrue(Approx(p1.box()[1]) == Interval(1,5.5))

    #DefaultFigure::draw_polygon(p1)

    p2 = ConvexPolygon([ [12,1],[14,5.5] ])
    self.assertTrue(Approx(p1,1e-10) == p2)

  def test_polygon_from_tubint_paper(self):

    tdomain = create_tdomain([4,5])
    x = SlicedTube(tdomain, (Interval(7)/2)|(Interval(17)/4))
    v = SlicedTube(tdomain, (-Interval(1)/2)|(Interval(1)/2))

    sx = x.first_slice()
    sv = v.first_slice()

    x.set((Interval(7)/2)|4,4)
    x.set([4],5)

    p = sx.polygon_slice(sv);
    self.assertTrue(p == ConvexPolygon([[4,4],[4,3.5],[5,4],[4.5,4.25]]))

    ctc_deriv = CtcDeriv(TimePropag.FWD_BWD, False)
    ctc_deriv.contract(sx,sv)
    p = sx.polygon_slice(sv);
    self.assertTrue(p == ConvexPolygon([[4,4],[4,3.5],[5,4],[4.5,4.25]]))

    y = Interval(41)/10
    self.assertTrue(Approx(x.invert(y, v, x.tdomain().t0_tf()),1e-10) == ((Interval(21)/5)|(Interval(24)/5)))


if __name__ ==  '__main__':
  unittest.main()