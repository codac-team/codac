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
from codac.unsupported import *

class TestThickInterval(unittest.TestCase):

  def check_thickinterval(self, y, res_lb, res_ub):
    self.assertEqual(y.lb(), res_lb)
    self.assertEqual(y.ub(), res_ub)

  def check_thickBox(self, y, res_lb, res_ub):
    for i in range(y.size):
      self.check_thickinterval(y[i], res_lb[i], res_ub[i])

  def test_constructor_01(self):
    a = ThickInterval([1,2], [2,3])
    b = ThickBox(3, a)
    self.check_thickBox(b, [Interval(1,2), Interval(1,2), Interval(1,2)],
                           [Interval(2,3), Interval(2,3), Interval(2,3)])

  def test_Assign(self):
    a = ThickInterval([1,2], [2,3])
    b = ThickBox(2, a)
    b[1] = ThickInterval([-2,0], [10,20])
    self.check_thickBox(b, [Interval(1,2), Interval(-2,0)],
                           [Interval(2,3), Interval(10,20)])

  def test_Init(self):
    lb = IntervalVector([1,2,3,4,5])
    ub = IntervalVector([2,3,4,5,6])
    a = ThickBox(lb, ub)
    self.check_thickBox(a, [Interval(i) for i in [1,2,3,4,5]],
                           [Interval(i) for i in [2,3,4,5,6]])

  def test_inner_box01(self):
    lb = IntervalVector(2, [1, 3])
    ub = IntervalVector(2, [2, 4])
    a = ThickBox(lb, ub)
    X, b  = a.inner_box()
    self.assertEqual(X, IntervalVector(2, [2,3]))
    self.assertEqual(b , [True, True])

  def test_equal(self):
    res = ThickBox(IntervalVector([[-3, -1], [-1,1]]), IntervalVector([[2,4],[3,5]]))
    res2 = ThickBox(IntervalVector([[-3, -1], [-1,1]]), IntervalVector([[2,4],[3,5]]))
    self.assertTrue( res2  == res)
    self.assertEqual( res2 , res)

  def test_inner_box02(self):
    lb = IntervalVector([[1, 2], [1,5]])
    ub = IntervalVector([[4,5], [2,7]])
    a = ThickBox(lb, ub)
    X, b  = a.inner_box()
    self.assertEqual(X, IntervalVector([[2,4], [2,5]]))
    self.assertEqual(b , [False, True])

  def test_inner_box03(self):
    lb = IntervalVector([[1, 2], [1,2]])
    ub = IntervalVector([[4,5], [3,7]])
    a = ThickBox(lb, ub)
    X, b  = a.inner_box()
    self.assertEqual(X, IntervalVector([[2,4], [2,3]]))
    self.assertEqual(b , [False, False])

  def test_inner_box04(self):
    lb = IntervalVector([[-9, 11], [-8,12]])
    ub = IntervalVector([[-9, 11], [-8,12]])
    a = ThickBox(lb, ub)
    X, b  = a.inner_box()
    self.assertEqual(X, IntervalVector([[-9,11], [-8,12]]))
    self.assertEqual(b , [True, True])

class TestIntersectThickBox(unittest.TestCase):
  def setUp(self):
    self.a = ThickBox( IntervalVector([[-6, -4],[-3, -1]]),IntervalVector([[2,4],[3,5]]) )
    self.b = ThickBox( IntervalVector([[-3, -1], [-1,1]]), IntervalVector([[7,9], [6,8]]) )

  def test_intersect_01(self):
    res = ThickBox(IntervalVector([[-3, -1], [-1,1]]), IntervalVector([[2,4],[3,5]]))
    self.assertEqual(self.a.inter(self.b) , res)

  def test_intersect_02(self):
    res = ThickBox(IntervalVector([[-3, -1], [-1,1]]), IntervalVector([[2,4],[3,5]]))
    res2 = ThickBox(IntervalVector([[-3, -1], [-1,1]]), IntervalVector([[2,4],[3,5]]))
    self.assertEqual( res2 , res)
    self.assertEqual( ( self.a & self.b ) , res)

if __name__ == '__main__':

  unittest.main()