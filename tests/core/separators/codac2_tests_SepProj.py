#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestSepProj(unittest.TestCase):

  def tests_SepProj(self):

    x = VectorVar(3)
    f_ellipsoid = AnalyticFunction([x], 2*sqr(x[0])+x[0]*x[1]+x[0]*x[2]+sqr(x[1])+sqr(x[2]))
    sep_ellipsoid = SepInverse(f_ellipsoid, [0.7,1])
    sep_proj = SepProj(sep_ellipsoid, [0,1], [[0.1,0.2]])

    # todo: test with SepProj sep_proj(sep_ellipsoid, {0,1}, [[-oo,oo]])

    #pave(IntervalVector([[-2,2],[-2,2]]), sep_proj, 0.1)

    b = IntervalVector([[0.2,0.4],[0.0,1]])
    inner,outer = sep_proj.separate(b,1e-3)
    self.assertTrue(Approx(outer,1e-2) == IntervalVector([[0.2,0.4],[0,0.94]]))

    b = IntervalVector([[-0.4,-0.2],[0.95,1.5]])
    inner,outer = sep_proj.separate(b,1e-3)
    self.assertTrue(Approx(outer,1e-2) == IntervalVector([[-0.4,-0.2],[0.95,1.15]]))

    b = IntervalVector([[0.65],[-0.5,-0.4]])
    inner,outer = sep_proj.separate(b,1e-3)
    self.assertTrue(inner.is_empty())

if __name__ ==  '__main__':
  unittest.main()