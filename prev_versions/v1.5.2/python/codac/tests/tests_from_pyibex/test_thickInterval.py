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

  def test_constructor_01(self):
    a = ThickInterval([1,2], [2,3])
    self.check_thickinterval(a, Interval(1,2), Interval(2,3))

  def test_constructor_02(self):
    a = ThickInterval(1,2)
    self.check_thickinterval(a, Interval(1), Interval(2))

  def test_constructor_03(self):
    a = ThickInterval(Interval(0, 3), Interval.POS_REALS);
    self.assertEqual(a.lb() , Interval(0, 3));
    self.assertEqual(a.ub() , Interval.POS_REALS);

  def test_constructor_04(self):
    a = ThickInterval(Interval.NEG_REALS, Interval(-4, 0));
    self.assertEqual(a.lb() , Interval.NEG_REALS);
    self.assertEqual(a.ub() , Interval(-4, 0));

  def test_constructor_05(self):
    a = ThickInterval(Interval.NEG_REALS, Interval(-4, 0));
    self.assertEqual(a.lb() , Interval.NEG_REALS);
    self.assertEqual(a.ub() , Interval(-4, 0));

  def test_constructor_06(self):
    a = ThickInterval(Interval.ALL_REALS, Interval.ALL_REALS);
    self.assertEqual(a.lb() , Interval.ALL_REALS);
    self.assertEqual(a.ub() , Interval.ALL_REALS);

  def test_constructor_07(self):
    a = ThickInterval(Interval.EMPTY_SET, Interval.EMPTY_SET);
    self.assertTrue( a.is_empty() );

  def test_invalid_constructor_01(self):
    a = ThickInterval(Interval(2,-2));
    self.assertTrue( a.is_empty() );

  def test_invalid_constructor_02(self):
    a = ThickInterval(Interval(2,3), Interval(1, 2));
    self.assertTrue( a.is_empty() );

  def test_invalid_constructor_03(self):
    a = ThickInterval(Interval(1,3), Interval(1, 2));
    self.assertTrue( a.is_empty() );

  def test_invalid_constructor_04(self):
    a = ThickInterval(Interval(1,2), Interval(1, 2));
    self.assertEqual(a.lb(), Interval(1,2));
    self.assertEqual(a.ub(), Interval(1,2));

  def test_invalid_constructor_01(self):
    a = ThickInterval(Interval.POS_REALS, Interval.NEG_REALS);
    self.assertTrue(a.is_empty());

  def test_intersect(self):
    a = ThickInterval(Interval(-2, 1), Interval(0, 3));
    b = ThickInterval(Interval(-2, 1), Interval(2, 3));
    c = ThickInterval(Interval(1, 2), Interval(1, 2));
    self.assertTrue(a.intersects(Interval(0, 1)) == True );
    self.assertTrue(a.intersects(Interval(1,2))  == False);
    self.assertTrue(b.intersects(Interval(1,2))  == True);
    self.assertTrue(c.intersects(Interval.EMPTY_SET) == False);
    self.assertTrue(c.intersects(Interval(1,2)) ==True );
    self.assertTrue(a.intersects(ThickInterval(Interval(-0.0001, 0.5),Interval(2, 3))) == False);
    self.assertTrue(a.intersects(ThickInterval(Interval(-1, 0), Interval(2, 3)))  == True);

  def test_is_degenerated(self):
    self.assertTrue(ThickInterval(Interval(-2,-2),Interval(-1,3)).is_degenerated());
    self.assertFalse(ThickInterval(Interval(-2,-2),Interval(-1,3)).is_interval());
    self.assertTrue(ThickInterval(Interval(-3,-1),Interval(4,4)).is_degenerated());
    self.assertFalse(ThickInterval(Interval(-3,-1),Interval(4,4)).is_interval());
    self.assertTrue(ThickInterval(Interval(-1,-1),Interval(4,4)).is_degenerated());
    self.assertTrue(ThickInterval(Interval(-1,-1),Interval(4,4)).is_interval());

  def test_superset(self):
    self.assertEqual(ThickInterval(Interval(-2, 1), Interval(0, 4)).superset(), Interval(-2, 4));
    self.assertEqual(ThickInterval(Interval(-2, 1), Interval(5, 4)).superset(), Interval.EMPTY_SET);

  def test_subset(self):
    self.assertEqual(ThickInterval(Interval(-2, 1), Interval(0, 4)).subset(), Interval.EMPTY_SET);
    self.assertEqual(ThickInterval(Interval(-2, 1), Interval(4, 5)).subset(), Interval(1, 4) );


class TestIntersectThickInterval(unittest.TestCase):

  def setUp(self):
    self.a = ThickInterval( Interval(0, 2), Interval(4, 5));
    self.b = ThickInterval( Interval(1,2), Interval(4, 10));

  def test_intersect_01(self):
    res = ThickInterval([1,2],  [4, 5])
    self.assertEqual(self.a.inter(self.b), res)

  def test_intersect_02(self):
    res = ThickInterval([1,2],  [4, 5])
    self.assertEqual((self.a & self.b), res)


class TestisSubsetThickInterval(unittest.TestCase):

  def setUp(self):
    self.a = ThickInterval(Interval(0, 2), Interval(4, 5));
    self.b = ThickInterval(Interval(1,2), Interval(4, 10));

  def test_isSubset_01(self):
    res = self.a.is_inside(Interval(-1, 6))
    self.assertEqual(res, IN)
    res = self.a.is_inside(Interval(0, 5))
    self.assertEqual(res, IN)

  def test_isSubset_02(self):
    res = self.a.is_inside(Interval(-4, -1))
    self.assertEqual(res, OUT)

  def test_isSubset_03(self):
    res = self.a.is_inside(Interval(3, 5))
    self.assertEqual(res, MAYBE)

  def test_isSubset_04(self):
    res = self.a.is_inside(Interval(1, 5))
    self.assertEqual(res, MAYBE_IN)

  def test_isSubset_05(self):
    res = self.a.is_inside(Interval(-1, 1))
    self.assertEqual(res, MAYBE_OUT)
    res = self.a.is_inside(Interval(-1, 0))
    self.assertEqual(res, MAYBE_OUT)


if __name__ == '__main__':

  unittest.main()