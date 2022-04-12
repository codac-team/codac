#!/usr/bin/env python
#============================================================================
#                                P Y I B E X
# File        : test_Image.py
# Author      : Benoit Desrochers
# Copyright   : Benoit Desrochers
# License     : See the LICENSE file
# Created     : Dec 28, 2015
#============================================================================

import unittest
import pyibex
from pyibex import *
from pyibex.thickset.thickimage import *
from pyibex.thickset import *
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



  # def test_SIVIA(self):




    # SIVIA(x, ctc, 1)


if __name__ == '__main__':

  unittest.main()
