#!/usr/bin/env python
#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2026
#  \author     Simon Rohou
#  \copyright  Copyright 2026 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import math
import unittest
from codac import *

class TestCtcDynamicManual(unittest.TestCase):

  def test_ctceval_scalar(self):

    # [ctceval-scalar-beg]
    td = create_tdomain([-PI,PI/2],0.01,True)
    s = ScalarVar()
    v = SlicedTube(td, AnalyticFunction([s],cos(s)+Interval(-0.1,0.1)))
    x = v.primitive(Interval(-0.1,0.1))

    t, z = Interval(-0.5,0.3), Interval(0.3,1.1)
    ctc_eval = CtcEval()
    ctc_eval.contract(t,z,x,v)
    # [ctceval-scalar-end]

    self.assertFalse(x.is_empty())
    self.assertTrue(t.is_subset(Interval(-0.5,0.3)))
    self.assertTrue(z.is_subset(Interval(0.3,1.1)))
    self.assertGreater(t.lb(),-0.5)
    self.assertLess(z.ub(),1.1)
    ref = math.sin(0.25)+0.1+0.03*0.25
    self.assertTrue(t.contains(0.25))
    self.assertTrue(z.contains(ref))

    # [ctceval-propagation-beg]
    ctc_deriv = CtcDeriv()
    ctc_deriv.contract(x,v)
    # [ctceval-propagation-end]

    self.assertFalse(x.is_empty())
    self.assertTrue(x(0.25).contains(ref))

  def test_ctceval_multi(self):

    td = create_tdomain([-PI,PI/2],0.01,True)
    s = ScalarVar()
    v = SlicedTube(td, AnalyticFunction([s],cos(s)+Interval(-0.1,0.1)))
    x = v.primitive(Interval(-0.1,0.1))

    # [ctceval-multi-beg]
    t = [Interval(-0.5,0.3),Interval(-0.6,0.8),Interval(-2.3,-2.2)]
    z = [Interval(0.3,1.1),Interval(-0.5,-0.4),Interval(-0.8,-0.7)]

    ctc_eval = CtcEval()
    ctc_deriv = CtcDeriv()
    primitives = v.partial_primitives()

    for iter in range(2):
      for i in range(len(t)):
        ctc_eval.contract(t[i],z[i],x,v,primitives) # Optional primitives argument for faster integration
      ctc_deriv.contract(x,v)
    # [ctceval-multi-end]

    self.assertFalse(x.is_empty())
    ref_times = [0.25,-0.55,-2.25]
    for i, ti in enumerate(ref_times):
      zi = math.sin(ti)+0.1+0.03*ti
      self.assertTrue(t[i].contains(ti))
      self.assertTrue(z[i].contains(zi))
      self.assertTrue(x(ti).contains(zi))

  def test_ctceval_localization(self):

    # [ctceval-localization-beg]
    td = create_tdomain([0,5],0.01,True)
    s = ScalarVar()
    v = SlicedTube(td, AnalyticFunction([s],[-2*sin(s),2*cos(2*s)]))
    v.inflate(0.01)

    x = SlicedTube(td, IntervalVector(2))
    t = Interval(4.3,4.4)
    z = IntervalVector([[-0.73,-0.69],[0.64,0.68]])

    ctc_eval = CtcEval()
    ctc_eval.contract(t,z,x,v)
    CtcDeriv().contract(x,v)
    # [ctceval-localization-end]

    self.assertFalse(x.is_empty())
    self.assertFalse(x.is_unbounded())
    self.assertEqual(t,Interval(4.3,4.4))
    self.assertEqual(z,IntervalVector([[-0.73,-0.69],[0.64,0.68]]))
    self.assertTrue(x(0.).contains(Vector([2.,0.])))
    self.assertTrue(x(4.35).contains(Vector([2*math.cos(4.35),math.sin(8.7)])))

    # [ctceval-inversion-beg]
    y = SlicedTube(td, AnalyticFunction([s],[2*cos(s),sin(2*s)]))
    y.inflate(0.05)
    t = td.t0_tf()
    z = IntervalVector([[-1,0],[0.4,1.2]])

    ctc_eval.contract(t,z,y,v)
    # [ctceval-inversion-end]

    self.assertFalse(y.is_empty())
    self.assertTrue(t.is_subset(Interval(4.,4.7)))
    self.assertTrue(t.contains(4.35))
    self.assertTrue(z.is_subset(IntervalVector([[-1,0],[0.4,1.2]])))
    self.assertLess(z[0].ub(),0.)
    self.assertLess(z[1].ub(),1.2)
    self.assertTrue(z.contains(Vector([2*math.cos(4.35),math.sin(8.7)])))

  def test_ctceval_known_time(self):

    td = create_tdomain([0,3],1.,True)
    x = SlicedTube(td, Interval())
    v = SlicedTube(td, Interval(1.))
    t, z = Interval(1.5), Interval(2.5)
    CtcEval().contract(t,z,x,v)
    self.assertEqual(x(1.5),Interval(2.5))
    CtcDeriv().contract(x,v)

    self.assertEqual(t,Interval(1.5))
    self.assertEqual(z,Interval(2.5))
    self.assertEqual(x(0.),Interval(1.))
    self.assertEqual(x(3.),Interval(4.))

if __name__ == '__main__':
  unittest.main()
