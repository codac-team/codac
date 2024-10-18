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

class MyCtc(Ctc):

  def __init__(self):
    Ctc.__init__(self, 2)

  def contract(self, box):
    box &= IntervalVector.empty(2)
    box.set_empty()
    return box

class MyCtc_noReturn(Ctc):
  def __init__(self):
    Ctc.__init__(self, 2)

  def contract(self, box):
    box &= IntervalVector.empty(2)
    box.set_empty()
    return box

class MyCtc2(Ctc):
  def __init__(self, ctc, f):
    Ctc.__init__(self, 2)
    self.ctc = ctc
    self.f = f

  def contract(self, box):
    Y = self.f.eval_vector(box)
    self.ctc.contract(box)
    return box

class TestCtcOpaque(unittest.TestCase):

  def setUp(self):
    ctc1 = MyCtc()
    ctc2 = MyCtc()
    ctc3 = MyCtc()
    self.ctcs = [ctc1, ctc2, ctc3]

  def test_customCtc01(self):
    # -------------------------------------------------
    # Intersection of the 3 contractors with CtcQInter
    # -------------------------------------------------
    v = IntervalVector(2, [-30, 30])
    ctc = CtcQInter(self.ctcs, 3)
    ctc.contract(v)
    self.assertEqual(v, IntervalVector.empty(v.size()))

  def test_customCtc02(self):
    # -------------------------------------------------
    # Intersection of the 3 contractors with CtcCompo
    # -------------------------------------------------
    v = IntervalVector(2, [-30, 30])
    ctc = CtcCompo(self.ctcs)
    ctc.contract(v)
    self.assertEqual(v, IntervalVector.empty(v.size()))

  def test_customCtc02_noReturn(self):
    ctc1 = MyCtc_noReturn()
    ctc2 = MyCtc_noReturn()
    ctc3 = MyCtc_noReturn()
    v = IntervalVector(2, [-30, 30])
    ctc = CtcQInter([ctc1, ctc2, ctc3], 3)
    ctc.contract(v)
    self.assertEqual(v, IntervalVector.empty(v.size()))
    
  def test_customCtc03(self):
    # -------------------------------------------------
    # Intersection of the 3 contractors with CtcCompo
    # -------------------------------------------------
    v = IntervalVector(2, [-30, 30])
    ctc = CtcUnion(self.ctcs)
    ctc.contract(v)
    self.assertEqual(v, IntervalVector.empty(v.size()))

  def test_customCtc04(self):
    # -------------------------------------------------
    # Intersection of the 3 contractors with CtcCompo
    # -------------------------------------------------
    f = Function("x[2]", "(x[0], x[1])")
    v = IntervalVector(2, [-30, 30])
    ctc = MyCtc2(CtcUnion(self.ctcs), f)
    ctc.contract(v)
    self.assertEqual(v, IntervalVector.empty(v.size()))

  def test_customCtc05(self):
    # -------------------------------------------------
    # Intersection of the 3 contractors with CtcCompo
    # -------------------------------------------------
    f = Function("x[2]", "(x[0], x[1])")
    v = IntervalVector(2, [-30, 30])
    ctc = CtcInverse(CtcUnion(self.ctcs), f)
    ctc.contract(v)
    self.assertEqual(v, IntervalVector.empty(v.size()))
  
  def test_customCtc06(self):
    # -------------------------------------------------
    # Intersection of the 3 contractors with CtcCompo
    # -------------------------------------------------
    f = Function("x[2]", "(x[0], x[1])")
    v = IntervalVector(2, [-30, 30])
    ctc = CtcCompo([CtcInverse(c, f) for c in self.ctcs])
    ctc.contract(v)
    self.assertEqual(v, IntervalVector.empty(v.size()))

  def test_customCtc07(self):
    # -------------------------------------------------
    # Intersection of the 3 contractors with CtcCompo
    # -------------------------------------------------
    f = Function("x[2]", "(x[0], x[1])")
    v = IntervalVector(2, [-30, 30])
    ctc = CtcInverse(self.ctcs[0], f)
    ctc.contract(v)
    self.assertEqual(v, IntervalVector.empty(v.size()))

if __name__ == '__main__':

  unittest.main()