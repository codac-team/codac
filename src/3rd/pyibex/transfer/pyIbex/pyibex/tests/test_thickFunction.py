#!/usr/bin/env python
#============================================================================
#                                P Y I B E X
# File        : test_thickFunction.py
# Author      : Benoit Desrochers
# Copyright   : Benoit Desrochers
# License     : See the LICENSE file
# Created     : Dec 28, 2015
#============================================================================
import unittest
import pyibex
from pyibex import Interval, IntervalVector
from pyibex.thickset import ThickInterval, ThickBox, ThickFunction

class TestThickInterval(unittest.TestCase):

  def test_thickFunction0(self):
    f = ThickFunction(lambda x: 2*x, lambda x: 4*x)
    a =  IntervalVector( [1,2,3] )
    print( f.eval( a ) )

  # def test_constructor_02(self):
  #   a = ThickInterval(1,2)
  #   self.check_thickinterval(a, Interval(1), Interval(2))
  #
  # def test_constructor_03(self):
  #   a = ThickInterval(Interval(0, 3), Interval.POS_REALS);
  #   self.assertEqual(a.lb() , Interval(0, 3));

  if __name__ == '__main__':

    unittest.main()
