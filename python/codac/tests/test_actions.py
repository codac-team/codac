#!/usr/bin/env python

# Codac tests
# ---------------------------------------------------------------------------
# \date      2023
# \author    Simon Rohou
# \copyright Copyright 2023 Codac Team
# \license   This program is distributed under the terms of
#            the GNU Lesser General Public License (LGPL).

import unittest
try:
  import numpy as np
except:
  print("NUMPY UNAVAILABLE")
from codac import *
from codac.codac2 import *

class TestActions(unittest.TestCase):

    def tests_action(self):

        x = IntervalVector([[-1,0],[5,6]])
        a = OctaSym([2,-1])

        print(x)                 # Result: ([-1, 0] ; [5, 6])
        print(a(x))              # Result: ([5, 6] ; [-0, 1])
        print(a.invert()(a(x)))  # Result: ([-1, 0] ; [5, 6])

        print(a)                 # Result: (2 -1)
        print(a.invert())        # Result: (-2 1)
        print(a*a)               # Result: (-1 -2)

        self.assertEqual(a(x), IntervalVector([[5,6],[-0,1]]))
        self.assertEqual(a.invert()(a(x)), IntervalVector([[-1,0],[5,6]]))
        self.assertEqual(a, OctaSym([2,-1]))
        self.assertEqual(a.invert(), OctaSym([-2,1]))
        self.assertEqual(a*a, OctaSym([-1,-2]))
        
if __name__ ==  '__main__':
  unittest.main()