#!/usr/bin/env python

import unittest
import math
from pyibex import Interval, IntervalVector
from codac import *

class TestCtcDelay(unittest.TestCase):

    def assertApproxIntv(self, first, second):
        if first.is_empty() is False:
        # if isinstance(second, Interval):
            self.assertAlmostEqual(first.lb(), second.lb())
            self.assertAlmostEqual(first.ub(), second.ub())
        else:
            self.assertEqual(first, second)

    def test_ctcdelay_1(self):
        tdomain = Interval(0.,10.)
        x = Tube(tdomain, 0.01, TFunction("cos(t)"))
        y = Tube(tdomain, 0.01)

        pi = Interval.PI
        ctc_delay = CtcDelay()
        ctc_delay.contract(pi, x, y)

        beginDrawing()
        fig_x = VIBesFigTube("delay x", x)
        fig_x.show()
        fig_y = VIBesFigTube("delay y", y)
        fig_y.show()
        endDrawing()

        self.assertEqual(y(Interval(0.,math.pi)), Interval.ALL_REALS)
        self.assertApproxIntv(y(Interval(math.pi+0.01,10.)), Interval(-1.,1.))
        self.assertTrue(y(5.).is_superset(x(5. - math.pi)))

    def test_ctcdelay_2(self):
        tdomain = Interval(0.,10.)
        dt = 0.01
        x = Tube(tdomain, dt, TFunction("cos(t)"))
        y = Tube(tdomain, dt, TFunction("sin(t)"))

        delay = Interval(0.,2.*math.pi)
        ctc_delay = CtcDelay()
        ctc_delay.contract(delay, x, y)

        beginDrawing()
        fig_x = VIBesFigTube("delay x", x)
        fig_x.show()
        fig_y = VIBesFigTube("delay y", y)
        fig_y.show()
        endDrawing()

        self.assertTrue(delay.contains(math.pi/2.))
        self.assertTrue(delay.diam() < 3.*dt)

if __name__ ==  '__main__':
  unittest.main()