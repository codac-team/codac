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

  def test_def_Matrix(self):

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


  def test_fnc_Matrix(self):

    x = Matrix([
      [ -1, 3  ],
      [ -6, -9 ]
    ])

    self.assertTrue(abs(x) == Matrix([
      [ 1, 3 ],
      [ 6, 9 ]
    ]))

    y = Matrix([
      [ -1.2, 3.9  ],
      [ -6.2, -9.0 ]
    ])

    self.assertTrue(floor(y) == Matrix([
      [ -2, 3  ],
      [ -7, -9 ]
    ]))

    self.assertTrue(ceil(y) == Matrix([
      [ -1, 4  ],
      [ -6, -9 ]
    ]))

    self.assertTrue(round(y) == Matrix([
      [ -1, 4  ],
      [ -6, -9 ]
    ]))

  def test_def_Matrix_zero_init(self):

    x = Matrix(2,2)
    print(x)
    self.assertTrue(x(0,0) == 0)
    self.assertTrue(x(1,0) == 0)
    self.assertTrue(x(0,1) == 0)
    self.assertTrue(x(1,1) == 0)

    v = Vector(2)
    self.assertTrue(v[0] == 0)
    self.assertTrue(v[1] == 0)

if __name__ ==  '__main__':
  unittest.main()