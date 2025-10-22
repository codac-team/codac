#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2025
#  \author     Maël Godard
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *
import sys
import math

class TestParallelepipedEval(unittest.TestCase):

  def test_parallelepiped_eval(self):
    x1 = ScalarVar()
    x2 = ScalarVar()
    x = VectorVar(2)

    f1 = AnalyticFunction([x1], [x1,sqr(x1)])

    f2 = AnalyticFunction([x1,x2], [x1, x2, sqr(x1)+sqr(x2)])
    f3 = AnalyticFunction([x], [x[0], x[1], sqr(x[0])+sqr(x[1])])

    p1 = f1.parallelepiped_eval(Interval(-0.1,0.1))
    
    self.assertTrue(Approx(p1.z,1e-6)==Vector([0.0,0.0]))
    self.assertTrue(Approx(p1.A,1e-6)==Matrix([[0.12,0.0],[0.0,0.02]]))

    p = f2.parallelepiped_eval(Interval(-0.1,0.1), Interval(-0.1,0.1))

    self.assertTrue(Approx(p.z,1e-6)==Vector([0,0,0]))
    self.assertTrue(Approx(p.A,1e-6)==Matrix([[0.14,0,0],[0,0.14,0],[0,0,0.04]]))


    dx = 0.4
    x0 = -2.0
    while x0<2.0:
      X0 = Interval(x0,x0+dx)
      y0 = -2.0
      while y0<2.0:
        Y0 = Interval(y0,y0+dx)
        
        p2 = f2.parallelepiped_eval(X0,Y0)
        p3 = f3.parallelepiped_eval(IntervalVector([X0,Y0]))

        self.assertTrue(Approx(p2.z,1e-6)==p3.z)
        self.assertTrue(Approx(p2.A,1e-6)==p3.A)
        y0 += dx
      x0 += dx
    
      
if __name__ ==  '__main__':
  unittest.main()