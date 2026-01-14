#!/usr/bin/env python
 
#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2025
#  \author     Damien Massé
#  \copyright  Copyright 2025 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestGaussJordan(unittest.TestCase):
   
    def test_GaussJordan(self):

       M = Matrix([
            [ 1, -4, 6, 7, 6 ],
            [ 2,  1, 3, 6, -2 ],
            [ 5,  2, 2, 9, -1 ] ])
       pre = gauss_jordan(M)
       # check that pre*M is "band matrix" */
       P = pre*M
       mx = 0.0
       for i in range(1,M.rows()-1):
          for j in range(0,i-1):
             mx = max(mx,abs(P(i,j)))
             mx = max(mx,abs(P(M.rows()-1-i,M.cols()-1-j)))
       self.assertTrue(mx<1e-10)


if __name__ ==  '__main__':
  unittest.main()

