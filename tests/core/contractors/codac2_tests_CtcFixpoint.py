#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *


class CtcCustom(Ctc):

  def __init__(self):
    Ctc.__init__(self, 2)

  def contract(self, x):
    if x[0].rad() > 1.:
      return IntervalVector(x.mid()).inflate(0.5*x.rad())
    return x


class TestCtcFixpoint(unittest.TestCase):
  
  def tests_CtcFixpoint(self):

    ctc_custom = CtcCustom()
    x = IntervalVector([[-1000,1000],[-1000,1000]])
    x = ctc_custom.contract(x)
    self.assertTrue(Approx(x) == IntervalVector([[-500,500],[-500,500]]))
    x = ctc_custom.contract(x)
    self.assertTrue(Approx(x) == IntervalVector([[-250,250],[-250,250]]))
    x = ctc_custom.contract(x)
    self.assertTrue(Approx(x) == IntervalVector([[-125,125],[-125,125]]))

    ctc_fixed = CtcFixpoint(ctc_custom, 0.)
    
    x = IntervalVector([[-1000,1000],[-1000,1000]])
    ctc_fixed.contract(x)
    self.assertTrue(Approx(x,1e-1) == IntervalVector([[-1,1],[-1,1]]))
    x = IntervalVector([[-500,500],[-500,500]])
    ctc_fixed.contract(x)
    self.assertTrue(Approx(x,1e-1) == IntervalVector([[-1,1],[-1,1]]))

    ctc_fixed = CtcFixpoint(ctc_custom, 0.8)
    
    x = IntervalVector([[-1000,1000],[-1000,1000]])
    ctc_fixed.contract(x)
    self.assertTrue(Approx(x) == IntervalVector([[-500,500],[-500,500]]))
    ctc_fixed.contract(x)
    self.assertTrue(Approx(x) == IntervalVector([[-250,250],[-250,250]]))

    ctc_fixed = CtcFixpoint(ctc_custom, 1.)
    
    x = IntervalVector([[-1000,1000],[-1000,1000]])
    ctc_fixed.contract(x)
    self.assertTrue(Approx(x) == IntervalVector([[-500,500],[-500,500]]))
    ctc_fixed.contract(x)
    self.assertTrue(Approx(x) == IntervalVector([[-250,250],[-250,250]]))


if __name__ ==  '__main__':
  unittest.main()