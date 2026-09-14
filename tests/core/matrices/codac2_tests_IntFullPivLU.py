#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Damien Massé
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)


import unittest
from codac import *

class TestIntvFullPivLU(unittest.TestCase):

  def test_IntvFullPivLU_1(self):
    M = Matrix([
    [ 1, -4, 3, 7 ],
    [ 2, 1, -4, 6 ],
    [ 5, 2, 1 , 9 ],
    [ -1, 0, 3, 2 ]
    ])

    LUdec = IntvFullPivLU(M)
    self.assertTrue(LUdec.is_injective()==BoolInterval.TRUE)
    self.assertTrue(LUdec.is_surjective()==BoolInterval.TRUE)
    self.assertTrue(LUdec.rank()==Interval(4))
    self.assertTrue((LUdec.determinant()+602).mag()<=1e-10)
    self.assertTrue((LUdec.reconstructed_matrix()-M).norm().ub()<=1e-10)
    I1 = LUdec.solve(IntervalMatrix.eye(4,4));
    self.assertTrue((I1*M-Matrix.eye(4,4)).norm().ub()<1e-10)

  def test_IntvFullPivLU_2(self):
    M = Matrix([
    [ 1, -4, 6, 7 ],
    [ 2, 1, 3, 6 ],
    [ 5, 2, 8 , 9 ],
    [ -1, 0, -2, 2 ]
    ])

    LUdec = IntvFullPivLU(M)
    self.assertTrue(LUdec.is_injective()==BoolInterval.UNKNOWN)
    self.assertTrue(LUdec.is_surjective()==BoolInterval.UNKNOWN)
    self.assertTrue(LUdec.rank()==Interval([3,4]))
    self.assertTrue((LUdec.determinant()).mag()<=1e-10)
    self.assertTrue((LUdec.reconstructed_matrix()-M).norm().ub()<=1e-10)
    K = LUdec.kernel()
    self.assertTrue(K.cols()==1)
    self.assertTrue((M*K).norm().ub()<1e-10)
    coK = LUdec.cokernel()
    self.assertTrue(coK.rows()==1)
    self.assertTrue((coK*M).norm().ub()<1e-10)
    Im = LUdec.image(M)
    self.assertTrue(Im.cols()==3)
    coIm = LUdec.coimage(M)
    self.assertTrue(coIm.rows()==3)

  def test_IntvFullPivLU_3(self):
    M = Matrix([
    [ 1, -4, 6, 7, 6 ],
    [ 2, 1, 3, 6, -2 ],
    [ 5, 2, 8 , 9, -1 ]
    ])
    LUdec = IntvFullPivLU(M)
    self.assertTrue(LUdec.is_injective()==BoolInterval.FALSE)
    self.assertTrue(LUdec.is_surjective()==BoolInterval.TRUE)
    self.assertTrue(LUdec.rank()==Interval(3))
    self.assertTrue((LUdec.reconstructed_matrix()-M).norm().ub()<=1e-10)
    K = LUdec.kernel()
    self.assertTrue(K.cols()==2)
    self.assertTrue((M*K).norm().ub()<1e-10)
    Im = LUdec.image(M)
    self.assertTrue(Im.cols()==3)
    I1 = LUdec.solve(IntervalMatrix.eye(3,3));
    self.assertTrue((M*I1-Matrix.eye(3,3)).norm().ub()<1e-10)
    A = IntervalMatrix([ [1], [[-20,20]], [2], [[-20,20]], [[-20,20]] ])
    B = IntervalMatrix([ [2.0], [1.0], [4.0] ])
    LUdec.solve(B,A)
    self.assertTrue((M*A-B).norm().ub()<=1e-10)

  def test_IntvFullPivLU_solve_tall_matrix(self):

    A = Matrix([
     [ 1, 0 ],
     [ 0, 1 ],
     [ 1, 1 ],
     [ 2,-1 ],
     [-1, 2 ],
    ])
 
    X = Matrix([
     [ 2,-1 ],
     [-1, 3 ],
    ])
 
    rhs_mid = A*X
    rhs = IntervalMatrix(rhs_mid)
 
    lu = IntvFullPivLU(A)
 
    sol = lu.solve(rhs)
 
    self.assertTrue(not sol.is_empty())
    self.assertTrue(sol.contains(X))
    self.assertTrue((A*sol).contains(rhs_mid))
 
  def test_IntvFullPivLU_solve_tall_matrix_with_box(self):

    A = Matrix([
     [ 1, 0 ],
     [ 0, 1 ],
     [ 1, 1 ],
     [ 2,-1 ],
     [-1, 2 ],
    ])
 
    X = Matrix([
     [ 2,-1 ],
     [-1, 3 ],
    ])
 
    rhs_mid = A*X
    rhs = IntervalMatrix(rhs_mid)
 
    lu = IntvFullPivLU(A)
    B = IntervalMatrix.constant(2,2,[-10,10])
 
    lu.solve(rhs, B)
 
    self.assertTrue(not B.is_empty())
    self.assertTrue(B.contains(X))
    self.assertTrue((A*B).contains(rhs_mid))

  def test_IntvFullPivLU_solve_tall_matrix_inconsistent_rhs(self):

    A = Matrix([
     [ 1, 0 ],
     [ 0, 1 ],
     [ 1, 1 ],
     [ 2,-1 ],
     [-1, 2 ],
    ])
 
    rhs_bad = IntervalMatrix([
      [[ 2, 2],[-1,-1]],
      [[-1,-1],[ 3, 3]],
      [[ 1, 1],[ 2, 2]],
      [[ 5, 5],[-5,-5]],
      [[-3,-3],[ 7, 7]], # should be [-4, 7] for consistency
    ])
 
    lu = IntvFullPivLU(A)
 
    sol = lu.solve(rhs_bad)
    self.assertTrue(sol.is_empty())
 
    B = IntervalMatrix.constant(2,2,[-10,10])
    lu.solve(rhs_bad, B)
    self.assertTrue(B.is_empty())
    
if __name__ ==  '__main__':
  unittest.main()