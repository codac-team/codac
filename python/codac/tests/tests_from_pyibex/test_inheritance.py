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

class myCtc(Ctc):
  def __init__(self, x=0, y=0, R=2):
    Ctc.__init__(self, 2)
    self.f = Function('x', 'y', '(x-%f)^2+(y-%f)^2-%f'%(x,y,R**2))
    self.ctc = CtcFwdBwd(self.f, CmpOp.LEQ)

  def contract(self, box):
    self.ctc.contract(box)
    return box

class mySep(Sep):
  def __init__(self, x=0, y=0, R=2):
    Sep.__init__(self, 2)
    self.f = Function('x', 'y', '(x-%f)^2+(y-%f)^2-%f'%(x,y,R**2))
    self.sep = SepFwdBwd(self.f, CmpOp.LEQ)

  def separate(self, xin, xout):
    self.sep.separate(xin, xout)
    return xin,xout

class TestInheritance(unittest.TestCase):

  def test_myCtc(self):
    ctc1 = myCtc()
    ctc2 = myCtc(1,1)
    ctc = CtcUnion([ctc1, ctc2])
    x = IntervalVector(2, [-10,10])
    ctc.contract(x)
    # SIVIA(x, ctc, 1)

  def test_mySep(self):

    sep1 = mySep()
    sep2 = mySep(1,1)
    sep = SepUnion([sep1, sep2])
    x = IntervalVector(2, [-10,10])
    xin, xout = map(IntervalVector, [x, x])
    sep.separate(xin, xout)
    # SIVIA(x, sep, 1)

if __name__ == '__main__':

  unittest.main()