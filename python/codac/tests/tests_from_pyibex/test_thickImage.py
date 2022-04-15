#!/usr/bin/env python

# Codac tests
# Originated from the former pyIbex library (Benoît Desrochers)
# ---------------------------------------------------------------------------
# \date      2022
# \author    Benoît Desrochers
# \copyright Copyright 2022 Codac Team
# \license   This program is distributed under the terms of
#            the GNU Lesser General Public License (LGPL).

import unittest
from codac import *
from codac.unsupported import *

try:
  import numpy as np
  has_np = True
except ImportError:
  print("NUMPY NOT FOUND")
  has_np = False

@unittest.skipUnless(has_np, "Numpy not found")
class TestThickImage(unittest.TestCase):

  def setUp(self):
    a = np.zeros((499,499))
    a[100:400, 100:400] = 1
    a = a.cumsum(0).cumsum(1)
    self.im = ThickGeoImage(a, a, -5,5,0.02, -0.02)

  def test_world_to_grid(self):
    X0 = IntervalVector([[-7, -6], [0,1]])
    self.assertEqual(self.im.test(X0), MAYBE)

if __name__ == '__main__':

  unittest.main()