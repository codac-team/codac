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
    self.assertTrue(z.z == Vector([4,2,0]))
    self.assertTrue(z.A == Matrix([[0,1,1],[0,1,0],[0.5,0,0]]))
      
if __name__ ==  '__main__':
  unittest.main()