#!/usr/bin/env python




#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Damien Massé
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import sys, os
import unittest
import math
from codac import *

class TestLinearManual(unittest.TestCase):

  def tests_linear_manual(test):

    # [linear-1-beg]
    A = Matrix([
      [ 1, 2, 0 ],
      [ 3, 4, 1 ],
      [ 0, 1, 0 ],
    ])

    B = inverse_enclosure(A)
    # B == [[     <1, 1> ,    <-0, 0> ,        <-2, -2> ]
    #       [    <-0, 0> ,    <-0, 0> , [0.9999, 1.001] ]
    #       [   <-3, -3> ,     <1, 1> ,  [1.999, 2.001] ]]

    i = (A*B).contains(Matrix.eye(3,3))
    # i == True
    # [linear-1-end]

    test.assertTrue((A*B).contains(Matrix.eye(3,3)))
    test.assertTrue(i == True)

if __name__ ==  '__main__':
  unittest.main()