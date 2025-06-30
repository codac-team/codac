#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *
import sys
import math

import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '.')))
import codac2_tests_predefined_tubes as predef

class TestSlicedTube_integral(unittest.TestCase):

  def test_computing_integration_from_0_double_argument__tube1(self):

    x = predef.tube_test_1()
    x.set_ith_slice(Interval(-4,2), 14)
  
    self.assertTrue(Approx(x.integral(0)) == Interval(0))
    self.assertTrue(Approx(x.integral(0.1)) == Interval(0.4,0.8))
    self.assertTrue(Approx(x.integral(1.1)) == Interval(4.2,8.7))
    self.assertTrue(Approx(x.integral(0.5)) == Interval(2,4))
    self.assertTrue(Approx(x.integral(1.0)) == Interval(4,8))
    self.assertTrue(Approx(x.integral(1.5)) == Interval(5,11.5))
    self.assertTrue(Approx(x.integral(2.0)) == Interval(6,15))
    self.assertTrue(Approx(x.integral(2.5)) == Interval(6.5,18))
    self.assertTrue(Approx(x.integral(2.9),1e-8) == Interval(6.9,20.4))
    self.assertTrue(Approx(x.integral(3.0)) == Interval(7,21))
    self.assertTrue(Approx(x.integral(3.1),1e-8) == Interval(6.6,21.4))
    self.assertTrue(Approx(x.integral(4.0),1e-8) == Interval(3,25))

  def test_computing_integration_from_0_double_argument__tube1_01(self):

    x = predef.tube_test_1_01()
    x.set_ith_slice(Interval(-4,2), 14)
  
    self.assertTrue(Approx(x.integral(0)) == Interval(0))
    self.assertTrue(Approx(x.integral(0.1)) == Interval(0.4,0.8))
    self.assertTrue(Approx(x.integral(1.1)) == Interval(4.2,8.7))
    self.assertTrue(Approx(x.integral(0.5)) == Interval(2,4))
    self.assertTrue(Approx(x.integral(1.0)) == Interval(4,8))
    self.assertTrue(Approx(x.integral(1.5)) == Interval(5,11.5))
    self.assertTrue(Approx(x.integral(2.0)) == Interval(6,15))
    self.assertTrue(Approx(x.integral(2.5)) == Interval(6.5,18))
    self.assertTrue(Approx(x.integral(2.9),1e-8) == Interval(6.9,20.4))
    self.assertTrue(Approx(x.integral(3.0)) == Interval(7,21))
    self.assertTrue(Approx(x.integral(3.1),1e-8) == Interval(6.6,21.4))
    self.assertTrue(Approx(x.integral(4.0)) == Interval(3,25))

  def test_computing_integration_from_0_double_argument__tube4(self):

    x = predef.tube_test4()
    x.set(Interval(-1,1), Interval(10,11))

    self.assertTrue(Approx(x.integral(0.0)) == Interval(0.0))
    self.assertTrue(Approx(x.integral(0.1)) == Interval(0.1,0.2))
    self.assertTrue(Approx(x.integral(0.9)) == Interval(0.9,1.8))
    self.assertTrue(Approx(x.integral(1.0)) == Interval(1,2))
    self.assertTrue(Approx(x.integral(1.2)) == Interval(1.2,2.4))
    self.assertTrue(Approx(x.integral(2.0)) == Interval(2.0,4.0))
    self.assertTrue(Approx(x.integral(3.0)) == Interval(3.0,6.0))
    self.assertTrue(Approx(x.integral(5.0)) == Interval(5.0,10.0))
    self.assertTrue(Approx(x.integral(7.0)) == Interval(7.0,14.0))
    self.assertTrue(Approx(x.integral(9.0)) == Interval(9.0,18.0))
    self.assertTrue(Approx(x.integral(10.0)) == Interval(9.5,19.5))
    self.assertTrue(Approx(x.integral(10.1)) == Interval(9.4,19.6))
    self.assertTrue(Approx(x.integral(11.0),1e-8) == Interval(8.5,20.5))
    self.assertTrue(Approx(x.integral(11.5),1e-8) == Interval(7.75,20.25))
    self.assertTrue(Approx(x.integral(12.5),1e-8) == Interval(6.5,20.5))
    self.assertTrue(Approx(x.integral(12.6),1e-8) == Interval(6.4,20.6))
    self.assertTrue(Approx(x.integral(14.5),1e-8) == Interval(7,23.5))

  def test_computing_integration_from_0_interval_argument__tube1(self):

    x = predef.tube_test_1()
    x.set_ith_slice(Interval(-4,2), 14)

    self.assertTrue(Approx(x.integral(Interval(0))) == Interval(0))
    self.assertTrue(Approx(x.integral(Interval(0.1))) == Interval(0.4,0.8))
    self.assertTrue(Approx(x.integral(Interval(0.5))) == Interval(2,4))
    self.assertTrue(Approx(x.integral(Interval(1.0))) == Interval(4,8))
    self.assertTrue(Approx(x.integral(Interval(1.5))) == Interval(5,11.5))
    self.assertTrue(Approx(x.integral(Interval(2.0))) == Interval(6,15))
    self.assertTrue(Approx(x.integral(Interval(2.5))) == Interval(6.5,18))
    self.assertTrue(Approx(x.integral(Interval(3.0))) == Interval(7,21))
    self.assertTrue(Approx(x.integral(Interval(3.0))) == Interval(7,21))
    self.assertTrue(Approx(x.integral(Interval(0.0,0.0))) == Interval(0,0))
    self.assertTrue(Approx(x.integral(Interval(0.0,0.5))) == Interval(0,4))
    self.assertTrue(Approx(x.integral(Interval(0.0,1.0))) == Interval(0,8))
    self.assertTrue(Approx(x.integral(Interval(0.0,1.5))) == Interval(0,11.5))
    self.assertTrue(Approx(x.integral(Interval(0.0,2.0))) == Interval(0,15))
    self.assertTrue(Approx(x.integral(Interval(1.0,1.0))) == Interval(4,8))
    self.assertTrue(Approx(x.integral(Interval(1.0,1.5))) == Interval(4,11.5))
    self.assertTrue(Approx(x.integral(Interval(1.0,2.0))) == Interval(4,15))
    self.assertTrue(Approx(x.integral(Interval(1.1,1.9))) == Interval(4.2,14.3))
    self.assertTrue(Approx(x.integral(Interval(1.1,2.1)),1e-8) == Interval(4.2,15.6))
    self.assertTrue(Approx(x.integral(Interval(6,7))) == Interval(-23,19))
    self.assertTrue(Approx(x.integral(Interval(0,46)),1e-8) == Interval(-85,194))

  def test_computing_integration_from_0_interval_argument__tube1_01(self):

    x = predef.tube_test_1_01()

    self.assertTrue(Approx(x.integral(Interval(0))) == Interval(0))
    self.assertTrue(Approx(x.integral(Interval(0.1))) == Interval(0.4,0.8))
    self.assertTrue(Approx(x.integral(Interval(0.5))) == Interval(2,4))
    self.assertTrue(Approx(x.integral(Interval(1.0))) == Interval(4,8))
    self.assertTrue(Approx(x.integral(Interval(1.5))) == Interval(5,11.5))
    self.assertTrue(Approx(x.integral(Interval(2.0))) == Interval(6,15))
    self.assertTrue(Approx(x.integral(Interval(2.5))) == Interval(6.5,18))
    self.assertTrue(Approx(x.integral(Interval(3.0))) == Interval(7,21))
    self.assertTrue(Approx(x.integral(Interval(3.0))) == Interval(7,21))
    self.assertTrue(Approx(x.integral(Interval(0.0,0.0))) == Interval(0,0))
    self.assertTrue(Approx(x.integral(Interval(0.0,0.5))) == Interval(0,4))
    self.assertTrue(Approx(x.integral(Interval(0.0,1.0))) == Interval(0,8))
    self.assertTrue(Approx(x.integral(Interval(0.0,1.5))) == Interval(0,11.5))
    self.assertTrue(Approx(x.integral(Interval(0.0,2.0))) == Interval(0,15))
    self.assertTrue(Approx(x.integral(Interval(1.0,1.0))) == Interval(4,8))
    self.assertTrue(Approx(x.integral(Interval(1.0,1.5))) == Interval(4,11.5))
    self.assertTrue(Approx(x.integral(Interval(1.0,2.0))) == Interval(4,15))
    self.assertTrue(Approx(x.integral(Interval(1.1,1.9)),1e-8) == Interval(4.2,14.3))
    self.assertTrue(Approx(x.integral(Interval(1.1,2.1)),1e-8) == Interval(4.2,15.6))
    self.assertTrue(Approx(x.integral(Interval(6,7))) == Interval(-23,19))
    self.assertTrue(x.tdomain().t0_tf() == Interval(0,46))
    self.assertTrue(x.codomain() == Interval(-11,13))
    self.assertTrue(Approx(x.integral(Interval(0,46))) == Interval(-85,194))

  def test_computing_integration_from_0_interval_argument__tube4(self):

    x = predef.tube_test4()
    x.set(Interval(-1,1), Interval(10,11))

    self.assertTrue(Approx(x.integral(Interval(12.5)),1e-8) == Interval(6.5,20.5))
    self.assertTrue(Approx(x.integral(Interval(14.5)),1e-8) == Interval(7,23.5))
    p_intv = x.partial_integral(Interval(12.5,14.5))
    self.assertTrue(Approx(p_intv[0],1e-8) == Interval(6.,7.))
    self.assertTrue(Approx(p_intv[1],1e-8) == Interval(20.5,23.5))
    self.assertTrue(Approx(x.integral(Interval(12.5,14.5)),1e-8) == Interval(6.0,23.5))
    self.assertTrue(Approx(x.integral(Interval(0)),1e-8) == Interval(0))
    self.assertTrue(Approx(x.integral(Interval(10.2)),1e-8) == Interval(9.3,19.7))
    self.assertTrue(Approx(x.integral(Interval(1.5,3.5)),1e-8) == Interval(1.5,7))
    self.assertTrue(Approx(x.integral(Interval(9,21)),1e-8) == Interval(6,36.5))

  def test_computing_integration_from_0_interval_argument__tube4_05(self):

    x = predef.tube_test4_05()
    x.set(Interval(-1,1), Interval(10,11))

    self.assertTrue(Approx(x.integral(Interval(12.5))) == Interval(6.5,20.5))
    self.assertTrue(Approx(x.integral(Interval(14.5))) == Interval(7,23.5))
    p_intv = x.partial_integral(Interval(12.5,14.5))
    self.assertTrue(p_intv[0] == Interval(6.,7.))
    self.assertTrue(p_intv[1] == Interval(20.5,23.5))
    self.assertTrue(Approx(x.integral(Interval(12.5,14.5))) == Interval(6.0,23.5))
    self.assertTrue(Approx(x.integral(Interval(0))) == Interval(0))
    self.assertTrue(Approx(x.integral(Interval(10.2))) == Interval(9.3,19.7))
    self.assertTrue(Approx(x.integral(Interval(1.5,3.5))) == Interval(1.5,7))
    self.assertTrue(Approx(x.integral(Interval(9,21))) == Interval(6,36.5))

  def test_computing_integration_from_0_partial_integration__tube4(self):

    x = predef.tube_test4()
    x.set(Interval(-1,1), Interval(10,11))

    self.assertTrue(Approx(x.partial_integral(Interval(0))) == (Interval(0.), Interval(0.)))
    self.assertTrue(Approx(x.partial_integral(Interval(0.2))) == (Interval(0.2), Interval(0.4)))
    self.assertTrue(Approx(x.partial_integral(Interval(0.1))) == (Interval(0.1), Interval(0.2)))
    self.assertTrue(Approx(x.partial_integral(Interval(0.9))) == (Interval(0.9), Interval(1.8)))
    self.assertTrue(Approx(x.partial_integral(Interval(1.0))) == (Interval(1), Interval(2)))
    self.assertTrue(Approx(x.partial_integral(Interval(1.2))) == (Interval(1.2), Interval(2.4)))
    self.assertTrue(Approx(x.partial_integral(Interval(2.0))) == (Interval(2.0), Interval(4.0)))
    self.assertTrue(Approx(x.partial_integral(Interval(3.0))) == (Interval(3.0), Interval(6.0)))
    self.assertTrue(Approx(x.partial_integral(Interval(5.0))) == (Interval(5.0), Interval(10.0)))
    self.assertTrue(Approx(x.partial_integral(Interval(7.0))) == (Interval(7.0), Interval(14.0)))
    self.assertTrue(Approx(x.partial_integral(Interval(9.0))) == (Interval(9.0), Interval(18.0)))
    self.assertTrue(Approx(x.partial_integral(Interval(10.0))) == (Interval(9.5), Interval(19.5)))
    self.assertTrue(Approx(x.partial_integral(Interval(10.1)),1e-8) == (Interval(9.4), Interval(19.6)))
    self.assertTrue(Approx(x.partial_integral(Interval(11.0)),1e-8) == (Interval(8.5), Interval(20.5)))
    self.assertTrue(Approx(x.partial_integral(Interval(12.5)),1e-8) == (Interval(6.5), Interval(20.5)))
    self.assertTrue(Approx(x.partial_integral(Interval(12.6)),1e-8) == (Interval(6.4), Interval(20.6)))
    self.assertTrue(Approx(x.partial_integral(Interval(14.5)),1e-8) == (Interval(7), Interval(23.5)))
    self.assertTrue(Approx(x.partial_integral(Interval(12.5,13)),1e-8) == (Interval(6,6.5), Interval(20.5,21)))
    self.assertTrue(Approx(x.partial_integral(Interval(13,14)),1e-8) == (Interval(6,6.5), Interval(21,22.5)))
    self.assertTrue(Approx(x.partial_integral(Interval(14,14.5)),1e-8) == (Interval(6.5,7), Interval(22.5,23.5)))
    self.assertTrue(Approx(x.partial_integral(Interval(12.5,14.5)),1e-8) == (Interval(6.0,7.0), Interval(20.5,23.5)))
    self.assertTrue(Approx(x.partial_integral(Interval(1.5,3.5)),1e-8) == (Interval(1.5,3.5), Interval(3,7)))
    self.assertTrue(Approx(x.partial_integral(Interval(9,21)),1e-8) == (Interval(6,13.5), Interval(18,36.5)))

  def test_computing_integration_from_0_partial_integration__tube4_05(self):

    x = predef.tube_test4_05()

    self.assertTrue(Approx(x.partial_integral(Interval(0))) == (Interval(0.), Interval(0.)))
    self.assertTrue(Approx(x.partial_integral(Interval(0.2))) == (Interval(0.2), Interval(0.4)))
    self.assertTrue(Approx(x.partial_integral(Interval(0.1))) == (Interval(0.1), Interval(0.2)))
    self.assertTrue(Approx(x.partial_integral(Interval(0.9))) == (Interval(0.9), Interval(1.8)))
    self.assertTrue(Approx(x.partial_integral(Interval(1.0))) == (Interval(1), Interval(2)))
    self.assertTrue(Approx(x.partial_integral(Interval(1.2))) == (Interval(1.2), Interval(2.4)))
    self.assertTrue(Approx(x.partial_integral(Interval(2.0))) == (Interval(2.0), Interval(4.0)))
    self.assertTrue(Approx(x.partial_integral(Interval(3.0))) == (Interval(3.0), Interval(6.0)))
    self.assertTrue(Approx(x.partial_integral(Interval(5.0))) == (Interval(5.0), Interval(10.0)))
    self.assertTrue(Approx(x.partial_integral(Interval(7.0))) == (Interval(7.0), Interval(14.0)))
    self.assertTrue(Approx(x.partial_integral(Interval(9.0))) == (Interval(9.0), Interval(18.0)))
    self.assertTrue(Approx(x.partial_integral(Interval(10.0))) == (Interval(9.5), Interval(19.5)))
    self.assertTrue(Approx(x.partial_integral(Interval(10.1)),1e-8) == (Interval(9.4), Interval(19.6)))
    self.assertTrue(Approx(x.partial_integral(Interval(11.0))) == (Interval(8.5), Interval(20.5)))
    self.assertTrue(Approx(x.partial_integral(Interval(12.5))) == (Interval(6.5), Interval(20.5)))
    self.assertTrue(Approx(x.partial_integral(Interval(12.6)),1e-8) == (Interval(6.4), Interval(20.6)))
    self.assertTrue(Approx(x.partial_integral(Interval(14.5))) == (Interval(7), Interval(23.5)))
    self.assertTrue(Approx(x.partial_integral(Interval(12.5,13))) == (Interval(6,6.5), Interval(20.5,21)))
    self.assertTrue(Approx(x.partial_integral(Interval(13,14))) == (Interval(6,6.5), Interval(21,22.5)))
    self.assertTrue(Approx(x.partial_integral(Interval(14,14.5))) == (Interval(6.5,7), Interval(22.5,23.5)))
    self.assertTrue(Approx(x.partial_integral(Interval(12.5,14.5))) == (Interval(6.0,7.0), Interval(20.5,23.5)))
    self.assertTrue(Approx(x.partial_integral(Interval(1.5,3.5))) == (Interval(1.5,3.5), Interval(3,7)))
    self.assertTrue(Approx(x.partial_integral(Interval(9,21))) == (Interval(6,13.5), Interval(18,36.5)))

  def test_computing_integration_from_0_partial_integration__tube1(self):

    x = predef.tube_test_1()
    x.set_ith_slice(Interval(-4,2), 14)

    self.assertTrue(Approx(x.partial_integral(x.tdomain().t0_tf())) == (Interval(-85,7), Interval(-16,194)))
    self.assertTrue(Approx(x.partial_integral(Interval(0.,46.))) == (Interval(-85,7), Interval(-16,194)))
    self.assertTrue(Approx(x.partial_integral(Interval(7.))) == (Interval(-23), Interval(13)))
    self.assertTrue(Approx(x.partial_integral(Interval(0.,7.))) == (Interval(-23,7.0), Interval(0,25)))
    self.assertTrue(Approx(x.partial_integral(Interval(2.,6.))) == (Interval(-13,7), Interval(15,25)))
    self.assertTrue(Approx(x.partial_integral(Interval(6.,7.))) == (Interval(-23,-13), Interval(13,19)))

    integrale_lb = Interval.empty()
    integrale_ub = Interval.empty()

    t = 0.0
    while t <= 46.0:
      integrale_lb |= x.partial_integral(Interval(0, t))[0]
      integrale_ub |= x.partial_integral(Interval(0, t))[1]
      t += 0.2

    self.assertTrue(integrale_lb.is_subset(Interval(-85,7)))
    self.assertTrue(integrale_ub.is_subset(Interval(-16,194)))

  def test_computing_integration_from_0_partial_integration__tube1_01(self):

    x = predef.tube_test_1_01()

    self.assertTrue(Approx(x.partial_integral(Interval(0.,46.))) == (Interval(-85,7), Interval(-16,194)))
    self.assertTrue(Approx(x.partial_integral(Interval(7.))) == (Interval(-23), Interval(13)))
    self.assertTrue(Approx(x.partial_integral(Interval(0.,7.))) == (Interval(-23,7.0), Interval(0,25)))
    self.assertTrue(Approx(x.partial_integral(Interval(2.,6.))) == (Interval(-13,7), Interval(15,25)))
    self.assertTrue(Approx(x.partial_integral(Interval(6.,7.))) == (Interval(-23,-13), Interval(13,19)))

  def test_computing_integration_two_interval_bounds__tube1(self):

    x = predef.tube_test_1()
    x.set_ith_slice(Interval(-4,2), 14)

    self.assertTrue(Approx(x.integral(Interval(20), Interval(20))) == Interval(0.))
    self.assertTrue(Approx(x.integral(Interval(2.1), Interval(2.1))) == Interval(0.))
    self.assertTrue(Approx(x.integral(Interval(12.2), Interval(12.2)),1e-8) == Interval(0.))
    self.assertTrue(Approx(x.integral(Interval(12.2), Interval(12.2)),1e-8) == Interval(0.))
    self.assertTrue(Approx(x.integral(Interval(17.), Interval(20.))) == Interval(12,24))
    self.assertTrue(Approx(x.integral(Interval(28.), Interval(33.))) == Interval(17,31))
    self.assertTrue(Approx(x.integral(Interval(20.), Interval(28.))) == Interval(51,88))
    self.assertTrue(Approx(x.integral(Interval(17.), Interval(33.))) == Interval(80,143))
    self.assertTrue(Approx(x.integral(Interval(22.5), Interval(23))) == Interval(4.5,6))
    self.assertTrue(Approx(x.integral(Interval(8.5), Interval(10.5))) == Interval(-18,-9.5))
    self.assertTrue(Approx(x.integral(Interval(0), Interval(7))) == Interval(-23,13))
    self.assertTrue(Approx(x.integral(Interval(2), Interval(6))) == Interval(-19,4))
    self.assertTrue(Approx(x.integral(Interval(0,2), Interval(6,7))) == Interval(-29,19))
    self.assertTrue(Approx(x.integral(Interval(0), Interval(17))) == Interval(-85,-4))
    self.assertTrue(Approx(x.integral(Interval(0), Interval(38))) == Interval(1,161))
    self.assertTrue(Approx(x.integral(Interval(0), Interval(17,38))) == Interval(-85,161))
    self.assertTrue(Approx(x.integral(Interval(10), Interval(13))) == Interval(-21,-11))
    self.assertTrue(Approx(x.integral(Interval(17), Interval(20))) == Interval(12,24))
    self.assertTrue(Approx(x.integral(Interval(13), Interval(17))) == Interval(-11,11))
    self.assertTrue(Approx(x.integral(Interval(10,13), Interval(17))) == Interval(-32,11))
    self.assertTrue(Approx(x.integral(Interval(13), Interval(17,20))) == Interval(-11,35))
    self.assertTrue(Approx(x.integral(Interval(10,13), Interval(17,20))) == Interval(-32,35))
    self.assertTrue(Approx(x.integral(Interval(5,10), Interval(31,42))) == Interval(-6,180))

  def test_computing_integration_two_interval_bounds__tube1_01(self):

    x = predef.tube_test_1_01()

    self.assertTrue(Approx(x.integral(Interval(20), Interval(20))) == Interval(0.))
    self.assertTrue(Approx(x.integral(Interval(2.1), Interval(2.1))) == Interval(0.))
    self.assertTrue(Approx(x.integral(Interval(12.2), Interval(12.2)),1e-8) == Interval(0.))
    self.assertTrue(Approx(x.integral(Interval(12.2), Interval(12.2)),1e-8) == Interval(0.))
    self.assertTrue(Approx(x.integral(Interval(17.), Interval(20.))) == Interval(12,24))
    self.assertTrue(Approx(x.integral(Interval(28.), Interval(33.))) == Interval(17,31))
    self.assertTrue(Approx(x.integral(Interval(20.), Interval(28.))) == Interval(51,88))
    self.assertTrue(Approx(x.integral(Interval(17.), Interval(33.))) == Interval(80,143))
    self.assertTrue(Approx(x.integral(Interval(22.5), Interval(23))) == Interval(4.5,6))
    self.assertTrue(Approx(x.integral(Interval(8.5), Interval(10.5))) == Interval(-18,-9.5))
    self.assertTrue(Approx(x.integral(Interval(0), Interval(7))) == Interval(-23,13))
    self.assertTrue(Approx(x.integral(Interval(2), Interval(6))) == Interval(-19,4))
    self.assertTrue(Approx(x.integral(Interval(0,2), Interval(6,7))) == Interval(-29,19))
    self.assertTrue(Approx(x.integral(Interval(0), Interval(17))) == Interval(-85,-4))
    self.assertTrue(Approx(x.integral(Interval(0), Interval(38))) == Interval(1,161))
    self.assertTrue(Approx(x.integral(Interval(0), Interval(17,38))) == Interval(-85,161))
    self.assertTrue(Approx(x.integral(Interval(10), Interval(13))) == Interval(-21,-11))
    self.assertTrue(Approx(x.integral(Interval(17), Interval(20))) == Interval(12,24))
    self.assertTrue(Approx(x.integral(Interval(13), Interval(17))) == Interval(-11,11))
    self.assertTrue(Approx(x.integral(Interval(10,13), Interval(17))) == Interval(-32,11))
    self.assertTrue(Approx(x.integral(Interval(13), Interval(17,20))) == Interval(-11,35))
    self.assertTrue(Approx(x.integral(Interval(10,13), Interval(17,20))) == Interval(-32,35))
    self.assertTrue(Approx(x.integral(Interval(5,10), Interval(31,42))) == Interval(-6,180))

  def test_computing_integration_two_interval_bounds__tube4(self):

    x = predef.tube_test4()
    x.set(Interval(-1,1), Interval(10,11))

    self.assertTrue(Approx(x.integral(Interval(0,8), Interval(15,21)),1e-8) == Interval(-0.5,36.5))
    self.assertTrue(Approx(x.integral(Interval(0,8), Interval(9,21)),1e-8) == Interval(-2,36.5))

  def test_computing_integration_two_interval_bounds__tube4_05(self):

    x = predef.tube_test4_05()

    self.assertTrue(Approx(x.integral(Interval(0,8), Interval(15,21))) == Interval(-0.5,36.5))
    self.assertTrue(Approx(x.integral(Interval(0,8), Interval(9,21))) == Interval(-2,36.5))

  def test_computing_partial_integration_two_interval_bounds__tube1(self):

    x = predef.tube_test_1()
    x.set_ith_slice(Interval(-4,2), 14)

    self.assertTrue(Approx(x.partial_integral(Interval(0), Interval(0))) == (Interval(0.), Interval(0.)))
    self.assertTrue(Approx(x.partial_integral(Interval(20), Interval(20))) == (Interval(0.), Interval(0.)))
    self.assertTrue(Approx(x.partial_integral(Interval(22,23), Interval(24,25))) == (Interval(8,24), Interval(13,37)))
    self.assertTrue(Approx(x.partial_integral(Interval(17,20), Interval(28,33))) == (Interval(51,80), Interval(88,143)))
    self.assertTrue(Approx(x.partial_integral(Interval(0.), Interval(7.))) == (Interval(-23), Interval(13)))
    self.assertTrue(Approx(x.partial_integral(Interval(2.), Interval(6.))) == (Interval(-19), Interval(4)))
    self.assertTrue(Approx(x.partial_integral(Interval(0,2), Interval(6,7))) == (Interval(-29,-13), Interval(-2,19)))
    self.assertTrue(Approx(x.partial_integral(Interval(10,13), Interval(17,20))) == (Interval(-32,1), Interval(0,35)))
    self.assertTrue(Approx(x.partial_integral(Interval(0.), Interval(46.))) == (Interval(-3), Interval(194)))

  def test_computing_partial_integration_two_interval_bounds__tube1_01(self):

    x = predef.tube_test_1_01()

    self.assertTrue(Approx(x.partial_integral(Interval(0), Interval(0))) == (Interval(0.), Interval(0.)))
    self.assertTrue(Approx(x.partial_integral(Interval(20), Interval(20))) == (Interval(0.), Interval(0.)))
    self.assertTrue(Approx(x.partial_integral(Interval(22,23), Interval(24,25))) == (Interval(8,24), Interval(13,37)))
    self.assertTrue(Approx(x.partial_integral(Interval(17,20), Interval(28,33))) == (Interval(51,80), Interval(88,143)))
    self.assertTrue(Approx(x.partial_integral(Interval(0.), Interval(7.))) == (Interval(-23), Interval(13)))
    self.assertTrue(Approx(x.partial_integral(Interval(2.), Interval(6.))) == (Interval(-19), Interval(4)))
    self.assertTrue(Approx(x.partial_integral(Interval(0,2), Interval(6,7))) == (Interval(-29,-13), Interval(-2,19)))
    self.assertTrue(Approx(x.partial_integral(Interval(10,13), Interval(17,20))) == (Interval(-32,1), Interval(0,35)))
    self.assertTrue(Approx(x.partial_integral(Interval(0.), Interval(46.))) == (Interval(-3), Interval(194)))

  def test_computing_partial_integration_two_interval_bounds__tube4(self):

    x = predef.tube_test4()
    x.set(Interval(-1,1), Interval(10,11))

    self.assertTrue(Approx(x.partial_integral(Interval(0.1,1.1), Interval(2.6,3.2)))
      == (Interval(1.5,3.1), Interval(3.0,6.2)))
    self.assertTrue(Approx(x.partial_integral(Interval(8.6,9.9), Interval(13.2,13.6)),1e-8)
      == (Interval(-3.35,-2.3), Interval(1.95,4.7)))

  def test_computing_partial_integration_two_interval_bounds__tube4_05(self):

    x = predef.tube_test4_05()

    self.assertTrue(Approx(x.partial_integral(Interval(0.1,1.1), Interval(2.6,3.2)))
      == (Interval(1.5,3.1), Interval(3.0,6.2)))
    self.assertTrue(Approx(x.partial_integral(Interval(8.6,9.9), Interval(13.2,13.6)),1e-8)
      == (Interval(-3.35,-2.3), Interval(1.95,4.7)))


if __name__ ==  '__main__':
  unittest.main()