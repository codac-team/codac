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

class TestIntervalVector(unittest.TestCase):

  def CHECK_diff(self, x, y, compactness, result):
    x=IntervalVector(x)
    print(x)
    y=IntervalVector(y)
    result=IntervalMatrix(result)
    c = x.diff(y, compactness)
    for ci in c:
      found = False
      for i in range(0,result.nb_rows()):
        if ci == IntervalMatrix(result.row(i)).transpose().col(0):
          found = True
          break
      self.assertTrue(found)

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
    self.assertTrue(x == IntervalVector(2,Interval(0,1)))
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
    x.resize(3)
    self.assertTrue(x.size() == 3)
    self.assertTrue(x[0] == Interval(1,2))
    self.assertTrue(x[1] == Interval(-oo,oo))
    self.assertTrue(x[2] == Interval(-oo,oo))

    x = IntervalVector(1)
    x[0] = Interval(1,2)
    x.resize(1)
    self.assertTrue(x.size() == 1)
    self.assertTrue(x[0] == Interval(1,2))

    x = IntervalVector(2)
    x[0] = Interval(1,2)
    x.set_empty()
    x.resize(3)
    self.assertTrue(x.size() == 3)
    self.assertTrue(x.is_empty())
    self.assertTrue(x[2] == Interval(-oo,oo))

    x = IntervalVector(5)
    x[0] = Interval(1,2)
    x[1] = Interval(3,4)
    x.resize(2)
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

    
    #self.CHECK_diff([[-2,2],[-2,2],[-2,2]], IntervalVector.empty(3), True,
    #  [[[-2,2],[-2,2],[-2,2]]]
    #)
    
if __name__ ==  '__main__':
  unittest.main()