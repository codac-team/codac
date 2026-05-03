#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2025
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestSepPolytope(unittest.TestCase):

  def test_SepPolytope(self):

    # Polytope, defined as a set of facets
    facets =[ [ Row([1,1,1]), 3.0 ],
              [ Row([-1,1,1]), 3.0 ],
              [ Row([1,-1,1]), 3.0 ],
              [ Row([1,1,-1]), 3.0 ],
              [ Row([-1,-1,1]), 3.0 ],
              [ Row([-1,1,-1]), 3.0 ],
              [ Row([1,-1,-1]), 3.0 ],
              [ Row([-1,-1,-1]), 3.0 ] ]

    p = Polytope(IntervalVector([[-2,2],[-2,2],[-2,2]]),facets,True)
    c = CtcPolytope(p)

    x = IntervalVector(3)
    c.contract(x)
    self.assertTrue(x == IntervalVector([[-2,2],[-2,2],[-2,2]]))

    x = IntervalVector([[1.02,2.0],[1.02,2.0],[1.0,2.0]])
    c.contract(x)
    self.assertTrue(x.is_empty())

if __name__ ==  '__main__':
  unittest.main()
