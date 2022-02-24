#!/usr/bin/env python
#============================================================================
#                                P Y I B E X
# File        : test_inheritance.py
# Author      : Benoit Desrochers
# Copyright   : Benoit Desrochers
# License     : See the LICENSE file
# Created     : Dec 28, 2014
#============================================================================

import unittest
import pyibex
from pyibex import *

class myCtc(pyibex.Ctc):
    def __init__(self, x=0, y=0, R=2):
        pyibex.Ctc.__init__(self, 2)
        self.f = Function('x', 'y', '(x-%f)^2+(y-%f)^2-%f'%(x,y,R**2))
        self.ctc = CtcFwdBwd(self.f, CmpOp.LEQ)

    def contract(self, box):
        # tmp = IntervalVector(box)
        self.ctc.contract(box)
        # box.assign(tmp)

class mySep(pyibex.Sep):
    def __init__(self, x=0, y=0, R=2):
        pyibex.Sep.__init__(self, 2)
        self.f = Function('x', 'y', '(x-%f)^2+(y-%f)^2-%f'%(x,y,R**2))
        self.sep = SepFwdBwd(self.f, CmpOp.LEQ)

    def separate(self, xin, xout):
        # tmp_in= IntervalVector(xin)
        # tmp_out = IntervalVector(xout)
        self.sep.separate(xin, xout)
        # xin.assign(tmp_in)
        # xout.assign(tmp_out)

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
