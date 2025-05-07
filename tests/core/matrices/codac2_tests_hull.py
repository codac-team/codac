#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Gilles Chabert, Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *
import sys
import math

class TestHull(unittest.TestCase):

  def tests_hull_IntervalVector(self):

    self.assertTrue(hull(Vector([1,2])) == IntervalVector([[1,1],[2,2]]))
    self.assertTrue(hull(Vector([1,2]),Vector([1,2]),Vector([1,2]),Vector([1,2])) == IntervalVector([[1,1],[2,2]]))
    self.assertTrue(hull(Vector([1,2]),Vector([9,10])) == IntervalVector([[1,9],[2,10]]))
    self.assertTrue(hull(Vector([1,2]),IntervalVector([9,10])) == IntervalVector([[1,9],[2,10]]))
    self.assertTrue(hull(IntervalVector([1,2]),Vector([9,10])) == IntervalVector([[1,9],[2,10]]))
    self.assertTrue(hull(IntervalVector([1,2]),IntervalVector([9,10])) == IntervalVector([[1,9],[2,10]]))
    self.assertTrue(hull(IntervalVector([1,2]),IntervalVector.empty(2)) == IntervalVector([[1,1],[2,2]]))
    self.assertTrue(hull(IntervalVector([1,2])) == IntervalVector([[1,1],[2,2]]))
    self.assertTrue(hull(2.*IntervalVector([1,2])) == IntervalVector([[2,2],[4,4]]))

  def tests_hull_IntervalMatrix(self):

    self.assertTrue(hull(Matrix([[1,2],[3,4]])) == IntervalMatrix(Matrix([[1,2],[3,4]])))
    self.assertTrue(hull(3.*Matrix([[1,2],[3,4]])) == IntervalMatrix(Matrix([[3,6],[9,12]])))

if __name__ ==  '__main__':
  unittest.main()