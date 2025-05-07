#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestSepTransform(unittest.TestCase):
  
  def tests_SepTransform(self):
    
    x = VectorVar(2)
    s1 = SepInverse(AnalyticFunction([x], sqr(x[0])+sqr(x[1])), [0,1])
    s2 = SepTransform(s1,
      AnalyticFunction([x], vec(x[0]+2,x[1]+2)),
      AnalyticFunction([x], vec(x[0]-2,x[1]-2)))

    #pave([[-5,5],[-5,5]], s2, 0.01)

    inner,outer = s2.separate(IntervalVector(2))
    self.assertTrue(inner == IntervalVector(2))
    self.assertTrue(Approx(outer,1e-1) == IntervalVector([[-3,-1],[-3,-1]]))

    inner,outer = s2.separate(IntervalVector([[-2.2,-1.8],[-2.2,-1.8]]))
    self.assertTrue(inner.is_empty())
    self.assertTrue(outer == IntervalVector([[-2.2,-1.8],[-2.2,-1.8]]))

    b = IntervalVector([[-2.5,-1.5],[-2,10]])
    #DefaultFigure.draw_box(b,Color.purple())
    inner,outer = s2.separate(b)
    #DefaultFigure.draw_box(xs.inner,Color.green())
    self.assertTrue(Approx(inner,1e-1) == IntervalVector([[-2.5,-1.5],[-1.134,10]]))
    self.assertTrue(Approx(outer,1e-1) == IntervalVector([[-2.5,-1.5],[-2,-1]]))

if __name__ ==  '__main__':
  unittest.main()