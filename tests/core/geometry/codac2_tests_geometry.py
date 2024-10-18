#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestGeometry(unittest.TestCase):

  def tests_geometry(self):
    
    # Align points
    self.assertTrue(orientation([0.,0.],[5.,0.],[8.,0.]) == BoolInterval.UNKNOWN)
    self.assertTrue(orientation([0.,0.],[5.,0.],[-2.,0.]) == BoolInterval.UNKNOWN)
    self.assertTrue(orientation([0.,0.],[-2.,0.],[5.,0.]) == BoolInterval.UNKNOWN)
    self.assertTrue(orientation([0.,0.],[0.,0.],[5.,0.]) == BoolInterval.UNKNOWN)
    self.assertTrue(orientation([0.,0.],[0.,0.],[0.,0.]) == BoolInterval.UNKNOWN)

    # Clockwise points
    self.assertTrue(orientation([0.,0.],[0.,-5.],[-2.,-5.]) == BoolInterval.TRUE)
    self.assertTrue(orientation([-999.,0.1],[0.,0.],[-999.,-0.1]) == BoolInterval.TRUE)

    # Anticlockwise points
    self.assertTrue(orientation([0.,0.],[0.,-5.],[2.,-5.]) == BoolInterval.FALSE)
    self.assertTrue(orientation([999.,0.1],[0.,0.],[999.,-0.1]) == BoolInterval.FALSE)

if __name__ ==  '__main__':
  unittest.main()