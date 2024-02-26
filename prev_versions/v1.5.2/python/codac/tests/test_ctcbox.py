#!/usr/bin/env python

# Codac tests
# ---------------------------------------------------------------------------
# \date      2022
# \author    Quentin Brateau, Simon Rohou
# \copyright Copyright 2022 Codac Team
# \license   This program is distributed under the terms of
#            the GNU Lesser General Public License (LGPL).

import unittest
from codac import *
import codac as codac

class TestCtcBox(unittest.TestCase):

  def test_ctcbox_1d(self):

    # 1D IntervalVector
    b1 = IntervalVector([[0, 1]])

    # CtcBox
    c = CtcBox(b1)

    # Contraction test
    x1 = IntervalVector([[-3, 4]])
    x2 = IntervalVector([[-1, 0.25]])

    # Testing x1
    c.contract(x1)
    self.assertEqual(x1[0], Interval(0, 1))

    # Testing x2
    c.contract(x2);
    self.assertEqual(x2[0], Interval(0, 0.25))

  def test_ctcbox_2d(self):

    # 2D IntervalVector
    b1 = IntervalVector([[1, 2], [3, 4]])

    # CtcBox
    c = CtcBox(b1)

    # Contraction test
    x1 = IntervalVector([[-3, 4], [2, 5]])
    x2 = IntervalVector([[1.5, 3], [2, 3.5]])

    # Testing x1
    c.contract(x1)
    self.assertEqual(x1, b1)

    # Testing x2
    c.contract(x2);
    self.assertEqual(x2[0], Interval(1.5, 2))
    self.assertEqual(x2[1], Interval(3, 3.5))
    

if __name__ ==  '__main__':
  unittest.main()