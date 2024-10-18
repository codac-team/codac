#!/usr/bin/env python
#============================================================================
#                                P Y I B E X
# File        : test_thickPaving.py
# Author      : Benoit Desrochers
# Copyright   : Benoit Desrochers
# License     : See the LICENSE file
# Created     : Dec 28, 2015
#============================================================================
import unittest
from codac import *
from codac.unsupported import *
import math
from vibes import vibes


class TestSepPaving(unittest.TestCase):

  def test_constructor_01(self):
    X0 = IntervalVector(2,[-5, 5]);
    A = SepPaving(X0);
    sep = SepFwdBwd(Function("x", "y", "(x^2+y^2)"), Interval(-1e-10, 4))
    # sep = SepFwdBwd(Function("x", "y", "(x^2+y^2)"), Interval(-1, 4))
    # sep = SepFwdBwd(Function("x", "y", "(x^2+y^2)"), Interval(-1, 4))
    A.Sivia(sep, 0.1)
    vibes.beginDrawing()
    A.visit(SepToVibes("Titi"))
    vibes.endDrawing()

if __name__ == '__main__':

  unittest.main()
