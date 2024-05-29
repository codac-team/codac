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

class TestApprox(unittest.TestCase):

  def tests_approx(self):

    self.assertFalse(Interval(0.1) == Interval(1.)/Interval(10.))
    self.assertTrue(Approx(Interval(0.1)) == Interval(1.)/Interval(10.))
      
if __name__ ==  '__main__':
  unittest.main()