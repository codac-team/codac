#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2026
#  \author     Auguste Bourgois, Maël Godard
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestCtcLohner(unittest.TestCase):

  def test_CtcLohner_1(self):

    x = VectorVar(1)
    f = AnalyticFunction ([x],x)
    ctc = CtcLohner(f)

    t = create_tdomain(Interval(0.5,1.))
    t_lb, t_ub = t.t0_tf().lb(), t.t0_tf().ub()
    tube = SlicedTube(t,IntervalVector(1))
    tube.set(IntervalVector([[1.5,1.5]]),t_lb)

    tube = ctc.contract(tube, TimePropag.FWD)

    expected = 1.5 + Interval(0, 0.5) * Interval(1., 2.)

    self.assertTrue(tube(t.t0_tf())[0].is_superset(expected)) # legacy test
    self.assertTrue(Approx(tube(t.t0_tf()),1e-5) == IntervalVector([[1.36106, 2.73938]])) # new test

  def test_CtcLohner_2(self):

    x = VectorVar(1)
    f = AnalyticFunction ([x],[-sin(x[0])])
    ctc = CtcLohner(f)

    t = create_tdomain(Interval(0.5,1.))
    t_lb, t_ub = t.t0_tf().lb(), t.t0_tf().ub()
    tube = SlicedTube(t,IntervalVector(1))
    tube.set(IntervalVector([2. * atan(exp(-t_lb) * tan(0.5))]),t_lb)

    tube = ctc.contract(tube, TimePropag.FWD)

    expected = Interval(2. * atan(exp(-t_lb) * tan(0.5))) | Interval(2. * atan(exp(-t_ub) * tan(0.5)))

    self.assertTrue(tube(t.t0_tf())[0].is_superset(expected)) # legacy test
    self.assertTrue(Approx(tube(t.t0_tf()),1e-5) == IntervalVector([[0.335538, 0.669785]])) # new test

  def test_CtcLohner_3(self):

    x = VectorVar(1)
    f = AnalyticFunction ([x],-x)
    ctc = CtcLohner(f)

    t = create_tdomain(Interval(0.5,1.))
    t_lb, t_ub = t.t0_tf().lb(), t.t0_tf().ub()
    tube = SlicedTube(t,IntervalVector(1))
    tube.set(IntervalVector([exp(-t_lb)]),t_lb)

    tube = ctc.contract(tube, TimePropag.FWD)

    expected = IntervalVector([exp(-t_lb)]) | IntervalVector([exp(-t_ub)])

    self.assertTrue(tube(t.t0_tf()).is_superset(expected)) # legacy test
    self.assertTrue(Approx(tube(t.t0_tf()),1e-5) == IntervalVector([[0.300877, 0.636858]])) # new test

  def test_CtcLohner_4(self):

    x = VectorVar(2)
    f = AnalyticFunction ([x],[-x[0],x[1]])
    ctc = CtcLohner(f)

    t = create_tdomain(Interval(0.,1.),0.1)
    t_lb, t_ub = t.t0_tf().lb(), t.t0_tf().ub()
    tube = SlicedTube(t,IntervalVector(2))
    tube.set(IntervalVector([exp(0),exp(0)]),t_lb)

    tube = ctc.contract(tube, TimePropag.FWD)

    expected_0 = IntervalVector([exp(-t_lb),exp(t_lb)])
    self.assertTrue(tube(0.).is_superset(expected_0)) # legacy test
    self.assertTrue(Approx(tube(0.),1e-5) == IntervalVector([[0.999999, 1.00001], [0.999999, 1.00001]])) # new test

    expected_1 = IntervalVector([exp(-t_ub),exp(t_ub)])
    self.assertTrue(tube(1.).is_superset(expected_1)) # legacy test
    self.assertTrue(Approx(tube(1.),1e-5) == IntervalVector([[0.366389, 0.368753], [2.71268, 2.7285]])) # new test

  def test_CtcLohner_5(self):

    x = VectorVar(2)
    f = AnalyticFunction ([x],[-x[0],x[1]])
    ctc = CtcLohner(f)

    t = create_tdomain(Interval(0.,1.),0.1)
    t_lb, t_ub = t.t0_tf().lb(), t.t0_tf().ub()
    tube = SlicedTube(t,IntervalVector(2))
    tube.set(IntervalVector([exp(-1.),exp(1.)]),t_ub)

    tube = ctc.contract(tube, TimePropag.BWD)

    expected_0 = IntervalVector([exp(-t_lb),exp(t_lb)])
    self.assertTrue(tube(0.).is_superset(expected_0)) # legacy test
    self.assertTrue(Approx(tube(0.),1e-5) == IntervalVector([[0.997939, 1.00376], [0.99595, 1.00238]])) # new test

    expected_1 = IntervalVector([exp(-t_ub),exp(t_ub)])
    self.assertTrue(tube(1.).is_superset(expected_1)) # legacy test
    self.assertTrue(Approx(tube(1.),1e-5) == IntervalVector([[0.367879, 0.36788], [2.71828, 2.71829]])) # new test
 
  def test_CtcLohner_6(self):

    x = VectorVar(2)
    f = AnalyticFunction ([x],[-x[0],x[1]])
    ctc = CtcLohner(f)

    t = create_tdomain(Interval(0.,1.),0.1)
    t_lb, t_ub = t.t0_tf().lb(), t.t0_tf().ub()
    tube = SlicedTube(t,IntervalVector(2))
    tube.set(IntervalVector([exp(-0.5),exp(0.5)]), t.t0_tf().mid())

    tube = ctc.contract(tube, TimePropag.FWD_BWD)

    expected_0 = IntervalVector([exp(-t_lb),exp(t_lb)])
    self.assertTrue(tube(0.).is_superset(expected_0)) # legacy test
    self.assertTrue(Approx(tube(0.),1e-5) == IntervalVector([[0.998973, 1.00188], [0.997977, 1.00119]])) # new test

    expected_1 = IntervalVector([exp(-t_ub),exp(t_ub)])
    self.assertTrue(tube(1.).is_superset(expected_1)) # legacy test
    self.assertTrue(Approx(tube(1.),1e-5) == IntervalVector([[0.367135, 0.368315], [2.71549, 2.72338]])) # new test
  
  def test_Example(self):

    x0 = IntervalVector([Interval(1,1),Interval(1,1)])
    x0.inflate(0.1)

    ta = create_tdomain([0.,10.0],0.2)
    a = SlicedTube_IntervalVector(ta, IntervalVector(2))
    a.set(x0,0.0)

    tb =create_tdomain([0.,10.0],0.01)
    b = SlicedTube_IntervalVector(tb, IntervalVector(2))
    b.set(x0,0.0)

    x = VectorVar(2)
    f = AnalyticFunction([x], [-x[0],-sin(x[1])])
    ctc_lohner = CtcLohner(f)

    a = ctc_lohner.contract(a)
    b = ctc_lohner.contract(b)

    # DefaultFigure.plot_tube(a[0])
    # DefaultFigure.plot_tube(b[0],StyleProperties([Color.blue(),Color.blue()]))
  
if __name__ ==  '__main__':
  unittest.main()
