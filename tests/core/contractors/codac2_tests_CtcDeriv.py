#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestCtcDeriv(unittest.TestCase):
  
  def test_CtcDeriv(self):
    
    # Test slice, envelope contraction

    tdomain = create_tdomain([-1,3])
    x = SlicedTube(tdomain, Interval(-10,20))
    v = SlicedTube(tdomain, Interval(-1,1))

    sx = x.first_slice()
    sv = v.first_slice()

    self.assertTrue(sx.t0_tf() == Interval(-1,3))
    self.assertTrue(sx.codomain() == Interval(-10,20))
    x.set([-1,2],-1)
    x.set([-2,0],3)

    ctc_deriv = CtcDeriv(TimePropag.FWD_BWD, False)
    ctc_deriv.contract(sx, sv)

    self.assertTrue(sx.input_gate() == Interval(-1,2))
    self.assertTrue(sx.output_gate() == Interval(-2,0))
    self.assertTrue(sx.codomain() == Interval(-3.5,3))

    # Test slice, output gate contraction

    tdomain = create_tdomain([-1,3])
    x = SlicedTube(tdomain, Interval(-5,3))
    v = SlicedTube(tdomain, Interval(-1))

    sx = x.first_slice()
    sv = v.first_slice()

    self.assertTrue(sx.t0_tf() == Interval(-1,3))
    self.assertTrue(sx.codomain() == Interval(-5,3))
    x.set([-1,3],-1)
    x.set([-5,0.5],3)

    ctc_deriv = CtcDeriv(TimePropag.FWD_BWD, False)
    ctc_deriv.contract(sx, sv)

    self.assertTrue(sx.input_gate() == Interval(-1,3))
    self.assertTrue(sx.output_gate() == Interval(-5,-1))
    self.assertTrue(sx.codomain() == Interval(-5,3))

    # Test slice, complete contraction (degenerate tube)

    tdomain = create_tdomain([-1,3])
    x = SlicedTube(tdomain, Interval(-5,3))
    v = SlicedTube(tdomain, Interval(-1,1))

    sx = x.first_slice()
    sv = v.first_slice()

    self.assertTrue(sx.t0_tf() == Interval(-1,3))
    self.assertTrue(sx.codomain() == Interval(-5,3))
    x.set([1,3],-1)
    x.set([-4,-3],3)

    ctc_deriv = CtcDeriv(TimePropag.FWD_BWD, False)
    ctc_deriv.contract(sx, sv)

    self.assertTrue(sx.input_gate() == Interval(1))
    self.assertTrue(sx.output_gate() == Interval(-3))
    self.assertTrue(sx.codomain() == Interval(-3,1))
    self.assertTrue(sx(-1, sv) == Interval(1))
    self.assertTrue(sx(1, sv) == Interval(-1))
    self.assertTrue(sx(3, sv) == Interval(-3))
    self.assertTrue(sx(0, sv) == Interval(0))

    # Test slice, empty case

    tdomain = create_tdomain([-1,3])
    x = SlicedTube(tdomain, Interval(-10,20))
    v = SlicedTube(tdomain, Interval.empty())

    sx = x.first_slice()
    sv = v.first_slice()

    self.assertTrue(sx.t0_tf() == Interval(-1,3))
    self.assertTrue(sx.codomain() == Interval(-10,20))
    x.set([-1,2],-1)
    x.set([-2,0],3)

    ctc_deriv = CtcDeriv(TimePropag.FWD_BWD, False)
    ctc_deriv.contract(sx, sv)

    self.assertTrue(sx.input_gate().is_empty())
    self.assertTrue(sx.output_gate().is_empty())
    self.assertTrue(sx.codomain().is_empty())

    # Test slice, empty case (bis)

    tdomain = create_tdomain([-1,3])
    x = SlicedTube(tdomain, Interval(-10,20))
    v = SlicedTube(tdomain, Interval(-1,1))

    sx = x.first_slice()
    sv = v.first_slice()

    self.assertTrue(sx.t0_tf() == Interval(-1,3))
    self.assertTrue(sx.codomain() == Interval(-10,20))
    x.set([-1,2],-1)
    x.set(Interval.empty(),3)

    ctc_deriv = CtcDeriv(TimePropag.FWD_BWD, False)
    ctc_deriv.contract(sx, sv)

    self.assertTrue(sx.input_gate().is_empty())
    self.assertTrue(sx.output_gate().is_empty())
    self.assertTrue(sx.codomain().is_empty())

    # Test slice, unbounded slice

    tdomain = create_tdomain([-1,3])
    x = SlicedTube(tdomain, Interval())
    v = SlicedTube(tdomain, Interval(0,1))

    sx = x.first_slice()
    sv = v.first_slice()

    self.assertTrue(sx.input_gate() == Interval())
    self.assertTrue(sx.output_gate() == Interval())
    self.assertTrue(sx.codomain() == Interval())

    ctc_deriv = CtcDeriv(TimePropag.FWD_BWD, False)
    ctc_deriv.contract(sx, sv)

    self.assertTrue(sx.input_gate() == Interval())
    self.assertTrue(sx.output_gate() == Interval())
    self.assertTrue(sx.codomain() == Interval())

    # Test slice, unbounded derivative (1)
  
    tdomain = create_tdomain([-1,3])
    x = SlicedTube(tdomain, Interval())
    v = SlicedTube(tdomain, Interval())

    sx = x.first_slice()
    sv = v.first_slice()

    self.assertTrue(sx.input_gate() == Interval())
    self.assertTrue(sx.output_gate() == Interval())
    self.assertTrue(sx.codomain() == Interval())

    ctc_deriv = CtcDeriv(TimePropag.FWD_BWD, False)
    ctc_deriv.contract(sx, sv)

    self.assertTrue(sx.input_gate() == Interval())
    self.assertTrue(sx.output_gate() == Interval())
    self.assertTrue(sx.codomain() == Interval())

    # Test slice, unbounded derivative ([-oo,1])

    tdomain = create_tdomain([-1,3])
    x = SlicedTube(tdomain, Interval())
    v = SlicedTube(tdomain, Interval(-oo,1))

    sx = x.first_slice()
    sv = v.first_slice()

    x.set([-1,2],-1)
    x.set([-2,0],3)

    self.assertTrue(sx.input_gate() == Interval(-1,2))
    self.assertTrue(sx.output_gate() == Interval(-2,0))
    self.assertTrue(sx.codomain() == Interval())
    self.assertTrue(sv.codomain() == Interval(-oo,1))

    ctc_deriv = CtcDeriv(TimePropag.FWD_BWD, False)
    ctc_deriv.contract(sx, sv)

    self.assertTrue(sx.input_gate() == Interval(-1,2))
    self.assertTrue(sx.output_gate() == Interval(-2,0))
    self.assertTrue(sx.codomain() == Interval(-6,6))
    self.assertTrue(sv.codomain() == Interval(-oo,1))

    # Test slice, unbounded derivative ([1,oo])

    tdomain = create_tdomain([-1,3])
    x = SlicedTube(tdomain, Interval())
    v = SlicedTube(tdomain, Interval(-1,oo))

    sx = x.first_slice()
    sv = v.first_slice()

    x.set([-1,2],-1)
    x.set([-2,0],3)

    self.assertTrue(sx.input_gate() == Interval(-1,2))
    self.assertTrue(sx.output_gate() == Interval(-2,0))
    self.assertTrue(sx.codomain() == Interval())
    self.assertTrue(sv.codomain() == Interval(-1,oo))

    ctc_deriv = CtcDeriv(TimePropag.FWD_BWD, False)
    ctc_deriv.contract(sx, sv)
    
    self.assertTrue(sx.input_gate() == Interval(-1,2))
    self.assertTrue(sx.output_gate() == Interval(-2,0))
    self.assertTrue(sx.codomain() == Interval(-5,4))
    self.assertTrue(sv.codomain() == Interval(-1,oo))

    # Test fwd

    tdomain = create_tdomain([0,6],1.)
    x_fwd = SlicedTube(tdomain, Interval())
    x_fwd.set([-1,1],[0,1])
    v = SlicedTube(tdomain, Interval(-0.5,1))
    x_fwdbwd = SlicedTube(x_fwd)

    ctc_deriv_fwd = CtcDeriv(TimePropag.FWD, False)
    ctc_deriv_fwdbwd = CtcDeriv(TimePropag.FWD_BWD, False)

    ctc_deriv_fwd.contract(x_fwd, v)
    ctc_deriv_fwdbwd.contract(x_fwdbwd, v)
    
    self.assertTrue(x_fwd == x_fwdbwd)

    # Checking structure
    sx_fwd = [s for s in x_fwd if not s.is_gate()]

    self.assertTrue(sx_fwd[0].codomain() == Interval(-1,1))
    self.assertTrue(sx_fwd[1].codomain() == Interval(-1.5,2))
    self.assertTrue(sx_fwd[2].codomain() == Interval(-2,3))
    self.assertTrue(sx_fwd[3].codomain() == Interval(-2.5,4))
    self.assertTrue(sx_fwd[4].codomain() == Interval(-3,5))
    self.assertTrue(sx_fwd[5].codomain() == Interval(-3.5,6))
    self.assertTrue(x_fwd.codomain() == Interval(-3.5,6))

    # Test bwd

    tdomain = create_tdomain([0,6],1.)
    x_bwd = SlicedTube(tdomain, Interval())
    x_bwd.set([-1,1],[5,6])
    v = SlicedTube(tdomain, Interval(-1,0.5))
    x_fwdbwd = SlicedTube(x_bwd)

    ctc_deriv_bwd = CtcDeriv(TimePropag.BWD, False)
    ctc_deriv_fwdbwd = CtcDeriv(TimePropag.FWD_BWD, False)

    ctc_deriv_bwd.contract(x_bwd, v)
    ctc_deriv_fwdbwd.contract(x_fwdbwd, v)
    
    self.assertTrue(x_bwd == x_fwdbwd)

    # Checking structure
    sx_bwd = [s for s in x_bwd if not s.is_gate()]
    self.assertTrue(sx_bwd[0].codomain() == Interval(-3.5,6))
    self.assertTrue(sx_bwd[1].codomain() == Interval(-3,5))
    self.assertTrue(sx_bwd[2].codomain() == Interval(-2.5,4))
    self.assertTrue(sx_bwd[3].codomain() == Interval(-2,3))
    self.assertTrue(sx_bwd[4].codomain() == Interval(-1.5,2))
    self.assertTrue(sx_bwd[5].codomain() == Interval(-1,1))
    self.assertTrue(x_bwd.codomain() == Interval(-3.5,6))

    # Test fwd/bwd

    tdomain = create_tdomain([0,6],1.)
    x = SlicedTube(tdomain, Interval())
    x.set([-1,1],[0,1])
    x.set([-1,1],[5,6])
    v = SlicedTube(tdomain, Interval(-1,0.5))

    ctc_deriv = CtcDeriv(TimePropag.FWD_BWD, False)
    ctc_deriv.contract(x, v)

    # Checking structure
    sx = [s for s in x if not s.is_gate()]
    self.assertTrue(sx[0].codomain() == Interval(-1,1))
    self.assertTrue(sx[1].codomain() == Interval(-2,1.5))
    self.assertTrue(Approx(sx[2].codomain()) == Interval(-7./3.,2.))
    self.assertTrue(Approx(sx[3].codomain()) == Interval(-2.,7./3.))
    self.assertTrue(sx[4].codomain() == Interval(-1.5,2.))
    self.assertTrue(sx[5].codomain() == Interval(-1,1))
    self.assertTrue(Approx(x.codomain()) == Interval(-7./3.,7./3.))

    # Test fwd/bwd (example from tubint paper)

    tdomain = create_tdomain([0,5],1.)
    x = SlicedTube(tdomain, Interval())
    x.set([0],0)
    x.set([4],5)
    v = SlicedTube(tdomain, Interval())
    v.set([1,2],[0,1])
    v.set([0.5,1.5],[1,2])
    v.set([0,0.5],[2,3])
    v.set([0],[3,4])
    v.set([-0.5,0.5],[4,5])

    ctc_deriv = CtcDeriv(TimePropag.FWD_BWD, False)
    ctc_deriv.contract(x, v)

    # Checking structure
    sx = [s for s in x if not s.is_gate()]
    self.assertTrue(sx[0].codomain() == Interval(0,2))
    self.assertTrue(sx[1].codomain() == Interval(1.5,3.5))
    self.assertTrue(sx[2].codomain() == Interval(3,4))
    self.assertTrue(sx[3].codomain() == Interval(3.5,4))
    self.assertTrue(sx[4].codomain() == Interval(3.5,4.25))
    self.assertTrue(x.codomain() == Interval(0,4.25))

    # Test fwd/bwd (other example)

    tdomain = create_tdomain([0,26])
    x = SlicedTube(tdomain, Interval(-1,7))
    x.set([2,3],0)
    x.set([3,4],4)
    x.set([1],8)
    x.set([1],12)
    x.set([5.5],14)
    x.set([5.5],16)
    x.set([-1],18)
    x.set([5.5],20)
    x.set([5.5],23)
    x.set([5.5],23) # setting gate again
    v = SlicedTube(tdomain, Interval(-1,1))

    sx = [s for s in x if not s.is_gate()]
    sv = [s for s in v if not s.is_gate()]

    sx[8].set([-oo,oo])

    sv[0].set([-1.5,4])
    sv[1].set([-0.75,-0.5])
    sv[2].set([-1./3,1])
    sv[3].set([4.5/2])
    sv[4].set([0])
    sv[5].set([-oo,-6.5/2])
    sv[6].set([6.5/2,oo])
    sv[7].set([-oo,oo])
    sv[8].set([-oo,oo])

    self.assertTrue(len(sx) == 9)
    self.assertTrue(len(sv) == 9)
    self.assertTrue(x.nb_slices() == 18)
    self.assertTrue(v.nb_slices() == 18)

    ctc_deriv = CtcDeriv(TimePropag.FWD_BWD, False)
    ctc_deriv.contract(x, v)

    self.assertTrue(sx[0].codomain() == Interval(-1,7))
    self.assertTrue(sx[1].codomain() == Interval(1,4))
    self.assertTrue(Approx(sx[2].codomain()) == Interval(0,2))
    self.assertTrue(sx[3].codomain() == Interval(1,5.5))
    self.assertTrue(sx[4].codomain() == Interval(5.5))
    self.assertTrue(x.codomain() == Interval(-oo,oo))

    ctc_deriv.contract(sx[0], sv[0])
    ctc_deriv.contract(sx[1], sv[1])
    ctc_deriv.contract(sx[2], sv[2])
    ctc_deriv.contract(sx[3], sv[3])
    ctc_deriv.contract(sx[4], sv[4])
    ctc_deriv.contract(sx[5], sv[5])
    ctc_deriv.contract(sx[6], sv[6])
    ctc_deriv.contract(sx[7], sv[7])
    ctc_deriv.contract(sx[8], sv[8])

    self.assertTrue(x(0.) == Interval(2,3))
    self.assertTrue(sx[0].codomain() == Interval(-1,7))
    self.assertTrue(x(4.) == Interval(3,4))
    self.assertTrue(sx[1].codomain() == Interval(1,4))
    self.assertTrue(x(8.) == Interval(1.))
    self.assertTrue(Approx(sx[2].codomain()) == Interval(0,2))
    self.assertTrue(x(12.) == Interval(1))
    self.assertTrue(sx[3].codomain() == Interval(1,5.5))
    self.assertTrue(x(14.) == Interval(5.5))
    self.assertTrue(sx[4].codomain() == Interval(5.5))
    self.assertTrue(x(16.) == Interval(5.5))
    self.assertTrue(sx[5].codomain() == Interval(-1,5.5))
    self.assertTrue(x(18.) == Interval(-1))
    self.assertTrue(sx[6].codomain() == Interval(-1,5.5))
    self.assertTrue(x(20.) == Interval(5.5))
    self.assertTrue(sx[7].codomain() == Interval(-1,7))
    self.assertTrue(x(23.) == Interval(5.5))

    self.assertTrue(sx[8].codomain() == Interval())
    self.assertTrue(x(26.) == Interval())
    self.assertTrue(x.codomain() == Interval())

    # From: Test slice, output gate contraction

    tdomain = create_tdomain([-1,3])
    x = SlicedTube(tdomain, Interval(-5,3))
    v = SlicedTube(tdomain, Interval(-1,-1))

    sx = x.first_slice()
    sv = v.first_slice()

    x.set([-1,3],-1)
    x.set([-5,0.5],3)

    ctc_deriv = CtcDeriv(TimePropag.FWD_BWD, False)
    ctc_deriv.contract(x,v)

    self.assertTrue(sx.input_gate() == Interval(-1,3))
    self.assertTrue(sx.output_gate() == Interval(-5,-1))
    self.assertTrue(sx(Interval(2.), sv) == Interval(-4,0))
    self.assertTrue(sx(Interval(-1,3), sv) == Interval(-5,3))

    # From: Test slice, complete contraction (degenerate tube)

    tdomain = create_tdomain([-1,3])
    x = SlicedTube(tdomain, Interval(-5,3))
    v = SlicedTube(tdomain, Interval(-1,1))

    sx = x.first_slice()
    sv = v.first_slice()

    x.set([1,3],-1)
    x.set([-4,-3],3)

    ctc_deriv = CtcDeriv(TimePropag.FWD_BWD, False)
    ctc_deriv.contract(x,v)

    self.assertTrue(sx(Interval(0.5,2), sv) == Interval(-2,-0.5))
    self.assertTrue(sx(Interval(1), sv) == Interval(-1))
    self.assertTrue(sx(Interval(-1,3), sv) == Interval(-3,1))


if __name__ ==  '__main__':
  unittest.main()