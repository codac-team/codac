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

class TestCtcCartProd(unittest.TestCase):

  def test_ctc(self):

    # C1
    y1 = Interval(0, 1)
    f1 = Function("x", "sin(x)")
    C1 = CtcFunction(f1, y1)

    # C2
    y2 = Interval(0, 1)
    f2 = Function("x", "sqr(x)")
    C2 = CtcFunction(f2, y2)

    # CtcCartProd
    C = CtcCartProd([C1, C2])

    # Expected
    e1 = IntervalVector([[-5, 1]])
    e2 = IntervalVector([[-3, 6]])
    C1.contract(e1)
    C2.contract(e2)

    # Testing
    x = IntervalVector([[-5, 1],[-3, 6]])
    C.contract(x)

    self.assertEqual(x[0], e1[0])
    self.assertEqual(x[1], e2[0])
  

  def test_fn_cart_prod(self):
  
    # C1
    y1 = Interval(0, 1)
    f1 = Function("x", "sin(x)")
    C1 = CtcFunction(f1, y1)

    # C2
    y2 = Interval(0, 1)
    f2 = Function("x", "sqr(x)")
    C2 = CtcFunction(f2, y2)

    # CtcCartProd 1
    Cx1 = CtcCartProd([C1, C2])
    x1 = IntervalVector([[-5, 1],[-3, 6]])
    Cx1.contract(x1)

    # CtcCartProd 2
    Cx2 = cart_prod([C1, C2])
    x2 = IntervalVector([[-5, 1],[-3, 6]])
    Cx2.contract(x2)

    self.assertEqual(x1[0], x2[0])
    self.assertEqual(x1[1], x2[1])


if __name__ ==  '__main__':
  unittest.main()