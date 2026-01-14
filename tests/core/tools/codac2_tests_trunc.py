#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *
import sys

class TestTrunc(unittest.TestCase):

  def test_trunc(self):

    l = next_float(-oo); u = prev_float(oo)
    self.assertTrue(trunc(0) == 0)
    self.assertTrue(trunc(-oo) == l)
    self.assertTrue(trunc(oo) == u)
    self.assertTrue(untrunc(l) == -oo)
    self.assertTrue(untrunc(u) == oo)
    self.assertTrue(untrunc(0) == 0)
    
    # trunc matrix types

    self.assertTrue(trunc(IntervalMatrix([
        [[-1,1],[-2,2]],
        [[-3,3],[-4,4]]
      ])) == IntervalMatrix([
        [[-1,1],[-2,2]],
        [[-3,3],[-4,4]]
      ]))
    
    self.assertTrue(trunc(IntervalMatrix([
        [[-1,oo],[-2,2]],
        [[-3,3],[-4,4]]
      ])) == IntervalMatrix([
        [[-1,u],[-2,2]],
        [[-3,3],[-4,4]]
      ]))
    
    self.assertTrue(trunc(IntervalMatrix([
        [[-oo,oo],[-2,2]],
        [[-3,3],[-4,4]]
      ])) == IntervalMatrix([
        [[l,u],[-2,2]],
        [[-3,3],[-4,4]]
      ]))
    
    self.assertTrue(trunc(IntervalMatrix([
        [[-oo,oo],[-oo,oo]],
        [[-oo,oo],[-oo,oo]]
      ])) == IntervalMatrix([
        [[l,u],[l,u]],
        [[l,u],[l,u]]
      ]))
    
    # untrunc matrix types

    self.assertTrue(untrunc(IntervalMatrix([
        [[-1,1],[-2,2]],
        [[-3,3],[-4,4]]
      ])) == IntervalMatrix([
        [[-1,1],[-2,2]],
        [[-3,3],[-4,4]]
      ]))
    
    self.assertTrue(untrunc(IntervalMatrix([
        [[-1,u],[-2,2]],
        [[-3,3],[-4,4]]
      ])) == IntervalMatrix([
        [[-1,oo],[-2,2]],
        [[-3,3],[-4,4]]
      ]))
    
    self.assertTrue(untrunc(IntervalMatrix([
        [[l,u],[-2,2]],
        [[-3,3],[-4,4]]
      ])) == IntervalMatrix([
        [[-oo,oo],[-2,2]],
        [[-3,3],[-4,4]]
      ]))
    
    self.assertTrue(untrunc(IntervalMatrix([
        [[l,u],[l,u]],
        [[l,u],[l,u]]
      ])) == IntervalMatrix([
        [[-oo,oo],[-oo,oo]],
        [[-oo,oo],[-oo,oo]]
      ]))

if __name__ ==  '__main__':
  unittest.main()