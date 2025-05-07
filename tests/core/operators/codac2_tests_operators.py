#!/usr/bin/env python

#  Codac tests
#  
#  Most of these tests come from the IBEX library (Gilles Chabert)
#  See more: https://ibex-lib.readthedocs.io
#  They have been revised to fit with Codac (v2)
#
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Gilles Chabert, (Simon Rohou)
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *
import sys
import math

class TestInterval_bwd(unittest.TestCase):

  def CHECK_bwd_trigo(self, y, x, expected_x):
    _x = Interval()
    _x = x; SinOp.bwd(y,_x); self.assertTrue(Approx(_x, 1e-5) == expected_x)
    _x = -x; SinOp.bwd(-y,_x); self.assertTrue(Approx(_x, 1e-5) == -expected_x)
    _x = x-Interval.half_pi(); CosOp.bwd(y,_x); self.assertTrue(Approx(_x, 1e-5) == expected_x-Interval.half_pi())
    _x = x+Interval.half_pi(); CosOp.bwd(-y,_x); self.assertTrue(Approx(_x, 1e-5) == expected_x+Interval.half_pi())

  def CHECK_bwd_pow(self, y, x, expected_x, p):
    _x = Interval()
    _x = x; PowOp.bwd(y,_x,p); self.assertTrue(Approx(_x,1e-5) == expected_x)
    _x = -x;
    if p%2 == 0:
      PowOp.bwd(y,_x,p);
    else:
      PowOp.bwd(-y,_x,p);
    self.assertTrue(Approx(_x,1e-5) == -expected_x)

  def CHECK_bwd_mul(self, y, x1, x2, expected_x1, expected_x2):
    _x1 = Interval(); _x2 = Interval()

    _x1 = x1; _x2 = x2; MulOp.bwd(y,_x1,_x2)
    self.assertTrue(Approx(_x1) == expected_x1)
    self.assertTrue(Approx(_x2) == expected_x2)

    _x1 = x1; _x2 = x2; MulOp.bwd(y,_x2,_x1)
    self.assertTrue(Approx(_x1) == expected_x1)
    self.assertTrue(Approx(_x2) == expected_x2)

    _x1 = -x1; _x2 = x2; MulOp.bwd(-y,_x2,_x1)
    self.assertTrue(Approx(_x1) == -expected_x1)
    self.assertTrue(Approx(_x2) == expected_x2)

    _x1 = x1; _x2 = -x2; MulOp.bwd(-y,_x2,_x1)
    self.assertTrue(Approx(_x1) == expected_x1)
    self.assertTrue(Approx(_x2) == -expected_x2)

  def CHECK_bwd_div(self, y, x1, x2, expected_x1, expected_x2):
    _x1 = Interval(); _x2 = Interval()

    _x1 = x1; _x2 = x2; DivOp.bwd(y,_x1,_x2)
    self.assertTrue(Approx(_x1) == expected_x1)
    self.assertTrue(Approx(_x2) == expected_x2)

    _x1 = -x1; _x2 = -x2; DivOp.bwd(y,_x1,_x2)
    self.assertTrue(Approx(_x1) == -expected_x1)
    self.assertTrue(Approx(_x2) == -expected_x2)

    _x1 = -x1; _x2 = x2; DivOp.bwd(-y,_x1,_x2)
    self.assertTrue(Approx(_x1) == -expected_x1)
    self.assertTrue(Approx(_x2) == expected_x2)

    _x1 = x1; _x2 = -x2; DivOp.bwd(-y,_x1,_x2)
    self.assertTrue(Approx(_x1) == expected_x1)
    self.assertTrue(Approx(_x2) == -expected_x2)

  def CHECK_bwd_max(self, y, x1, x2, expected_x1, expected_x2):
    _x1 = Interval(); _x2 = Interval()

    _x1 = x1; _x2 = x2; MaxOp.bwd(y,_x1,_x2)
    self.assertTrue(_x1 == expected_x1)
    self.assertTrue(_x2 == expected_x2)

    _x1 = x1; _x2 = x2; MaxOp.bwd(y,_x2,_x1)
    self.assertTrue(_x1 == expected_x1)
    self.assertTrue(_x2 == expected_x2)

    _x1 = -x1; _x2 = -x2; MinOp.bwd(-y,_x1,_x2)
    self.assertTrue(_x1 == -expected_x1)
    self.assertTrue(_x2 == -expected_x2)

    _x1 = -x1; _x2 = -x2; MinOp.bwd(-y,_x2,_x1)
    self.assertTrue(_x1 == -expected_x1)
    self.assertTrue(_x2 == -expected_x2)

  def CHECK_bwd_atan(self, y, x, expected_x):
    _x = Interval()

    _x = x; AtanOp.bwd(y,_x)
    self.assertTrue(_x == expected_x)

    _x = -x; AtanOp.bwd(-y,_x)
    self.assertTrue(_x == -expected_x)

  def CHECK_bwd_add(self, y, x1, x2, expected_x1, expected_x2):
    _x1 = Interval(); _x2 = Interval()

    _x1 = x1; _x2 = x2; AddOp.bwd(y,_x1,_x2)
    self.assertTrue(_x1 == expected_x1)
    self.assertTrue(_x2 == expected_x2)

    _x1 = x1; _x2 = x2; AddOp.bwd(y,_x2,_x1)
    self.assertTrue(_x1 == expected_x1)
    self.assertTrue(_x2 == expected_x2)

    _x1 = -x1; _x2 = -x2; AddOp.bwd(-y,_x1,_x2)
    self.assertTrue(_x1 == -expected_x1)
    self.assertTrue(_x2 == -expected_x2)

    _x1 = -x1; _x2 = -x2; AddOp.bwd(-y,_x2,_x1)
    self.assertTrue(_x1 == -expected_x1)
    self.assertTrue(_x2 == -expected_x2)

  def CHECK_bwd_sub(self, y, x1, x2, expected_x1, expected_x2):
    _x1 = Interval(); _x2 = Interval()

    _x1 = x1; _x2 = x2; SubOp.bwd(y,_x1,_x2)
    self.assertTrue(_x1 == expected_x1)
    self.assertTrue(_x2 == expected_x2)

    _x1 = -x1; _x2 = -x2; SubOp.bwd(-y,_x1,_x2)
    self.assertTrue(_x1 == -expected_x1)
    self.assertTrue(_x2 == -expected_x2)

    _x1 = x1; _x2 = x2; SubOp.bwd(-y,_x2,_x1)
    self.assertTrue(_x1 == expected_x1)
    self.assertTrue(_x2 == expected_x2)

    _x1 = -x1; _x2 = -x2; SubOp.bwd(y,_x2,_x1)
    self.assertTrue(_x1 == -expected_x1)
    self.assertTrue(_x2 == -expected_x2)

  def CHECK_bwd_imod(self, p, x1, x2, expected_x1, expected_x2):
    _x1 = Interval(); _x2 = Interval()
    _x1 = x1; _x2 = x2; ModOp.bwd(_x1,_x2,p)
    self.assertTrue(Approx(_x1) == expected_x1)
    self.assertTrue(Approx(_x2) == expected_x2)

  def tests_interval_bwd(self):

    pi_lb = Interval.pi().lb()
    pi_ub = Interval.pi().ub()

    self.CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(0,pi_ub/2.0),          Interval(0.5,1.5))
    self.CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(0,5*pi_ub/2.0),        Interval(0.5,2*pi_ub+1.5))
    self.CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(-2*pi_ub,pi_ub/2.0),   Interval(-2*pi_ub+0.5,1.5))
    self.CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(-2*pi_ub,5*pi_ub/2.0), Interval(-2*pi_ub+0.5,2*pi_ub+1.5))
    self.CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(-pi_lb-0.4,pi_ub/2.0), Interval(0.5,1.5))
    self.CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(-pi_ub-0.5,pi_ub/2.0), Interval(-pi_ub-0.5,1.5))
    self.CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(-3*pi_ub/2,pi_ub/2.0), Interval(-pi_ub-1.5,1.5))
    self.CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(0.5,pi_lb-1.6),        Interval(0.5,1.5))
    self.CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(0.5,pi_ub-1.5),        Interval(0.5,pi_ub-1.5))
    self.CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(0.5,pi_ub),            Interval(0.5,pi_ub-0.5))
    self.CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(-pi_ub-0.5,pi_ub-1.5), Interval(-pi_ub-0.5,pi_ub-1.5))
    self.CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(-3*pi_ub/2,pi_ub),     Interval(-pi_ub-1.5,pi_ub-0.5))
    self.CHECK_bwd_trigo(sin(Interval(0.5,1.5)), Interval(-pi_ub/2,pi_ub/2.0),   Interval(0.5,1.5))
    self.CHECK_bwd_trigo(Interval(2,3),          Interval(-oo,oo),               Interval.empty())
    self.CHECK_bwd_trigo(Interval(math.sin(0.5),1.0), Interval(0,pi_ub/2.0),          Interval(0.5,pi_ub/2.0))
    self.CHECK_bwd_trigo(Interval(math.sin(0.5),1.0), Interval(0,5*pi_ub/2.0),        Interval(0.5,5*pi_ub/2.0))
    self.CHECK_bwd_trigo(Interval(math.sin(0.5),1.0), Interval(-2*pi_ub,pi_ub/2.0),   Interval(-2*pi_ub+0.5,pi_ub/2.0))
    self.CHECK_bwd_trigo(Interval(math.sin(0.5),1.0), Interval(-2*pi_ub,5*pi_ub/2.0), Interval(-2*pi_ub+0.5,5*pi_ub/2.0))
    self.CHECK_bwd_trigo(Interval(math.sin(0.5),1.0), Interval(-pi_lb-0.4,pi_ub/2.0), Interval(0.5,pi_ub/2.0))
    self.CHECK_bwd_trigo(Interval(math.sin(0.5),1.0), Interval(-pi_ub-0.5,pi_ub/2.0), Interval(-pi_ub-0.5,pi_ub/2.0))
    self.CHECK_bwd_trigo(Interval(math.sin(0.5),1.0), Interval(-3*pi_ub/2,pi_ub/2.0), Interval(-3*pi_ub/2,pi_ub/2.0))
    self.CHECK_bwd_trigo(Interval(math.sin(0.5),1.0), Interval(0.5,pi_ub),            Interval(0.5,pi_ub-0.5))
    self.CHECK_bwd_trigo(Interval(math.sin(0.5),1.0), Interval(-3*pi_ub/2,pi_ub),     Interval(-3*pi_ub/2,pi_ub-0.5))
    self.CHECK_bwd_trigo(Interval(1.0),          Interval(4.0,6.0),              Interval.empty())

    x = Interval()

    x = Interval(0,4);    SqrOp.bwd(Interval(1,9),x);   self.assertTrue(x == Interval(1,3))
    x = Interval(0,2);    SqrOp.bwd(Interval(1,9),x);   self.assertTrue(x == Interval(1,2))
    x = Interval(-4,2);   SqrOp.bwd(Interval(1,9),x);   self.assertTrue(x == Interval(-3,2))
    x = Interval(-4,-3);  SqrOp.bwd(Interval(1,9),x);   self.assertTrue(x == Interval(-3,-3))
    x = Interval(-4,1);   SqrOp.bwd(Interval(-oo,9),x); self.assertTrue(x == Interval(-3,1))
    x = Interval(-1,5);   SqrOp.bwd(Interval(4,9),x);   self.assertTrue(x == Interval(2,3))
    x = Interval(-oo,oo); SqrOp.bwd(Interval(-4,-2),x); self.assertTrue(x == Interval.empty())

    x = Interval(-oo,oo);     LogOp.bwd(Interval(-oo,oo),x); self.assertTrue(x == Interval(0,oo))
    x = Interval(-oo,oo);     LogOp.bwd(Interval(-oo,0),x);  self.assertTrue(x == Approx(Interval(0,1)))
    x = Interval(0,1);        LogOp.bwd(Interval(0,oo),x);   self.assertTrue(x == Approx(Interval(1)))
    x = Interval(-1,3);       LogOp.bwd(Interval(0,1),x);    self.assertTrue(x == Approx(Interval(1,math.exp(1.))))
    x = Interval(-1,3);       LogOp.bwd(Interval(-oo,1),x);  self.assertTrue(x == Approx(Interval(0,math.exp(1.))))
    x = Interval(-1,2);       LogOp.bwd(Interval(-oo,1),x);  self.assertTrue(x == Interval(0,2))
    x = Interval(3,4);        LogOp.bwd(Interval(-oo,1),x);  self.assertTrue(x == Interval.empty())
    x = Interval(-0.01,0.01); LogOp.bwd(Interval(-1,1),x);   self.assertTrue(x == Interval.empty())

    self.CHECK_bwd_pow(Interval(16,81),  Interval(1,4),    Interval(2,3), 4)
    self.CHECK_bwd_pow(Interval(16,81),  Interval(3,4),    Interval(3,3), 4)
    self.CHECK_bwd_pow(Interval(16,81),  Interval(-1,4),   Interval(2,3), 4)
    self.CHECK_bwd_pow(Interval(16,81),  Interval(-2,4),   Interval(-2,3), 4)
    self.CHECK_bwd_pow(Interval(16,81),  Interval(-5,4),   Interval(-3,3), 4)
    self.CHECK_bwd_pow(Interval(16,81),  Interval(1,1),    Interval.empty(), 4)
    self.CHECK_bwd_pow(Interval(16,81),  Interval(4,4),    Interval.empty(), 4)
    self.CHECK_bwd_pow(Interval(8,27),   Interval(1,4),    Interval(2,3), 3)
    self.CHECK_bwd_pow(Interval(8,27),   Interval(3,4),    Interval(3,3), 3)
    self.CHECK_bwd_pow(Interval(8,27),   Interval(-5,4),   Interval(2,3), 3)
    self.CHECK_bwd_pow(Interval(-8,27),  Interval(-5,4),   Interval(-2,3), 3)
    self.CHECK_bwd_pow(Interval(-27,27), Interval(-5,4),   Interval(-3,3), 3)
    self.CHECK_bwd_pow(Interval(8,27),   Interval(1,1),    Interval.empty(), 3)
    self.CHECK_bwd_pow(Interval(8,27),   Interval(4,4),    Interval.empty(), 3)
    self.CHECK_bwd_pow(Interval(-27,-8), Interval(-1,-1),  Interval.empty(), 3)
    self.CHECK_bwd_pow(Interval(-27,-8), Interval(-4,-4),  Interval.empty(), 3)
    self.CHECK_bwd_pow(Interval(0,1),    Interval(-10,10), Interval(-10,10), -2)

    self.CHECK_bwd_mul(Interval(1,2),Interval(0.1,2.0),Interval(-10,2),Interval(0.5,2.0),Interval(0.5,2))
    self.CHECK_bwd_mul(Interval.empty(),Interval(0.1,2.0),Interval(-10,2),Interval.empty(),Interval.empty())
    self.CHECK_bwd_mul(Interval(0,0),Interval(0.1,2.0),Interval(-10,2),Interval(0.1,2.0),Interval(0,0))
    self.CHECK_bwd_mul(Interval(0,0),Interval(-1,1),Interval(-1,1),Interval(-1,1),Interval(-1,1))
    self.CHECK_bwd_mul(Interval(1,1),Interval(0,10),Interval(0,10),Interval(0.1,10.0),Interval(0.1,10.0))

    self.CHECK_bwd_div(Interval(1,2), Interval(0,1), Interval(2,3), Interval.empty(), Interval.empty())
    self.CHECK_bwd_div(Interval(1,2), Interval(0,1), Interval(1,3), 1., 1.)
    self.CHECK_bwd_div(Interval(1,2), Interval(1,3), Interval(0,1), Interval(1,2), Interval(0.5,1))
    self.CHECK_bwd_div(Interval(-1,1), Interval(-2,2), Interval(0,1), Interval(-1,1), Interval(0,1))
    self.CHECK_bwd_div(Interval(-1,1), Interval(-2,2), 0., 0., 0.)
    self.CHECK_bwd_div(0, Interval(-2,2), Interval(-2,2), 0, Interval(-2,2))
    self.CHECK_bwd_div(Interval(0,oo), Interval(0,1), Interval(-1,0), 0, Interval(-1,0))
    self.CHECK_bwd_div(Interval(next_float(0.),oo), Interval(0,1), Interval(-1,0), 0, 0)
    self.CHECK_bwd_div(Interval(0,oo), Interval(next_float(0.),1), Interval(-1,0), Interval.empty(), Interval.empty())

    self.CHECK_bwd_max(Interval.empty(), Interval(-2,-1), Interval(-2,3), Interval.empty(), Interval.empty())
    self.CHECK_bwd_max(Interval(0,1), Interval(-2,-1), Interval(-2,3), Interval(-2,-1), Interval(0,1))
    self.CHECK_bwd_max(Interval(0,1), Interval(-2,-1), Interval(2,3), Interval.empty(), Interval.empty())
    self.CHECK_bwd_max(Interval(0,1), Interval(-2,0), Interval(-2,3), Interval(-2,0), Interval(-2,1))
    self.CHECK_bwd_max(Interval(0,1), Interval(-2,2), Interval(-2,3), Interval(-2,1), Interval(-2,1))

    a = Interval(); y = Interval()

    a = Interval(math.pi/6.,math.pi/3.); y = Interval(.5,10.); x = Interval(.5,2.);             
    Atan2Op.bwd(a,y,x); self.assertTrue(Approx(y) == Interval(.5,2.*math.sqrt(3.))); self.assertTrue(Approx(x) == Interval(.5,2.))
    a = Interval(math.pi/6.,math.pi/3.); y = Interval(.5,2.); x = Interval(.5,10.);            
    Atan2Op.bwd(a,y,x); self.assertTrue(Approx(y) == Interval(.5,2.)); self.assertTrue(Approx(x) == Interval(.5,2.*math.sqrt(3.)))
    a = Interval(-math.pi/4.,math.pi/4.); y = Interval(1.,2.); x = Interval(.5,2.);             
    Atan2Op.bwd(a,y,x); self.assertTrue(Approx(y) == Interval(1.,2.)); self.assertTrue(Approx(x) == Interval(1.,2.))
    a = Interval(-math.pi/2.,0.); y = Interval(.5,2.); x = Interval(.5,10.);            
    Atan2Op.bwd(a,y,x); self.assertTrue(Approx(y) == Interval.empty()); self.assertTrue(Approx(x) == Interval.empty())
    a = Interval(2.*math.pi,3.*math.pi); y = Interval(-.5,2.); x = Interval(.5,10.);            
    Atan2Op.bwd(a,y,x); self.assertTrue(Approx(y) == Interval.empty()); self.assertTrue(Approx(x) == Interval.empty())
    a = Interval(2*math.pi/3.,5.*math.pi/6.); y = Interval(0.,100.); x = Interval(-20.,-math.sqrt(3.)/2.); 
    Atan2Op.bwd(a,y,x); self.assertTrue(Approx(y,1e-10) == Interval(.5,20.*math.sqrt(3.))); self.assertTrue(Approx(x) == Interval(-20.,-math.sqrt(3.)/2))
    a = Interval(-3*math.pi/4.,-2*math.pi/3.); y = Interval(-math.sqrt(3.)/2.,2.); x = Interval(-math.sqrt(2.)/2.,0.);   
    Atan2Op.bwd(a,y,x); self.assertTrue(Approx(y) == Interval(-math.sqrt(3.)/2.,0.)); self.assertTrue(Approx(x) == Interval(-math.sqrt(2.)/2.,0.))
    a = Interval(-3*math.pi/4.,-2*math.pi/3.); y = Interval(-math.sqrt(3.)/2.,2.); x = Interval(-1.,-.5);           
    Atan2Op.bwd(a,y,x); self.assertTrue(Approx(y) == Interval(-math.sqrt(3.)/2.,-.5)); self.assertTrue(Approx(x) == Interval(-math.sqrt(3.)/2.,-.5))
    a = Interval(-3*math.pi/4.,-math.pi/4.); y = Interval(-5.,-.5); x = Interval(-oo,oo);            
    Atan2Op.bwd(a,y,x); self.assertTrue(Approx(y) == Interval(-5.,-.5)); self.assertTrue(Approx(x) == Interval(-5.,5.))
    a = Interval(-math.pi/3.,math.pi/4.); y = Interval(-oo,oo); x = Interval(sqrt(3.)/2.);       
    Atan2Op.bwd(a,y,x); self.assertTrue(Approx(y) == Interval(-1.5,math.sqrt(3.)/2.)); self.assertTrue(Approx(x) == Interval(math.sqrt(3.)/2.))
    a = Interval.half_pi(); y = Interval(1); x = 0.;                          
    Atan2Op.bwd(a,y,x); self.assertTrue(Approx(y) == 1.); self.assertTrue(Approx(x) == 0.)

    x = Interval(0,15);   SqrtOp.bwd(Interval(1,3), x);   self.assertTrue(x == Interval(1,9))
    x = Interval(0,2);    SqrtOp.bwd(Interval(1,3), x);   self.assertTrue(x == Interval(1,2))
    x = Interval(-4,2);   SqrtOp.bwd(Interval(1,3), x);   self.assertTrue(x == Interval(1,2))
    x = Interval(-4,-3);  SqrtOp.bwd(Interval(1,3), x);   self.assertTrue(x == Interval.empty())
    x = Interval(-4,1);   SqrtOp.bwd(Interval(-oo,9), x); self.assertTrue(x == Interval(0,1))
    x = Interval(-1,5);   SqrtOp.bwd(Interval(2,5), x);   self.assertTrue(x == Interval(4,5))
    x = Interval(-oo,oo); SqrtOp.bwd(Interval(-4,-2), x); self.assertTrue(x == Interval.empty())

    x = Interval(-1.,3.);       AtanOp.bwd(Interval(0.,math.pi/6.),x);                self.assertTrue(x == tan(Interval(0.,math.pi/6.)))
    x = Interval(0,5*math.pi/2.0); AtanOp.bwd(Interval(-math.pi,1.5),x);              self.assertTrue(x == Interval(0,5*math.pi/2.0))
    x = Interval(.2,.5);        AtanOp.bwd(Interval(0.,math.pi/6.),x);                self.assertTrue(x == Interval(.2,.5))
    x = Interval(-100,100);     AtanOp.bwd(Interval(-math.pi/2-0.1,math.pi/2+0.1),x); self.assertTrue(x == Interval(-100,100))
    x = Interval(-100,100);     AtanOp.bwd(Interval(math.pi/2+0.1,math.pi),x);        self.assertTrue(x == Interval.empty())
    x = Interval(-100,100);     AtanOp.bwd(Interval(-math.pi,-math.pi/2-0.1),x);      self.assertTrue(x == Interval.empty())


    x = Interval(-oo,oo); AtanOp.bwd(Interval(-math.pi/4,math.pi/2.), x); self.assertTrue(x.lb() == -1); self.assertTrue(x.ub() > 1000)
    x = Interval(-oo,oo); AtanOp.bwd(Interval(-math.pi/2,math.pi/4.), x); self.assertTrue(x.ub() == +1); self.assertTrue(x.lb() < -1000)

    self.CHECK_bwd_add(Interval(1,3),Interval(1,2),Interval(-10,5),Interval(1,2),Interval(-1,2))
    self.CHECK_bwd_add(Interval.empty(),Interval(0.1,2.0),Interval(-10,2),Interval.empty(),Interval.empty())
    self.CHECK_bwd_add(Interval(0,0),Interval(0,0),Interval(.1,5),Interval.empty(),Interval.empty())
    self.CHECK_bwd_add(Interval(0,0),Interval(-1,1),Interval(-1,1),Interval(-1,1),Interval(-1,1))
    self.CHECK_bwd_add(Interval(-1,1),Interval(1,2),Interval(-10,5),Interval(1,2),Interval(-3,0))

    self.CHECK_bwd_sub(Interval(1,3),Interval(1,2),Interval(-10,5),Interval(1,2),Interval(-2,1))
    self.CHECK_bwd_sub(Interval.empty(),Interval(0.1,2.0),Interval(-10,2),Interval.empty(),Interval.empty())
    self.CHECK_bwd_sub(Interval(0,0),Interval(0,0),Interval(.1,5),Interval.empty(),Interval.empty())
    self.CHECK_bwd_sub(Interval(0,0),Interval(-1,1),Interval(-1,1),Interval(-1,1),Interval(-1,1))
    self.CHECK_bwd_sub(Interval(-1,1),Interval(1,2),Interval(-10,5),Interval(1,2),Interval(0,3))

    self.CHECK_bwd_imod(3.,Interval(3.,5.),Interval(1.,2.),Interval(4.,5.),Interval(1.,2.))
    self.CHECK_bwd_imod(2.,Interval(7.,8.),Interval(.5,2.),Interval(7.,8.),Interval(1.,2.))
    self.CHECK_bwd_imod(2.,Interval(7.,8.),Interval(0.,2.),Interval(7.,8.),Interval(0.,2.))
    self.CHECK_bwd_imod(2.*math.pi,Interval(2.*math.pi,3.*math.pi),Interval(math.pi/6,math.pi/2.),Interval(13.*math.pi/6.,5.*math.pi/2.),Interval(math.pi/6,math.pi/2.))
    self.CHECK_bwd_imod(2.*math.pi,Interval(3.*math.pi,4.*math.pi),Interval(math.pi/3,math.pi/2.),Interval.empty(),Interval.empty())
    self.CHECK_bwd_imod(2.*math.pi,Interval(3.*math.pi,4.*math.pi),Interval(0.,math.pi/2.),Interval(4*math.pi),Interval(0.))
    self.CHECK_bwd_imod(2.*math.pi,Interval(2.*math.pi,4.*math.pi),Interval(-math.pi/6,math.pi/2.),Interval(2.*math.pi,4.*math.pi),Interval(-math.pi/6,math.pi/2.))
    self.CHECK_bwd_imod(2.*math.pi,Interval(7.*math.pi/4.,8.*math.pi/3),Interval(-math.pi/2,math.pi/2.),Interval(7.*math.pi/4.,5.*math.pi/2.),Interval(-math.pi/4,math.pi/2.))

    x = Interval(-oo,oo);        FloorOp.bwd(Interval.empty(),x);        self.assertTrue(x == Interval.empty())
    x = Interval(-oo,-0.000001); FloorOp.bwd(Interval(-oo,-1),x);        self.assertTrue(x == Interval(-oo,-0.000001))
    x = Interval(-oo, 0.000001); FloorOp.bwd(Interval(-oo,-1),x);        self.assertTrue(x == Interval(-oo,0))
    x = Interval(-oo, 0.000001); FloorOp.bwd(Interval(-oo,-0.000001),x); self.assertTrue(x == Interval(-oo,0))
    x = Interval(0.000001,oo);   FloorOp.bwd(Interval(1,oo),x);          self.assertTrue(x == Interval(1,oo))
    x = Interval(-0.000001,oo);  FloorOp.bwd(Interval(1,oo),x);          self.assertTrue(x == Interval(1,oo))
    x = Interval(-0.000001,oo);  FloorOp.bwd(Interval(0.000001,oo),x);   self.assertTrue(x == Interval(1,oo))
    x = Interval(0.01,2.99);     FloorOp.bwd(Interval(1,2),x);           self.assertTrue(x == Interval(1,2.99))
    x = Interval(2.99,3.01);     FloorOp.bwd(Interval(1,2),x);           self.assertTrue(x == Interval(2.99,3))
    x = Interval(3.01,3.99);     FloorOp.bwd(Interval(1,2),x);           self.assertTrue(x == Interval.empty())
    x = Interval(0.01,2.99);     FloorOp.bwd(Interval(0.01,2.99),x);     self.assertTrue(x == Interval(1,2.99))

    x = Interval(-oo,oo);        CeilOp.bwd(Interval.empty(),x);        self.assertTrue(x == Interval.empty())
    x = Interval(-oo,-0.000001); CeilOp.bwd(Interval(-oo,-1),x);        self.assertTrue(x == Interval(-oo,-1))
    x = Interval(-oo,0.000001);  CeilOp.bwd(Interval(-oo,-1),x);        self.assertTrue(x == Interval(-oo,-1))
    x = Interval(-oo,0.000001);  CeilOp.bwd(Interval(-oo,-0.000001),x); self.assertTrue(x == Interval(-oo,-1))
    x = Interval(0.000001,oo);   CeilOp.bwd(Interval(1,oo),x);          self.assertTrue(x == Interval(0.000001,oo))
    x = Interval(-0.000001,oo);  CeilOp.bwd(Interval(1,oo),x);          self.assertTrue(x == Interval(0,oo))
    x = Interval(-0.000001,oo);  CeilOp.bwd(Interval(0.000001,oo),x);   self.assertTrue(x == Interval(0,oo))
    x = Interval(0.01,2.99);     CeilOp.bwd(Interval(1,2),x);           self.assertTrue(x == Interval(0.01,2))
    x = Interval(-0.01,0.01);    CeilOp.bwd(Interval(1,2),x);           self.assertTrue(x == Interval(0,0.01))
    x = Interval(-1.01,-0.01);   CeilOp.bwd(Interval(1,2),x);           self.assertTrue(x == Interval.empty())
    x = Interval(0.01,2.99);     CeilOp.bwd(Interval(0.01,2.99),x);     self.assertTrue(x == Interval(0.01,2))
    
    x = Interval(-1.57079632679489678, 1.1780972450961728626)
    self.assertFalse(tan(x).is_empty())

  def tests_cross_prod(self):

    self.assertTrue(
      CrossProdOp.fwd(Vector([1,0,0]),Vector([0,1,0])) == Vector([0,0,1]))
    self.assertTrue(
      CrossProdOp.fwd(Vector([0,1,0]),Vector([0,0,1])) == Vector([1,0,0]))
    self.assertTrue(
      CrossProdOp.fwd(Vector([0,0,1]),Vector([1,0,0])) == Vector([0,1,0]))
    self.assertTrue(
      CrossProdOp.fwd(Vector([1,2,3]),Vector([2,4,6])) == Vector([0,0,0]))

  def tests_mat_operator(self):

    x1,x2,x3 = IntervalVector([2,3]), IntervalVector([4,5]), IntervalVector([6,7]) 
    self.assertTrue(MatrixOp.fwd(x1,x2,x3) == IntervalMatrix([[2,4,6],[3,5,7]]))
    self.assertTrue(MatrixOp.fwd(x1) == IntervalMatrix([[2],[3]]))
  
if __name__ ==  '__main__':
  unittest.main()