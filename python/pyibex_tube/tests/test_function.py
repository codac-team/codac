#!/usr/bin/env python

import unittest
import pyibex
from pyibex_tube import *
import pyibex_tube as tubex
import numpy as np

class TestFunctionOnTube(unittest.TestCase):

    def assertApproxIntv(self, first, second):
        if first.is_empty() is False:
        # if isinstance(second, Interval):
            self.assertAlmostEqual(first.lb(), second.lb())
            self.assertAlmostEqual(first.ub(), second.ub())
        else:
            self.assertEqual(first, second)
    def test_Functions_1(self):

        x = TubeVector(Interval(0.,10.), 0.01, tubex.Function("sin(t)+[-0.01,0.01]"));
        f = tubex.Function("x", "t/10.+x");
        y1 = TubeVector(f.eval_vector(x));
        y2 = TubeVector(Interval(0.,10.), 0.01, tubex.Function("t/10.+sin(t)+[-0.01,0.01]"));
        self.assertAlmostEqual(y1.volume(), y2.volume());

    