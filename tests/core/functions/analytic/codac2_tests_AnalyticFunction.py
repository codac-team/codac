#!/usr/bin/env python

#  Codac tests
#
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    This program is distributed under the terms of
#              the GNU Lesser General Public License (LGPL).

import unittest
from codac import *

class TestAnalyticFunction(unittest.TestCase):

  def tests_simple_analytic_function(self):

    #x = VectorVar(2)
    #f = ScalarAnalyticFunction([x], x[0]*(x[0]+x[1])+sqr(x[1]))
    #self.assertTrue(f.diff(Vector({2,3}))(0,0) == 7)
    #self.assertTrue(f.diff(Vector({2,3}))(0,1) == 8)

    x = ScalarVar()
    self.assertTrue(x.size() == 1)

    y = VectorVar(3)
    self.assertTrue(y.size() == 3)
    self.assertTrue(type(y[0]) == ScalarExpr)
    self.assertTrue(type(y[2]) == ScalarExpr)

    f = AnalyticFunction([x], x)
    self.assertTrue(Approx(f.eval(Interval(0))) == 0)
    f = AnalyticFunction([x], x+x)
    self.assertTrue(Approx(f.eval(Interval(2))) == 4)
    f = AnalyticFunction([x], x+x+2)
    self.assertTrue(Approx(f.eval(Interval(2))) == 6)
    f = AnalyticFunction([x], cos(x))
    self.assertTrue(Approx(f.eval(Interval(0))) == 1)

    f = AnalyticFunction([x], vec(x,x))

    #fvec = AnalyticFunction([x], vec(x,x))
    #self.assertTrue(Approx(f.eval(0)) == IntervalVector([[1],[1]]))

  #def tests_analytic_function(self):

    #x = VectorVar(2)
    #f = ScalarAnalyticFunction([x], x[0]*(x[0]+x[1])+sqr(x[1]))
    #self.assertTrue(f.diff(Vector({2,3}))(0,0) == 7)
    #self.assertTrue(f.diff(Vector({2,3}))(0,1) == 8)
#
#    x,y = ScalarVar(), ScalarVar()
#    f = ScalarAnalyticFunction([x,y], x*(x+y)+sqr(y))
#    self.assertTrue(f.diff(Vector({2,3}))(0,0) == 7)
#    self.assertTrue(f.diff(Vector({2,3}))(0,1) == 8)

if __name__ ==  '__main__':
  unittest.main()