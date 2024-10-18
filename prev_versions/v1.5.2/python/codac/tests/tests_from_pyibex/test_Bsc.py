#!/usr/bin/env python

# Codac tests
# Originated from the former pyIbex library (Benoît Desrochers)
# ---------------------------------------------------------------------------
# \date      2022
# \author    Benoît Desrochers
# \copyright Copyright 2022 Codac Team
# \license   This program is distributed under the terms of
#            the GNU Lesser General Public License (LGPL).

import unittest
from codac import *

class TestBsc(unittest.TestCase):

  def assertApproxItvVec(self, first, second):
    for i in range(len(first)):
      self.assertAlmostEqual(first[i].lb(), second[i].lb())
      self.assertAlmostEqual(first[i].ub(), second[i].ub())

  def test_LargestFirst(self):
    bsc = LargestFirst(0.1)
    a = IntervalVector([[1,2], [2,6]])
    (c,b) = bsc.bisect(a)
    self.assertApproxItvVec(c, IntervalVector([[1,2],[2,3.8]]))
    self.assertApproxItvVec(b, IntervalVector([[1,2],[3.8,6]]))
    self.assertTrue(True)

  def test_LargestFirst2(self):
    bsc = LargestFirst([0.1, 1])
    a = IntervalVector([[1,2], [2,6]])
    (c,b) = bsc.bisect(a)
    self.assertApproxItvVec(c, IntervalVector([[1,1.45],[2,6]]))
    self.assertApproxItvVec(b, IntervalVector([[1.45,2],[2,6]]))
    self.assertTrue(True)


if __name__ == '__main__':

  unittest.main()