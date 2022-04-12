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
import math


class TestThickPaving(unittest.TestCase):

  def test_constructor_01(self):
    X0 = IntervalVector(2,[-2, 3]);
    A = ThickPaving(X0, UNK);
    self.assertEqual(A.X0, IntervalVector(2, [-2, 3]));
    del A
    A = ThickPaving(X0, IN);
    self.assertEqual(A.X0, X0);


class TestThickPavingBisection(unittest.TestCase):

  def setUp(self):
    X0 = IntervalVector(2,[0, 1]);
    self.f = lambda x:  UNK
    self.A = ThickPaving(X0, UNK, LargestFirst(0, 0.5));

  def test_eps_05(self):
    self.A.Sivia(self.f, 0.5, opInter);
    self.assertEqual(self.A.size, 4);

  def test_eps_025(self):
    self.A.Sivia(self.f, 0.25, opInter);
    self.assertEqual(self.A.size, 16);

  def test_eps_0125(self):
    self.A.Sivia(self.f, 0.125, opInter);
    self.assertEqual(self.A.size, 64);

  def test_eps_1_n(self):
    n = 8
    self.A.Sivia(self.f, 1.0/math.pow(2,n), opInter);
    self.assertEqual(self.A.size, math.pow(2,2*n));
      # // SECTION(" test eps = 1/2**n"){
    	# // 	A.Sivia(f, 1.0/std::pow(2,10));
    	# // 	CHECK(A.root.countLeaves() == std::pow(2,10)*std::pow(2,10)	);
    	# // }


#class TestThickTest(unittest.TestCase):
#
#  def test_ThickfInLambda(self):
#    flb = lambda x: x + [1., 1.]
#    fub = lambda x: x + [2., 2.]
#    test  = ThickfIn(flb, fub, IntervalVector(2, [0, 2]))
#    flb, fub = None, None
#    P = ThickPaving( IntervalVector(2, [-10,10])+[1,1], test, 0.1, opInter, False)
#    from vibes import vibes
#    vibes.beginDrawing()
#    P.visit(ToVibes("Test"))
#    vibes.endDrawing()
#
#  def test_ThickfIn(self):
#
#    flb = Function("x1", "x2","( x1 + 1, x2 + 1)")
#    fub = Function("x1", "x2","( x1 + 2, x2 + 2)")
#    test  = ThickfIn(flb, fub, IntervalVector(2, [0, 2]))
#    flb, fub = None, None
#    P = ThickPaving( IntervalVector(2, [-10,10]) , test, 0.5, opInter, False)
#    from vibes import vibes
#    vibes.beginDrawing()
#    P.visit(ToVibes("Test2"))
#    vibes.endDrawing()


#class TestThickSetSave(unittest.TestCase):
#
#  def test_ThickfInLambda(self):
#    # flb = lambda x: x + [1., 1.]
#    # fub = lambda x: x + [2., 2.]
#    # flb, fub = None, None
#    # test  = ThickfIn(flb, fub, IntervalVector(2, [0, 2]))
#    from vibes import vibes
#    vibes.beginDrawing()
#    test = ThickDisk(Interval(0),Interval(0), Interval(0,3), Interval(0,4))
#    P = ThickPaving( IntervalVector(2, [-10,10])+[1,1], test, 0.1, opInter, False)
#    P.save("Test.paving")
#    P2 = ThickPaving("Test.paving")
#    P2.visit(ToVibes("Test3"))
#    vibes.endDrawing()

# TEST_CASE( " THICK DISk"){
#   IntervalVector X0(2,Interval(-5, 5));
#   Interval mx(0);
#   Interval my(0);
#   Interval Rmin(0, 4);
#   Interval Rmax(0, 6);
#   ThickDisk t(mx, my, Rmin, Rmax);
#   ThickPaving A(X0, UNK);
#   A.Sivia(t, 0.5);
# }

if __name__ == '__main__':

  unittest.main()