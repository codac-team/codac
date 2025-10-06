#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
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

    z = p.proj([2,1,0])
    self.assertTrue(z.z == Vector([4,2,0]))
    self.assertTrue(z.A == Matrix([[0,1,1],[0,1,0],[0.5,0,0]]))
      
if __name__ ==  '__main__':
  unittest.main()