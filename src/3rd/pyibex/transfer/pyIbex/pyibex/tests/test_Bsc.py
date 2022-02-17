#!/usr/bin/env python
#============================================================================
#                                P Y I B E X
# File        : test_Bsc.py
# Author      : Benoit Desrochers
# Copyright   : Benoit Desrochers
# License     : See the LICENSE file
# Created     : Dec 28, 2014
#============================================================================

import unittest
import pyibex
from pyibex import *

class TestBsc(unittest.TestCase):

    def assertApproxItvVec(self, first, second):
        for i in range(len(first)):
            self.assertAlmostEqual(first[i][0], second[i][0])
            self.assertAlmostEqual(first[i][1], second[i][1])

    def test_LargestFirst(self):
        bsc = LargestFirst(0.1)
        a = IntervalVector([[1,2], [2,6]])
        (c,b) = bsc.bisect(a)
        self.assertApproxItvVec(c, IntervalVector([[1,2],[2,3.8]]))
        self.assertApproxItvVec(b, IntervalVector([[1,2],[3.8,6]]))
        self.assertTrue(True)

    def test_LargestFirst2(self):
        bsc = LargestFirst([0.1, 1])
        a = IntervalVector([[1,2], [2,6]])
        (c,b) = bsc.bisect(a)
        self.assertApproxItvVec(c, IntervalVector([[1,1.45],[2,6]]))
        self.assertApproxItvVec(b, IntervalVector([[1.45,2],[2,6]]))
        self.assertTrue(True)


if __name__ == '__main__':

    unittest.main()
