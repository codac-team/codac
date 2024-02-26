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
from codac.unsupported import *

class TestThickInterval(unittest.TestCase):

  def test_thickFunction0(self):
    f = ThickFunction(lambda x: 2*x, lambda x: 4*x)
    a = IntervalVector([1,2,3])
    print(f.eval(a))

  def test_constructor_02(self):
    a = ThickInterval(1,2)
    self.check_thickinterval(a, Interval(1), Interval(2))
   
  def test_constructor_03(self):
    a = ThickInterval(Interval(0, 3), Interval.POS_REALS);
    self.assertEqual(a.lb(), Interval(0, 3));

  if __name__ == '__main__':

    unittest.main()