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

  def tests_Polygon(self):

    p = Polygon([[3,-1],[3,4],[5,6],[-1,1]])
    self.assertTrue(p.contains([3.1,3]) == BoolInterval.FALSE)
    self.assertTrue(p.contains([2.9,3]) == BoolInterval.TRUE)
    self.assertTrue(p.contains([3,3]) == BoolInterval.UNKNOWN)

if __name__ ==  '__main__':
  unittest.main()