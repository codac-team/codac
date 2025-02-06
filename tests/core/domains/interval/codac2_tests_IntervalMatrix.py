#!/usr/bin/env python

#  Codac tests
#  
#  Most of these tests come from the IBEX library (Gilles Chabert)
#  See more: https://ibex-lib.readthedocs.io
#  They have been revised to fit with Codac (v2)
#
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Gilles Chabert, Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
from codac import *
import sys
import math

class TestIntervalMatrix(unittest.TestCase):

  def M1(self):
    return IntervalMatrix([
      [[0,1],[0,2],[0,3]],
      [[-1,0],[-2,0],[-3,0]]
    ])

  def M2(self): # the transpose of M1
    return IntervalMatrix([
      [[0,1],[-1,0]],
      [[0,2],[-2,0]],
      [[0,3],[-3,0]]
    ])

  def M3(self): # non-null intersection with M1
    return IntervalMatrix([
      [[1,2],[1,2],[2,4]],
      [[-2,-1],[-2,-1],[-4,-2]]
    ])

  def tests_0_intervalmatrix(self):

    x = IntervalMatrix([
      [ [-1,1], [-2,2] ],
      [ [-3,3], [-4,4] ],
    ])

    x[0,1] = Interval()
    x[1,1] = Interval()

    self.assertTrue(x == IntervalMatrix([
      [ [-1,1], [-oo,oo] ],
      [ [-3,3], [-oo,oo] ],
    ]))

    #m = IntervalMatrix(x)
    
  def tests_intervalmatrix(self):

    m1 = IntervalMatrix(2,3)
    m2 = IntervalMatrix(3,2)
    self.assertTrue(m1 != m2)
    self.assertTrue(not (m1 == m2))

    m1 = IntervalMatrix(3,2)
    m2 = IntervalMatrix(2,2)
    self.assertTrue(m1 != m2)
    self.assertTrue(not (m1 == m2))

    m1 = IntervalMatrix(2,3)
    m2 = IntervalMatrix(2,3)

    self.assertTrue(m1.rows() == 2)
    self.assertTrue(m1.cols() == 3)
    self.assertTrue(m2.rows() == 2)
    self.assertTrue(m2.cols() == 3)

    m1[0,0] = 1.
    m1[0,1] = 2.
    m1[0,2] = 3.
    m1[1,0] = 4.
    m1[1,1] = 5.
    m1[1,2] = 6.
    m2[0,0] = 1.
    m2[0,1] = 2.
    m2[0,2] = 3.
    m2[1,0] = 4.
    m2[1,1] = 5.
    m2[1,2] = 6.

    self.assertTrue(m1 == m2)
    self.assertTrue(not (m1 != m2))

    m2[1,2] = 7.
    self.assertTrue(m1 != m2)
    self.assertTrue(not (m1 == m2))

    m1 = IntervalMatrix(2,3)
    m2 = IntervalMatrix(2,3)
    m1[1,1] = -1
    m2[1,1] = -2
    self.assertTrue(m1 != m2)
    self.assertTrue(not (m1 == m2))
    m1.set_empty()
    m2.set_empty()
    self.assertTrue(m1 == m2)
    self.assertTrue(not (m1 != m2))

    m = IntervalMatrix(2,3)
    self.assertTrue(m.rows() == 2)
    self.assertTrue(m.cols() == 3)
    self.assertTrue(m(0,0) == Interval(-oo,oo))
    self.assertTrue(m(0,1) == Interval(-oo,oo))
    self.assertTrue(m(0,2) == Interval(-oo,oo))
    self.assertTrue(m(1,0) == Interval(-oo,oo))
    self.assertTrue(m(1,1) == Interval(-oo,oo))
    self.assertTrue(m(1,2) == Interval(-oo,oo))

    self.assertTrue(m == IntervalMatrix(m))
    #self.assertTrue(m == (IntervalMatrix(2,3)=m))

    m1 = IntervalMatrix.zero(2,3)
    r1_0 = IntervalVector([[0,1],[0,2],[0,3]])
    self.assertTrue(r1_0.cols() == 1)
    self.assertTrue(r1_0.rows() == 3)
    r1 = IntervalVector([[0,1],[0,2],[0,3]]).transpose()
    self.assertTrue(r1.cols() == 3)
    self.assertTrue(r1.rows() == 1)
    r2 = IntervalVector([[-1,0],[-2,0],[-3,0]]).transpose()
    self.assertTrue(r2.cols() == 3)
    self.assertTrue(r2.rows() == 1)
    self.assertTrue(r1 == IntervalMatrix([
      [ [0,1], [0,2], [0,3] ]
    ]))

    m1.set_row(0,r1)
    m1.set_row(1,r2)

    c1 = IntervalVector([[0,1],[-1,0]])
    c2 = IntervalVector([[0,2],[-2,0]])
    c3 = IntervalVector([[0,3],[-3,0]])

    self.assertTrue(m1.rows() == 2)
    self.assertTrue(m1.cols() == 3)

    a1 = IntervalRow(r1)
    a2 = IntervalRow(m1.row(0))
    self.assertTrue(a1 == a2)

    self.assertTrue((m1.row(0) == r1))
    self.assertTrue((m1.row(1) == r2))
    self.assertTrue((m1.row(0) == r1))
    self.assertTrue((m1.row(1) == r2))

    self.assertTrue((m1.col(0) == c1))
    self.assertTrue((m1.col(1) == c2))
    self.assertTrue((m1.col(2) == c3))
    self.assertTrue(m1(0,0) == Interval(0,1))
    self.assertTrue(m1(0,1) == Interval(0,2))
    self.assertTrue(m1(0,2) == Interval(0,3))
    self.assertTrue(m1(1,0) == Interval(-1,0))
    self.assertTrue(m1(1,1) == Interval(-2,0))
    self.assertTrue(m1(1,2) == Interval(-3,0))

    self.assertTrue(m1 == IntervalMatrix(m1))
    #self.assertTrue(m1 == (IntervalMatrix(2,3)=m1))

    x = Interval(-1,2)
    m = IntervalMatrix.constant(2,3,x)

    self.assertTrue(m.rows() == 2)
    self.assertTrue(m.cols() == 3)
    for i in range (0,2):
      for j in range (0,3):
        self.assertTrue(m(i,j) == x)

    self.assertTrue(m == IntervalMatrix(m))
    #self.assertTrue(m == (IntervalMatrix(2,3)=m))

    #double _m[][2] = [
    #  [0,1], [0,2], [0,3],
    #  [-1,0],[-2,0],[-3,0]
    #]
    #m = IntervalMatrix(2,3,_m)
    #self.assertTrue(m == self.M1())

    m = IntervalMatrix([
      [[0,1], [0,2], [0,3]],
      [[-1,0],[-2,0],[-3,0]]
    ])
    self.assertTrue(m == self.M1())

    m = IntervalMatrix(2,3)
    # accessor (row,col)
    m[0,0] = Interval(0,1)
    m[0,1] = Interval(0,2)
    m[0,2] = Interval(0,3)
    m[1,0] = Interval(-1,0)
    m[1,1] = Interval(-2,0)
    m[1,2] = Interval(-3,0)
    self.assertTrue(m == self.M1())

    self.assertTrue(IntervalMatrix.empty(2,3).rows() == 2)
    self.assertTrue(IntervalMatrix.empty(2,3).cols() == 3)

    self.assertTrue(IntervalMatrix(IntervalMatrix.empty(2,3)) == IntervalMatrix.empty(2,3))
    #self.assertTrue((IntervalMatrix(2,3)=IntervalMatrix.empty(2,3)) == IntervalMatrix.empty(2,3))

    self.assertTrue(not IntervalMatrix(2,3).is_empty())
    self.assertTrue(IntervalMatrix.empty(2,3).is_empty())

    m = IntervalMatrix(2,3)
    m.set_empty()
    self.assertTrue(m.is_empty())

    # Intersection of a matrix with itself
    m1 = self.M1()
    m1 &= self.M1()
    self.assertTrue(m1 == self.M1())

    # Intersection of two overlapping matrices
    m = IntervalMatrix([
      [[1,1],  [1,2],  [2,3]],
      [[-1,-1],[-2,-1],[-3,-2]]
    ])

    m1 = self.M1()
    m1 &= self.M3()
    self.assertTrue(m1 == m)

    # Intersection of two non-overlapping matrices
    m3 = IntervalMatrix(self.M3())
    m3[1,2] = Interval(-5,-4)
    m1 = IntervalMatrix(self.M1())
    m1 &= m3
    self.assertTrue(m1.is_empty())

    m1 = IntervalMatrix(self.M1())

    v = IntervalVector(2)
    v[0] = Interval(1,2)
    v[1] = Interval(-2,-1)

    m1.set_col(1,v)

    m2 = IntervalMatrix([
      [[0,1], [1,2], [0,3]],
      [[-1,0],[-2,-1],[-3,0]]
    ])
    self.assertTrue(m1 == m2)

    self.assertTrue(self.M1().block(0,0,2,3) == self.M1())
    self.assertTrue(self.M1().block(0,0,1,3) == IntervalMatrix([[[0,1],[0,2],[0,3]]]))
    self.assertTrue(self.M1().block(1,0,1,3) == IntervalMatrix([[[-1,0],[-2,0],[-3,0]]]))
    self.assertTrue(self.M1().block(0,0,2,1) == IntervalMatrix([[[0,1]],[[-1,0]]]))
    self.assertTrue(self.M1().block(0,1,2,1) == IntervalMatrix([[[0,2]],[[-2,0]]]))
    self.assertTrue(self.M1().block(0,2,2,1) == IntervalMatrix([[[0,3]],[[-3,0]]]))
    self.assertTrue(self.M1().block(0,1,2,2) == IntervalMatrix([[[0,2],[0,3]],[[-2,0],[-3,0]]]))

    m1 = IntervalMatrix(2,2)
    r1 = IntervalVector([[0,1],[0,2]])
    r2 = IntervalVector([[-1,0],[-2,0]])
    m1.set_row(0,r1.transpose())
    m1.set_row(1,r2.transpose())
    m1.resize_save_values(2,3)
    m1[0,2] = Interval(0,3)
    m1[1,2] = Interval(-3,0)

    self.assertTrue(m1 == self.M1())

    m1 = IntervalMatrix(1,3)
    r1 = IntervalVector([[0,1],[0,2],[0,3]])
    m1.set_row(0,r1.transpose())
    m1.resize_save_values(2,3)
    m1[1,0] = Interval(-1,0)
    m1[1,1] = Interval(-2,0)
    m1[1,2] = Interval(-3,0)

    self.assertTrue(m1 == self.M1())

    e = IntervalMatrix.empty(1,1)
    e.resize_save_values(2,3)
    self.assertTrue(e.is_empty())

    m1 = IntervalMatrix(self.M1())
    m2 = IntervalMatrix(-m1)
    for i in range (0,2):
      for j in range(0,3):
        self.assertTrue(m2(i,j) == -m1(i,j))

    self.assertTrue((-IntervalMatrix.empty(2,3)).is_empty())

    m1 = IntervalMatrix(self.M1())
    m2 = IntervalMatrix(m1+m1)

    for i in range (0,2):
      for j in range(0,3):
        self.assertTrue(m2(i,j) == m1(i,j)+m1(i,j))

    m0 = IntervalMatrix(m1)
    m0 += m1
    self.assertTrue(m2 == m0)

    m1 = IntervalMatrix.empty(2,3)
    m2 = IntervalMatrix(2,3)

    self.assertTrue((m1+m2).is_empty())
    m1+=m2
    self.assertTrue(m1.is_empty())
    m2+=m1
    self.assertTrue(m2.is_empty())

    m1 = IntervalMatrix(self.M1())
    m2 = IntervalMatrix(m1-m1)
    for i in range (0,2):
      for j in range(0,3):
        self.assertTrue(m2(i,j) == m1(i,j)-m1(i,j))

    m0 = IntervalMatrix(m1)
    m0 -= m1
    self.assertTrue(m2 == m0)

    m1 = IntervalMatrix.empty(2,3)
    m2 = IntervalMatrix(2,3)

    self.assertTrue((m1-m2).is_empty())
    m1-=m2
    self.assertTrue(m1.is_empty())
    m2-=m1
    self.assertTrue(m2.is_empty())

    m1 = IntervalMatrix(self.M1())
    m2 = IntervalMatrix(self.M2())
    m3 = IntervalMatrix(m1*m2)
    self.assertTrue(m3.rows() == 2)
    self.assertTrue(m3.cols() == 2)

    for i in range (0,2):
      for j in range(0,2):
        self.assertTrue(m3(i,j) == m1(i,0)*m2(0,j)+m1(i,1)*m2(1,j)+m1(i,2)*m2(2,j))

    # Not supported by Eigen: self.assertTrue(m3 == (IntervalMatrix(m1)*=m2))

    m1 = IntervalMatrix.empty(2,3)
    m2 = IntervalMatrix(3,2)

    self.assertTrue((m1*m2).is_empty())
    # Not supported by Eigen: self.assertTrue((m1*=m2).is_empty())
    # Not supported by Eigen: self.assertTrue((m2*=m1).is_empty())

    M1 = IntervalMatrix(Matrix.eye(2,2))
    self.assertTrue(M1 == IntervalMatrix([
      [ [1,1],[0,0] ],
      [ [0,0],[1,1] ]
    ]))

    M2 = IntervalMatrix(2.*Matrix.eye(2,2))
    self.assertTrue(M2 == IntervalMatrix([
      [ [2,2],[0,0] ],
      [ [0,0],[2,2] ]
    ]))

    M3_degenerate = Matrix(-1.*Matrix.eye(2,2))
    self.assertTrue(M3_degenerate == Matrix([
      [ -1,0 ],
      [ 0,-1 ]
    ]))

    M3_Matrix = IntervalMatrix(Interval(-1,1)*Matrix.eye(2,2))
    self.assertTrue(M3_Matrix == IntervalMatrix([
      [ [-1,1],[0,0] ],
      [ [0,0],[-1,1] ]
    ]))

    M3_IntervalMatrix = IntervalMatrix(Interval(-1,1)*IntervalMatrix.eye(2,2))
    self.assertTrue(M3_IntervalMatrix == IntervalMatrix([
      [ [-1,1],[0,0] ],
      [ [0,0],[-1,1] ]
    ]))

    M1 = IntervalMatrix(2.*Matrix.eye(3,3))
    V1 = IntervalVector(3)
    V1[0] = 3
    V1[1] = 4
    V1[2] = 5
    res = IntervalMatrix(4,4)
    res.set_block(0,0,3,3,M1)
    res.set_block(0,3,3,1,V1)
    res.set_block(3,0,1,3,IntervalVector.ones(3).transpose())
    res[3,3] = 6

    self.assertTrue((res == Matrix([
      [2,0,0,3],
      [0,2,0,4],
      [0,0,2,5],
      [1,1,1,6] 
    ])))

    m1 = Matrix([
      [ 0,2 ],
      [ 3,10 ]
    ])
    m2 = Matrix([
      [ -1,7 ],
      [ 8,4 ]
    ])

    self.assertTrue((IntervalMatrix(m1) | m2) == IntervalMatrix([
      [ [-1,0],[2,7] ],
      [ [3,8],[4,10] ]
    ]))

    m1 = IntervalMatrix([
      [ [0,1],[0,2] ],
      [ [0,0],Interval.empty() ]
    ])
    m2 = IntervalMatrix([
      [ [-oo,oo],[-1,3] ],
      [ [2,4],[1,1] ]
    ])

    self.assertTrue((m1 | m2) == IntervalMatrix([
      [ [-oo,oo],[-1,3] ],
      [ [0,4],[1,1] ]
    ]))

    m1 = IntervalMatrix([
      [ [0,1],[0,2] ],
      [ [0,0],Interval.empty() ]
    ])
    m2 = IntervalMatrix([
      [ [-oo,oo],[-1,3] ],
      [ [2,4],[1,1] ]
    ])

    self.assertTrue((m1 & m2) == IntervalMatrix([
      [ [0,1],[0,2] ],
      [ Interval.empty(),Interval.empty() ]
    ]))


  def tests_intervalmatrix_mixing_type(self):

    m1 = Matrix([
      [ 1,2 ],
      [ 3,4 ]
    ])

    # Interval - Matrix
    self.assertTrue((Interval(-1,1) * m1) == IntervalMatrix([
      [ [-1,1],[-2,2] ],
      [ [-3,3],[-4,4] ]
    ]))

    # Matrix - Interval
    self.assertTrue((m1 / Interval(2)) == IntervalMatrix([
      [ [1./2.],[2./2.] ],
      [ [3./2.],[4./2.] ]
    ]))

    v1 = IntervalVector([
      -1,
      -2
    ])

    self.assertTrue(v1[0] == -1)
    self.assertTrue(v1[1] == -2)

    iv1 = IntervalVector([
      [-1,1],
      [-1,1]
    ])

    # Matrix - IntervalVector
    self.assertTrue((m1 * iv1) == IntervalVector([
      [-3,3],
      [-7,7]
    ]))

    # double - IntervalVector
    self.assertTrue((-3 * iv1) == IntervalVector([
      [-3,3],
      [-3,3]
    ]))

    # Interval - Vector
    self.assertTrue((Interval(-1,1) * v1) == IntervalVector([
      [-1,1],
      [-2,2]
    ]))

    im1 = IntervalMatrix([
      [ [-1,1],[-2,2] ],
      [ [-3,3],[-4,4] ]
    ])

    # Matrix - IntervalMatrix
    self.assertTrue((m1 + im1) == IntervalMatrix([
      [ [0,2],[0,4] ],
      [ [0,6],[0,8] ]
    ]))

    # IntervalMatrix - Matrix
    self.assertTrue((im1 + m1) == IntervalMatrix([
      [ [0,2],[0,4] ],
      [ [0,6],[0,8] ]
    ]))

    # IntervalMatrix - Matrix block
    self.assertTrue((im1 + m1.block(0,0,2,2)) == IntervalMatrix([
      [ [0,2],[0,4] ],
      [ [0,6],[0,8] ]
    ]))

    # Matrix block - IntervalMatrix
    self.assertTrue((m1.block(0,0,2,2) + im1) == IntervalMatrix([
      [ [0,2],[0,4] ],
      [ [0,6],[0,8] ]
    ]))

    # Matrix - IntervalMatrix
    self.assertTrue((m1 - im1) == IntervalMatrix([
      [ [0,2],[0,4] ],
      [ [0,6],[0,8] ]
    ]))

    # IntervalMatrix - Matrix
    self.assertTrue((im1 - m1) == IntervalMatrix([
      [ [-2,0],[-4,0] ],
      [ [-6,0],[-8,0] ]
    ]))

    # Matrix - col of IntervalMatrix
    self.assertTrue((m1 * im1.col(0)) == IntervalMatrix([
      [ [-7,7] ],
      [ [-15,15] ]
    ]))

    # Row of Matrix - col of Matrix
    # operations on Row types not supported yet in Python: self.assertTrue((m1.row(1) * m1.col(0)) == Matrix([[15]]))

    # Row of Matrix - col of IntervalMatrix
    # operations on Row types not supported yet in Python: self.assertTrue((m1.row(1) * im1.col(0)) == IntervalMatrix([[[-15,15]]]))

    # Row of IntervalMatrix - col of IntervalMatrix
    # operations on Row types not supported yet in Python: self.assertTrue((im1.row(1) * im1.col(0)) == IntervalMatrix([[[-15,15]]]))

    m1 = Matrix([
      [ 1,2 ],
      [ 3,4 ]
    ])

    m2 = Matrix([
      [ 2,3 ],
      [ 4,5 ]
    ])

    im = IntervalMatrix(m1,m2)
    self.assertTrue(im == IntervalMatrix([
        [ [1,2],[2,3] ],
        [ [3,4],[4,5] ]
      ]))

    m2[0,1] = 1.

    im_empty = IntervalMatrix(m1,m2)
    self.assertTrue(im_empty == IntervalMatrix.empty(2,2))

    m1 = IntervalMatrix.ones(3,3)
    m2 = IntervalMatrix.zero(3,3)
    self.assertTrue(m1.volume() == 0.)
    self.assertTrue(m2.volume() == 0.)
    self.assertTrue((m1+m2).volume() == 0.)

    m1 = IntervalMatrix([
      [ [1,2],[2,3] ],
      [ [3,4],[4,5] ]
    ])
    m2 = Matrix([
      [ 1.5, 2.5 ],
      [ 3.5, 4.5 ]
    ])

    self.assertTrue(m1.contains(m2))
    self.assertTrue(IntervalMatrix(m2).is_strict_subset(m1))

    m3 = Matrix(2,2)
    m3.init(m2+m2)
    self.assertTrue(m3 == Matrix([
      [ 3, 5 ],
      [ 7, 9 ]
    ]))

    m4 = IntervalMatrix(m2+m2)
    self.assertTrue(m4 == IntervalMatrix([
      [[3,3],[5,5]],
      [[7,7],[9,9]]
    ]))

    m5 = IntervalMatrix(m2*m2)
    self.assertTrue(m5 == IntervalMatrix([
      [[11,11],[15,15]],
      [[21,21],[29,29]]
    ]))

    test1 = IntervalMatrix([
      [ [0,1], [0,2], [0,0] ]
    ])
    self.assertTrue(test1.rows() == 1)
    self.assertTrue(test1.cols() == 3)
    self.assertTrue(test1(0,0) == Interval(0,1))
    self.assertTrue(test1(0,1) == Interval(0,2))
    self.assertTrue(test1(0,2) == Interval(0,0))

    test2 = IntervalMatrix([
      [0,1],
      [2,3],
      [4,5]
    ])
    self.assertTrue(test2.rows() == 3)
    self.assertTrue(test2.cols() == 2)
    self.assertTrue(test2(0,0) == Interval(0))
    self.assertTrue(test2(0,1) == Interval(1))
    self.assertTrue(test2(1,0) == Interval(2))
    self.assertTrue(test2(1,1) == Interval(3))
    self.assertTrue(test2(2,0) == Interval(4))
    self.assertTrue(test2(2,1) == Interval(5))

    test3 = IntervalMatrix([
      [[1,oo],[2,oo]],
      [[3,oo],[4,oo]],
      [[5,oo],[6,oo]]
    ])
    self.assertTrue(test3.rows() == 3)
    self.assertTrue(test3.cols() == 2)
    self.assertTrue(test3(0,0) == Interval(1,oo))
    self.assertTrue(test3(0,1) == Interval(2,oo))
    self.assertTrue(test3(1,0) == Interval(3,oo))
    self.assertTrue(test3(1,1) == Interval(4,oo))
    self.assertTrue(test3(2,0) == Interval(5,oo))
    self.assertTrue(test3(2,1) == Interval(6,oo))

    test4 = IntervalMatrix([
      [[1,2]],
      [[3,4]],
      [[5,6]]
    ])
    self.assertTrue(test4.rows() == 3)
    self.assertTrue(test4.cols() == 1)
    self.assertTrue(test4(0,0) == Interval(1,2))
    self.assertTrue(test4(1,0) == Interval(3,4))
    self.assertTrue(test4(2,0) == Interval(5,6))

    iv = IntervalVector([[1,2],[3,4],[5,6]])
    test5 = IntervalMatrix(iv)
    self.assertTrue(test5.rows() == 3)
    self.assertTrue(test5.cols() == 1)
    self.assertTrue(test5(0,0) == Interval(1,2))
    self.assertTrue(test5(1,0) == Interval(3,4))
    self.assertTrue(test5(2,0) == Interval(5,6))


if __name__ ==  '__main__':
  unittest.main()