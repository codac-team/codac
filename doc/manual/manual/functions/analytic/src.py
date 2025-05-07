#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import sys, os
import unittest
import math
from codac import *

class TestAnalyticFunctionManual(unittest.TestCase):

  def tests_AnalyticFunction_manual(test):

    # [1-beg]
    x1 = ScalarVar()
    x2 = ScalarVar()
    v = VectorVar(3)

    # Example of scalar function: from R to R
    f1 = AnalyticFunction([x1], x1*cos(x1))

    # Example of vectorial function: from R to R²
    f2 = AnalyticFunction([x1], [ x1*cos(x1), x1*sin(x1) ])

    # Example of vectorial function: from R³ to R²
    f3 = AnalyticFunction([v], Interval(-1,1)*v)

    # Example of multivariate vectorial function: from R×R to R³
    f4 = AnalyticFunction([x1,x2], [ x1+x2, Interval(0,1)*exp(x1), x2^(1+x1) ])
    # [1-end]


    # [2-beg]
    # Example of function: from R³ to R²
    f5 = AnalyticFunction([v], (v[0]*v).subvector(1,2))

    # Example of scalar function: from R^(2x2) to R
    M = MatrixVar(2,2)
    f6 = AnalyticFunction([M], M(0,0)*M(1,1)-M(1,0)*M(0,1))
    # [2-end]

    test.assertTrue(f5.eval(Vector([5,6,7])) == Vector([30,35]))
    test.assertTrue(f6.eval(Matrix([[1,2],[3,4]])) == -2)


    # [3-beg]
    def create_f():
      x = ScalarVar()
      return AnalyticFunction([x], x*cos(x))

    f = create_f() # x is no longer useful here
    # [3-end]

    test.assertTrue(Approx(f.eval(PI)) == -PI)


    # [4-beg]
    f = AnalyticFunction([x1], x1*cos(x1))
    g = AnalyticFunction([x1], [ f(2*x1), x1*sin(x1) ])
    h = AnalyticFunction([x1], g(x1)[0]) # output is 2*x*cos(2*x)
    # [4-end]
    
    test.assertTrue(Approx(h.eval(PI)) == 2*PI)


    # [4b-beg]
    a = ScalarVar()
    v = VectorVar(2)

    Rot = AnalyticFunction([a], mat(vec(cos(a),sin(a)),vec(-sin(a),cos(a))))
    f = AnalyticFunction([v], Rot(PI/4)*v)
    
    y = f.eval([1,1]) # y == [ [-2.22045e-16, 4.4409e-16] ; [1.41421, 1.41422] ]
    # [4b-end]
    
    test.assertTrue(Approx(y,1e-5) == Vector([0,1.41421]))


    # [5-beg]
    x1 = ScalarVar() # scalar argument
    f1 = AnalyticFunction([x1], x1*cos(x1))

    y1 = f1.eval(0.)
    y1 = f1.eval(PI)
    y1 = f1.eval(Interval(0,1))

    x2 = ScalarVar()
    f2 = AnalyticFunction([x1,x2], x1^x2) # example of multivariate function

    y2 = f2.eval(Interval(2,3), 2)

    v = VectorVar(3) # vector arguments
    w = VectorVar(3)
    f3 = AnalyticFunction([v,w], v-w) # example of vectorial function

    y3 = f3.eval(Vector([5,4,3]), IntervalVector([[3,oo],[2],[1,2]]))
    # [5-end]

    test.assertTrue(f1.eval(0.) == 0)
    test.assertTrue(Approx(f1.eval(PI),1e-6) == Interval(-PI,-PI))
    test.assertTrue(Approx(f1.eval(Interval(0,1)),1e-6) == Interval(0, 0.938792))
    test.assertTrue(y2 == Interval(4,9))
    test.assertTrue(y3 == IntervalVector([[-oo,2],2,[1,2]]))


    # [6-beg]
    f1.eval(EvalMode.NATURAL, Interval(0,1))
    f1.eval(EvalMode.CENTERED, Interval(0,1))

    f1.eval(EvalMode.NATURAL | EvalMode.CENTERED, Interval(0,1))
    # which is equivalent to: f1.eval(Interval(0,1))
    # [6-end]

    test.assertTrue(Approx(f1.eval(EvalMode.NATURAL, Interval(0,1)), 1e-6) == Interval(0,1))
    test.assertTrue(Approx(f1.eval(EvalMode.CENTERED, Interval(0,1)), 1e-6) == Interval(-0.0612088, 0.938792))
    test.assertTrue(Approx(f1.eval(EvalMode.NATURAL | EvalMode.CENTERED, Interval(0,1)), 1e-6) == Interval(0, 0.938792))


    # [7-beg]
    x1 = ScalarVar()
    f1 = AnalyticFunction([x1], x1*cos(x1))
    J1 = f1.diff(Interval(0,PI/2))
    # J1 = intv. matrix 1x1: [[ [-(PI/2),1] ]]

    x2 = ScalarVar()
    f2 = AnalyticFunction([x1,x2], x1^x2) # example of multivariate function
    J2 = f2.diff(2.,Interval(2,3))
    # J2 = intv. matrix 1x2: [[ [4,12], [0,0] ]]

    v = VectorVar(3)
    f3 = AnalyticFunction([v], [ # vectorial function
      v[0]-(v[1]^2),
      Interval(-1,0)*v[2]
    ])
    J3 = f3.diff(Vector([5,8,10]))
    # J3 = intv. matrix 2x3: [[ 1,-16,0 ],[ 0,0,[-1,0] ]]
    # [7-end]

    test.assertTrue(Approx(J1) == IntervalMatrix([[[-(PI/2),1]]]))
    test.assertTrue(Approx(J2,1e-8) == IntervalMatrix([[[4,12],0]]))
    test.assertTrue(J3 == IntervalMatrix([[1,-16,0],[0,0,[-1,0]]]))

    f = f3
    # [8-beg]
    n = f.input_size()
    m = f.output_size()
    # [8-end]

    test.assertTrue(n == 3)
    test.assertTrue(m == 2)

    # [9-beg]
    # Forward evaluation
    y = CosOp.fwd([0,PI/2]) # y = [0,1]

    # Backward evaluation
    x = Interval(0,PI) # prior value of [x]
    CosOp.bwd([0,0.5], x) # [x] is contracted to [PI/3,PI/2]
    # [9-end]

    test.assertTrue(Approx(y) == Interval(0,1))
    test.assertTrue(Approx(x) == Interval(PI/3,PI/2))

    
if __name__ ==  '__main__':
  unittest.main()