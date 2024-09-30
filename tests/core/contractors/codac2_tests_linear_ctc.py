#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *

class TestLinearCtc(unittest.TestCase):
  
  def tests_linear_ctc(self):
      
    A = IntervalMatrix([
      [[4,5],[-1,1],[1.5,2.5]],
      [[-0.5,0.5],[-7,-5],[1,2]],
      [[-1.5,-0.5],[-0.7,-0.5],[2,3]]
    ])
    x = IntervalVector([[-oo,oo],[-oo,oo],[-oo,oo]])
    b = IntervalVector([[3,4],[0,2],[3,4]])

    # Numerical values come from the book:
    #   Applied Interval Analysis
    #   Luc Jaulin, Michel Kieffer, Olivier Didrit, Éric Walter
    #   2001, Springer London
    #   doi: https:#doi.org/10.1007/978-1-4471-0249-6

    # CtcGaussElim

    ctc_ge = CtcGaussElim()
    ctc_ge.contract(A,x,b)
    self.assertTrue(Approx(x) == IntervalVector([
      [-1.819278874340436,1.168720384766974],
      [-0.4140698379116945,1.725229626734416],
      [0.7002320584443487,3.42075434825093]
    ]))

    # CtcGaussSeidel

    ctc_gs = CtcGaussSeidel()
    x = IntervalVector([[-10,10],[-10,10],[-10,10]])
    for k in range(0,100):
      ctc_gs.contract(A,x,b)
    self.assertTrue(Approx(x) == IntervalVector([
      [-2.084512144759555,1.63672286912456],
      [-0.7369829550424718,1.74356809144887],
      [-0.3213286433350618,3.837791484351055]
    ]))

    # CtcGaussSeidel with preconditioning

    ctc_gsp = CtcLinearPrecond(ctc_gs)
    x = IntervalVector([[-10,10],[-10,10],[-10,10]])
    for k in range(0,100):
      ctc_gsp.contract(A,x,b)
    self.assertTrue(Approx(x) == IntervalVector([
      [-1.106970597649814,1.383669626776028],
      [-0.8113846871807701,1.332001097010008],
      [0.5757415290046493,2.984085335831012]
    ]))

    # CtcGaussElim with preconditioning

    ctc_gep = CtcLinearPrecond(ctc_ge)
    x = IntervalVector([[-oo,oo],[-oo,oo],[-oo,oo]])
    ctc_gep.contract(A,x,b)
    self.assertTrue(Approx(x) == IntervalVector([
      [-1.106970597649814,1.383669626776028],
      [-0.7852408901116992,1.332001097010009],
      [0.7583501672635362,2.984085335831012]
    ]))

if __name__ ==  '__main__':
  unittest.main()