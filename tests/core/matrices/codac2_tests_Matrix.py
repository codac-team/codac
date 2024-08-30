#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestMatrix(unittest.TestCase):

  def tests_Matrix(self):

    x = Matrix([
      [ 1, 2 ],
      [ 3, 4 ],
    ])

    x[0,1] = 42
    x[1,1] = 42

    self.assertTrue(x == Matrix([
      [ 1, 42 ],
      [ 3, 42 ],
    ]))

    # The following simple call was a problem before
    # (order of def of constructors):
    m = Matrix(x)

if __name__ ==  '__main__':
  unittest.main()