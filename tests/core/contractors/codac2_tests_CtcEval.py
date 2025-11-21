#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestCtcEval(unittest.TestCase):
  
  def test_CtcEval_1(self):

    tdomain = create_tdomain([-1,7], 2., True)
    x = SlicedTube(tdomain, Interval())
    v = SlicedTube(tdomain, Interval())

    self.assertTrue(x.nb_slices() == 9)

    v.set_ith_slice([-1], 0*2+1)
    v.set_ith_slice([-1,1], 1*2+1)
    v.set_ith_slice([-1], 2*2+1)
    v.set_ith_slice([-1,1], 3*2+1)

    self.assertTrue(not v.is_empty())

    t = Interval(0,6)
    z = Interval(-1,1)

    ctc_eval = CtcEval()
    ctc_eval.contract(t, z, x, v)

    self.assertTrue(x.nb_slices() == 13)

    self.assertTrue(x(-1) == Interval())
    self.assertTrue(x(0) == Interval(-2,7))
    self.assertTrue(x(1) == Interval(-3,6))
    self.assertTrue(x(3) == Interval(-4,4))
    self.assertTrue(x(5) == Interval(-6,2))
    self.assertTrue(x(6) == Interval(-7,2))
    self.assertTrue(x(7) == Interval())

  def test_CtcEval_2(self):
    
    t = ScalarVar()
    fv = AnalyticFunction([t], -sin(t)+1e-3*Interval(-1,1))

    tdomain = create_tdomain([0,20],0.1,True)
    x = SlicedTube(tdomain, Interval(-10,10))
    v = SlicedTube(tdomain, fv)
    x.set(1,0)
    
    ctc_deriv = CtcDeriv()
    ctc_deriv.contract(x, v)

    ctc_eval = CtcEval()
    xc = SlicedTube(x)
    vc = SlicedTube(v)

    y1 = IntervalVector([11.98,1])
    ctc_eval.contract(y1[0], y1[1], xc, vc)

    y2 = IntervalVector([6.5,1])
    ctc_eval.contract(y2[0], y2[1], xc, vc)

    ctc_deriv.contract(xc, vc)

    self.assertTrue(xc(tdomain.t0_tf().lb()) == 1.)
    self.assertTrue(xc(y1[0]) == y1[1])
    self.assertTrue(xc(y2[0]) == y2[1])
    self.assertTrue(Approx(xc(tdomain.t0_tf().ub()),1e-5) == Interval(0.294849,0.857381))

  def test_CtcEval_3_from_tubeval(self):
    
    tdomain = create_tdomain([-PI,PI/2],1e-2,True)
    t = ScalarVar()
    fv = AnalyticFunction([t], cos(t)+Interval(-0.1,0.1))
    v = SlicedTube(tdomain, fv)
    x = v.primitive()
    for sx in x:
      if(not sx.is_gate()):
        sx.set(sx.codomain() + Interval(-0.1,0.1))
    for sx in x:
      if(sx.is_gate()):
        sx.set(sx.codomain() + Interval(-0.1,0.1))

    ti = Interval(-0.5,0.3)
    yi = Interval(0.3,1.1)

    #DefaultFigure.plot_tube(x, [Color.light_gray(),Color.light_gray()])
    #DefaultFigure.draw_box(cart_prod(ti,yi), Color.gray())

    ctc_eval = CtcEval()
    ctc_eval.contract(ti, yi, x, v)

    #ctc_deriv = CtcDeriv()
    #ctc_deriv.contract(x, v)

    #DefaultFigure.plot_tube(x, [Color.dark_blue(),Color.blue()])
    #DefaultFigure.draw_box(cart_prod(ti,yi), Color.red())

    self.assertTrue(Approx(ti,1e-5) == Interval(-0.113064,0.3))
    self.assertTrue(Approx(yi,1e-5) == Interval(0.299999,0.749899))
    self.assertTrue(Approx(x.codomain(),1e-5) == Interval(-1.26758,1.58623))
    self.assertTrue(Approx(v.codomain(),1e-4) == Interval(-1.10001,1.10001))
    self.assertTrue(Approx(ti.lb(),1e-5) == -0.113063)
    self.assertTrue(Approx(x(ti.lb()),1e-5) == Interval(-0.149899,0.300296))
    self.assertTrue(Approx(ti.ub(),1e-5) == 0.3)
    self.assertTrue(Approx(x(ti.ub()),1e-5) == Interval(0.299999,0.749899))

  def test_CtcEval_4(self):
    
    tdomain = create_tdomain([0,11],1,True)
    x = SlicedTube(tdomain, Interval())
    v = SlicedTube(tdomain, Interval(-1.5,-1))

    sx = []
    for s in x:
      if(not s.is_gate()):
        sx.append(s)

    sx[0].set([6,8])

    ctc_eval = CtcEval()
    ctc_deriv = CtcDeriv()
    ctc_deriv.contract(x, v)

    self.assertTrue(sx[0].codomain() == Interval(6,8)) 
    self.assertTrue(sx[1].codomain() == Interval(4.5,7))
    self.assertTrue(sx[2].codomain() == Interval(3,6))
    self.assertTrue(sx[3].codomain() == Interval(1.5,5))
    self.assertTrue(sx[4].codomain() == Interval(-0,4))
    self.assertTrue(sx[5].codomain() == Interval(-1.5,3))
    self.assertTrue(sx[6].codomain() == Interval(-3,2))
    self.assertTrue(sx[7].codomain() == Interval(-4.5,1))
    self.assertTrue(sx[8].codomain() == Interval(-6,0))
    self.assertTrue(sx[9].codomain() == Interval(-7.5,-1))
    self.assertTrue(sx[10].codomain() == Interval(-9,-2))

    t = Interval()
    y = Interval()
    x_raw = SlicedTube(x)
    v_raw = SlicedTube(v)

    x = SlicedTube(x_raw)
    v = SlicedTube(v_raw)
    t = Interval(3.5,6)
    y = Interval(3.5,6)
    ctc_eval.contract(t, y, x, v)

    sx.clear()
    for s in x:
      if(not s.is_gate()):
        sx.append(s)

    self.assertTrue(t == Interval(3.5,4.5))
    self.assertTrue(y == Interval(3.5,4.5))
    self.assertTrue(sx[0].codomain() == Interval(6,8))
    self.assertTrue(sx[1].codomain() == Interval(4.5,7))
    self.assertTrue(sx[2].codomain() == Interval(3,6))
    self.assertTrue(sx[3].codomain() == Interval(1.5,5))
    self.assertTrue(sx[3].t0_tf() == Interval(3,3.5))
    self.assertTrue(sx[4].codomain() == Interval(2.75,4.75))
    self.assertTrue(sx[4].t0_tf() == Interval(3.5,4))
    self.assertTrue(sx[5].codomain() == Interval(2,4))
    self.assertTrue(sx[5].t0_tf() == Interval(4,4.5))
    self.assertTrue(sx[6].codomain() == Interval(0,4))
    self.assertTrue(sx[6].t0_tf() == Interval(4.5,5))
    self.assertTrue(sx[7].codomain() == Interval(-1.5,3))
    self.assertTrue(sx[7].t0_tf() == Interval(5,6))

    #ctc_deriv.contract(x, v)

    #DefaultFigure.plot_tube(x_raw, [Color.light_gray(),Color.light_gray()])
    #DefaultFigure.plot_tube(x, [Color.dark_blue(),Color.light_blue()])
    #DefaultFigure.draw_line(Segment([[3.5,4.5],[11,4.5-1*(11-3.5)]]), Color.purple())
    #DefaultFigure.draw_line(Segment([[3.5,3.5],[11,3.5-1.5*(11-3.5)]]), Color.purple())
    #DefaultFigure.draw_box([[3.5,6],[3.5,6]], Color.light_gray())
    #DefaultFigure.draw_box(cart_prod(t,y), Color.red())

  def test_CtcEval_5_multi(self):
    
    tdomain = create_tdomain([0,10*PI],1e-1,True)
    t = ScalarVar()
    fv = AnalyticFunction([t], cos(t))
    fx = AnalyticFunction([t], sin(t)+Interval(-0.1,0.1))
    v = SlicedTube(tdomain, fv)
    x = SlicedTube(tdomain, fx)

    ctc_deriv = CtcDeriv()
    ctc_deriv.contract(x, v)

    #DefaultFigure.plot_tube(x, [Color.light_gray(),Color.light_gray()])

    ti = Interval(11.42,23.15)
    yi = Interval(-1.2,-1)
    #DefaultFigure.draw_box(cart_prod(ti,yi), Color.gray())

    ctc_eval = CtcEval()
    ctc_eval.contract(ti,yi,x,v)
    ctc_deriv.contract(x, v)

    #DefaultFigure.plot_tube(x, [Color.blue(),Color.light_blue()])
    #DefaultFigure.draw_box(cart_prod(ti,yi), Color.red())

    self.assertTrue(ti == Interval(11.42,23.15))
    self.assertTrue(Approx(yi) == Interval(-1.1,-1))
    self.assertTrue(Approx(x(17.27),1e-4) == Interval(-1.10001,-0.897816))

if __name__ ==  '__main__':
  unittest.main()