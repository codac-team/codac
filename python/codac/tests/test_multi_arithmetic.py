#!/usr/bin/env python

import unittest
from codac import *

class TestArithmeticOnTube(unittest.TestCase):

  def assertApproxIntv(self, first, second):
    if first.is_empty() is False:
    # if isinstance(second, Interval):
      self.assertAlmostEqual(first.lb(), second.lb())
      self.assertAlmostEqual(first.ub(), second.ub())
    else:
      self.assertEqual(first, second)

  def test_interval(self):

    a = Interval(1,2)
    b = Interval(6,7)
    self.assertEqual(a+b, Interval(7,9))

  def test_cos(self):

    testd = 3.0
    self.assertAlmostEqual(cos(testd), -0.9899924966004454);

    testintv = Interval(0,oo)
    self.assertEqual(cos(testintv),Interval(-1,1))

    testtube = Tube(Interval(0,1),0.1)
    self.assertEqual(cos(testtube).codomain(),Interval(-1,1))

    testtraj = Trajectory(Interval(0,1), TFunction("sin(t)"), 0.1)
    self.assertApproxIntv(cos(testtraj).codomain(),Interval(0.6663667453928805, 1))

  def test_sqr(self):

    testd = 3.0
    self.assertAlmostEqual(sqr(testd), 9);

    testintv = Interval(1,2)
    self.assertEqual(sqr(testintv),Interval(1,4))

    testtube = Tube(Interval(0,1),0.1,Interval(3,4))
    self.assertEqual(sqr(testtube).codomain(),Interval(9,16))

    testtraj = Trajectory(Interval(3,4), TFunction("t"), 0.1)
    self.assertAlmostEqual(sqr(testtraj).codomain().lb(),9)
    self.assertAlmostEqual(sqr(testtraj).codomain().ub(),16)

  def test_pow(self):

    testdx = 3.0
    testdy = 4.0
    self.assertAlmostEqual(pow(testdx,testdy), 81);

    testintvx = Interval(3)
    testintvy = Interval(4)
    self.assertEqual(pow(testintvx,testdy), Interval(81.));
    self.assertEqual(pow(testdx,testintvy), Interval(81.));
    self.assertEqual(pow(testintvx,testintvy),Interval(81.))

    testcodac = Tube(Interval(0,1),0.1,Interval(3))
    testtubey = Tube(Interval(0,1),0.1,Interval(4))
    self.assertApproxIntv(pow(testcodac,testdy).codomain(),Interval(81.))
    self.assertApproxIntv(pow(testcodac,testintvy).codomain(),Interval(81.))

    testtrajx = Trajectory(Interval(3,4), TFunction("3"), 0.1)
    testtrajy = Trajectory(Interval(3,4), TFunction("4"), 0.1)
    self.assertApproxIntv(pow(testtrajx,testdy).codomain(),Interval(81.))

  def test_atan2(self):

    testdx = 3.0
    testdy = 4.0
    self.assertAlmostEqual(atan2(testdy,testdx), 0.9272952180016122);

    testintvx = Interval(3)
    testintvy = Interval(4)
    self.assertApproxIntv(atan2(testintvy,testdx), Interval(0.9272952180016122));
    self.assertApproxIntv(atan2(testdy,testintvx), Interval(0.9272952180016122));
    self.assertApproxIntv(atan2(testintvy,testintvx),Interval(0.9272952180016122))

    testcodac = Tube(Interval(0,1),0.1,Interval(3))
    testtubey = Tube(Interval(0,1),0.1,Interval(4))
    self.assertApproxIntv(atan2(testtubey,testcodac).codomain(),Interval(0.9272952180016122))
    self.assertApproxIntv(atan2(testtubey,testdx).codomain(),Interval(0.9272952180016122))
    self.assertApproxIntv(atan2(testdy,testcodac).codomain(),Interval(0.9272952180016122))
    self.assertApproxIntv(atan2(testtubey,testintvx).codomain(),Interval(0.9272952180016122))
    self.assertApproxIntv(atan2(testintvy,testcodac).codomain(),Interval(0.9272952180016122))

    testtrajx = Trajectory(Interval(3,4), TFunction("3"), 0.1)
    testtrajy = Trajectory(Interval(3,4), TFunction("4"), 0.1)
    self.assertApproxIntv(atan2(testtrajy,testtrajx).codomain(),Interval(0.9272952180016122))
    self.assertApproxIntv(atan2(testtrajy,testdx).codomain(),Interval(0.9272952180016122))
    self.assertApproxIntv(atan2(testdy,testtrajx).codomain(),Interval(0.9272952180016122))
        
if __name__ ==  '__main__':
  unittest.main()