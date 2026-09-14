#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestSepInter(unittest.TestCase):

  def test_SepInter_box(self):

    v = VectorVar(1)
    s1 = SepInverse(AnalyticFunction([v], v), IntervalVector([[-1,1]]))
    s2 = SepInverse(AnalyticFunction([v], v), IntervalVector([[5,6]]))

    s3 = SepInter(s1,s2)

    # Testing constructors
    s4 = SepInter(s1,s2)
    s5 = SepInter(s1,s2,s3) # different types
    self.assertTrue(s4.nb() == 2)
    self.assertTrue(s5.nb() == 3)

    # Testing constructors (lists)
    s4 = SepInter([s1,s2])
    s5 = SepInter([s1,s2,s3]) # different types
    self.assertTrue(s4.nb() == 2)
    self.assertTrue(s5.nb() == 3)

if __name__ ==  '__main__':
  unittest.main()