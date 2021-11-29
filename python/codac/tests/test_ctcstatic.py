#!/usr/bin/env python

import unittest
from pyibex import Interval, IntervalVector
from codac import *

class TestCtcStatic(unittest.TestCase):

    def test_ctcstatic_1(self):

        dt = 0.1
        tdomain = Interval(0.,10.)

        ctc_f = CtcFunction(Function("t", "x[2]", "(x[0]-t;x[1]-t)"))
        ctc_static = CtcStatic(ctc_f, True)

        x = TubeVector(tdomain, dt, 2)

        cn = ContractorNetwork()
        cn.add(ctc_static, [x])
        cn.contract()

        expected = Tube(tdomain, dt, TFunction("t"))
        self.assertEqual(x[0], expected)
        self.assertEqual(x[1], expected)

if __name__ ==  '__main__':
  unittest.main()