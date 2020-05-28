#!/usr/bin/env python

import unittest
from pyibex import Interval, IntervalVector
from tubex_lib import *
import tubex_lib as tubex

class TestFunctionOnCN(unittest.TestCase):

    # def assertApproxIntv(self, first, second):
    #     if first.is_empty() is False:
    #     # if isinstance(second, Interval):
    #         self.assertAlmostEqual(first.lb(), second.lb())
    #         self.assertAlmostEqual(first.ub(), second.ub())
    #     else:
    #         self.assertEqual(first, second)

    def test_CN_1(self):
        ctc_plus = CtcFunction("a", "b", "c", "a+b-c")
        cn = ContractorNetwork()

        a = Interval(0,1)
        b = Interval(-1,1)
        # c = Interval(1.5,2)
        c = cn.create_dom(Interval(1.5,2))
        print(f"[python] a = {a}")
        print(f"[python] b = {b}")
        print(f"[python] c = {c}")
        cn.add(ctc_plus, [a, b, c])
        cn.contract()
        self.assertEqual(a, Interval(0.5,1))
        self.assertEqual(b, Interval(0.5,1))
        self.assertEqual(c, Interval(1.5,2))

        print(f"[python] a = {a}")
        print(f"[python] b = {b}")
        print(f"[python] c = {c}")
        # a == [0.5,1]
        # b == [0.5,1]
        # c == [1.5,2]
        # x = TubeVector(Interval(0.,10.), 0.01, TFunction("sin(t)+[-0.01,0.01]"));
        # f = TFunction("x", "t/10.+x");
        # y1 = TubeVector(f.eval_vector(x));
        # y2 = TubeVector(Interval(0.,10.), 0.01, TFunction("t/10.+sin(t)+[-0.01,0.01]"));
        # self.assertAlmostEqual(y1.volume(), y2.volume());

if __name__ ==  '__main__':
  unittest.main()