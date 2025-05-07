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

class TestIntervalVector(unittest.TestCase):

  def tests_cart_prod_vector(self):

    self.assertTrue(cart_prod(Vector([3])) == Vector([3]))
    self.assertTrue(cart_prod(1,2,3.5) == Vector([1,2,3.5]))
    self.assertTrue(cart_prod(1,2,3.5,Vector.ones(2)) == Vector([1,2,3.5,1,1]))
    self.assertTrue(cart_prod(Vector.ones(2),1,2,3.5) == Vector([1,1,1,2,3.5]))
    self.assertTrue(cart_prod(Vector.zero(3)) == Vector.zero(3))
    self.assertTrue(cart_prod(Vector.zero(3),Vector.ones(4)) == Vector([0,0,0,1,1,1,1]))

  def tests_cart_prod_intervalvector(self):

    self.assertTrue(cart_prod(IntervalVector([[0,1]])) == IntervalVector([[0,1]]))
    self.assertTrue(cart_prod(IntervalVector([[0,1],[2,3],[4,5]]),IntervalVector.empty(3)).is_empty())
    self.assertTrue(cart_prod(IntervalVector.empty(3),IntervalVector([[0,1],[2,3],[4,5]])).is_empty())
    self.assertTrue(cart_prod([[0,1],[2,3],[4,5]]) == IntervalVector([[0,1],[2,3],[4,5]]))
    self.assertTrue(cart_prod(IntervalVector.empty(3)) == IntervalVector.empty(3))
    self.assertTrue(cart_prod([[0,1],[2,3],[4,5]],[[8,9]]) == IntervalVector([[0,1],[2,3],[4,5],[8,9]]))
    self.assertTrue(cart_prod(25.,[[0,1],[2,3],[4,5]],[[8,9]],Vector.ones(3)) == IntervalVector([[25],[0,1],[2,3],[4,5],[8,9],[1],[1],[1]]))

if __name__ ==  '__main__':
  unittest.main()