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

try:
  import cPickle as pickle  # Use cPickle on Python 2.7
except ImportError:
  import pickle

class TestBoolInterval(unittest.TestCase):
  def test_import(self):
    BoolInterval.YES
    BoolInterval.NO
    BoolInterval.MAYBE
    BoolInterval.EMPTY_BOOL

class TestInterval(unittest.TestCase):

  def setUp(self):
    Interval.__str__ = interval_str
    self.a = Interval(-2,3)

  def test_Interal2array(self):
    self.assertEqual(self.a.lb(), -2)
    self.assertEqual(self.a.ub(), 3)
    # self.assertEqual(self.a[4], np.nan)

  def test_constructor(self):
    c1 = Interval()
    c2 = Interval(-2)
    c3 = Interval(-2,4)

  def test_ConstantItv(self):
    L = [ "PI",
          "TWO_PI",
          "HALF_PI",
          "ALL_REALS",
          "ZERO",
          "ONE",
          "POS_REALS",
          "NEG_REALS"]
    for name in L:
      a = getattr(Interval, name)
      a.set_empty()
      self.assertEqual(a, Interval.EMPTY_SET)
      self.assertFalse(getattr(Interval, name).is_empty())

  def test_constructor_oo(self):
    self.assertEqual(oo, float('inf'))
    self.assertEqual(Interval(0, oo), Interval.POS_REALS)
    self.assertEqual(Interval(-oo, 0), Interval.NEG_REALS)

  # def test_constructor_with_list(self):
  #   self.assertEqual( Interval([1,2]), Interval(1,2))

  def test_pickling(self):
    Lp = [Interval(0, 5), Interval(0, oo), Interval.EMPTY_SET]
    for p in Lp:
      data = pickle.dumps(p, 2)  # Must use pickle protocol >= 2
      p2 = pickle.loads(data)
      self.assertEqual(p, p2)

  def test_inflate(self):
    c1 = Interval(0).inflate(1)
    self.assertTrue(c1 == Interval(-1, 1))

  def test_set_empty(self):
    self.a.set_empty()
    self.assertTrue(self.a.is_empty())

  def test_mid(self):
    self.assertEqual(self.a.mid(),0.5)

  def test_rad(self):
    self.assertEqual(self.a.rad(),2.5)

  def test_diam(self):
    self.assertEqual(self.a.diam(),5)

  def test_mig(self):
    self.assertEqual(self.a.mig(),0)

  def test_mag(self):
    self.assertEqual(self.a.mag(),3)

  def test_is_subset(self):
    c = Interval(-2,1)
    self.assertFalse(self.a.is_subset(c))
    self.assertTrue(c.is_subset(self.a))

  def test_is_strict_subset(self):
    c = Interval(-2,1)
    self.assertTrue(c.is_strict_subset(self.a))
    d = Interval(-1.9,1)
    self.assertTrue(d.is_strict_subset(self.a))

  def test_is_interior_subset(self):
    c = Interval(-2,1)
    self.assertFalse(c.is_interior_subset(self.a))
    d = Interval(-1.9,1)
    self.assertTrue(d.is_interior_subset(self.a))

  def test_is_strict_interior_subset(self):
    c = Interval(-2,1)
    self.assertFalse(c.is_strict_interior_subset(self.a))
    d = Interval(-1.9,1)
    self.assertTrue(d.is_strict_interior_subset(self.a))

  def test_is_superset(self):
    c = Interval(1)
    self.assertTrue(self.a.is_superset(c))

  def test_is_strict_superset(self):
    c = Interval(1)
    self.assertTrue(self.a.is_superset(c))

  def test_contains(self):
    self.assertTrue(self.a.contains(0))
    self.assertTrue( 0 in self.a)

  def test_interior_contains(self):
    self.assertTrue(self.a.interior_contains(0))
    self.assertFalse(self.a.interior_contains(-2))

  def test_intersects(self):
    c1 = Interval(0,10)
    self.assertTrue(self.a.intersects(c1))

  def test_overlaps(self):
    c1 = Interval(3)
    self.assertTrue(self.a.intersects(c1))

  def test_is_disjoint(self):
    c1 = Interval(-1, 2)
    c2 = Interval(2.01,4)
    self.assertTrue(c1.is_disjoint(c2))

  def test_is_degenerated(self):
    c = Interval(-1)
    self.assertTrue(c.is_degenerated)

  def test_is_unbounded(self):
    c = Interval.POS_REALS
    self.assertTrue(c.is_unbounded())

  def test_is_bisectable(self):
    self.assertTrue(self.a.is_bisectable())
    c = Interval(0)
    self.assertFalse(c.is_bisectable())

  def test_rel_distance(self):
    c = Interval(-1, 2)

  def test_complementary(self):
    b, c = self.a.complementary()
    self.assertEqual(Interval(float('-inf'), -2),b)
    self.assertEqual(Interval(3, float('inf')), c)

  def test_diff(self):
    c, d = self.a.diff(Interval(2,4))
    self.assertEqual( Interval(-2, 2), c)
    self.assertTrue(d.is_empty())

  def test_bisect(self):
    a = Interval(3,5)
    (c,d) = a.bisect()

  def test_ops(self):
    c = Interval(2,3)
    d = Interval(1,2)
    res = c + d
    self.assertEqual(res, Interval(3,5), " test + ops")
    res = c - d
    self.assertEqual(res, Interval(0,2)  , " test - ops")
    res = c * d
    self.assertEqual(res, Interval(2, 6)  , " test * ops")
    res = c / d
    self.assertEqual(res, Interval(1, 3)  , " test / ops")
    res = c & d
    self.assertEqual(res, Interval(2)  , " test & ops")
    res = c | d
    self.assertEqual(res, Interval(1, 3)  , " test | ops")
    res = c
    res += Interval(-2, 3)
    self.assertEqual(res, Interval(0, 6) , " test += ops")
    res = Interval(2,3)
    res -= Interval(-2, 3)
    self.assertEqual(res, Interval(-1, 5) , " test -= ops")
    res = Interval(2,3)
    res *= Interval(-2, 3)
    self.assertEqual(res, Interval(-6, 9) , " test *= ops")
    res = Interval(2,3)
    res /= Interval(-2, 3)
    self.assertEqual(res, Interval.ALL_REALS , " test /= ops")

  def test_ops_float(self):
    c = Interval(2,3)
    res = c + 1
    self.assertEqual(res, Interval(3, 4)  , " test + 1 ops")
    res = 1.0 + c
    self.assertEqual(res, Interval(3, 4)  , " test + 1 ops")
    res = c + 1.0
    self.assertEqual(res, Interval(3, 4)  , " test + 1 ops")
    res = c - 1
    self.assertEqual(res, Interval(1, 2)  )
    res = 1.0 - c
    self.assertEqual(res, Interval(-2, -1))
    res = c - 1.0
    self.assertEqual(res, Interval(1, 2))
    res = c * 3
    self.assertEqual(res, Interval(6, 9)   )
    res = 3.0 * c
    self.assertEqual(res, Interval(6, 9)   )
    res = c * 3.0
    self.assertEqual(res, Interval(6, 9)   )
    res = c / 2
    self.assertEqual(res, Interval(1, 1.5)  )
    res = 2.0 / c
    self.assertEqual(res, Interval(2)/3.0 | Interval(1)  )
    res = c / 2.0
    self.assertEqual(res, Interval(1, 1.5)  )

  def test_abs(self):
    c = Interval(-2,4)
    self.assertEqual(abs(c), Interval(0, 4))
  def test_sqr(self):
    c = Interval(-2,4)
    self.assertEqual(sqr(c), Interval(0,16))
  def test_root(self):
    c = Interval(-2,4)
    root(c,3)
  def test_exp(self):
    c = Interval(-2,4)
    exp(c)
  def test_log(self):
    c = Interval(-2,4)
    log(c)
  def test_cos(self):
    c = Interval(-2,4)
    cos(c)
  def test_sin(self):
    c = Interval(-2,4)
    sin(c)
  def test_tan(self):
    c = Interval(-2,4)
    tan(c)
  def test_acos(self):
    c = Interval(-2,4)
    acos(c)
  def test_asin(self):
    c = Interval(-2,4)
    asin(c)
  def test_atan(self):
    c = Interval(-2,4)
    atan(c)
  def test_atan2(self):
    c = Interval(-2,4)
    atan2(c,self.a)
  def test_cosh(self):
    c = Interval(-2,4)
    cosh(c)
  def test_sinh(self):
    c = Interval(-2,4)
    sinh(c)
  def test_tanh(self):
    c = Interval(-2,4)
    tanh(c)
  def test_acosh(self):
    c = Interval(-2,4)
    acosh(c)
  def test_asinh(self):
    c = Interval(-2,4)
    asinh(c)
  def test_atanh(self):
    c = Interval(-2,4)
    atanh(c)
  def test_max(self):
    c = Interval(-2,4)
    max(c,self.a)
  def test_min(self):
    c = Interval(-2,4)
    min(c,self.a)
  def test_sign(self):
    c = Interval(-2,4)
    sign(c)
  def test_chi(self):
    c = Interval(-2,4)
    b = Interval(-2,4)
    chi(c,self.a,b)
  def test_integer(self):
    c = Interval(-2,4)
    integer(c)
  def test_pow(self):
    a = Interval(2, 3)
    self.assertEqual(a**2, Interval(4, 9))
    a**3
    a**2.3
    a**Interval(2,3)
  def test_bwd_add(self):
    c = Interval(-2,4)
    b = Interval(4,2)
    bwd_add(c,self.a,b)
  def test_bwd_sub(self):
    c = Interval(-2,4)
    b = Interval(4,2)
    bwd_sub(c,self.a,b)
  def test_bwd_mul(self):
    c = Interval(-2,4)
    b = Interval(4,2)
    bwd_mul(c,self.a,b)
  def test_bwd_div(self):
    b = Interval(4,2)
    c = Interval(-2,4)
    bwd_div(c,self.a,b)
  def test_bwd_sqr(self):
    c = Interval(-2,4)
    bwd_sqr(c,self.a)
  def test_bwd_sqrt(self):
    c = Interval(-2,4)
    bwd_sqrt(c,self.a)
  def test_bwd_pow(self):
    c = Interval(-2,4)
    b = Interval(4,2)
    bwd_pow(c,b, self.a)
  def test_bwd_pow(self):
    c = Interval(-2,4)
    bwd_pow(c, 3, self.a)
  def test_bwd_root(self):
    c = Interval(-2,4)
    bwd_root(c,3, self.a)
  def test_bwd_exp(self):
    c = Interval(-2,4)
    bwd_exp(c,self.a)
  def test_bwd_log(self):
    c = Interval(-2,4)
    bwd_log(c,self.a)
  def test_bwd_cos(self):
    c = Interval(-2,4)
    bwd_cos(c,self.a)
  def test_bwd_sin(self):
    c = Interval(-2,4)
    bwd_sin(c,self.a)
  def test_bwd_tan(self):
    c = Interval(-2,4)
    bwd_tan(c,self.a)
  def test_bwd_acos(self):
    c = Interval(-2,4)
    bwd_acos(c,self.a)
  def test_bwd_asin(self):
    c = Interval(-2,4)
    bwd_asin(c,self.a)
  def test_bwd_atan(self):
    c = Interval(-2,4)
    bwd_atan(c,self.a)
  def test_bwd_atan2(self):
    c = Interval(-2,4)
    b = Interval(-2,4)
    bwd_atan2(c,self.a,b)
  def test_bwd_cosh(self):
    c = Interval(-2,4)
    bwd_cosh(c,self.a)
  def test_bwd_sinh(self):
    c = Interval(-2,4)
    bwd_sinh(c,self.a)
  def test_bwd_tanh(self):
    c = Interval(-2,4)
    bwd_tanh(c,self.a)
  def test_bwd_acosh(self):
    c = Interval(-2,4)
    bwd_acosh(c,self.a)
  def test_bwd_asinh(self):
    c = Interval(-2,4)
    bwd_asinh(c,self.a)
  def test_bwd_atanh(self):
    c = Interval(-2,4)
    bwd_atanh(c,self.a)
  def test_bwd_abs(self):
    c = Interval(-2,4)
    bwd_abs(c,self.a)
  def test_bwd_max(self):
    c = Interval(-2,4)
    b = Interval(2,10)
    bwd_max(c,self.a,b)
  def test_bwd_min(self):
    c = Interval(-2,4)
    b = Interval(2,10)
    bwd_min(c,self.a,b)
  def test_bwd_sign(self):
    c = Interval(-2,4)
    bwd_sign(c,self.a)
  def test_bwd_chi(self):
    c = Interval(-2,4)
    b = Interval(4,2)
    d = Interval(-2,1)
    bwd_chi(c,self.a,b,d)
  def test_bwd_floor(self):
    x = Interval(0.01,2.99)
    bwd_floor(Interval(1,2), x)
    self.assertEqual(x,  Interval(1,2.99))
  def test_bwd_ceil(self):
    x = Interval(0.01,2.99)
    bwd_ceil(Interval(1,2), x)
    self.assertEqual(x, Interval(0.01,2))


# a regular function
def interval_str(self):
    return "[ %f, %f ]" % ( self.lb(), self.ub() )

if __name__ == '__main__':

  unittest.main()