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

class TestSlice(unittest.TestCase):

  def test_slice_inversion(self):

    tdomain = create_tdomain([0,1])
    x = SlicedTube(tdomain, Interval(0,10))
    sx = x.first_slice()
    x.set([2,3],0)
    x.set([5,6],1)
    self.assertTrue(sx.invert([4,6], 0) == Interval.empty())
    self.assertTrue(sx.invert([2.5,6], 0) == 0.)
    self.assertTrue(sx.invert([0,1], 1) == Interval.empty())
    self.assertTrue(sx.invert([2.5,6], 1) == 1.)
    self.assertTrue(sx.invert([2.5,6], [0.2,0.5]) == Interval(0.2,0.5))
    self.assertTrue(sx.invert([2.5,6], 0.2) == Interval(0.2))

  def test_slice_inversion_unbounded_derivative(self):

    tdomain = create_tdomain([0,1])
    x = SlicedTube(tdomain, Interval(0,10))
    v = SlicedTube(tdomain, Interval())
    sx = x.first_slice()
    sv = v.first_slice()
    self.assertTrue(sx.invert(5., sv, sx.t0_tf()) == Interval(0,1))
    self.assertTrue(sx.invert(15., sv, sx.t0_tf()) == Interval.empty())

  def test_slice_inversion_other_case(self):

    tdomain = create_tdomain([6.4,6.5])
    x = SlicedTube(tdomain, Interval(0.759,1.205))
    v = SlicedTube(tdomain, Interval(-0.216,-0.115))
    sx = x.first_slice()
    sv = v.first_slice()

    x.set([0.781,1.205],6.4)
    x.set([0.759,1.194],6.5)

    ctc_deriv = CtcDeriv()
    ctc_deriv.contract(x,v)

    self.assertTrue(not sx.codomain().is_empty())
    self.assertTrue(Approx(sx.codomain(),1e-4) == Interval(0.7594, 1.20501))
    self.assertTrue(Approx(sx.output_gate(),1e-4) == Interval(0.7594, 1.19351))

    p = sx.polygon_slice(sv)

    self.assertTrue(Approx(p,1e-4) == ConvexPolygon([
      [[6.49999, 6.5],[0.7594, 0.759401]],
      [[6.49999, 6.5],[1.1935, 1.19351]],
      [[6.4, 6.40001],[1.20499, 1.20501]],
      [[6.4, 6.40001],[0.780999, 0.781001]]
    ]))

    pt = IntervalVector([sx.t0_tf().ub(),1])

    self.assertTrue(p.contains(pt) == BoolInterval.UNKNOWN)
    self.assertTrue(p.box()[0].ub() == sx.t0_tf().ub())
    
    p_inter = (p&ConvexPolygon(pt))
    self.assertTrue(p_inter == ConvexPolygon(pt))


if __name__ ==  '__main__':
  unittest.main()