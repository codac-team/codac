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

class TestSepBox(unittest.TestCase):

  def test_sepbox_1d(self):

    # 1D IntervalVector
    b1 = IntervalVector([[0, 1]])

    # CtcBox
    s = SepBox(b1)

    # Contraction test
    x1 = IntervalVector([[0.5, 1.5]])
    x2 = IntervalVector([[-1, 2]])

    # Testing x1
    x1_in = IntervalVector(x1)
    x1_out = IntervalVector(x1)
    s.separate(x1_in, x1_out)
    self.assertEqual(x1_in[0], Interval(1, 1.5))
    self.assertEqual(x1_out[0], Interval(0.5, 1))

    # Testing x2
    x2_in = IntervalVector(x2)
    x2_out = IntervalVector(x2)
    s.separate(x2_in, x2_out)
    self.assertEqual(x2_in[0], Interval(-1, 2))
    self.assertEqual(x2_out[0], Interval(0, 1))


  def test_sepbox_2d(self):

    # 2D IntervalVector
    b1 = IntervalVector([[1, 2], [3, 4]])

    # CtcBox
    s = SepBox(b1)

    # Contraction test
    x1 = IntervalVector([[0, 5], [0, 5]])
    x2 = IntervalVector([[1.25, 1.75], [3.5, 4.5]])

    # Testing x1
    x1_in = IntervalVector(x1)
    x1_out = IntervalVector(x1)
    s.separate(x1_in, x1_out)
    self.assertEqual(x1_in[0], Interval(0, 5))
    self.assertEqual(x1_in[1], Interval(0, 5))
    self.assertEqual(x1_out[0], Interval(1, 2))
    self.assertEqual(x1_out[1], Interval(3, 4))

    # Testing x2
    x2_in = IntervalVector(x2)
    x2_out = IntervalVector(x2)
    s.separate(x2_in, x2_out)
    self.assertEqual(x2_in[0], Interval(1.25, 1.75))
    self.assertEqual(x2_in[1], Interval(4, 4.5))
    self.assertEqual(x2_out[0], Interval(1.25, 1.75))
    self.assertEqual(x2_out[1], Interval(3.5, 4))
    

if __name__ ==  '__main__':
  unittest.main()