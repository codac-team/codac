#!/usr/bin/env python

#  Codac tests
#  
#  Most of these tests come from the IBEX library (Gilles Chabert)
#  See more: https://ibex-lib.readthedocs.io
#  They have been revised to fit with Codac (v2)
#
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Gilles Chabert, Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *
import sys
import math

MAX_DOUBLE = sys.float_info.max

class TestInterval_operations(unittest.TestCase):

  def CHECK_add(self, x, y, expected):
    self.assertTrue((x + y) == expected)
    self.assertTrue((y + x) == expected) # symmetrical case
    x_ = Interval(x); x_ += y
    self.assertTrue(x_ == expected) # +=operator
    y_ = Interval(y); y_ += x
    self.assertTrue(y_ == expected) # +=operator in the other direction
    self.assertTrue(((-x)-y) == -expected) # substraction
    self.assertTrue(((-y)-x) == -expected) # symmetrical case
    x_ = Interval(-x); x_ -= y
    self.assertTrue(x_ == -expected) # -=operator
    y_ = Interval(-y); y_ -= x
    self.assertTrue(y_ == -expected) # -=operator in the other direction

  def CHECK_add_scalar(self, x, y, expected):
    self.assertTrue((x+y) == expected)
    self.assertTrue((y+x) == expected)
    x_ = Interval(x); x_ += y
    self.assertTrue(x_ == expected) # test the +=operator
    self.assertTrue(((-x)-y) == -expected)
    self.assertTrue(((-y)-x) == -expected)
    x_ = Interval(-x); x_ -= y
    self.assertTrue(x_ == -expected) # test the -=operator

  def CHECK_mul(self, x, y, expected):
    self.assertTrue(x * y == expected)
    self.assertTrue(y * x == expected) # symmetrical case
    x_ = Interval(x); x_ *= y
    self.assertTrue(x_ == expected) # *=operator

  def CHECK_mul_scalar(self, x, y, expected):
    self.assertTrue(x * y == expected)
    self.assertTrue(y * x == expected) # symmetrical case
    x_ = Interval(x); x_ *= y
    self.assertTrue(x_ == expected) # *=operator

  def CHECK_div(self, x, y, expected):
    self.assertTrue(x / Interval(y) == expected)
    self.assertTrue(Interval(x) / y == expected)
    x_ = Interval(x); x_ /= y
    self.assertTrue(x_ == expected) # /=operator

  def CHECK_div_scalar(self, x, y, expected):
    self.assertTrue(x / y == expected)
    x_ = Interval(x); x_ /= y
    self.assertTrue(x_ == expected) # /=operator

  def CHECK_trigo(self, x, expected):
    self.assertTrue(Approx(sin(x), 1e-9) == expected)
    self.assertTrue(Approx(sin(Interval.pi()-x), 1e-9) == expected)
    self.assertTrue(Approx(sin(x+Interval.two_pi()), 1e-9) == expected)
    self.assertTrue(Approx(sin(-x), 1e-9) == -expected)
    self.assertTrue(Approx(cos(x-Interval.half_pi()), 1e-9) == expected)
    self.assertTrue(Approx(cos(Interval.half_pi()-x), 1e-9) == expected)
    self.assertTrue(Approx(cos(x+Interval.half_pi()), 1e-9) == -expected)
    self.assertTrue(Approx(cos(x+Interval.two_pi()-Interval.half_pi()), 1e-9) == expected)

  def CHECK_pow(self, x, p, expected, eps = 0.):
    self.assertTrue(Approx(pow(x,p),eps) == expected)
    if p%2==0:
      self.assertTrue(Approx(pow(-x,p),eps) == expected)
    else:
      self.assertTrue(Approx(pow(-x,p),eps) == -expected)

  def tests_interval_operations(self):

    self.assertTrue(0 < next_float(0))
    self.assertTrue(0 > previous_float(0))
    self.assertTrue(1 < next_float(1))
    self.assertTrue(1 > previous_float(1))
    self.assertTrue(oo == next_float(oo))
    self.assertTrue(-oo == previous_float(-oo))
    self.assertTrue(-MAX_DOUBLE >= next_float(-oo))
    self.assertTrue(MAX_DOUBLE <= previous_float(oo))
    self.assertTrue(oo > MAX_DOUBLE)
    self.assertTrue(-oo < -MAX_DOUBLE)
    self.assertTrue(-Interval(0,1) == Interval(-1,0))
    self.assertTrue(-Interval(-oo,oo) == Interval(-oo,oo))
    self.assertTrue(-Interval(-oo,0) == Interval(0,oo))
    self.assertTrue(-Interval(-oo,1) == Interval(-1,oo))

    self.CHECK_add(Interval.empty(), Interval(0,1), Interval.empty())
    self.CHECK_add(Interval(0,1), Interval.empty(), Interval.empty())
    self.CHECK_add(Interval(-oo,1), Interval(0,1), Interval(-oo, 2))
    self.CHECK_add(Interval(1,oo), Interval(0,1), Interval(1,oo))
    self.CHECK_add(Interval(-oo,oo), Interval(0,1), Interval(-oo,oo))
    self.CHECK_add(Interval(MAX_DOUBLE,oo), 1, Interval(MAX_DOUBLE,oo))
    self.CHECK_add(Interval(MAX_DOUBLE,oo), -1, Interval(previous_float(MAX_DOUBLE),oo))
    self.CHECK_add(Interval(MAX_DOUBLE,oo), Interval(MAX_DOUBLE,oo), Interval(MAX_DOUBLE,oo))
    self.CHECK_add(Interval(MAX_DOUBLE,oo), -oo, Interval.empty())
    self.CHECK_add(Interval(MAX_DOUBLE,oo), oo, Interval.empty())
    self.CHECK_add(Interval(-oo,-MAX_DOUBLE), 1, Interval(-oo,next_float(-MAX_DOUBLE)))
    self.CHECK_add(Interval(-oo,-MAX_DOUBLE), -1, Interval(-oo,-MAX_DOUBLE))
    self.CHECK_add(Interval(-oo,-MAX_DOUBLE), Interval(-oo,-MAX_DOUBLE), Interval(-oo,-MAX_DOUBLE))

    self.CHECK_add_scalar(Interval.empty(), oo, Interval.empty())
    self.CHECK_add_scalar(Interval.empty(), 0, Interval.empty())
    self.CHECK_add_scalar(Interval(0,1), 1, Interval(1,2))
    self.CHECK_add_scalar(Interval(0,1), -oo, Interval.empty())
    self.CHECK_add_scalar(Interval(0,1), oo, Interval.empty())
    self.CHECK_add_scalar(Interval(-oo,1), 1, Interval(-oo,2))

    self.CHECK_mul(Interval.empty(), Interval(0,1), Interval.empty())
    self.CHECK_mul(0, Interval(-oo,oo), 0)
    self.CHECK_mul(Interval(-1,1), Interval(-oo,0), Interval(-oo,oo))
    self.CHECK_mul(Interval(-oo,-1), Interval(-1,0), Interval(0,oo))
    self.CHECK_mul(Interval(-oo, 1), Interval(-1,0), Interval(-1,oo))
    self.CHECK_mul(Interval(0, 1), Interval(1,oo), Interval(0,oo))
    self.CHECK_mul(Interval(0, 1), Interval(-1,oo), Interval(-1,oo))
    self.CHECK_mul(Interval(-oo,-1), Interval(0,1), Interval(-oo,0))
    self.CHECK_mul(Interval(-oo, 1), Interval(0,1), Interval(-oo,1))
    self.CHECK_mul(Interval(0, 1), Interval(-oo,-1), Interval(-oo,0))
    self.CHECK_mul(Interval(0, 1), Interval(-oo,1), Interval(-oo,1))
    self.CHECK_mul(Interval(1,oo), Interval(0,1), Interval(0,oo))
    self.CHECK_mul(Interval(-1,oo), Interval(0,1), Interval(-1,oo))
    self.CHECK_mul(Interval(1,2), Interval(1,2), Interval(1,4))
    self.CHECK_mul(Interval(1,2), Interval(-2,3), Interval(-4,6))
    self.CHECK_mul(Interval(-1,1), Interval(-2,3), Interval(-3,3))
    self.CHECK_mul_scalar(Interval(1,2), -oo, Interval.empty())
    self.CHECK_mul_scalar(Interval(1,2), oo, Interval.empty())
    self.CHECK_mul_scalar(Interval(1,2), -1, Interval(-2,-1))

    self.CHECK_div(Interval.empty(), Interval(0,1), Interval.empty())
    self.CHECK_div(0, 0, Interval.empty())
    self.CHECK_div(Interval(1,2), 0, Interval.empty())
    self.CHECK_div(Interval(-oo,oo), 0, Interval.empty())
    self.CHECK_div(0, Interval(0,1), 0)
    self.CHECK_div(0, Interval(-oo,oo), 0)
    self.CHECK_div(Interval(6,12), Interval(2,3), Interval(2,6))
    self.CHECK_div(Interval(6,12), Interval(-3,-2), Interval(-6,-2))
    self.CHECK_div(Interval(6,12), Interval(-2,3), Interval(-oo,oo))
    self.CHECK_div(Interval(-oo,-1), Interval(-1,0), Interval(1,oo))
    self.CHECK_div(Interval(-oo,-1), Interval(0,1), Interval(-oo,-1))
    self.CHECK_div(Interval(1,oo), Interval(-1,0), Interval(-oo,-1))
    self.CHECK_div(Interval(1,oo), Interval(0,1), Interval(1,oo))
    self.CHECK_div(Interval(-1,1), Interval(-1,1), Interval(-oo,oo))
    self.CHECK_div_scalar(Interval(1,2), -oo, Interval.empty())
    self.CHECK_div_scalar(Interval(1,2), oo, Interval.empty())
    self.CHECK_div_scalar(Interval(1,2), -1, Interval(-2,-1))

    self.assertTrue(sqrt(Interval(-oo,oo)) == Interval(0,oo))
    self.assertTrue(sqrt(Interval(-oo,0)) == 0)
    self.assertTrue(sqrt(Interval(-9,4)) == Interval(0,2))
    self.assertTrue(sqrt(Interval(4,9)) == Interval(2,3))
    self.assertTrue(sqrt(Interval(-9,-4)) == Interval.empty())
    self.assertTrue(sqrt(Interval(-9,oo)) == Interval(0,oo))

    self.assertTrue(log(Interval.empty()) == Interval.empty())
    self.assertTrue(log(Interval(-oo,oo)) == Interval(-oo,oo))
    self.assertTrue(log(Interval(0,oo)) == Interval(-oo,oo))
    self.assertTrue(log(Interval(-oo,0)) ==  Interval.empty())
    self.assertTrue(Approx(log(Interval(1,2))) == Interval(0,math.log(2)))
    self.assertTrue(Approx(log(Interval(-1,1))) == Interval(-oo,0))
    self.assertTrue(Approx(log(Interval(0,1))) == Interval(-oo,0))
    self.assertTrue(Approx(log(Interval(1,oo))) == Interval(0,oo))
    self.assertTrue(log(Interval(0)) == Interval.empty())
    self.assertTrue(log(Interval(-2,-1)) == Interval.empty())
    self.assertTrue((log(Interval(0,next_float(0)))).ub() > -744.5)

    self.assertTrue(exp(Interval.empty()) == Interval.empty())
    self.assertTrue(exp(Interval(-oo,oo)) == Interval(0,oo))
    self.assertTrue(Approx(exp(Interval(0,oo))) == Interval(1,oo))
    self.assertTrue(Approx(exp(Interval(-oo,0))) == Interval(0,1))
    self.assertTrue(Approx(exp(Interval(0,2))) == Interval(1,math.exp(2)))
    self.assertTrue(Approx(exp(Interval(-1,1))) == Interval(math.exp(-1),math.exp(1)))
    self.assertTrue(exp(Interval(1.e100,1.e111)) == Interval(MAX_DOUBLE,oo))
    self.assertTrue(exp(Interval(MAX_DOUBLE,oo)) == Interval(MAX_DOUBLE,oo))
    self.assertTrue(Approx(exp(Interval(0,MAX_DOUBLE))) == Interval(1,oo))

    pi_lb = Interval.pi().lb()
    pi_ub = Interval.pi().ub()

    self.CHECK_trigo(Interval(-oo,oo), Interval(-1,1))
    self.CHECK_trigo(Interval.empty(), Interval.empty())
    self.CHECK_trigo(Interval(0,pi_ub/2.0), Interval(0,1))
    self.CHECK_trigo(Interval(0,pi_ub), Interval(0,1))
    self.CHECK_trigo(Interval(0,3*pi_ub/2.0), Interval(-1,1))
    self.CHECK_trigo(Interval(pi_lb,3*pi_ub/2.0), Interval(-1,0))
    self.CHECK_trigo(Interval(0.5,1.5), Interval(math.sin(0.5),math.sin(1.5)))
    self.CHECK_trigo(Interval(1.5,3), Interval(math.sin(3.0),1))
    self.CHECK_trigo(Interval(3,4), Interval(math.sin(4.0),math.sin(3.0)))
    self.CHECK_trigo(Interval(3,5), Interval(-1,math.sin(3.0)))
    self.CHECK_trigo(Interval(3,2*pi_ub+1.5), Interval(-1,math.sin(1.5)))
    self.CHECK_trigo(Interval(5,2*pi_ub+1.5), Interval(math.sin(5.0),math.sin(1.5)))
    self.CHECK_trigo(Interval(5,2*pi_ub+3), Interval(math.sin(5.0),1))

    self.assertTrue(floor(Interval.empty()) == Interval.empty())
    self.assertTrue(floor(Interval(-oo,-0.000001)) == Interval(-oo,-1))
    self.assertTrue(floor(Interval(0.00000001,oo)) == Interval(0,oo))
    self.assertTrue(floor(Interval(-oo,oo)) == Interval(-oo,oo))
    self.assertTrue(floor(Interval(0.01,2.99)) == Interval(0,2))
    self.assertTrue(floor(Interval(-0.01,2.99)) == Interval(-1,2))
    self.assertTrue(floor(Interval(1.000000001,1.9999999999)) == 1.)
    self.assertTrue(floor(Interval(1e8, MAX_DOUBLE)) == Interval(1e8,MAX_DOUBLE))

    self.assertTrue(ceil(Interval.empty()) == Interval.empty())
    self.assertTrue(ceil(Interval(-oo,-0.000001)) == Interval(-oo,0))
    self.assertTrue(ceil(Interval(0.00000001,oo)) == Interval(1.0,oo))
    self.assertTrue(ceil(Interval(-oo,oo)) == Interval(-oo,oo))
    self.assertTrue(ceil(Interval(0.01,2.99)) == Interval(1,3))
    self.assertTrue(ceil(Interval(-0.01,2.99)) == Interval(0,3))
    self.assertTrue(ceil(Interval(1.000000001,1.9999999999)) == Interval(2,2))
    self.assertTrue(ceil(Interval(1e8, MAX_DOUBLE)) == Interval(1e8,MAX_DOUBLE))

    self.assertTrue(integer(Interval.empty()) == Interval.empty())
    self.assertTrue(integer(Interval(-oo,-0.000001)) == Interval(-oo,-1))
    self.assertTrue(integer(Interval(0.00000001,oo)) == Interval(1.0,oo))
    self.assertTrue(integer(Interval(-oo,oo)) == Interval(-oo,oo))
    self.assertTrue(integer(Interval(0.01,2.99)) == Interval(1,2))
    self.assertTrue(integer(Interval(-0.01,2.99)) == Interval(0,2))
    self.assertTrue(integer(Interval(1.000000001,1.9999999999)) == Interval.empty())
    self.assertTrue(integer(Interval(1e8, MAX_DOUBLE)) == Interval(1e8,MAX_DOUBLE))

    self.assertTrue(tan(Interval.pi()/2.0) == Interval(-oo,oo))
    x = tan(-Interval.pi())
    self.assertTrue(x.diam() < 1e-8)
    self.assertTrue(x.contains(0.))

    self.assertTrue(Approx(tan((3*Interval.pi()/4.0)|(5*Interval.pi()/4.0))) == Interval(-1,1))
    self.assertTrue(tan(Interval(-oo,oo)) == Interval(-oo,oo))
    self.assertTrue(Approx(tan((-Interval.pi()/4.0)|(Interval.pi()/4.0))) == Interval(-1,1))

    # tan(pi/4,pi/2)=[1,+oo)
    x = Interval(pi_lb/4.0,(1-1e-10)*pi_lb/2.0)
    y = tan(x)
    self.assertTrue(Approx(Interval(y.lb())) == Interval(1.0))
    self.assertTrue(y.ub() > 1000)

    # tan(-pi/2,pi/4)=(-oo,1]
    y = tan(Interval(-(1-1e-10)*pi_lb/2.0,pi_lb/4.0))
    self.assertTrue(y.lb() < -1000)
    self.assertTrue(y.ub() == 1.0)

    self.assertTrue(atan2(Interval(-oo,oo),Interval.empty()) == Interval.empty())
    self.assertTrue(Approx(atan2(Interval(1),1)) == Interval.pi()/4.0)
    self.assertTrue(Approx(atan2(Interval(-1),-1)) == -3*Interval.pi()/4.0)
    self.assertTrue(Approx(atan2(Interval(-1),1)) == -Interval.pi()/4.0)
    self.assertTrue(Approx(atan2(Interval(1),-1)) == 3*Interval.pi()/4.0)
    self.assertTrue(Approx(atan2(Interval(0,oo),Interval(0,oo))) == Interval(0,1)*Interval.half_pi())
    self.assertTrue(Approx(atan2(Interval(-oo,previous_float(0.0)),Interval(-oo,0))) == -(Interval.half_pi() | Interval.pi()))
    self.assertTrue(Approx(atan2(Interval(-oo,0),Interval(0,oo))) == Interval(-1,0)*Interval.half_pi())
    self.assertTrue(Approx(atan2(Interval(0,oo),Interval(-oo,0))) == (Interval.half_pi() | Interval.pi()))
    self.assertTrue(Approx(atan2(Interval(1,oo),Interval(-1,1))) == (Interval.pi()/4.0 | 3*Interval.pi()/4.0))
    self.assertTrue(Approx(atan2(Interval(-oo,-1),Interval(-1,1))) == -(Interval.pi()/4.0 | 3*Interval.pi()/4.0))
    self.assertTrue(atan2(Interval(-1,1),Interval(1,oo)) == Interval(-1,1)*Interval.pi()/4.0)
    self.assertTrue(atan2(Interval(-1,1),Interval(-oo,-1)) == Interval(-1,1)*Interval.pi())
    self.assertTrue(atan2(Interval(-oo,oo),Interval(-oo,oo)) == Interval(-1,1)*Interval.pi())
    self.assertTrue(atan2(Interval(0),0) == Interval.empty())

    self.CHECK_pow(Interval(-oo,oo), 4, Interval(0,oo))
    self.CHECK_pow(Interval.empty(), 4, Interval.empty())
    self.CHECK_pow(Interval(2,3), 4, Interval(16,81))
    self.CHECK_pow(Interval(-2,3), 4, Interval(0,81))
    self.CHECK_pow(Interval(-3,2), 4, Interval(0,81))
    self.CHECK_pow(Interval(2,oo), 4, Interval(16,oo))
    self.CHECK_pow(Interval(-oo,oo), 3, Interval(-oo,oo))
    self.CHECK_pow(Interval.empty(), 3, Interval.empty())
    self.CHECK_pow(Interval(2,3), 3, Interval(8,27))
    self.CHECK_pow(Interval(-2,3), 3, Interval(-8,27))
    self.CHECK_pow(Interval(-3,2), 3, Interval(-27,8))
    self.CHECK_pow(Interval(2,oo), 3, Interval(8,oo))
    self.CHECK_pow(Interval(-10,10), -2, Interval(1.0/100,oo), sys.float_info.epsilon)

    self.assertTrue(root(Interval(0,1),-1) == Interval(1.0,oo))
    self.assertTrue(Approx(root(Interval(-27,-8),3)) == Interval(-3,-2))
    self.assertTrue(root(Interval(-4,1),2) == Interval(0,1))
    self.assertTrue(Approx(root(Interval(-8,1),3)) == Interval(-2,1))

if __name__ ==  '__main__':
  unittest.main()