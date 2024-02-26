#!/usr/bin/env python

# Codac tests
# ---------------------------------------------------------------------------
# \date      2023
# \author    Simon Rohou
# \copyright Copyright 2023 Codac Team
# \license   This program is distributed under the terms of
#            the GNU Lesser General Public License (LGPL).

import unittest
import numpy as np
from codac import *

class TestNumpyMatrices(unittest.TestCase):

  def test_numpytocodac(self):

    cdA=IntervalMatrix(2,6)
    cdA[0][0]=Interval(1)
    cdA[0][1]=Interval(3)
    cdA[0][2]=Interval(5)
    cdA[0][3]=Interval(7)
    cdA[0][4]=Interval(9)
    cdA[0][5]=Interval(11)
    cdA[1][0]=Interval(2)
    cdA[1][1]=Interval(4)
    cdA[1][2]=Interval(6)
    cdA[1][3]=Interval(8)
    cdA[1][4]=Interval(10)
    cdA[1][5]=Interval(12)

    npA=np.array([[1.,3.,5.,7.,9.,11.],[2.,4.,6.,8.,10.,12.]])
    self.assertEqual(IntervalMatrix(npA), cdA)

  def test_numpytocodac_withtranspose(self):

    cdA=IntervalMatrix(2,6)
    cdA[0][0]=Interval(1)
    cdA[0][1]=Interval(3)
    cdA[0][2]=Interval(5)
    cdA[0][3]=Interval(7)
    cdA[0][4]=Interval(9)
    cdA[0][5]=Interval(11)
    cdA[1][0]=Interval(2)
    cdA[1][1]=Interval(4)
    cdA[1][2]=Interval(6)
    cdA[1][3]=Interval(8)
    cdA[1][4]=Interval(10)
    cdA[1][5]=Interval(12)

    npA=np.array([[1.,2.],[3.,4.],[5.,6.],[7.,8.],[9.,10.],[11.,12.]]).T
    self.assertEqual(IntervalMatrix(npA), cdA)
        
if __name__ ==  '__main__':
  unittest.main()