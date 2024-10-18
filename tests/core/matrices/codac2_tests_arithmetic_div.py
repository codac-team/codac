#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestArithmeticDiv(unittest.TestCase):

  def tests_ArithmeticDiv(self):

    #inline Vector operator/(const Vector& x1, double x2)
    self.assertTrue(Approx(Vector([1,2,3])/10.) == Vector([.1,.2,.3]))

    #inline IntervalVector operator/(const Vector& x1, const Interval& x2)
    self.assertTrue(Approx(Vector([1,2,3])/Interval(10.,oo)) == IntervalVector([[0,.1],[0,.2],[0,.3]]))

    #Matrix operator/(const M& x1, double x2)
    self.assertTrue(Approx(Matrix([[1,2],[3,4]])/10.) == Matrix([[.1,.2],[.3,.4]]))
    self.assertTrue(Approx(Matrix([[1,2],[3,4]]).block(0,0,2,2)/10.) == Matrix([[.1,.2],[.3,.4]]))

    #IntervalMatrix operator/(const M& x1, const Interval& x2)
    self.assertTrue(Approx(Matrix([[1,2],[3,4]])/Interval(10.,oo)) == IntervalMatrix([[[0,.1],[0,.2]],[[0,.3],[0,.4]]]))
    self.assertTrue(Approx(Matrix([[1,2],[3,4]]).block(0,0,2,2)/Interval(10.,oo)) == IntervalMatrix([[[0,.1],[0,.2]],[[0,.3],[0,.4]]]))

    #inline IntervalVector operator/(const IntervalVector& x1, double x2)
    self.assertTrue(Approx(IntervalVector([[1],[2],[3]])/10.) == IntervalVector([[.1],[.2],[.3]]))

    #inline IntervalVector operator/(const IntervalVector& x1, const Interval& x2)
    self.assertTrue(Approx(IntervalVector([[1],[2],[3]])/Interval(10.,oo)) == IntervalVector([[0,.1],[0,.2],[0,.3]]))

    #IntervalMatrix operator/(const IM& x1, double x2)
    self.assertTrue(Approx(IntervalMatrix([[[1],[2]],[[3],[4]]])/10.) == IntervalMatrix([[[.1],[.2]],[[.3],[.4]]]))
    self.assertTrue(Approx(IntervalMatrix([[[1],[2]],[[3],[4]]]).block(0,0,2,2)/10.) == IntervalMatrix([[[.1],[.2]],[[.3],[.4]]]))

    #IntervalMatrix operator/(const IM& x1, const Interval& x2)
    self.assertTrue(Approx(IntervalMatrix([[[1],[2]],[[3],[4]]])/Interval(10.,oo)) == IntervalMatrix([[[0,.1],[0,.2]],[[0,.3],[0,.4]]]))
    self.assertTrue(Approx(IntervalMatrix([[[1],[2]],[[3],[4]]]).block(0,0,2,2)/Interval(10.,oo)) == IntervalMatrix([[[0,.1],[0,.2]],[[0,.3],[0,.4]]]))

if __name__ ==  '__main__':
  unittest.main()