#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2026
#  \author     Maël Godard
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestSepTest(unittest.TestCase):

  def test_SepTest(self):

    X = IntervalVector([[1,3],[2,8],[-1,1]])
    sep_wrapper = SepWrapper_IntervalVector(X)
    sep_test = SepTest(sep_wrapper)

    x1 = IntervalVector([[1.5,2.5],[2.5,7.5],[-0.5,0.5]])
    x2 = IntervalVector([[10,11],[10,11],[10,11]])
    x3 = IntervalVector([[1,3],[2,8],[-1,1]])

    inner,outer = sep_test.separate(x1)
    self.assertTrue(inner==IntervalVector.empty(3))
    self.assertTrue(outer==x1)

    inner,outer = sep_test.separate(x2)
    self.assertTrue(inner==x2)
    self.assertTrue(outer==IntervalVector.empty(3))

    inner,outer = sep_test.separate(x3)
    self.assertTrue(inner==x3)
    self.assertTrue(outer==x3)

if __name__ ==  '__main__':
  unittest.main()