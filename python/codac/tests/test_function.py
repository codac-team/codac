#!/usr/bin/env python

import unittest
from pyibex import Interval, IntervalVector
from codac import *
import codac as codac

class TestFunctionOnTube(unittest.TestCase):

    def assertApproxIntv(self, first, second):
        if first.is_empty() is False:
        # if isinstance(second, Interval):
            self.assertAlmostEqual(first.lb(), second.lb())
            self.assertAlmostEqual(first.ub(), second.ub())
        else:
            self.assertEqual(first, second)
    def test_Functions_1(self):

        x = TubeVector(Interval(0.,10.), 0.01, TFunction("sin(t)+[-0.01,0.01]"));
        f = TFunction("x", "t/10.+x");
        y1 = TubeVector(f.eval_vector(x));
        y2 = TubeVector(Interval(0.,10.), 0.01, TFunction("t/10.+sin(t)+[-0.01,0.01]"));
        self.assertAlmostEqual(y1.volume(), y2.volume());

    
if __name__ ==  '__main__':
  unittest.main()