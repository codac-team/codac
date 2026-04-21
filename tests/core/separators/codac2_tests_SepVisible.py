#!/usr/bin/env python

#  Codac tests - Visibility
# ----------------------------------------------------------------------------
#  \date       2026
#  \author     Quentin Brateau
#  \copyright  Copyright 2026 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestVisibility(unittest.TestCase):

  def test_SepVisible(self):
    a = [0.0, 0.0]
    s = Segment([1.0, 1.0], [1.0, -1.0])
    sep = SepVisible(a, s)

    # 1. Space Partitioning
    x = IntervalVector([[0.0, 2.0], [-2.0, 2.0]])
    x_in, x_out = sep.separate(x)
    self.assertFalse(x_in.is_empty())
    self.assertFalse(x_out.is_empty())

    # 2. Box exactly on observation point
    x_point = IntervalVector([[0.0, 0.0], [0.0, 0.0]])
    x_in, x_out = sep.separate(x_point)
    self.assertTrue(x_in.is_empty())
    self.assertTrue(x_out == x_point)


if __name__ == '__main__':
  unittest.main()