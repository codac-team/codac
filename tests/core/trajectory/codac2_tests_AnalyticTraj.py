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

def prim_value(x):
  return 1.+(1./3.)*math.pow(x,3)

class TestAnalyticTraj(unittest.TestCase):

  def tests_AnalyticTraj(self):

    t = ScalarVar()
    f = AnalyticFunction(
      [t],
      sqr(t)
    )

    traj = AnalyticTraj(f, [-1,10])

    self.assertTrue(traj.tdomain() == Interval(-1,10))
    self.assertTrue(traj.codomain() == Interval(0,100))
    self.assertTrue(traj.size() == 1)
    self.assertTrue(Approx(traj(5.3),1e-3) == 28.09)
    self.assertTrue(traj(traj.tdomain()) == traj.codomain())
    self.assertTrue(traj([-oo,oo]) == Interval(-oo,oo))

    # Testing sampled trajectory from analytic trajectory
    sampled_traj = traj.sampled(0.01)
    self.assertTrue(sampled_traj.tdomain() == Interval(-1,10))
    self.assertTrue(Approx(sampled_traj.codomain()) == Interval(0,100))
    self.assertTrue(sampled_traj.size() == 1)
    self.assertTrue(Approx(sampled_traj(sampled_traj.tdomain())) == sampled_traj.codomain())
    self.assertTrue(sampled_traj([-oo,oo]) == Interval(-oo,oo))
    self.assertTrue(Approx(sampled_traj(0.)) == 0.)
    self.assertTrue(Approx(sampled_traj(2.),1e-2) == 4.)
    self.assertTrue(Approx(sampled_traj(5.),1e-2) == 25.)
    self.assertTrue(Approx(sampled_traj(5.3),1e-2) == 28.09)
    self.assertTrue(Approx(sampled_traj(9.),1e-2) == 81.)

    # Testing primitive computation from analytic trajectory
    x0 = f.real_eval(-1) # == 1.
    sampled_prim_traj = traj.primitive(x0,0.01)
    self.assertTrue(sampled_prim_traj.tdomain() == Interval(-1,10))
    self.assertTrue(Approx(sampled_prim_traj.codomain(),4e-1) == Interval(prim_value(0),prim_value(10.)))
    self.assertTrue(sampled_prim_traj(-1.) == x0)
    self.assertTrue(Approx(sampled_prim_traj(0.),4e-1) == prim_value(0.))
    self.assertTrue(Approx(sampled_prim_traj(2.),4e-1) == prim_value(2.))
    self.assertTrue(Approx(sampled_prim_traj(5.),4e-1) == prim_value(5.))
    self.assertTrue(Approx(sampled_prim_traj(5.3),4e-1) == prim_value(5.3))
    self.assertTrue(Approx(sampled_prim_traj(9.),4e-1) == prim_value(9.))

if __name__ ==  '__main__':
  unittest.main()