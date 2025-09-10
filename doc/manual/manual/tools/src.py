#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import sys, os
import unittest
import math
from codac import *

class TestToolsManual(unittest.TestCase):

  def tests_tools_manual_serialization(test):

    # [tools-1-beg]
    x1 = IntervalVector([[0,1],[-oo,0],Interval.pi()])
    # x1 = [ [0, 1] ; [-inf, 0] ; [3.14159, 3.1416] ]

    # Serializing the interval vector
    with open("data.cdc", "wb") as f:
      serialize(f, x1)

    # Deserializing the interval vector (possibly in another project)
    x2 = IntervalVector()
    with open("data.cdc", "rb") as f:
      deserialize(f, x2)

    # x1 == x2
    # [tools-1-end]
    test.assertTrue(x1 == x2)

if __name__ ==  '__main__':
  unittest.main()