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

class TestZonotope(unittest.TestCase):

  def test_zonotope(self):

    Z1 = Zonotope(Vector([2,1]), Matrix([[0.2  ,0.08], [0.04,0.18]]))
    Z2 = Zonotope(Vector([2,0.5]), Matrix([[-0.2], [0.1]]))
    Zs = Z1+Z2

    c = Vector([4,1.5])
    A = Matrix([[0.2  ,0.08, -0.2],
                [0.04, 0.18, 0.1]])
    self.assertTrue(Zs.c == c)
    self.assertTrue(Zs.A == A)
      
if __name__ ==  '__main__':
  unittest.main()