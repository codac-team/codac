#!/usr/bin/env python

#  Codac tests
# ----------------------------------------------------------------------------
#  \date       2024
#  \author     Simon Rohou
#  \copyright  Copyright 2024 Codac Team
#  \license    GNU Lesser General Public License (LGPL)

import unittest
import math
from codac import *


class TestAnalyticFunction(unittest.TestCase):

  def test_function_invalid_args(self):

    def invalid_function():  
      x = ScalarVar()
      f = AnalyticFunction([3], cos(x))

    self.assertRaises(ValueError, invalid_function)

  def tests_simple_analytic_function(self):

    x = ScalarVar()
    self.assertTrue(x.size() == 1)

    y = VectorVar(3)
    self.assertTrue(y.size() == 3)
    self.assertTrue(type(y[0]) == ScalarExpr)
    self.assertTrue(type(y[2]) == ScalarExpr)

    f = AnalyticFunction([x], x)
    self.assertTrue(Approx(f.eval(Interval(0))) == 0)
    f = AnalyticFunction([x], x+x)
    self.assertTrue(Approx(f.eval(Interval(2))) == 4)
    f = AnalyticFunction([x], x+x+2)
    self.assertTrue(Approx(f.eval(Interval(2))) == 6)
    f = AnalyticFunction([x], pow(x,2))
    self.assertTrue(Approx(f.eval(Interval(3))) == 9)
    f = AnalyticFunction([x], cos(x))
    self.assertTrue(Approx(f.eval(Interval(0))) == 1)    

    f = AnalyticFunction([x], vec(x,x))

    fvec = AnalyticFunction([x], vec(x,x))
    self.assertTrue(Approx(f.eval(1)) == IntervalVector([[1],[1]]))

  def tests_operators(self):

    self.assertTrue(AnalyticFunction([], +4).eval() == 4)
    self.assertTrue(AnalyticFunction([], +4.).eval() == 4.)
    self.assertTrue(AnalyticFunction([], +Interval(4,5)).eval() == Interval(4,5))
    self.assertTrue(AnalyticFunction([], Vector([2,9])).eval() == Vector([2,9]))
    self.assertTrue(AnalyticFunction([], IntervalVector(3)).eval() == IntervalVector([[-oo,oo],[-oo,oo],[-oo,oo]]))

    x1 = ScalarVar()
    x2 = ScalarVar()
    v1 = VectorVar(2)
    v2 = VectorVar(2)

    # ======> ScalarVar

    # .def("__pos__",  [](const ScalarVar& e1)
    self.assertTrue(AnalyticFunction([x1], +x1).eval(5.) == 5.)
    # .def("__add__",  [](const ScalarVar& e1, const ScalarVar& e2)
    self.assertTrue(AnalyticFunction([x1,x2], x1+x2).eval(5.,6.) == 11.)
    # .def("__add__",  [](const ScalarVar& e1, const Interval& e2)
    self.assertTrue(AnalyticFunction([x1], x1+Interval(3)).eval(5.) == 8.)
    # .def("__radd__", [](const ScalarVar& e1, const Interval& e2)
    self.assertTrue(AnalyticFunction([x1], Interval(3)+x1).eval(5.) == 8.)
    # .def("__neg__",  [](const ScalarVar& e1)
    self.assertTrue(AnalyticFunction([x2], -x2).eval(6.) == -6.)
    # .def("__sub__",  [](const ScalarVar& e1, const ScalarVar& e2)
    self.assertTrue(AnalyticFunction([x1,x2], x1-x2).eval(5.,6.) == -1.)
    # .def("__sub__",  [](const ScalarVar& e1, const Interval& e2)
    self.assertTrue(AnalyticFunction([x1], x1-Interval(2.)).eval(5.) == 3.)
    # .def("__rsub__", [](const ScalarVar& e1, const Interval& e2)
    self.assertTrue(AnalyticFunction([x1], Interval(2.)-x1).eval(5.) == -3.)
    # .def("__mul__",  [](const ScalarVar& e1, const ScalarVar& e2)
    self.assertTrue(AnalyticFunction([x1,x2], x1*x2).eval(5.,6.) == 30.)
    # .def("__mul__",  [](const ScalarVar& e1, const Interval& e2)
    self.assertTrue(AnalyticFunction([x1], x1*Interval(6.)).eval(5.) == 30.)
    # .def("__rmul__", [](const ScalarVar& e1, const Interval& e2)
    self.assertTrue(AnalyticFunction([x1], Interval(6.)*x1).eval(5.) == 30.)
    # .def("__mul__",  [](const ScalarVar& e1, const VectorVar& e2)
    self.assertTrue(AnalyticFunction([v1,v2], v1[0]*v2).eval(Vector([5.,10.]),IntervalVector(2,3.)) == Vector(2,15.))
    # .def("__mul__",  [](const ScalarVar& e1, const IntervalVector& e2)
    self.assertTrue(AnalyticFunction([x1], x1*IntervalVector([[-2,3],[0,1]])).eval(5.) == IntervalVector([[-10,15],[0,5]]))
    # .def("__truediv__",  [](const ScalarVar& e1, const ScalarVar& e2)
    self.assertTrue(Approx(AnalyticFunction([x1,x2], x1/x2).eval(1.,10.)) == 0.1)
    # .def("__truediv__",  [](const ScalarVar& e1, const Interval& e2)
    self.assertTrue(Approx(AnalyticFunction([x1], x1/Interval(10)).eval(1.)) == 0.1)
    # .def("__rtruediv__", [](const ScalarVar& e1, const Interval& e2)
    self.assertTrue(Approx(AnalyticFunction([x1], Interval(2.)/x1).eval(10.)) == 0.2)

    # ======> VectorVar
    
    #.def("__pos__",  [](const VectorVar& e1)
    self.assertTrue(AnalyticFunction([v1], +v1).eval(Vector([5.,6.])) == Vector([5.,6.]))
    #.def("__add__",  [](const VectorVar& e1, const VectorVar& e2)
    self.assertTrue(AnalyticFunction([v1,v2], v1+v2).eval(Vector([5.,6.]),Vector([2.,3.])) == Vector([7.,9.]))
    #.def("__add__",  [](const VectorVar& e1, const IntervalVector& e2)
    self.assertTrue(AnalyticFunction([v1], v1+IntervalVector([[2],[3]])).eval(Vector([5.,6.])) == IntervalVector([[7.],[9.]]))
    #.def("__radd__", [](const VectorVar& e1, const IntervalVector& e2)
    self.assertTrue(AnalyticFunction([v1], IntervalVector([[2],[3]])+v1).eval(Vector([5.,6.])) == IntervalVector([[7.],[9.]]))
    #.def("__neg__",  [](const VectorVar& e1)
    self.assertTrue(AnalyticFunction([v1], -v1).eval(Vector([5.,6.])) == -Vector([5.,6.]))
    #.def("__sub__",  [](const VectorVar& e1, const VectorVar& e2)
    self.assertTrue(AnalyticFunction([v1,v2], v1-v2).eval(Vector([5.,6.]),Vector([2.,3.])) == Vector([3.,3.]))
    #.def("__sub__",  [](const VectorVar& e1, const IntervalVector& e2)
    self.assertTrue(AnalyticFunction([v1], v1-IntervalVector([[2],[3]])).eval(Vector([5.,6.])) == IntervalVector([[3.],[3.]]))
    #.def("__rsub__", [](const VectorVar& e1, const IntervalVector& e2)
    self.assertTrue(AnalyticFunction([v1], IntervalVector([[2],[3]])-v1).eval(Vector([5.,6.])) == IntervalVector([[-3.],[-3.]]))

    # ======> ScalarExpr
    
    #.def("__pos__",  [](const ScalarExpr& e1)
    self.assertTrue(Approx(AnalyticFunction([x1], +cos(x1)).eval(Interval(0.))) == Interval(1.))
    #.def(py::self + py::self)
    self.assertTrue(Approx(AnalyticFunction([x1], cos(x1)+cos(x1)).eval(Interval(0.))) == Interval(2.))
    #.def("__add__",  [](const ScalarExpr& e1, const ScalarVar& e2)
    self.assertTrue(Approx(AnalyticFunction([x1], cos(x1)+x1).eval(math.pi)) == math.pi-1)
    #.def("__radd__", [](const ScalarExpr& e1, const ScalarVar& e2)
    self.assertTrue(Approx(AnalyticFunction([x1], x1+cos(x1)).eval(math.pi)) == math.pi-1)
    #.def("__add__",  [](const ScalarExpr& e1, const Interval& e2)
    self.assertTrue(Approx(AnalyticFunction([x1], cos(x1)+Interval(10.)).eval(math.pi)) == Interval(9))
    #.def("__radd__", [](const ScalarExpr& e1, const Interval& e2)
    self.assertTrue(Approx(AnalyticFunction([x1], 10+cos(x1)).eval(math.pi)) == Interval(9))
    #.def(- py::self)
    self.assertTrue(Approx(AnalyticFunction([x1], -cos(x1)).eval(Interval(0.))) == Interval(-1.))
    #.def(py::self - py::self)
    self.assertTrue(Approx(AnalyticFunction([x1], cos(x1)-cos(x1)).eval(Interval(0.))) == Interval(0.))
    #.def("__sub__",  [](const ScalarExpr& e1, const ScalarVar& e2)
    self.assertTrue(Approx(AnalyticFunction([x1], cos(x1)-x1).eval(math.pi)) == -math.pi-1)
    #.def("__rsub__", [](const ScalarExpr& e1, const ScalarVar& e2)
    self.assertTrue(Approx(AnalyticFunction([x1], x1-cos(x1)).eval(math.pi)) == math.pi+1)
    #.def("__sub__",  [](const ScalarExpr& e1, const Interval& e2)
    self.assertTrue(Approx(AnalyticFunction([x1], cos(x1)-Interval(10.)).eval(math.pi)) == -Interval(11))
    #.def("__rsub__", [](const ScalarExpr& e1, const Interval& e2)
    self.assertTrue(Approx(AnalyticFunction([x1], 10-cos(x1)).eval(math.pi)) == Interval(11))
    #.def(py::self * py::self)
    self.assertTrue(Approx(AnalyticFunction([x1], cos(x1)*cos(x1)).eval(Interval(0.))) == Interval(1.))
    #.def("__mul__",  [](const ScalarExpr& e1, const ScalarVar& e2)
    self.assertTrue(Approx(AnalyticFunction([x1], cos(x1)*x1).eval(math.pi)) == -1*math.pi)
    #.def("__rmul__", [](const ScalarExpr& e1, const ScalarVar& e2)
    self.assertTrue(Approx(AnalyticFunction([x1], x1*cos(x1)).eval(math.pi)) == -1*math.pi)
    #.def("__mul__",  [](const ScalarExpr& e1, const Interval& e2)
    self.assertTrue(Approx(AnalyticFunction([x1], cos(x1)*Interval(10.)).eval(math.pi),1e-9) == -Interval(10))
    #.def("__rmul__", [](const ScalarExpr& e1, const Interval& e2)
    self.assertTrue(Approx(AnalyticFunction([x1], Interval(10.)*cos(x1)).eval(math.pi),1e-9) == -10)
    #.def("__mul__",  [](const ScalarExpr& e1, const VectorExpr& e2)
    self.assertTrue(Approx(AnalyticFunction([v1,v2], cos(v1[0])*(v2+v2)).eval(Vector([math.pi,-1]),Vector([2,3])),1e-9) == IntervalVector([[-4],[-6]]))
    #.def("__truediv__",  [](const ScalarExpr& e1, const ScalarExpr& e2)
    self.assertTrue(Approx(AnalyticFunction([x1], cos(x1)/cos(x1)).eval(Interval(0.))) == Interval(1.))
    #.def("__truediv__",  [](const ScalarExpr& e1, const ScalarVar& e2)
    self.assertTrue(Approx(AnalyticFunction([x1], cos(x1)/x1).eval(math.pi)) == -1/math.pi)
    #.def("__rtruediv__", [](const ScalarExpr& e1, const ScalarVar& e2)
    self.assertTrue(Approx(AnalyticFunction([x1], x1/cos(x1)).eval(math.pi)) == -math.pi)
    #.def("__truediv__",  [](const ScalarExpr& e1, const Interval& e2)
    self.assertTrue(Approx(AnalyticFunction([x1], cos(x1)/Interval(4.)).eval(math.pi)) == -1./4)
    #.def("__rtruediv__", [](const ScalarExpr& e1, const Interval& e2)
    self.assertTrue(Approx(AnalyticFunction([x1], 4./cos(x1)).eval(math.pi)) == -4)

    # ======> VectorExpr

    #.def("__pos__",  [](const VectorExpr& e1)
    self.assertTrue(Approx(AnalyticFunction([v1], +(v1+v1)).eval(IntervalVector([[0.],[-oo,5]]))) == IntervalVector([[0.],[-oo,10]]))
    #.def(py::self + py::self)
    self.assertTrue(Approx(AnalyticFunction([v1], v1+v1).eval(IntervalVector([[0.],[-oo,5]]))) == IntervalVector([[0.],[-oo,10]]))
    #.def("__radd__", [](const VectorExpr& e1, const VectorVar& e2)
    self.assertTrue(Approx(AnalyticFunction([v1], (v1+v1)+v1).eval(IntervalVector([[0.],[-oo,5]]))) == IntervalVector([[0.],[-oo,15]]))
    #.def("__radd__", [](const VectorExpr& e1, const IntervalVector& e2)
    self.assertTrue(Approx(AnalyticFunction([v1], v1+(v1+v1)).eval(IntervalVector([[0.],[-oo,5]]))) == IntervalVector([[0.],[-oo,15]]))
    #.def(- py::self)
    self.assertTrue(Approx(AnalyticFunction([v1], -(v1+v1)).eval(IntervalVector([[0.],[-oo,5]]))) == -IntervalVector([[0.],[-oo,10]]))
    #.def(py::self - py::self)
    self.assertTrue(Approx(AnalyticFunction([v1,v2], (v1-v2)).eval(IntervalVector([[2],[3]]),Vector([1,5]))) == IntervalVector([[1],[-2]]))
    #.def("__sub__",  [](const VectorExpr& e1, const VectorVar& e2)
    self.assertTrue(Approx(AnalyticFunction([v1,v2], (v1-v2)-v1).eval(IntervalVector([[2],[3]]),Vector([1,5]))) == IntervalVector([[-1],[-5]]))
    #.def("__rsub__", [](const VectorExpr& e1, const VectorVar& e2)
    self.assertTrue(Approx(AnalyticFunction([v1,v2], v2-(v1-v2)).eval(IntervalVector([[2],[3]]),Vector([1,5]))) == IntervalVector([[0],[7]]))
    #.def("__sub__",  [](const VectorExpr& e1, const IntervalVector& e2)
    self.assertTrue(Approx(AnalyticFunction([v1,v2], (v1-v2)-IntervalVector([[2],[3]])).eval(IntervalVector([[2],[3]]),Vector([1,5]))) == IntervalVector([[-1],[-5]]))
    #.def("__rsub__", [](const VectorExpr& e1, const IntervalVector& e2)
    self.assertTrue(Approx(AnalyticFunction([v1,v2], Vector([1,5])-(v1-v2)).eval(IntervalVector([[2],[3]]),Vector([1,5]))) == IntervalVector([[0],[7]]))
    #.def("__rmul__", [](const VectorExpr& e1, const ScalarExpr& e2)
    self.assertTrue(Approx(AnalyticFunction([v1,v2], cos(v1[0])*(v2+v2)).eval(Vector([math.pi,-1]),Vector([2,3])),1e-9) == IntervalVector([[-4],[-6]]))
    #.def("__rmul__", [](const VectorExpr& e1, const ScalarVar& e2)
    self.assertTrue(Approx(AnalyticFunction([x1], x1*vec(3*x1,2*x1)).eval(3),1e-9) == IntervalVector([[27],[18]]))
    
  def tests_analytic_function(self):

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

if __name__ ==  '__main__':
  unittest.main()