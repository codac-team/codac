#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2025
#  \author     Maël Godard
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *
import sys
import math

class TestParallelepiped(unittest.TestCase):

  def test_parallelepiped(self):

    p = Parallelepiped(Vector([0,2,4]), Matrix([[0.5,0,0],[0,1,0],[0,1,1]]))

    self.assertTrue(p.box() == IntervalVector([[-0.5,0.5],[1,3],[2,6]]))
    self.assertTrue((p.contains(Vector([0.1,2.1,4.1])))==BoolInterval.TRUE)
    self.assertTrue((p.contains(Vector([20.,20.,20.])))==BoolInterval.FALSE)

    self.assertTrue((p.is_superset(IntervalVector([[0.,0.1],[2.,2.1],[4.,4.1]])))==BoolInterval.TRUE)
    self.assertTrue((p.is_superset(IntervalVector([[-10.,-9.],[15.,16.],[12.,13.]])))==BoolInterval.FALSE)
    self.assertTrue((p.is_superset(IntervalVector([[0.,5.],[2.,7.],[4.,9.]])))==BoolInterval.UNKNOWN)

    z = p.proj([2,1,0])
    self.assertTrue(z.c == Vector([4,2,0]))
    self.assertTrue(z.A == Matrix([[0,1,1],[0,1,0],[0.5,0,0]]))
    self.assertTrue(z.box() == IntervalVector([[2,6],[1,3],[-0.5,0.5]]))


    p1 = Parallelepiped([1.5,2.8],Matrix([[0.5,0.4],[0,0.2]]))

    x0 = IntervalVector([[0,5],[0,5]])
    x1 = IntervalVector([[0,0.5],[0,0.5]])
    x2 = IntervalVector([[1.7,5],[0,3]])

    self.assertTrue(Approx(x0 & p1) == IntervalVector([[0.6,2.4],[2.6,3]]))
    self.assertTrue(Approx((p1 & x0).c) == p1.c)
    self.assertTrue(Approx((p1 & x0).A) == p1.A)

    self.assertTrue((x1 & p1).is_empty())
    self.assertTrue((p1 & x1).is_empty())

    self.assertTrue(Approx(x2 & p1) == IntervalVector([[1.7,2.4],[2.6,3]]))
    self.assertTrue(Approx((p1 & x2).c) == Vector([1.65, 2.8]))
    self.assertTrue(Approx((p1 & x2).A) == Matrix([[0.35,0.4],[0,0.2]]))

    p2 = Parallelepiped([-5,-4,3,8],Matrix([[0.5,0.4,0.1,0],[0,0.2,0.3,0],[0,0,0.1,0.4],[0,0,0,0.2]]))
    p_empty = Parallelepiped.empty(4)

    x3 = IntervalVector([[-10,10],[-10,10],[-10,10],[-10,10]])
    x4 = IntervalVector([[-0.5,0.5],[-0.5,0.5],[-0.5,0.5],[-0.5,0.5]])
    x_empty = IntervalVector.empty(4)

    self.assertTrue(Approx(x3 & p2) == IntervalVector([[-6,-4],[-4.5,-3.5],[2.5,3.5],[7.8,8.2]]))
    self.assertTrue(Approx((p2 & x3).c) == Vector([-5,-4,3,8]))
    self.assertTrue(Approx((p2 & x3).A) == Matrix([[0.5,0.4,0.1,0],[0,0.2,0.3,0],[0,0,0.1,0.4],[0,0,0,0.2]]))

    self.assertTrue((x4 & p2).is_empty())
    self.assertTrue((p2 & x4).is_empty())

    self.assertTrue((x_empty & p2).is_empty())
    self.assertTrue((p_empty & x3).is_empty())
      
if __name__ ==  '__main__':
  unittest.main()