#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2026
#  \author     Maël Godard
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import sys, os
import unittest
import math
from codac import *

class TestCtcShapeManual(unittest.TestCase):

  def tests_CtcWrapper_Parallelepiped_manual(test):

    # [ctcparallelepiped-1-beg]
    x = IntervalVector([[0,5], [0,5]])
    p = Parallelepiped(Vector([1.5,2.8]), Matrix([[0.5,0.4],[0,0.2]]))
    c = CtcWrapper_Parallelepiped(p)
    x = c.contract(x)
    # x = [ [0.599999, 2.40001] ; [2.59999, 3] ]
    # [ctcparallelepiped-1-end]

    test.assertTrue(Approx(x) == IntervalVector([[0.6, 2.4], [2.6, 3]]))

if __name__ ==  '__main__':
  unittest.main()