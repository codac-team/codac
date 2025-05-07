#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou, Damien Massé
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import sys, os
import unittest
import math
from codac import *

def create_f():
  x = ScalarVar()
  return AnalyticFunction([x], x*cos(x))

class TestAnalyticFunction(unittest.TestCase):

  def tests_AnalyticFunction(self):

    def invalid_function():
      x = ScalarVar()
      f = AnalyticFunction([3], cos(x))

    self.assertRaises(ValueError, invalid_function)

    x = ScalarVar()
    self.assertTrue(x.size() == 1)

    y = VectorVar(3)
    self.assertTrue(y.size() == 3)
    self.assertTrue(type(y[0]) == ScalarExpr)
    self.assertTrue(type(y[2]) == ScalarExpr)

    for i in range(0,2):

      def test_eval(f,*args):

        if(i == 0): # natural
          return f.eval(EvalMode.NATURAL,*args)

        elif(i == 1): # centered
          return f.eval(EvalMode.CENTERED,*args)

        else: # centered and natural
          return f.eval(*args)

      f = AnalyticFunction([x], x)
      self.assertTrue(Approx(test_eval(f,Interval(0))) == 0)
      f = AnalyticFunction([x], x+Interval(2))
      self.assertTrue(Approx(test_eval(f,Interval(3))) == 5)
      f = AnalyticFunction([x], x+x)
      self.assertTrue(Approx(test_eval(f,Interval(2))) == 4)
      f = AnalyticFunction([x], x+x+2)
      self.assertTrue(Approx(test_eval(f,Interval(2))) == 6)
      f = AnalyticFunction([x], pow(x,2))
      self.assertTrue(Approx(test_eval(f,Interval(3))) == 9)
      f = AnalyticFunction([x], x^2)
      self.assertTrue(Approx(test_eval(f,Interval(3))) == 9)
      f = AnalyticFunction([x], (0.+x)^(1.*x))
      self.assertTrue(Approx(test_eval(f,Interval(3))) == 27)
      f = AnalyticFunction([x], x**2)
      self.assertTrue(Approx(test_eval(f,Interval(3))) == 9)
      f = AnalyticFunction([x], (0.+x)**(1.*x))
      self.assertTrue(Approx(test_eval(f,Interval(3))) == 27)
      f = AnalyticFunction([x], cos(x))
      self.assertTrue(Approx(test_eval(f,Interval(0))) == 1)    

      f = AnalyticFunction([x], [x,x])

      fvec = AnalyticFunction([x], [x,x])
      self.assertTrue(Approx(test_eval(f,1)) == IntervalVector([[1],[1]]))

      self.assertTrue(test_eval(AnalyticFunction([], +4)) == 4)
      self.assertTrue(test_eval(AnalyticFunction([], +4.)) == 4.)
      self.assertTrue(test_eval(AnalyticFunction([], +Interval(4,5))) == Interval(4,5))
      self.assertTrue(test_eval(AnalyticFunction([], Vector([2,9]))) == Vector([2,9]))
      self.assertTrue(test_eval(AnalyticFunction([], IntervalVector(3))) == IntervalVector([[-oo,oo],[-oo,oo],[-oo,oo]]))

      x1 = ScalarVar()
      x2 = ScalarVar()
      v1 = VectorVar(2)
      v2 = VectorVar(2)

      # ======> ScalarVar

      # .def("__pos__",  [](const ScalarVar& e1)
      self.assertTrue(test_eval(AnalyticFunction([x1], +x1), 5.) == 5.)
      # .def("__add__",  [](const ScalarVar& e1, const ScalarVar& e2)
      self.assertTrue(test_eval(AnalyticFunction([x1,x2], x1+x2), 5.,6.) == 11.)
      # .def("__add__",  [](const ScalarVar& e1, const Interval& e2)
      self.assertTrue(test_eval(AnalyticFunction([x1], x1+Interval(3)), 5.) == 8.)
      # .def("__radd__", [](const ScalarVar& e1, const Interval& e2)
      self.assertTrue(test_eval(AnalyticFunction([x1], Interval(3)+x1), 5.) == 8.)
      # .def("__neg__",  [](const ScalarVar& e1)
      self.assertTrue(test_eval(AnalyticFunction([x2], -x2), 6.) == -6.)
      # .def("__sub__",  [](const ScalarVar& e1, const ScalarVar& e2)
      self.assertTrue(test_eval(AnalyticFunction([x1,x2], x1-x2), 5.,6.) == -1.)
      # .def("__sub__",  [](const ScalarVar& e1, const Interval& e2)
      self.assertTrue(test_eval(AnalyticFunction([x1], x1-Interval(2.)), 5.) == 3.)
      # .def("__rsub__", [](const ScalarVar& e1, const Interval& e2)
      self.assertTrue(test_eval(AnalyticFunction([x1], Interval(2.)-x1), 5.) == -3.)
      # .def("__mul__",  [](const ScalarVar& e1, const ScalarVar& e2)
      self.assertTrue(test_eval(AnalyticFunction([x1,x2], x1*x2), 5.,6.) == 30.)
      # .def("__mul__",  [](const ScalarVar& e1, const Interval& e2)
      self.assertTrue(test_eval(AnalyticFunction([x1], x1*Interval(6.)), 5.) == 30.)
      # .def("__rmul__", [](const ScalarVar& e1, const Interval& e2)
      self.assertTrue(test_eval(AnalyticFunction([x1], Interval(6.)*x1), 5.) == 30.)
      # .def("__mul__",  [](const ScalarVar& e1, const VectorVar& e2)
      self.assertTrue(test_eval(AnalyticFunction([v1,v2], v1[0]*v2), Vector([5.,10.]),IntervalVector([[3],[3]])) == Vector([15,15]))
      # .def("__mul__",  [](const ScalarVar& e1, const IntervalVector& e2)
      self.assertTrue(test_eval(AnalyticFunction([x1], x1*IntervalVector([[-2,3],[0,1]])), 5.) == IntervalVector([[-10,15],[0,5]]))
      # .def("__truediv__",  [](const ScalarVar& e1, const ScalarVar& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1,x2], x1/x2), 1.,10.)) == 0.1)
      # .def("__truediv__",  [](const ScalarVar& e1, const Interval& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], x1/Interval(10)), 1.)) == 0.1)
      # .def("__rtruediv__", [](const ScalarVar& e1, const Interval& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], Interval(2.)/x1), 10.)) == 0.2)

      # ======> VectorVar
      
      #.def("__pos__",  [](const VectorVar& e1)
      self.assertTrue(test_eval(AnalyticFunction([v1], +v1), Vector([5.,6.])) == Vector([5.,6.]))
      #.def("__add__",  [](const VectorVar& e1, const VectorVar& e2)
      self.assertTrue(test_eval(AnalyticFunction([v1,v2], v1+v2), Vector([5.,6.]),Vector([2.,3.])) == Vector([7.,9.]))
      #.def("__add__",  [](const VectorVar& e1, const IntervalVector& e2)
      self.assertTrue(test_eval(AnalyticFunction([v1], v1+IntervalVector([[2],[3]])), Vector([5.,6.])) == IntervalVector([[7.],[9.]]))
      #.def("__radd__", [](const VectorVar& e1, const IntervalVector& e2)
      self.assertTrue(test_eval(AnalyticFunction([v1], IntervalVector([[2],[3]])+v1), Vector([5.,6.])) == IntervalVector([[7.],[9.]]))
      #.def("__neg__",  [](const VectorVar& e1)
      self.assertTrue(test_eval(AnalyticFunction([v1], -v1), Vector([5.,6.])) == -Vector([5.,6.]))
      #.def("__sub__",  [](const VectorVar& e1, const VectorVar& e2)
      self.assertTrue(test_eval(AnalyticFunction([v1,v2], v1-v2), Vector([5.,6.]),Vector([2.,3.])) == Vector([3.,3.]))
      #.def("__sub__",  [](const VectorVar& e1, const IntervalVector& e2)
      self.assertTrue(test_eval(AnalyticFunction([v1], v1-IntervalVector([[2],[3]])), Vector([5.,6.])) == IntervalVector([[3.],[3.]]))
      #.def("__rsub__", [](const VectorVar& e1, const IntervalVector& e2)
      self.assertTrue(test_eval(AnalyticFunction([v1], IntervalVector([[2],[3]])-v1), Vector([5.,6.])) == IntervalVector([[-3.],[-3.]]))

      # ======> ScalarExpr
      
      #.def("__pos__",  [](const ScalarExpr& e1)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], +cos(x1)), Interval(0.))) == Interval(1.))
      #.def(py::self + py::self)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], cos(x1)+cos(x1)), Interval(0.))) == Interval(2.))
      #.def("__add__",  [](const ScalarExpr& e1, const ScalarVar& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], cos(x1)+x1), math.pi)) == math.pi-1)
      #.def("__radd__", [](const ScalarExpr& e1, const ScalarVar& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], x1+cos(x1)), math.pi)) == math.pi-1)
      #.def("__add__",  [](const ScalarExpr& e1, const Interval& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], cos(x1)+Interval(10.)), math.pi)) == Interval(9))
      #.def("__radd__", [](const ScalarExpr& e1, const Interval& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], 10+cos(x1)), math.pi)) == Interval(9))
      #.def(- py::self)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], -cos(x1)), Interval(0.))) == Interval(-1.))
      #.def(py::self - py::self)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], cos(x1)-cos(x1)), Interval(0.))) == Interval(0.))
      #.def("__sub__",  [](const ScalarExpr& e1, const ScalarVar& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], cos(x1)-x1), math.pi)) == -math.pi-1)
      #.def("__rsub__", [](const ScalarExpr& e1, const ScalarVar& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], x1-cos(x1)), math.pi)) == math.pi+1)
      #.def("__sub__",  [](const ScalarExpr& e1, const Interval& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], cos(x1)-Interval(10.)), math.pi)) == -Interval(11))
      #.def("__rsub__", [](const ScalarExpr& e1, const Interval& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], 10-cos(x1)), math.pi)) == Interval(11))
      #.def(py::self * py::self)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], cos(x1)*cos(x1)), Interval(0.))) == Interval(1.))
      #.def("__mul__",  [](const ScalarExpr& e1, const ScalarVar& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], cos(x1)*x1), math.pi)) == -1*math.pi)
      #.def("__rmul__", [](const ScalarExpr& e1, const ScalarVar& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], x1*cos(x1)), math.pi)) == -1*math.pi)
      #.def("__mul__",  [](const ScalarExpr& e1, const Interval& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], cos(x1)*Interval(10.)), math.pi),1e-9) == -Interval(10))
      #.def("__rmul__", [](const ScalarExpr& e1, const Interval& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], Interval(10.)*cos(x1)), math.pi),1e-9) == -10)
      #.def("__mul__",  [](const ScalarExpr& e1, const VectorExpr& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([v1,v2], cos(v1[0])*(v2+v2)), Vector([math.pi,-1]),Vector([2,3])),1e-9) == IntervalVector([[-4],[-6]]))
      #.def("__truediv__",  [](const ScalarExpr& e1, const ScalarExpr& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], cos(x1)/cos(x1)), Interval(0.))) == Interval(1.))
      #.def("__truediv__",  [](const ScalarExpr& e1, const ScalarVar& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], cos(x1)/x1), math.pi)) == -1/math.pi)
      #.def("__rtruediv__", [](const ScalarExpr& e1, const ScalarVar& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], x1/cos(x1)), math.pi)) == -math.pi)
      #.def("__truediv__",  [](const ScalarExpr& e1, const Interval& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], cos(x1)/Interval(4.)), math.pi)) == -1./4)
      #.def("__rtruediv__", [](const ScalarExpr& e1, const Interval& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], 4./cos(x1)), math.pi)) == -4)

      # ======> VectorExpr

      #.def("__pos__",  [](const VectorExpr& e1)
      self.assertTrue(Approx(test_eval(AnalyticFunction([v1], +(v1+v1)), IntervalVector([[0.],[-oo,5]]))) == 
        IntervalVector([[0.],[-oo,oo]]) if i==1 else IntervalVector([[0.],[-oo,10]]))
      #.def(py::self + py::self)
      self.assertTrue(Approx(test_eval(AnalyticFunction([v1], v1+v1), IntervalVector([[0.],[-oo,5]]))) == 
        IntervalVector([[0.],[-oo,oo]]) if i==1 else IntervalVector([[0.],[-oo,10]]))
      #.def("__radd__", [](const VectorExpr& e1, const VectorVar& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([v1], (v1+v1)+v1), IntervalVector([[0.],[-oo,5]]))) == 
        IntervalVector([[0.],[-oo,oo]]) if i==1 else IntervalVector([[0.],[-oo,15]]))
      #.def("__radd__", [](const VectorExpr& e1, const IntervalVector& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([v1], v1+(v1+v1)), IntervalVector([[0.],[-oo,5]]))) == 
        IntervalVector([[0.],[-oo,oo]]) if i==1 else IntervalVector([[0.],[-oo,15]]))
      #.def(- py::self)
      self.assertTrue(Approx(test_eval(AnalyticFunction([v1], -(v1+v1)), IntervalVector([[0.],[-oo,5]]))) == 
        -IntervalVector([[0.],[-oo,oo]]) if i==1 else -IntervalVector([[0.],[-oo,10]]))
      #.def(py::self - py::self)
      self.assertTrue(Approx(test_eval(AnalyticFunction([v1,v2], (v1-v2)), IntervalVector([[2],[3]]),Vector([1,5]))) == IntervalVector([[1],[-2]]))
      #.def("__sub__",  [](const VectorExpr& e1, const VectorVar& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([v1,v2], (v1-v2)-v1), IntervalVector([[2],[3]]),Vector([1,5]))) == IntervalVector([[-1],[-5]]))
      #.def("__rsub__", [](const VectorExpr& e1, const VectorVar& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([v1,v2], v2-(v1-v2)), IntervalVector([[2],[3]]),Vector([1,5]))) == IntervalVector([[0],[7]]))
      #.def("__sub__",  [](const VectorExpr& e1, const IntervalVector& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([v1,v2], (v1-v2)-IntervalVector([[2],[3]])), IntervalVector([[2],[3]]),Vector([1,5]))) == IntervalVector([[-1],[-5]]))
      #.def("__rsub__", [](const VectorExpr& e1, const IntervalVector& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([v1,v2], Vector([1,5])-(v1-v2)), IntervalVector([[2],[3]]),Vector([1,5]))) == IntervalVector([[0],[7]]))
      #.def("__rmul__", [](const VectorExpr& e1, const ScalarExpr& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([v1,v2], cos(v1[0])*(v2+v2)), Vector([math.pi,-1]),Vector([2,3])),1e-9) == IntervalVector([[-4],[-6]]))
      #.def("__rmul__", [](const VectorExpr& e1, const ScalarVar& e2)
      self.assertTrue(Approx(test_eval(AnalyticFunction([x1], x1*vec(3*x1,2*x1)), 3),1e-9) == IntervalVector([[27],[18]]))
    
    x = VectorVar(2)
    f = AnalyticFunction([x], x[0]*(x[0]+x[1])+sqr(x[1]))
    self.assertTrue(f.diff(Vector([2,3]))(0,0) == 7)
    self.assertTrue(f.diff(Vector([2,3]))(0,1) == 8)

    # int values
    x,y = ScalarVar(), ScalarVar()
    f = AnalyticFunction([x,y], x*(x+y)+sqr(y))
    self.assertTrue(f.diff(2,3)(0,0) == 7)
    self.assertTrue(f.diff(2,3)(0,1) == 8)

    # double values
    x,y = ScalarVar(), ScalarVar()
    f = AnalyticFunction([x,y], x*(x+y)+sqr(y))
    self.assertTrue(f.diff(2.,3.)(0,0) == 7)
    self.assertTrue(f.diff(2.,3.)(0,1) == 8)

    # Interval values
    x,y = ScalarVar(), ScalarVar()
    f = AnalyticFunction([x,y], x*(x+y)+sqr(y))
    self.assertTrue(f.diff(Interval(2.),Interval(3.))(0,0) == 7)
    self.assertTrue(f.diff(Interval(2.),Interval(3.))(0,1) == 8)

    # Evaluation modes
    x = ScalarVar()
    f = AnalyticFunction([x], x-x)
    self.assertTrue(f.eval(EvalMode.NATURAL,Interval(-1,1)) == Interval(-2,2))
    self.assertTrue(f.eval(EvalMode.CENTERED,Interval(-1,1)) == Interval(0))
    self.assertTrue(f.eval(Interval(-1,1)) == Interval(0))

    # Scalar outputs
    f1 = AnalyticFunction([], 3)
    self.assertTrue(f1.eval() == Interval(3))
    f2 = AnalyticFunction([], 3.)
    self.assertTrue(f2.eval() == Interval(3))
    f3 = AnalyticFunction([], Interval(3.))
    self.assertTrue(f3.eval() == Interval(3))
    x = ScalarVar()
    f4 = AnalyticFunction([x], x*x)
    self.assertTrue(f4.eval(2.) == Interval(4))

    # Vectorial outputs

    f1 = AnalyticFunction([], [ 3 ])
    self.assertTrue(f1.eval() == IntervalVector([3]))
    f2 = AnalyticFunction([], [ 3. ])
    self.assertTrue(f2.eval() == IntervalVector([3]))
    f3 = AnalyticFunction([], [ Interval(3.) ])
    self.assertTrue(f3.eval() == IntervalVector([3]))
    x = ScalarVar()
    f4 = AnalyticFunction([x], [ x*x ])
    self.assertTrue(f4.eval(2.) == IntervalVector([4]))

    f_2args = AnalyticFunction([x], [ x*x,x*x ])
    self.assertTrue(f_2args.eval(1.) == IntervalVector.constant(2,[1]))
    f_3args = AnalyticFunction([x], [ x,x*x,1 ])
    self.assertTrue(f_3args.eval(1.) == IntervalVector.constant(3,[1]))
    f_4args = AnalyticFunction([x], [ x,x*x,1,x ])
    self.assertTrue(f_4args.eval(1.) == IntervalVector.constant(4,[1]))
    f_5args = AnalyticFunction([x], [ x,x*x,1,x,x ])
    self.assertTrue(f_5args.eval(1.) == IntervalVector.constant(5,[1]))
    f_6args = AnalyticFunction([x], [ x,x*x,1,x,x,1*x ])
    self.assertTrue(f_6args.eval(1.) == IntervalVector.constant(6,[1]))
    f_7args = AnalyticFunction([x], [ x,x*x,1,x,x,1*x,x*x ])
    self.assertTrue(f_7args.eval(1.) == IntervalVector.constant(7,[1]))
    f_8args = AnalyticFunction([x], [ x,x*x,1,x,x,1*x,x*x,1 ])
    self.assertTrue(f_8args.eval(1.) == IntervalVector.constant(8,[1]))
    f_9args = AnalyticFunction([x], [ x,x*x,1,x,x,1*x,x*x,1,x ])
    self.assertTrue(f_9args.eval(1.) == IntervalVector.constant(9,[1]))
    f_10args = AnalyticFunction([x], [ x,x*x,1,x,x,1*x,x*x,1,x,1*x ])
    self.assertTrue(f_10args.eval(1.) == IntervalVector.constant(10,[1]))

    # Subvector on variables
    p = VectorVar(2)
    x = VectorVar(4)
    f = AnalyticFunction([p], p[0]*p[1])
    g = AnalyticFunction([x], f(x.subvector(0,1)) + f(x.subvector(2,3)))

    a = IntervalVector(4)

    a = IntervalVector([[1],[2],[3],[4]])
    self.assertTrue(g.eval(EvalMode.NATURAL,a) == 14)
    self.assertTrue(g.eval(EvalMode.CENTERED,a) == 14)
    self.assertTrue(g.eval(a) == 14)

    a = IntervalVector([[0],[2],[5],[4]])
    self.assertTrue(g.eval(EvalMode.NATURAL,a) == 20)
    self.assertTrue(g.eval(EvalMode.CENTERED,a) == 20)
    self.assertTrue(g.eval(a) == 20)


    # Sign, floor, ceil, min, max
    x1 = ScalarVar()
    x2 = ScalarVar()

    f = AnalyticFunction([x1,x2], 2*max(x1,x2+1))
    self.assertTrue(f.eval(0.,1.) == 4.)
    self.assertTrue(f.eval(2.,1.) == 4.)
    self.assertTrue(f.eval(3.,1.) == 6.)

    f = AnalyticFunction([x1,x2], 2*min(x1,x2+1))
    self.assertTrue(f.eval(0.,1.) == 0.)
    self.assertTrue(f.eval(2.,1.) == 4.)
    self.assertTrue(f.eval(3.,1.) == 4.)

    f = AnalyticFunction([x1], 2*sign(x1+1))
    self.assertTrue(f.eval(0.) == 2.)
    self.assertTrue(sign(Interval.zero()) == Interval(-1,1))
    self.assertTrue(sign(Interval(-0,0)) == Interval(-1,1))
    self.assertTrue(f.eval(-1.) == Interval(-2,2))
    self.assertTrue(f.eval(-2.) == -2.)

    f = AnalyticFunction([x1], 2*floor(x1))
    self.assertTrue(f.eval(0.) == 0.)
    self.assertTrue(f.eval(1.5) == 2.)
    self.assertTrue(f.eval(-1.5) == -4.)

    f = AnalyticFunction([x1], 2*ceil(x1))
    self.assertTrue(f.eval(0.) == 0.)
    self.assertTrue(f.eval(1.5) == 4.)
    self.assertTrue(f.eval(-1.5) == -2.)


    # Issue #201
    # Input argument is a py::list instead of a Vector
    x1 = VectorVar(2)
    f = AnalyticFunction([x1], 2.*x1)
    self.assertTrue(f.eval([2,3]) == IntervalVector([[4],[6]]))
    self.assertTrue(f.eval([[2,3],[4,5]]) == IntervalVector([[4,6],[8,10]]))


    I = Matrix([[0,2],[-1,0]])
    x = VectorVar(2)
    f = AnalyticFunction([x], I*x)
    self.assertTrue(f.eval(IntervalVector([[0,1],[2,3]])) == IntervalVector([[4,6],[-1,0]]))

    I = Matrix([[1,0],[0,1]])
    x = VectorVar(2)
    f = AnalyticFunction([x], I*I*x)
    self.assertTrue(f.eval(IntervalVector([[-1,1],[2,3]])) == IntervalVector([[-1,1],[2,3]]))

    A = MatrixVar(2,2)
    x = VectorVar(2)
    h = AnalyticFunction([A], A*A)
    f = AnalyticFunction([x,A], h(A)*x)
    g = AnalyticFunction([x], f(x,Matrix([[0,2],[-1,0]])))
    self.assertTrue(g.eval(IntervalVector([[-1,1],[2,3]])) == IntervalVector([[-2,2],[-6,-4]]))

    A = MatrixVar(2,2)
    f_det = AnalyticFunction([A], A(0,0)*A(1,1)-A(1,0)*A(0,1))
    self.assertTrue(f_det.eval(Matrix([[1,2],[3,4]])) == -2)
    self.assertTrue(f_det.eval(IntervalMatrix([[[0,1],[1,2]],[[2,3],[3,4]]])) == Interval(-6,2))

    f = create_f()
    self.assertTrue(Approx(f.eval(PI)) == -PI)

    x = ScalarVar()
    f = AnalyticFunction([x],sqrt(x))
    self.assertTrue(Interval(0.).is_subset([0,oo]))
    self.assertTrue(Interval(0.,10.).is_subset([0,oo]))
    self.assertTrue(Approx(f.eval(EvalMode.NATURAL, 0.)) == 0.)
    self.assertTrue(Approx(f.eval(EvalMode.NATURAL, 1e-10),1e-3) == 0.)
    # Cannot compute in pure centered form due to the
    # definition domain of the derivative of sqrt:
    self.assertTrue(f.eval(EvalMode.CENTERED, 0.).is_empty())
    self.assertTrue(Approx(f.eval(EvalMode.CENTERED, 1e-10),1e-3) == 0.)
    self.assertTrue(Approx(f.eval(0.)) == 0.)
    self.assertTrue(Approx(f.eval(1e-10),1e-3) == 0.)

    x1,x2,x3 = VectorVar(2),VectorVar(2),VectorVar(2)
    f = AnalyticFunction([x1,x2,x3], mat(+x1,-x2,2*x3))
    self.assertTrue(f.eval(EvalMode.NATURAL, Vector([1,2]),Vector([-1,8]),IntervalVector([[-1,1],[2,oo]]))
      == IntervalMatrix([[1,1,[-2,2]],[2,-8,[4,oo]]]))
  
    x1 = VectorVar(2)
    f = AnalyticFunction([x1],det(mat(+x1,2*x1)))
    self.assertTrue(Approx(f.eval(EvalMode.NATURAL, IntervalVector([[0.9,1.1],[0.4,0.5]])),1e-9) == Interval(-0.38,0.38))
    self.assertTrue(Approx(f.eval(EvalMode.CENTERED, IntervalVector([[0.9,1.1],[0.4,0.5]])),1e-9) == Interval(-0.04,0.04))

    M1 = MatrixVar(2,3)
    M2 = MatrixVar(3,2)
    f = AnalyticFunction([M1,M2], M1*M2-M1*M2)
    self.assertTrue(Approx(f.eval(EvalMode.NATURAL, 
			Matrix([[1,0,1],[0,1,0]]),
			IntervalMatrix([[[-0.2,0.2],[-0.1,0.1]],
					[[0.2,0.4],[-0.4,-0.1]],
					[[1.0,2.0],[-0.2,-0.1]]])),1e-9)
			== IntervalMatrix([[[-1.4,1.4],[-0.3,0.3]],
					   [[-0.2,0.2],[-0.3,0.3]]]))
    self.assertTrue(Approx(f.eval(EvalMode.CENTERED, 
			Matrix([[1,0,1],[0,1,0]]),
			IntervalMatrix([[[-0.2,0.2],[-0.1,0.1]],
					[[0.2,0.4],[-0.4,-0.1]],
					[[1.0,2.0],[-0.2,-0.1]]])),1e-9)
			== Matrix([[0,0],[0,0]]))

if __name__ ==  '__main__':
  unittest.main()
