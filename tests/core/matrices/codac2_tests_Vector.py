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
    

  def tests_vector_specific_to_python(self):

    x = Vector([1,2,3])
    y = Vector.zero(3)

    i = 0
    for xi in x: # using __iter__
      y[i] = xi
      i = i+1

    self.assertTrue(x == y)

    a,b,c = x # using __iter__
    self.assertTrue(a == x[0])
    self.assertTrue(b == x[1])
    self.assertTrue(c == x[2])

    v = Vector([*x, 42]) # using __iter__
    self.assertTrue(v == [1,2,3,42])

if __name__ ==  '__main__':
  unittest.main()