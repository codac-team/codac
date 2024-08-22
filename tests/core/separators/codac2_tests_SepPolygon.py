#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestSepPolygon(unittest.TestCase):

  def tests_SepPolygon(self):

    s = SepPolygon([[[3],[-1]],[[3],[4]],[[5],[6]],[[-1],[1]]])

    x = IntervalVector(2)
    xs = s.separate(IntervalVector(2))
    self.assertTrue(xs.inner == IntervalVector(2))
    self.assertTrue(xs.outer == IntervalVector([[-1,5],[-1,6]]))

    x = IntervalVector([[3.02,3.16],[2.5,3.2]]) # possible bug
    xs = s.separate(x)
    self.assertTrue(xs.inner == x)
    self.assertTrue(xs.outer.is_empty())

if __name__ ==  '__main__':
  unittest.main()