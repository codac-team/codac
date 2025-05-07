#!/usr/bin/env python

#  Codac tests
#  
#  Most of these tests come from the IBEX library (Gilles Chabert)
#  See more: https://ibex-lib.readthedocs.io
#  They have been revised to fit with Codac (v2)
#
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Gilles Chabert, Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *
import sys
import math

class TestIntervalVector(unittest.TestCase):

  def CHECK_diff(self, x, y, compactness, result):
    x=IntervalVector(x)
    y=IntervalVector(y)
    result_=IntervalMatrix(result)
    c = x.diff(y, compactness)

    if not c:
      return not result
    
    for ci in c:
      found = False
      for i in range(0,result_.rows()):
        if IntervalMatrix(ci).transpose() == IntervalMatrix(result_.row(i)):
          found = True
          break
      if(not found):
        return False
    return True

  def tests_intervalvector(self):

    x = IntervalVector(2)
    x[0] = Interval(-oo,oo)
    x[1] = Interval(-oo,oo)
    self.assertTrue(x == IntervalVector(2))
    self.assertTrue(x == IntervalVector(x))
    #self.assertTrue(x == (IntervalVector(2)=x))

    x = IntervalVector(2)
    x[0] = Interval(0,1)
    x[1] = Interval(0,1)
    self.assertTrue(x == IntervalVector([[0,1],[0,1]]))
    self.assertTrue(x == IntervalVector(x))
    #self.assertTrue(x == (IntervalVector(2)=x))

    x = IntervalVector(2)
    x[0] = Interval(0,1)
    x[1] = Interval(2,3)
    self.assertTrue(x == IntervalVector(x))
    #self.assertTrue(x == (IntervalVector(2)=x))

    x = IntervalVector(2)
    x[0] = Interval(0,1)
    x[1] = Interval(2,3)
    self.assertTrue(x == IntervalVector([[0,1],[2,3]]))
    #self.assertTrue(x == (IntervalVector(2)=x))

    x = IntervalVector(2)
    x[0].set_empty()
    x[1].set_empty()
    self.assertTrue(x == IntervalVector.empty(2))
    self.assertTrue(x.is_empty())
    #self.assertTrue((IntervalVector(2)=x).is_empty())

    x = IntervalVector([
      [1.0, 2.0],
      [2.0, 3.0],
      [4]
    ])
    self.assertTrue(x.size() == 3)
    self.assertTrue(x[0] == Interval(1.0, 2.0))
    self.assertTrue(x[1] == Interval(2.0, 3.0))
    self.assertTrue(x[2] == Interval(4.0, 4.0))

    x = IntervalVector(2)
    self.assertTrue(not x.is_empty())
    x.set_empty()
    self.assertTrue(x.is_empty())
    self.assertTrue(IntervalVector.empty(2).is_empty())
    self.assertTrue(not IntervalVector(2).is_empty())

    x = IntervalVector(1)
    x[0] = Interval(1,2)
    x.resize_save_values(3)
    self.assertTrue(x.size() == 3)
    self.assertTrue(x[0] == Interval(1,2))
    self.assertTrue(x[1] == Interval(-oo,oo))
    self.assertTrue(x[2] == Interval(-oo,oo))

    x = IntervalVector(1)
    x[0] = Interval(1,2)
    x.resize_save_values(1)
    self.assertTrue(x.size() == 1)
    self.assertTrue(x[0] == Interval(1,2))

    x = IntervalVector(2)
    x[0] = Interval(1,2)
    x.set_empty()
    x.resize_save_values(3)
    self.assertTrue(x.size() == 3)
    self.assertTrue(x.is_empty())
    self.assertTrue(x[2] == Interval(-oo,oo))

    x = IntervalVector(5)
    x[0] = Interval(1,2)
    x[1] = Interval(3,4)
    x.resize_save_values(2)
    self.assertTrue(x.size() == 2)
    self.assertTrue(x[0] == Interval(1,2))
    self.assertTrue(x[1] == Interval(3,4))

    xi = Interval(-5,6)
    x = IntervalVector([xi])
    self.assertTrue(x.size() == 1)
    self.assertTrue(x[0] == Interval(-5,6))

    x = IntervalVector([Interval(4.0397,5.40),Interval(1.9089,2.45)])
    self.assertTrue(x[0] == Interval(4.0397,5.40))
    self.assertTrue(x[1] == Interval(1.9089,2.45))

    x = IntervalVector([[4.0397,5.40],[1.9089,2.45]])
    self.assertTrue(x[0] == Interval(4.0397,5.40))
    self.assertTrue(x[1] == Interval(1.9089,2.45))

    self.assertTrue(True)

    self.assertTrue(IntervalVector([[0,1],[2,3],[4,5]]).subvector(0,1) == IntervalVector([[0,1],[2,3]]))
    self.assertTrue(IntervalVector([[0,1],[2,3],[4,5]]).subvector(1,2) == IntervalVector([[2,3],[4,5]]))
    self.assertTrue(IntervalVector([[0,1],[2,3],[4,5]]).subvector(1,1) == IntervalVector([[2,3]]))
    self.assertTrue(IntervalVector([[0,1],[2,3],[4,5]]).subvector(2,2) == IntervalVector([[4,5]]))
    self.assertTrue(IntervalVector([[0,1],[2,3],[4,5]]).subvector(0,2) == IntervalVector([[0,1],[2,3],[4,5]]))
    self.assertTrue(IntervalVector.empty(3).subvector(1,2).is_empty())

    x1_ = IntervalVector([[0,2],[4,6]]) ; x1_ &= IntervalVector([[1,3],[5,7]])
    self.assertTrue(x1_ == IntervalVector([[1,2],[5,6]]))
    self.assertTrue(((IntervalVector([[0,2],[4,6]])) & IntervalVector([[1,3],[5,7]])) == IntervalVector([[1,2],[5,6]]))

    x1_ = IntervalVector([[0,2],[4,6]]) ; x1_ &= IntervalVector([[1,3],[7,8]])
    self.assertTrue(x1_.is_empty())
    self.assertTrue(((IntervalVector([[0,2],[4,6]])) & IntervalVector([[1,3],[7,8]])).is_empty())

    x1_ = IntervalVector([[0,2],[4,6]]) ; x1_ &= IntervalVector.empty(2)
    self.assertTrue(x1_.is_empty())
    self.assertTrue(((IntervalVector([[0,2],[4,6]])) & IntervalVector.empty(2)).is_empty())

    x1_ = IntervalVector.empty(2) ; x1_ &= IntervalVector([[0,2],[4,6]])
    self.assertTrue(x1_.is_empty())
    self.assertTrue(((IntervalVector.empty(2)) & IntervalVector([[0,2],[4,6]])).is_empty())

    x1_ = IntervalVector([[0,1],[4,5]]) ; x1_ |= IntervalVector([[2,3],[6,7]])
    self.assertTrue(x1_ == IntervalVector([[0,3],[4,7]]))
    self.assertTrue(((IntervalVector([[0,1],[4,5]])) | IntervalVector([[2,3],[6,7]])) == IntervalVector([[0,3],[4,7]]))

    x = IntervalVector(3)
    x1 = IntervalVector(2)
    x2 = IntervalVector(2)
    x3 = IntervalVector(2)

    x1 = IntervalVector([[0,1],[4,5]])
    x1_ = IntervalVector(x1) ; x1_ |= x1
    self.assertTrue(x1_ == x1)
    self.assertTrue((x1 | x1) == x1)

    x1 = IntervalVector([[0,2],[4,6]])
    x1_ = IntervalVector(x1) ; x1_ |= IntervalVector.empty(2)
    self.assertTrue(x1_ == x1)
    self.assertTrue((x1 | IntervalVector.empty(2)) == x1)

    x1 = IntervalVector([[0,2],[4,6]])
    x1_ = IntervalVector.empty(2) ; x1_ |= x1
    self.assertTrue(x1_ == x1)
    self.assertTrue(((IntervalVector.empty(2)) | x1) == x1)

    x = IntervalVector([[0,1],[2,3],[4,5]])
    self.assertTrue(x == x)
    self.assertTrue(not (x!=x))

    x = IntervalVector([[0,1],[2,3],[4,5]])
    x1 = IntervalVector([[0,1],[2,3]])
    self.assertTrue(not (x == x1))
    self.assertTrue(x!=x1)

    x1 = IntervalVector([[0,1],[4,5]])
    x2 = IntervalVector([[2,3],[6,7]])
    x1.set_empty()
    x2.set_empty()
    self.assertTrue(x1 == x2)
    self.assertTrue(not (x1!=x2))

    self.assertTrue(IntervalVector.empty(2) == IntervalVector.empty(2))
    self.assertTrue(IntervalVector.empty(2) != IntervalVector.empty(3))
    x = IntervalVector(2)
    x.set_empty()
    self.assertTrue(IntervalVector.empty(2) == x)

    x = IntervalVector([[0,1],[2,3],[4,5]])
    m = x.mid()
    self.assertTrue(m[0] == 0.5)
    self.assertTrue(m[1] == 2.5)
    self.assertTrue(m[2] == 4.5)

    self.assertTrue(not IntervalVector([[0,1],[2,3],[4,5]]).is_flat())
    self.assertTrue(IntervalVector.empty(3).is_flat())
    self.assertTrue(IntervalVector([[0,0]]).is_flat())
    self.assertTrue(not IntervalVector([[0,1]]).is_flat())
    self.assertTrue(IntervalVector([[0,1],[2,2],[3,4]]).is_flat())
    self.assertTrue(IntervalVector([[0,1],[2,3],[4,4]]).is_flat())
    self.assertTrue(not IntervalVector.empty(3).is_unbounded())
    self.assertTrue(IntervalVector([[0,1],[0,2],[-oo,0]]).is_unbounded())
    self.assertTrue(not IntervalVector([[0,1],[0,2]]).is_unbounded())
    self.assertTrue(IntervalVector(1).is_unbounded())

    x1 = IntervalVector([[0,2],[2,4]])
    x2 = IntervalVector([[0,1],[3,4]])

    self.assertTrue(x1.is_superset(x2))
    self.assertTrue(x2.is_subset(x1))
    self.assertTrue(x1.is_strict_superset(x2))
    self.assertTrue(not x2.is_strict_interior_subset(x1))

    x1 = IntervalVector([[0,2],[2,4]])
    x2 = IntervalVector([[1,1],[3,4]])

    self.assertTrue(x1.is_superset(x2))
    self.assertTrue(x2.is_subset(x1))
    self.assertTrue(x1.is_strict_superset(x2))
    self.assertTrue(not x2.is_strict_interior_subset(x1))

    x1 = IntervalVector([[0,2],[2,4]])
    x2 = IntervalVector([[0,1],[3,3]])

    self.assertTrue(x1.is_superset(x2))
    self.assertTrue(x2.is_subset(x1))
    self.assertTrue(x1.is_strict_superset(x2))
    self.assertTrue(not x2.is_strict_interior_subset(x1))

    x1 = IntervalVector([[0,2],[2,4]])
    x2 = IntervalVector([[1,1],[3,3]])

    self.assertTrue(x1.is_superset(x2))
    self.assertTrue(x2.is_subset(x1))
    self.assertTrue(x1.is_strict_superset(x2))
    self.assertTrue(x2.is_strict_interior_subset(x1))

    x1 = IntervalVector([[0,2],[2,4]])
    x2 = IntervalVector(IntervalVector.empty(2))

    self.assertTrue(x1.is_superset(x2))
    self.assertTrue(x2.is_subset(x1))
    self.assertTrue(x1.is_strict_superset(x2))
    self.assertTrue(x2.is_strict_interior_subset(x1))

    x1 = IntervalVector(IntervalVector.empty(2))
    x2 = IntervalVector([[1,1],[3,3]])

    self.assertTrue(not x1.is_superset(x2))
    self.assertTrue(not x2.is_subset(x1))
    self.assertTrue(not x1.is_strict_superset(x2))
    self.assertTrue(not x2.is_strict_interior_subset(x1))

    x1 = IntervalVector([[0,2],[2,4]])
    x2 = IntervalVector([[1,1],[3,5]])

    self.assertTrue(not x1.is_superset(x2))
    self.assertTrue(not x2.is_subset(x1))
    self.assertTrue(not x1.is_strict_superset(x2))
    self.assertTrue(not x2.is_strict_interior_subset(x1))

    x1 = IntervalVector([[0,2],[0,1],[0,3]])
    self.assertTrue(x1.extr_diam_index(True) == 1)
    self.assertTrue(x1.extr_diam_index(False) == 2)
    self.assertTrue(x1.min_diam() == 1)
    self.assertTrue(x1.max_diam() == 3)

    x1 = IntervalVector([[0,1],[0,3],[0,2]])
    self.assertTrue(x1.extr_diam_index(True) == 0)
    self.assertTrue(x1.extr_diam_index(False) == 1)
    self.assertTrue(x1.min_diam() == 1)
    self.assertTrue(x1.max_diam() == 3)

    x1 = IntervalVector([[0,3],[0,2],[0,1]])
    self.assertTrue(x1.extr_diam_index(True) == 2)
    self.assertTrue(x1.extr_diam_index(False) == 0)
    self.assertTrue(x1.min_diam() == 1)
    self.assertTrue(x1.max_diam() == 3)

    x1 = IntervalVector([[0,1],[0,2],[-oo,0]])
    self.assertTrue(x1.extr_diam_index(True) == 0)
    self.assertTrue(x1.extr_diam_index(False) == 2)
    self.assertTrue(x1.min_diam() == 1)
    self.assertTrue(x1.max_diam() == oo)

    x1 = IntervalVector([[-oo,0]])
    self.assertTrue(x1.extr_diam_index(True) == 0)
    self.assertTrue(x1.extr_diam_index(False) == 0)
    self.assertTrue(x1.min_diam() == oo)
    self.assertTrue(x1.max_diam() == oo)

    x1 = IntervalVector([[-oo,0],[0,1],[-oo,1],[1,3]])
    self.assertTrue(x1.extr_diam_index(True) == 1)
    self.assertTrue(x1.extr_diam_index(False) == 2)
    self.assertTrue(x1.min_diam() == 1)
    self.assertTrue(x1.max_diam() == oo)

    x1 = IntervalVector([[-oo,0],[-2,oo],[-oo,1]])
    self.assertTrue(x1.extr_diam_index(True) == 0)
    self.assertTrue(x1.extr_diam_index(False) == 1)
    self.assertTrue(x1.min_diam() == oo)
    self.assertTrue(x1.max_diam() == oo)

    x1 = IntervalVector([[-oo,0],[-oo,1],[-2,oo]])
    self.assertTrue(x1.extr_diam_index(True) == 0)
    self.assertTrue(x1.extr_diam_index(False) == 2)
    self.assertTrue(x1.min_diam() == oo)
    self.assertTrue(x1.max_diam() == oo)

    x1 = IntervalVector([[-2,oo],[-oo,0],[-oo,1]])
    self.assertTrue(x1.extr_diam_index(True) == 1)
    self.assertTrue(x1.extr_diam_index(False) == 0)
    self.assertTrue(x1.min_diam() == oo)
    self.assertTrue(x1.max_diam() == oo)

    x1 = IntervalVector([[-2,oo],[-oo,1],[-oo,0]])
    self.assertTrue(x1.extr_diam_index(True) == 2)
    self.assertTrue(x1.extr_diam_index(False) == 0)
    self.assertTrue(x1.min_diam() == oo)
    self.assertTrue(x1.max_diam() == oo)

    self.assertTrue(IntervalVector([[0,1],[0,oo]]).volume() == oo)
    self.assertTrue(IntervalVector([[0,1],[1,1]]).volume() == 0)
    self.assertTrue(Approx(Interval(24.0),1e-10) == IntervalVector([[0,2],[2,5],[4,8]]).volume())
    self.assertTrue((-IntervalVector([[0,3],[0,2],[0,1]])) == IntervalVector([[-3,0],[-2,0],[-1,0]]))

    self.assertTrue(-IntervalVector([[0,1],[0,oo]]) == IntervalVector([[-1,0],[-oo,0]]))
    self.assertTrue(-IntervalVector.empty(2) == IntervalVector.empty(2))

    x1 = IntervalVector([[0,3],[0,2],[0,1]])
    x2 = IntervalVector([[0,1],[0,1],[0,1]])
    x3 = IntervalVector([[0,4],[0,3],[0,2]])
    e = IntervalVector.empty(3)

    self.assertTrue(x1+x2 == x3)
    self.assertTrue(x1+e == e)
    self.assertTrue((x1+e).is_empty())
    x1_ = IntervalVector(x1) ; x1_ += e
    self.assertTrue(x1_ == e)
    self.assertTrue(x1_.is_empty())
    self.assertTrue(e+x1 == e)
    self.assertTrue((e+x1).is_empty())
    e_ = IntervalVector(e) ; e_ += x1
    self.assertTrue(e_ == e)
    self.assertTrue(e_.is_empty())
    self.assertTrue(e+e == e)
    self.assertTrue((e+e).is_empty())
    e_ = IntervalVector(e) ; e_ += e
    self.assertTrue(e_ == e)
    self.assertTrue(e_.is_empty())
    x1_ = IntervalVector(x1) ; x1_ += x2
    self.assertTrue(x1_ == x3)
    x1_ = IntervalVector(x1) ; x1_ += e
    self.assertTrue(x1_ == e)
    x1_ = IntervalVector(x1) ; x1_ += e
    self.assertTrue(x1_.is_empty())
    x2_ = IntervalVector(x2) ; x2_ += x1
    self.assertTrue(x2_ == x3)

    x1 = IntervalVector([[0,3],[0,2],[0,1]])
    x2 = IntervalVector([[0,1],[0,1],[0,1]])
    x3 = IntervalVector([[-1,3],[-1,2],[-1,1]])
    e = IntervalVector.empty(3)

    self.assertTrue(x1-x2 == x3)
    self.assertTrue(x2-x1 == -x3)
    self.assertTrue(x1-e == e)
    self.assertTrue((x1-e).is_empty())
    x1_ = IntervalVector(x1) ; x1_ -= e
    self.assertTrue(x1_ == e)
    self.assertTrue(x1_.is_empty())
    self.assertTrue(e-x1 == e)
    self.assertTrue((e-x1).is_empty())
    e_ = IntervalVector(e) ; e_ -= x1
    self.assertTrue(e_ == e)
    self.assertTrue(e_.is_empty())
    self.assertTrue(e-e == e)
    self.assertTrue((e-e).is_empty())
    e_ = IntervalVector(e) ; e_ -= e
    self.assertTrue(e_ == e)
    self.assertTrue(e_.is_empty())
    x1_ = IntervalVector(x1) ; x1_ -= x2
    self.assertTrue(x1_ == x3)
    x2_ = IntervalVector(x2) ; x2_ -= x1
    self.assertTrue(x2_ == -x3)

    b = IntervalVector([[0,1],[0,1]])
    c = b.complementary()

    self.assertTrue(len(c) == 4)
    self.assertTrue(c[0].size() == 2)

    self.assertTrue(c[0][0] == Interval(-oo,0))
    self.assertTrue(c[0][1] == Interval(-oo,oo))

    self.assertTrue(c[1][0] == Interval(1,oo))
    self.assertTrue(c[1][1] == Interval(-oo,oo))

    self.assertTrue(c[2][0] == Interval(0,1))
    self.assertTrue(c[2][1] == Interval(-oo,0))

    self.assertTrue(c[3][0] == Interval(0,1))
    self.assertTrue(c[3][1] == Interval(1,oo))

    c = IntervalVector.empty(2).complementary()
    self.assertTrue(len(c) == 1)
    self.assertTrue(c[0].size() == 2)
    self.assertTrue(c[0][0] == Interval(-oo,oo))
    self.assertTrue(c[0][1] == Interval(-oo,oo))
    
    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], IntervalVector.empty(3), True,
      [[[-2,2],[-2,2],[-2,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], IntervalVector.empty(3), True,
      [[[-2,2],[-2,2],[-2,2]]]
    )

    self.assertTrue(IntervalVector.empty(3) == IntervalVector([Interval.empty(),Interval.empty(),Interval.empty()]))

    self.CHECK_diff(IntervalVector.empty(3), [[-2,2],[-2,2],[-2,2]], True,
      [[Interval.empty(),Interval.empty(),Interval.empty()]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[-1,1],[3,4],[-1,1]], True,
      [[[-2,2],[-2,2],[-2,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[-1,1],[-1,-1],[-1,1]], True,
      [[[-2,2],[-2,2],[-2,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[-1,1],[-1,1],[-1,1]], True,
      [[[-2,-1],[-2,2],[-2,2]],
       [[1,2],[-2,2],[-2,2]],
       [[-1,1],[-2,-1],[-2,2]],
       [[-1,1],[1,2],[-2,2]],
       [[-1,1],[-1,1],[-2,-1]],
       [[-1,1],[-1,1],[1,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[-1,1],[-1,1],[-2,1]], True,
      [[[-2,-1],[-2,2],[-2,2]],
       [[1,2],[-2,2],[-2,2]],
       [[-1,1],[-2,-1],[-2,2]],
       [[-1,1],[1,2],[-2,2]],
       [[-1,1],[-1,1],[1,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[-1,1],[-1,1],[-2,2]], True,
      [[[-2,-1],[-2,2],[-2,2]],
       [[1,2],[-2,2],[-2,2]],
       [[-1,1],[-2,-1],[-2,2]],
       [[-1,1],[1,2],[-2,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[-1,1],[-2,1],[-1,1]], True,
      [[[-2,-1],[-2,2],[-2,2]],
       [[1,2],[-2,2],[-2,2]],
       [[-1,1],[1,2],[-2,2]],
       [[-1,1],[-2,1],[-2,-1]],
       [[-1,1],[-2,1],[1,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[-1,1],[-2,2],[-1,1]], True,
      [[[-2,-1],[-2,2],[-2,2]],
       [[1,2],[-2,2],[-2,2]],
       [[-1,1],[-2,2],[-2,-1]],
       [[-1,1],[-2,2],[1,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[-2,1],[-1,1],[-1,1]], True,
      [[[1,2],[-2,2],[-2,2]],
       [[-2,1],[-2,-1],[-2,2]],
       [[-2,1],[1,2],[-2,2]],
       [[-2,1],[-1,1],[-2,-1]],
       [[-2,1],[-1,1],[1,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[-2,2],[-1,2],[-1,1]], True,
      [[[-2,2],[-2,-1],[-2,2]],
       [[-2,2],[-1,2],[-2,-1]],
       [[-2,2],[-1,2],[1,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[-1,1],[-2,1],[-2,1]], True,
      [[[-2,-1],[-2,2],[-2,2]],
       [[1,2],[-2,2],[-2,2]],
       [[-1,1],[1,2],[-2,2]],
       [[-1,1],[-2,1],[1,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[-1,1],[-2,1],[-2,2]], True,
      [[[-2,-1],[-2,2],[-2,2]],
       [[1,2],[-2,2],[-2,2]],
       [[-1,1],[1,2],[-2,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[-2,1],[-1,1],[-2,1]], True,
      [[[1,2],[-2,2],[-2,2]],
       [[-2,1],[-2,-1],[-2,2]],
       [[-2,1],[1,2],[-2,2]],
       [[-2,1],[-1,1],[1,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[-2,1],[-1,1],[-2,2]], True,
      [[[1,2],[-2,2],[-2,2]],
       [[-2,1],[-2,-1],[-2,2]],
       [[-2,1],[1,2],[-2,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[-2,1],[-2,1],[-1,1]], True,
      [[[1,2],[-2,2],[-2,2]],
       [[-2,1],[1,2],[-2,2]],
       [[-2,1],[-2,1],[-2,-1]],
       [[-2,1],[-2,1],[1,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[-2,1],[-2,2],[-1,1]], True,
      [[[1,2],[-2,2],[-2,2]],
       [[-2,1],[-2,2],[-2,-1]],
       [[-2,1],[-2,2],[1,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[-2,1],[-2,1],[-2,1]], True,
      [[[1,2],[-2,2],[-2,2]],
       [[-2,1],[1,2],[-2,2]],
       [[-2,1],[-2,1],[1,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[-2,1],[-2,1],[-2,2]], True,
      [[[1,2],[-2,2],[-2,2]],
       [[-2,1],[1,2],[-2,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[-2,1],[-2,2],[-2,1]], True,
      [[[1,2],[-2,2],[-2,2]],
       [[-2,1],[-2,2],[1,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[-2,2],[-2,1],[-2,1]], True,
      [[[-2,2],[1,2],[-2,2]],
       [[-2,2],[-2,1],[1,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[-2,1],[-2,2],[-2,2]], True,
      [[[1,2],[-2,2],[-2,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[-2,2],[-2,2],[-2,1]], True,
      [[[-2,2],[-2,2],[1,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[-2,2],[-2,1],[-2,2]], True,
      [[[-2,2],[1,2],[-2,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[-2,2],[-2,2],[-2,2]], True,
      [[Interval.empty(),Interval.empty(),Interval.empty()]]
    )

    self.CHECK_diff([[0,0],[-2,2],[-2,2]], [[0,0],[-1,1],[-1,1]], True,
      [[[0,0],[-2,-1],[-2,2]],
       [[0,0],[1,2],[-2,2]],
       [[0,0],[-1,1],[-2,-1]],
       [[0,0],[-1,1],[1,2]]]
    )

    self.CHECK_diff([[0,0],[0,0],[-2,2]], [[0,0],[0,0],[-1,1]], True,
      [[[0,0],[0,0],[-2,-1]],
       [[0,0],[0,0],[1,2]]]
    )

    self.CHECK_diff([[0,0],[-2,2],[0,0]], [[0,0],[-1,1],[0,0]], True,
      [[[0,0],[-2,-1],[0,0]],
       [[0,0],[1,2],[0,0]]]
    )

    self.CHECK_diff([[-2,2],[0,0],[0,0]], [[-1,1],[0,0],[0,0]], True,
      [[[-2,-1],[0,0],[0,0]],
       [[1,2],[0,0],[0,0]]]
    )

    self.CHECK_diff([[0,0],[-2,2],[-2,2]], [[-1,1],[-1,1],[-1,1]], True,
      [[[0,0],[-2,-1],[-2,2]],
       [[0,0],[1,2],[-2,2]],
       [[0,0],[-1,1],[-2,-1]],
       [[0,0],[-1,1],[1,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[2,4],[-2,2],[-2,2]], True,
      [[[-2,2],[-2,2],[-2,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2],[-2,2]], [[2,4],[-1,1],[-1,1]], True,
      [[[-2,2],[-2,2],[-2,2]]]
    )

    self.CHECK_diff([[-2,2],[-2,2]], [[-2,2],[1,1]], False,
      [[[-2,2],[-2,1]],
       [[-2,2],[1,2]]]
    )

    self.CHECK_diff([[-2,2],[1,1]], [[0,2],[-2,2]], False,
      [[[-2,0],[1,1]]]
    )

    self.CHECK_diff([[-1,-1],[-1,1],[-1,1]], [[0,2],[0,2],[0,2]], True,
      [[[-1,-1],[-1,1],[-1,1]]]
    )

    a = IntervalVector([Interval.empty(),[-1,1]])
    b = IntervalVector([[-1,1],Interval.empty()])
    c = a | b
    self.assertTrue(c.is_empty())
    

  def tests_intervalvector_specific_to_python(self):

    x = IntervalVector([[1,2],[2,3],[3,4]])
    y = IntervalVector(3)

    i = 0
    for xi in x: # using __iter__
      y[i] = xi
      i = i+1

    self.assertTrue(x == y)

    a,b,c = x # using __iter__
    self.assertTrue(a == x[0])
    self.assertTrue(b == x[1])
    self.assertTrue(c == x[2])

    v = IntervalVector([*x, [3,6]]) # using __iter__
    self.assertTrue(v == [[1,2],[2,3],[3,4],[3,6]])

if __name__ ==  '__main__':
  unittest.main()