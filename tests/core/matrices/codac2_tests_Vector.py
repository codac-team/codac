#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestVector(unittest.TestCase):

  def tests_Vector(self):
    
    self.assertTrue(Vector([1,2])+Vector([3,4]) == Vector([4,6]))

if __name__ ==  '__main__':
  unittest.main()