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
import sys
import math
import tempfile

class TestContractors(unittest.TestCase):

  def assertApproxItvVec(self, first, second):
    for i in range(len(first)):
      self.assertAlmostEqual(first[i].lb(), second[i].lb())
      self.assertAlmostEqual(first[i].ub(), second[i].ub())

  def test_Function(self):
    # generate memory leak !!!
    try:
      f = Function("x", "y", "sqr(x) + sqr(y)")
    except RuntimeError as err:
      self.assertTrue(str(err) == "\n############# IBEX FUNCTION PARSER ###############\nSyntax error near \"sqr\" line 2: syntax error")

  def test_Function_vector(self):
    f = Function("x[2]", "y[3]", "(x[1]^2 - y[1]*y[0], y[0])")
    del f

  def test_function_matrix(self):
    try:
      f = Function("x[2]", "y[3]", "(( x[1]^2 - y[1]*y[0], y[0]);(x[0] + y[2], x[0]))")
    except:
      print("Unexpected error:", sys.exc_info()[0])

  def test_Function_string(self):
    fp = tempfile.NamedTemporaryFile(mode="w", delete=False)
    fp.write("""function f(x,y,z)
            return (x^2-y, y-2*z, z^3-sqrt(y-x));
        end""")
    filename = fp.name
    fp.close()
    f = Function(filename)
    self.assertEqual(str(f), "f:(x,y,z)->((x^2-y),(y-(2*z)),(z^3-sqrt((y-x))))")

  def test_CtcFwdBwd_default_arg(self):
    f = Function("x", "y", "(x)^2 + (y)^2 - [3.61, 4.41]")
    ctc1 = CtcFwdBwd(f)
    ctc1 = CtcFwdBwd(f, CmpOp.LEQ)

  def test_CtcFwdBwd_default_arg_with_list(self):
    f = Function("x", "y", "(x)^2 + (y)^2 - [3.61, 4.41]")
    ctc1 = CtcFwdBwd(f)
    ctc1 = CtcFwdBwd(f, [-oo,0])

  def test_CtcUnion_2_arguments(self):
    f = Function("x", "y", "(x)^2 + (y)^2 - [3.61, 4.41]")
    ctc1 = CtcFwdBwd(f, CmpOp.EQ)
    f2 = Function("x", "y", "(x-1)^2 + (y-1)^2 - [3.61, 4.41]")
    ctc2 = CtcFwdBwd(f2, CmpOp.EQ)
    ctc = CtcUnion([ctc1, ctc2])
    del f, ctc1, f2, ctc2 # delete tmp object
    a = IntervalVector(2)
    ctc.contract(a)
    self.assertApproxItvVec(a, IntervalVector(2, Interval(-2.1, 3.1)))

  def test_CtcUnion_Array(self):
    f = Function("x", "y", "(x)^2 + (y)^2 - [3.61, 4.41]")
    ctc1 = CtcFwdBwd(f, CmpOp.EQ)
    f2 = Function("x", "y", "(x-1)^2 + (y-1)^2 - [3.61, 4.41]")
    ctc2 = CtcFwdBwd(f2, CmpOp.EQ)
    ctc = ctc1 | ctc2 | ctc1
    del ctc2, ctc1 , f, f2 # Test if references are kept by python
    a = IntervalVector(2)
    ctc.contract(a)
    self.assertApproxItvVec(a, IntervalVector(2, Interval(-2.1, 3.1)))

  def test_CtcCompo(self):
    f = Function("x", "y", "(x)^2 + (y)^2 - [3.61, 4.41]")
    ctc1 = CtcFwdBwd(f, CmpOp.EQ)
    f2 = Function("x", "y", "(x-1)^2 + (y-1)^2 - [3.61, 4.41]")
    ctc2 = CtcFwdBwd(f2, CmpOp.EQ)
    f3 = Function("x", "y", "(x-1)^2 + (y-1)^2 - [3.61, 4.41]")
    ctc3 = CtcFwdBwd(f3, CmpOp.EQ)
    ctc = ctc1 & ctc2 & ctc3
    del ctc1, ctc2, ctc3 # optionnal test if references are kept by python
    a = IntervalVector(2)
    ctc.contract(a)
    self.assertApproxItvVec(a, IntervalVector(2, Interval(-1.1, 2.1)))

  def test_CtcArray(self):
    f = Function("x", "y", "(x)^2 + (y)^2 - [3.61, 4.41]")
    ctc1 = CtcFwdBwd(f, CmpOp.EQ)
    f2 = Function("x", "y", "(x-1)^2 + (y-1)^2 - [3.61, 4.41]")
    ctc2 = CtcFwdBwd(f2, CmpOp.EQ)
    f , f2 = None, None
    lst = [ctc1, ctc2, ctc2, ctc1,ctc1, ctc2, ctc2, ctc1]
    ctc = CtcUnion( lst)
    del lst, ctc1, ctc2
    a = IntervalVector(2)
    ctc.contract(a)
    self.assertApproxItvVec(a, IntervalVector(2, Interval(-2.1, 3.1)))

  def test_CtcPolar(self):
    ctc = CtcPolar()
    x, y, rho, theta = Interval(3,4), Interval(3,6), Interval(2,6), Interval(0, math.pi/4.0)
    ctc.contract(x,y,rho, theta)
    # [3, 4], [3, 4], [4.24264, 5.65685], [0.643501, 0.785398]

  def test_CtcQInter_withRefereces(self):
    data = [Interval(3.61, 4.41), Interval(3.61, 4.41)]
    cx = [0, 1]
    cy = [0, 1]
    ctcs = []

    for x, y, r  in zip(cx, cy, data):
      f = Function('x', 'y', '(x-%f)^2 + (y-%f)^2 - %s'%(x,y,r))
      ctcs.append(CtcFwdBwd(f, CmpOp.LEQ))

    ctc = CtcQInter(ctcs, len(ctcs)-1) # Union
    ctcs = None
    a = IntervalVector(2, Interval(-20, 20))
    ctc.contract(a)
    self.assertApproxItvVec(a, IntervalVector(2, Interval(-2.1, 3.1)))

  def test_CtcExist(self):
    f = Function("x", "y", "sqrt(x^2+y^2)-1")
    ctc = CtcFwdBwd(f, CmpOp.EQ)
    y = IntervalVector(1, [-0.5, -0.4])
    ctc_exist = CtcExist(ctc, y)
    X = IntervalVector(1, [0, 3])
    ctc_exist.contract(X)
    xres = sin(acos(Interval(-0.5, -0.4))) 
    self.assertTrue( X.is_subset(IntervalVector([xres])))

  def test_CtcForAll(self):
    f = Function("x", "y", "sqrt(x^2+y^2)-1")
    ctc = CtcFwdBwd(f, CmpOp.EQ)
    y = IntervalVector(1, [-0.5, -0.4])
    ctc_forall = CtcForAll(ctc, y)
    X = IntervalVector(1, [0, 3])
    ctc_forall.contract(X)
    xres = sin(acos(Interval(-0.5, -0.4))) 
    self.assertTrue( X.is_subset(IntervalVector([xres])))

if __name__ == '__main__':

  unittest.main()