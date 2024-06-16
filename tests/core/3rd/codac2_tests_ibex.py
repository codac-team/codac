#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestIbex(unittest.TestCase):
  #pass # IBEX is not available in Python # Gives "NO TESTS RAN" error with Python 3.12...

  def tests_ibex(self):
     self.assertTrue(True)

if __name__ ==  '__main__':
  unittest.main()