#!/usr/bin/env python

#  Codac tests
#  
#  Most of these tests come from the IBEX library (Gilles Chabert)
#  See more: https://ibex-lib.readthedocs.io
#  They have been revised to fit with Codac (v2)
#
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Gilles Chabert, (Simon Rohou)
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *
import sys
import math

class TestInterval(unittest.TestCase):

  def tests_interval(self):

    self.assertTrue(Interval() == Interval(-oo,oo))
    self.assertTrue(Interval(1.0) == Interval(1.0,1.0))
    self.assertTrue(Interval(-oo) == Interval.empty())
    self.assertTrue(Interval(oo) == Interval.empty())
    self.assertTrue(Interval(1,0) == Interval.empty())

    self.assertTrue(Interval(-oo,oo) == Interval(-oo,oo))
    self.assertTrue(Interval(-oo,oo) != Interval(0,oo))
    self.assertTrue(Interval(0,1) == Interval(0,1))
    self.assertTrue(Interval(0,2) != Interval(0,1))
    self.assertTrue(Interval(0,2) != Interval(1,3))
    self.assertTrue(Interval(0,oo) == Interval(0,oo))
    self.assertTrue(Interval.empty() == Interval.empty())
    self.assertTrue(Interval(0,0) == Interval(0,0))

    self.assertTrue(Interval([1.]) == Interval(1.));
    self.assertTrue(Interval([2,3]) == Interval(2,3));
    self.assertTrue(Interval([3,2]) == Interval.empty());
    self.assertTrue(Interval([-oo]) == Interval.empty());
    self.assertTrue(Interval([oo]) == Interval.empty());
    self.assertTrue(Interval([oo,0]) == Interval.empty());
    self.assertTrue(Interval([-oo,oo]) == Interval(-oo,oo));
    self.assertTrue(Interval([oo,-oo]) == Interval.empty());

    x = Interval() ; y = Interval() ; z = Interval()

    x = Interval(0,1); x.set_empty()
    self.assertTrue(x == Interval.empty())
    self.assertTrue(x.is_empty())

    x = Interval(Interval.empty()); x.set_empty()
    self.assertTrue(x == Interval.empty())

    x = Interval(0,1); y = Interval(2,3); z = Interval(0,3)
    self.assertTrue((x|y) == z); self.assertTrue((y|x) == z)
    x_ = x; x_|=y; self.assertTrue(x_ == z); y_ = y; y_|=x; self.assertTrue(y_ == z)

    x = Interval(0,1); y = Interval.empty(); z = Interval(0,1)
    self.assertTrue((x|y) == z); self.assertTrue((y|x) == z)
    x_ = x; x_|=y; self.assertTrue(x_ == z); y_ = y; y_|=x; self.assertTrue(y_ == z)

    x = Interval(-1); y = Interval(0,oo); z = Interval(-1,oo)
    self.assertTrue((x|y) == z); self.assertTrue((y|x) == z)
    x_ = x; x_|=y; self.assertTrue(x_ == z); y_ = y; y_|=x; self.assertTrue(y_ == z)

    x = Interval(0,1); y = Interval(2,3); z = Interval.empty()
    self.assertTrue((x&y) == z); self.assertTrue((y&x) == z)
    x_ = x; x_&=y; self.assertTrue(x_ == z); y_ = y; y_&=x; self.assertTrue(y_ == z)

    x = Interval(0,1); y = Interval.empty(); z = Interval.empty()
    self.assertTrue((x&y) == z); self.assertTrue((y&x) == z)
    x_ = x; x_&=y; self.assertTrue(x_ == z); y_ = y; y_&=x; self.assertTrue(y_ == z)

    x = Interval(0,2); y = Interval(-1,1); z = Interval(0,1)
    self.assertTrue((x&y) == z); self.assertTrue((y&x) == z)
    x_ = x; x_&=y; self.assertTrue(x_ == z); y_ = y; y_&=x; self.assertTrue(y_ == z)

    x = Interval(0,1); y = Interval(0,oo); z = Interval(0,1)
    self.assertTrue((x&y) == z); self.assertTrue((y&x) == z)
    x_ = x; x_&=y; self.assertTrue(x_ == z); y_ = y; y_&=x; self.assertTrue(y_ == z)

    x = Interval(0,1); y = Interval(-oo,0); z = Interval(0)
    self.assertTrue((x&y) == z); self.assertTrue((y&x) == z)
    x_ = x; x_&=y; self.assertTrue(x_ == z); y_ = y; y_&=x; self.assertTrue(y_ == z)

    x = Interval(0,1); y = Interval(1,2); z = Interval(1,1)
    self.assertTrue((x&y) == z); self.assertTrue((y&x) == z)
    x_ = x; x_&=y; self.assertTrue(x_ == z); y_ = y; y_&=x; self.assertTrue(y_ == z)

    x = Interval(0,2)

    self.assertTrue(x.lb() == 0)
    self.assertTrue(x.ub() == 2)
    self.assertTrue(x.mid() == 1)
    self.assertTrue(x.rad() == 1)
    self.assertTrue(x.diam() == 2)

    x = Interval(-3,-1)

    self.assertTrue(x.lb() == -3)
    self.assertTrue(x.ub() == -1)
    self.assertTrue(x.mid() == -2)
    self.assertTrue(x.rad() == 1)
    self.assertTrue(x.diam() == 2)

    x = Interval(-3,1)

    self.assertTrue(x.lb() == -3)
    self.assertTrue(x.ub() == 1)
    self.assertTrue(x.mid() == -1)
    self.assertTrue(x.rad() == 2)
    self.assertTrue(x.diam() == 4)

    x = Interval(-oo,0)

    self.assertTrue(x.lb() == -oo)
    self.assertTrue(x.ub() == 0)
    self.assertTrue(x.mid() == -sys.float_info.max)
    self.assertTrue(x.rad() == oo)
    self.assertTrue(x.diam() == oo)

    x = Interval(-oo,oo)

    self.assertTrue(x.lb() == -oo)
    self.assertTrue(x.ub() == oo)
    self.assertTrue(x.mid() == 0)
    self.assertTrue(x.rad() == oo)
    self.assertTrue(x.diam() == oo)

    x = Interval(sys.float_info.max,oo)

    self.assertTrue(x.lb() == sys.float_info.max)
    self.assertTrue(x.ub() == oo)
    self.assertTrue(x.mid() == sys.float_info.max)
    self.assertTrue(x.rad() == oo)
    self.assertTrue(x.diam() == oo)

    x = Interval(-1,1)
    for i in range(10):
      self.assertTrue(x.contains(x.rand()))
    x = Interval(-oo,0);
    for i in range(10):
      self.assertTrue(x.contains(x.rand()))
    x = Interval(0,oo);
    for i in range(10):
      self.assertTrue(x.contains(x.rand()))
    x = Interval(-oo,oo);
    for i in range(10):
      self.assertTrue(x.contains(x.rand()))
    x = Interval.empty()
    self.assertTrue(math.isnan(x.rand()))

    self.assertTrue(Interval(0,1).is_subset(Interval(0,2)))
    self.assertTrue(not Interval(0,1).is_subset(Interval(1,2)))
    self.assertTrue(not Interval(0,1).is_strict_interior_subset(Interval(0,2)))
    self.assertTrue(Interval(-oo,0).is_strict_interior_subset(Interval(-oo,oo)))
    self.assertTrue(Interval.empty().is_strict_interior_subset(Interval(0,1)))
    self.assertTrue(Interval(0,2).is_superset(Interval(0,1)))
    self.assertTrue(not Interval(1,2).is_superset(Interval(0,1)))
    self.assertTrue(not Interval(0,2).is_strict_superset(Interval(0,2)))
    self.assertTrue(Interval(-oo,oo).is_strict_superset(Interval(-oo,0)))
    self.assertTrue(not Interval.empty().is_strict_superset(Interval.empty()))
    self.assertTrue(Interval(0,2).contains(0))
    self.assertTrue(Interval(-oo,oo).contains(oo))
    self.assertTrue(not Interval(0,2).interior_contains(0))
    self.assertTrue(Interval(0,2).interior_contains(1))
    self.assertTrue(not Interval(-oo,0).is_disjoint(Interval(0,oo)))
    self.assertTrue(Interval(0,1).is_disjoint(Interval(2,3)))
    self.assertTrue(Interval(oo).is_empty())
    self.assertTrue(not Interval(0,1).is_empty())
    self.assertTrue(Interval(0).is_degenerated())
    self.assertTrue(Interval.empty().is_degenerated())
    self.assertTrue(not Interval.empty().is_unbounded())
    self.assertTrue(not Interval(0,1).is_unbounded())
    self.assertTrue(Interval(-oo,0).is_unbounded())
    self.assertTrue(Interval(0,oo).is_unbounded())
    self.assertTrue(not Interval.empty().is_bisectable())
    self.assertTrue(not Interval(sys.float_info.max,oo).is_bisectable())
    self.assertTrue(not Interval(-oo,-sys.float_info.max).is_bisectable())
    self.assertTrue(not Interval(0,next_float(0)).is_bisectable())
    self.assertTrue(Interval(0,next_float(next_float(0))).is_bisectable())
    self.assertTrue(Interval(10,next_float(next_float(10))).is_bisectable())
    self.assertTrue(Interval(previous_float(previous_float(0)),0).is_bisectable())
    self.assertTrue(Interval(previous_float(previous_float(10)),10).is_bisectable())
    self.assertTrue(not Interval(10,next_float(10)).is_bisectable())
    self.assertTrue(not Interval(previous_float(0),0).is_bisectable())

    self.assertTrue(Interval(0,2).mid() == 1.0)
    self.assertTrue(Interval(-oo,oo).mid() == 0)
    self.assertTrue(Interval(0,oo).mid() == sys.float_info.max)
    self.assertTrue(Interval(-oo,0).mid() == -sys.float_info.max)
    d = Interval(sys.float_info.max,oo).mid()
    self.assertTrue((d == sys.float_info.max) or (d == -sys.float_info.max))
    self.assertTrue(Interval(0).mid() == 0.0)

    x = Interval(2.7629416257693772963,2.7629416257693835135)
    y = x
    m = x.mid()
    x2 = x+m
    x3 = x2-m
    self.assertTrue(x3.is_superset(y))

    self.assertTrue(Interval(0).diff(Interval(0)) == [])
    self.assertTrue(Interval(0,0).diff(Interval(1,2)) == [Interval(0)])
    self.assertTrue(Interval(0,0).diff(Interval(0,1)) == [])
    self.assertTrue(Interval(0,0).diff(Interval.empty()) == [Interval(0)])
    self.assertTrue(Interval.empty().diff(Interval(0.)) == [])
    self.assertTrue(Interval(0,1).diff(Interval(0,1)) == [])
    self.assertTrue(Interval(0,1).diff(Interval.empty()) == [Interval(0,1)])
    self.assertTrue(Interval(0,1).diff(Interval(1,2)) == [Interval(0,1)])
    self.assertTrue(Interval(0,1).diff(Interval(-1,1)) == [])
    self.assertTrue(Interval(0,1).diff(Interval(0,0)) == [Interval(0,1)])
    self.assertTrue(Interval(0,1).diff(Interval(1,1)) == [Interval(0,1)])
    self.assertTrue(Interval(0,1).diff(Interval(2,3)) == [Interval(0,1)])
    self.assertTrue(Interval(0,3).diff(Interval(1,2)) == [Interval(0,1),Interval(2,3)]) # order may not be respected
    self.assertTrue(Interval(0,2).diff(Interval(1,4)) == [Interval(0,1)])
    self.assertTrue(Interval(0,2).diff(Interval(-1,1)) == [Interval(1,2)])
    self.assertTrue(Interval(0,2).diff(Interval(1,1), False) == [Interval(0,1),Interval(1,2)]) # order may not be respected
    self.assertTrue(Interval(1,1).diff(Interval(0,2), False) == [])

    a = Interval(3,8)
    b = Interval(1,3) & Interval(6,7) # [b] is empty
    c = a+b
    self.assertTrue(c.is_empty())
      
if __name__ ==  '__main__':
  unittest.main()