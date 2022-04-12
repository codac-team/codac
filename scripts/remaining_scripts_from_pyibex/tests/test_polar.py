#!/usr/bin/env python
# ============================================================================
#                                P Y I B E X
# File        : test_polar.py
# Author      : Benoit Desrochers
# Copyright   : Benoit Desrochers
# License     : See the LICENSE file
# Created     : Dec 28, 2014
# ============================================================================
from pyibex import Interval, IntervalVector
from pyibex.geometry import SepPolarXY
from pyibex import Interval, IntervalVector
import unittest


def hex2Itv(lb, ub):
    if lb == "NAN" or ub == "NAN":
        return Interval.EMPTY_SET
    return Interval(float.fromhex(lb), float.fromhex(ub))


class TestPolar(unittest.TestCase):
    def test_Polar01(self):
        self.assertTrue(True)

    def test_sepPolar(self):
        sep = SepPolarXY(Interval(1, 2), Interval(0.1, 2.1))

    def test_SepPolarXY_01(self):
        x = hex2Itv('-0x1.9032ae28d8664p+4', '-0x1.9032ae28d8664p+4')
        y = hex2Itv('-0x1.68d6cf2e8c8b8p+0', '-0x1.68d6cf2e8c8b8p+0')
        rho = Interval(25.0607, 25.1231)
        theta = Interval(-3.14159, -2.79253)
        S = SepPolarXY(rho, theta)
        xin, xout = IntervalVector([x, y]), IntervalVector([x, y])
        S.separate(xin, xout)
        self.assertTrue(xout.is_empty())
        self.assertEqual(xin, IntervalVector([x, y]))

    def test_SepPolarXY_02(self):
        rho = hex2Itv('0x1.b9adea897635fp+2', '0x1.c01450efdc9c5p+2')
        theta = hex2Itv('-0x1.95b92733b853ep+2', '-0x1.690af69de9cacp+2')
        S = SepPolarXY(rho, theta)
        X = IntervalVector(2)
        X[0] = hex2Itv('0x1.be002da3cfcc6p+2', '0x1.be002da3cfcc6p+2')
        X[1] = hex2Itv('-0x1.32b8f5d87284bp-2', '-0x1.32b8f5d87284bp-2')
        xin, xout = X.copy(), X.copy()
        S.separate(xin, xout)

        self.assertFalse(xin.is_empty() and xout.is_empty())


if __name__ == '__main__':

    unittest.main()
