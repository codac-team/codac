#!/usr/bin/env python
#============================================================================
#                                P Y I B E X
# File        : test_Separators.py
# Author      : Benoit Desrochers
# Copyright   : Benoit Desrochersss
# License     : See the LICENSE file
# Created     : Dec 28, 2014
#============================================================================


import unittest
import pyibex
from pyibex import *
import sys
import math
class TestSeparator(unittest.TestCase):

  def test_SepCtcPair(self):
    f = Function("x", "y", "x^2 + y^2 - 25")
    ctcIn = CtcFwdBwd(f, CmpOp.GEQ)
    ctcOut = CtcFwdBwd(f, CmpOp.LEQ)
    sep = SepCtcPair(ctcIn, ctcOut)
    self.assertEqual(sep.nb_var, 2)
    b0 = IntervalVector([[-10, -6],  [-10, 10]])
    b1 = IntervalVector([[-2, 2], [-2, 2]])

    xin = IntervalVector(b0)
    xout = IntervalVector(b0)
    sep.separate(xin, xout)
    self.assertFalse(xin.is_empty())
    self.assertTrue(xout.is_empty())


    xin = IntervalVector(b1)
    xout = IntervalVector(b1)
    sep.separate(xin, xout)
    self.assertFalse(xout.is_empty())
    self.assertTrue(xin.is_empty())

  def test_SepFwdBwd_CmdOp(self):
    f = Function("x", "y", "x^2 + y^2 - 25")
    sep = SepFwdBwd(f, CmpOp.LEQ)
    b0 = IntervalVector([[-10, -6],  [-10, 10]])
    b1 = IntervalVector([[-2, 2], [-2, 2]])

    xin = IntervalVector(b0)
    xout = IntervalVector(b0)
    sep.separate(xin, xout)
    self.assertFalse(xin.is_empty())
    self.assertTrue(xout.is_empty())


    xin = IntervalVector(b1)
    xout = IntervalVector(b1)
    sep.separate(xin, xout)
    self.assertFalse(xout.is_empty())
    self.assertTrue(xin.is_empty())


  def test_SepFwdBwd_Interval(self):
    f = Function("x", "y", "x^2 + y^2")
    sep = SepFwdBwd(f, Interval(0, 16))
    b0 = IntervalVector([[-10, -6],  [-10, 10]])
    b1 = IntervalVector([[1, 2], [1, 2]])

    xin = IntervalVector(b0)
    xout = IntervalVector(b0)
    sep.separate(xin, xout)
    self.assertFalse(xin.is_empty())
    self.assertTrue(xout.is_empty())


    xin = IntervalVector(b1)
    xout = IntervalVector(b1)
    sep.separate(xin, xout)
    self.assertFalse(xout.is_empty())
    # print(xin)
    self.assertTrue(xin.is_empty())

  def test_SepFwdBwd_IntervalAsList(self):
    f = Function("x", "y", "x^2 + y^2")
    sep = SepFwdBwd(f, [0., oo])


  def test_SepFwdBwd_IntervalVector(self):
    f = Function("x", "y", "(x^2 + y^2, x^2 + y^2)")
    sep = SepFwdBwd(f, IntervalVector(2, Interval(0, 16)))
    b0 = IntervalVector([[-10, -6],  [-10, 10]])
    b1 = IntervalVector([[1, 2], [1, 2]])

    xin = IntervalVector(b0)
    xout = IntervalVector(b0)
    sep.separate(xin, xout)
    self.assertFalse(xin.is_empty())
    self.assertTrue(xout.is_empty())


    xin = IntervalVector(b1)
    xout = IntervalVector(b1)
    sep.separate(xin, xout)
    self.assertFalse(xout.is_empty())
    # print(xin)
    self.assertTrue(xin.is_empty())


  def test_SepNot(self):

    f = Function("x", "y", "x^2 + y^2 - 25")
    sep = SepFwdBwd(f, CmpOp.LEQ)
    sepN = SepNot(sep)
    del sep # [optionnal] test for memory manadgment
    b0 = IntervalVector([[-10, -6],  [-10, 10]])
    b1 = IntervalVector([[-2, 2], [-2, 2]])

    xin = IntervalVector(b0)
    xout = IntervalVector(b0)
    sepN.separate(xin, xout)
    self.assertTrue(xin.is_empty())
    self.assertFalse(xout.is_empty())


    xin = IntervalVector(b1)
    xout = IntervalVector(b1)
    sepN.separate(xin, xout)
    self.assertTrue(xout.is_empty())
    self.assertFalse(xin.is_empty())

    pass

  def test_SepUnionInter(self):
    f1 = Function("x", "y", "x^2 + y^2 - 25")
    f2 = Function("x", "y", "(x-2)^2 + (y+2)^2 - 25")
    f3 = Function("x", "y", "(x+2)^2 + (y-2)^2 - 25")

    sep1 = SepFwdBwd(f1, CmpOp.LEQ)
    sep2 = SepFwdBwd(f2, Interval(4, 16))
    sep3 = SepFwdBwd(f3, CmpOp.LEQ)

    sepU = SepUnion([sep1, sep2, sep3])
    sepI = SepInter([sep1, sep2, sep3])

    # [optionnal] test for memory manadgment
    del f1, f2, f3, sep1, sep2, sep3

    b00 = IntervalVector([[-10, -6],  [-10, 10]])
    b01 = IntervalVector(b00)

    b10 = IntervalVector([[-10, -6],  [-10, 10]])
    b11 = IntervalVector(b10)

    sepU.separate(b00, b01)
    sepI.separate(b10, b11)


  # def test_SepProj(self):
  #   f = Function('x', 'y', 'x^2 + y^2 - 4')
  #   sep = SepFwdBwd(f,CmpOp.LEQ)
  #   y_init = IntervalVector(1, Interval(1/math.sqrt(2), 0.9))
  #   sep2 = SepProj(sep, y_init, 0.1, 10)
  #   # [optionnal] test for memory manadgment
  #   del sep, f
  #   Xin = IntervalVector([[-10,0]])
  #   Xout = IntervalVector([[-10,0]])
  #   sep2.separate(Xin, Xout)
  #   print(Xin, Xout)


  def test_UnionInter_with_array(self):
    cx = [3, 7,-3]
    cy = [4, 3, 7]
    d  = IntervalVector([3, 6, 6]).inflate(0.5)
    seps = []
    for i,(x,y) in enumerate(zip(cx,cy)):
      f = Function('x', 'y', '(x - %f)^2 + (y - %f)^2'%(x,y))
      seps.append(SepFwdBwd(f, sqr(d[i])))

    sepUnion = SepUnion(seps)
    sepInter = SepInter(seps)
    del seps


    Xin = IntervalVector(2, Interval(-10,10))
    Xout = IntervalVector(2, Interval(-10,10))
    sepUnion.separate(Xin, Xout)
    # print(Xin, Xout)

    Xin = IntervalVector(2, Interval(-10,10))
    Xout = IntervalVector(2, Interval(-10,10))
    sepInter.separate(Xin, Xout)


  def test_SepFixPoint(self):
    class FakeSep(Sep):
      def __init__(self):
        Sep.__init__(self, 1)

      def separate(self, x_in, x_out):
        # print(x_in, x_out)
        xpos = (x_in & x_out) & IntervalVector(1, Interval.POS_REALS)
        xneg = (x_in & x_out) & IntervalVector(1, Interval.NEG_REALS)

        x_in &= (IntervalVector(1, Interval.NEG_REALS) | IntervalVector(xpos.mid()) ) #| IntervalVector([0]).inflate(1e-10)
        x_out &= (IntervalVector(1, Interval.POS_REALS) | IntervalVector(xneg.mid()) ) #| IntervalVector([0]).inflate(1e-10)
        # print(x_in, x_out)
        return x_in, x_out


    sep = SepFixPoint(FakeSep())
    x_in, x_out = IntervalVector(1, [-10, 10]), IntervalVector(1, [-10, 10])

    sep.separate(x_in, x_out)
    self.assertTrue([0] in x_out)
    self.assertTrue([0] in x_in)
    self.assertTrue((x_in | x_out) == IntervalVector(1, [-10, 10]))
    self.assertTrue((x_in & x_out) == IntervalVector(1, [0, 0]))
    self.assertTrue(x_out == IntervalVector(1, [0, 10]))
    self.assertTrue(x_in == IntervalVector(1, [-10, 0]))


  def test_sepproj(self):
    f = Function('x', 'y', 'z', 'x^2 + y^2 + 2*x*y + z^2 + 3*z*x -3')
    sepfb = SepFwdBwd(f, LEQ)
    yinit = IntervalVector(1, [1, 2])
    sep = SepProj(sepfb, yinit, 0.01)

    sep = SepProj(sepfb, [1, 2])


  def test_SepFixPoint(self):
    class WrongSep(Sep):
      def __init__(self):
        Sep.__init__(self, 1)

      def separate(self, x_in, x_out):
        return tuple(x_in)
        
    sep = SepFixPoint(WrongSep())
    x_in, x_out = IntervalVector(1, [-10, 10]), IntervalVector(1, [-10, 10])
    sep.separate(x_in, x_out)
    



if __name__ == '__main__':

  unittest.main()
