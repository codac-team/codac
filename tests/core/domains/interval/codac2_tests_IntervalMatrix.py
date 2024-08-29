#!/usr/bin/env python

#  Codac tests
#  
#  Most of these tests come from the IBEX library (Gilles Chabert)
#  See more: https://ibex-lib.readthedocs.io
#  They have been revised to fit with Codac (v2)
#
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Gilles Chabert, (Simon Rohou)
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *
import sys
import math

class TestIntervalMatrix(unittest.TestCase):

  def tests_intervalmatrix(self):

    x = IntervalMatrix([
      [ [-1,1], [-2,2] ],
      [ [-3,3], [-4,4] ],
    ])

    x[0,1] = Interval()
    x[1,1] = Interval()

    self.assertTrue(x == IntervalMatrix([
      [ [-1,1], [-oo,oo] ],
      [ [-3,3], [-oo,oo] ],
    ]))

    #m = IntervalMatrix(x)
    
if __name__ ==  '__main__':
  unittest.main()