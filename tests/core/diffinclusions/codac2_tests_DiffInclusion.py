#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2026
#  \author     Simon Rohou
#  \copyright  Copyright 2026 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *


class TestDiffInclusion(unittest.TestCase):

  def test_DiffInclusion(self):

      x = VectorVar(1)
      u = VectorVar(1)
      f = AnalyticFunction([x,u],-x+u)

      di = DiffInclusion(f)

      integration = Integration_Box(di)

      i1 = IntervalVector([[-5,5]])
      i2 = IntervalVector([[-5,5]])
      r = integration.contract_one_step(Interval(0.0,0.1),
		IntervalVector([[1.0]]), i1, i2, IntervalVector([[0.0,0.2]]))

      self.assertTrue(r)
      self.assertTrue(i1[0].is_superset(Interval(0.9048,1)))
      self.assertTrue(i2[0].is_superset(Interval(0.9048,0.9239)))

if __name__ ==  '__main__':
  unittest.main()
