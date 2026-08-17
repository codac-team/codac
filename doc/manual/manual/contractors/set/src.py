#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2026
#  \author     Simon Rohou
#  \copyright  Copyright 2026 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import sys, os
import unittest
import math
from codac import *

class TestCtcSetManual(unittest.TestCase):

  def tests_CtcInter_manual(test):

    # [ctcinter-1-beg]
    c1 = CtcWrapper([[-10,10],[-2,2]])
    c2 = CtcWrapper([[-12,2],[0,4]])

    c3 = CtcInter(c1,c2)
    # or...
    c3 = c1 & c2
    # c3 is a CtcInter gathering the two contractors.
    # [ctcinter-1-end]

    # [ctcinter-2-beg]
    x = IntervalVector([[-oo,oo],[-oo,oo]])
    x = c3.contract(x)
    # x = [ [-10, 2] ; [0, 2] ]
    # [ctcinter-2-end]

    # [ctcinter-3-beg]
    c4 = CtcInter(2) # initially neutral conjunction on 2d boxes
    c4 &= c1
    c4 &= c2

    n = c4.nb()
    # n = 2
    # [ctcinter-3-end]

if __name__ ==  '__main__':
  unittest.main()