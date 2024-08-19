/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL);
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_AnalyticFunction.h>
#include <codac2_analytic_operations.h>
#include <codac2_Approx.h>

using namespace std;
using namespace codac2;

TEST_CASE("AnalyticFunction")
{
  {
    /*
    def invalid_function():  
      x = ScalarVar();
      f = AnalyticFunction({3}, cos(x));

    self.assertRaises(ValueError, invalid_function);
    */
  }

  {
    ScalarVar x;
    CHECK(x.size() == 1);

    VectorVar y(3);
    CHECK(y.size() == 3);

    {
      AnalyticFunction f({x}, x);
      CHECK(Approx(f.eval(Interval(0))) == 0);
    }
    {
      AnalyticFunction f({x}, x+x);
      CHECK(Approx(f.eval(Interval(2))) == 4);
    }
    {
      AnalyticFunction f({x}, x+x+2);
      CHECK(Approx(f.eval(Interval(2))) == 6);
    }
    {
      AnalyticFunction f({x}, pow(x,2));
      CHECK(Approx(f.eval(Interval(3))) == 9);
    }
    {
      AnalyticFunction f({x}, cos(x));
      CHECK(Approx(f.eval(Interval(0))) == 1);
    }

    AnalyticFunction f({x}, vec(x,x));

    AnalyticFunction fvec({x}, vec(x,x));
    CHECK(Approx(f.eval(1)) == IntervalVector({{1},{1}}));
  }

  {
    // todo: remove call to "const_value" method
    CHECK(AnalyticFunction({}, const_value(+Interval(4,5))).eval() == Interval(4,5));
    CHECK(AnalyticFunction({}, const_value(+4)).eval() == 4);
    CHECK(AnalyticFunction({}, const_value(+4.)).eval() == 4.);
    CHECK(AnalyticFunction({}, const_value(Vector({2,9}))).eval() == Vector({2,9}));
    CHECK(AnalyticFunction({}, const_value(IntervalVector(3))).eval() == IntervalVector({{-oo,oo},{-oo,oo},{-oo,oo}}));

    ScalarVar x1;
    ScalarVar x2;
    VectorVar v1(2);
    VectorVar v2(2);

    // ======> ScalarVar

    // .def("__pos__",  {}(const ScalarVar& e1);
    CHECK(AnalyticFunction({x1}, +x1).eval(5.) == 5.);
    // .def("__add__",  {}(const ScalarVar& e1, const ScalarVar& e2);
    CHECK(AnalyticFunction({x1,x2}, x1+x2).eval(5.,6.) == 11.);
    // .def("__add__",  {}(const ScalarVar& e1, const Interval& e2);
    CHECK(AnalyticFunction({x1}, x1+Interval(3)).eval(5.) == 8.);
    // .def("__radd__", {}(const ScalarVar& e1, const Interval& e2);
    CHECK(AnalyticFunction({x1}, Interval(3)+x1).eval(5.) == 8.);
    // .def("__neg__",  {}(const ScalarVar& e1);
    CHECK(AnalyticFunction({x2}, -x2).eval(6.) == -6.);
    // .def("__sub__",  {}(const ScalarVar& e1, const ScalarVar& e2);
    CHECK(AnalyticFunction({x1,x2}, x1-x2).eval(5.,6.) == -1.);
    // .def("__sub__",  {}(const ScalarVar& e1, const Interval& e2);
    CHECK(AnalyticFunction({x1}, x1-Interval(2.)).eval(5.) == 3.);
    // .def("__rsub__", {}(const ScalarVar& e1, const Interval& e2);
    CHECK(AnalyticFunction({x1}, Interval(2.)-x1).eval(5.) == -3.);
    // .def("__mul__",  {}(const ScalarVar& e1, const ScalarVar& e2);
    CHECK(AnalyticFunction({x1,x2}, x1*x2).eval(5.,6.) == 30.);
    // .def("__mul__",  {}(const ScalarVar& e1, const Interval& e2);
    CHECK(AnalyticFunction({x1}, x1*Interval(6.)).eval(5.) == 30.);
    // .def("__rmul__", {}(const ScalarVar& e1, const Interval& e2);
    CHECK(AnalyticFunction({x1}, Interval(6.)*x1).eval(5.) == 30.);
    // .def("__mul__",  {}(const ScalarVar& e1, const VectorVar& e2);
    CHECK(AnalyticFunction({v1,v2}, v1[0]*v2).eval(Vector({5.,10.}),IntervalVector(2,3.)) == Vector(2,15.));
    // .def("__mul__",  {}(const ScalarVar& e1, const IntervalVector& e2);
    CHECK(AnalyticFunction({x1}, x1*IntervalVector({{-2,3},{0,1}})).eval(5.) == IntervalVector({{-10,15},{0,5}}));
    // .def("__truediv__",  {}(const ScalarVar& e1, const ScalarVar& e2);
    CHECK(Approx(AnalyticFunction({x1,x2}, x1/x2).eval(1.,10.)) == 0.1);
    // .def("__truediv__",  {}(const ScalarVar& e1, const Interval& e2);
    CHECK(Approx(AnalyticFunction({x1}, x1/Interval(10)).eval(1.)) == 0.1);
    // .def("__rtruediv__", {}(const ScalarVar& e1, const Interval& e2);
    CHECK(Approx(AnalyticFunction({x1}, Interval(2.)/x1).eval(10.)) == 0.2);

    // ======> VectorVar
    
    //.def("__pos__",  {}(const VectorVar& e1);
    CHECK(AnalyticFunction({v1}, +v1).eval(Vector({5.,6.})) == Vector({5.,6.}));
    //.def("__add__",  {}(const VectorVar& e1, const VectorVar& e2);
    CHECK(AnalyticFunction({v1,v2}, v1+v2).eval(Vector({5.,6.}),Vector({2.,3.})) == Vector({7.,9.}));
    //.def("__add__",  {}(const VectorVar& e1, const IntervalVector& e2);
    CHECK(AnalyticFunction({v1}, v1+IntervalVector({{2},{3}})).eval(Vector({5.,6.})) == IntervalVector({{7.},{9.}}));
    //.def("__radd__", {}(const VectorVar& e1, const IntervalVector& e2);
    CHECK(AnalyticFunction({v1}, IntervalVector({{2},{3}})+v1).eval(Vector({5.,6.})) == IntervalVector({{7.},{9.}}));
    //.def("__neg__",  {}(const VectorVar& e1);
    CHECK(AnalyticFunction({v1}, -v1).eval(Vector({5.,6.})) == -Vector({5.,6.}));
    //.def("__sub__",  {}(const VectorVar& e1, const VectorVar& e2);
    CHECK(AnalyticFunction({v1,v2}, v1-v2).eval(Vector({5.,6.}),Vector({2.,3.})) == Vector({3.,3.}));
    //.def("__sub__",  {}(const VectorVar& e1, const IntervalVector& e2);
    CHECK(AnalyticFunction({v1}, v1-IntervalVector({{2},{3}})).eval(Vector({5.,6.})) == IntervalVector({{3.},{3.}}));
    //.def("__rsub__", {}(const VectorVar& e1, const IntervalVector& e2);
    CHECK(AnalyticFunction({v1}, IntervalVector({{2},{3}})-v1).eval(Vector({5.,6.})) == IntervalVector({{-3.},{-3.}}));

    // ======> ScalarExpr
    
    //.def("__pos__",  {}(const ScalarExpr& e1);
    CHECK(Approx(AnalyticFunction({x1}, +cos(x1)).eval(Interval(0.))) == Interval(1.));
    //.def(py::self + py::self);
    CHECK(Approx(AnalyticFunction({x1}, cos(x1)+cos(x1)).eval(Interval(0.))) == Interval(2.));
    //.def("__add__",  {}(const ScalarExpr& e1, const ScalarVar& e2);
    CHECK(Approx(AnalyticFunction({x1}, cos(x1)+x1).eval(M_PI)) == M_PI-1);
    //.def("__radd__", {}(const ScalarExpr& e1, const ScalarVar& e2);
    CHECK(Approx(AnalyticFunction({x1}, x1+cos(x1)).eval(M_PI)) == M_PI-1);
    //.def("__add__",  {}(const ScalarExpr& e1, const Interval& e2);
    CHECK(Approx(AnalyticFunction({x1}, cos(x1)+Interval(10.)).eval(M_PI)) == Interval(9));
    //.def("__radd__", {}(const ScalarExpr& e1, const Interval& e2);
    CHECK(Approx(AnalyticFunction({x1}, 10+cos(x1)).eval(M_PI)) == Interval(9));
    //.def(- py::self);
    CHECK(Approx(AnalyticFunction({x1}, -cos(x1)).eval(Interval(0.))) == Interval(-1.));
    //.def(py::self - py::self);
    CHECK(Approx(AnalyticFunction({x1}, cos(x1)-cos(x1)).eval(Interval(0.))) == Interval(0.));
    //.def("__sub__",  {}(const ScalarExpr& e1, const ScalarVar& e2);
    CHECK(Approx(AnalyticFunction({x1}, cos(x1)-x1).eval(M_PI)) == -M_PI-1);
    //.def("__rsub__", {}(const ScalarExpr& e1, const ScalarVar& e2);
    CHECK(Approx(AnalyticFunction({x1}, x1-cos(x1)).eval(M_PI)) == M_PI+1);
    //.def("__sub__",  {}(const ScalarExpr& e1, const Interval& e2);
    CHECK(Approx(AnalyticFunction({x1}, cos(x1)-Interval(10.)).eval(M_PI)) == -Interval(11));
    //.def("__rsub__", {}(const ScalarExpr& e1, const Interval& e2);
    CHECK(Approx(AnalyticFunction({x1}, 10-cos(x1)).eval(M_PI)) == Interval(11));
    //.def(py::self * py::self);
    CHECK(Approx(AnalyticFunction({x1}, cos(x1)*cos(x1)).eval(Interval(0.))) == Interval(1.));
    //.def("__mul__",  {}(const ScalarExpr& e1, const ScalarVar& e2);
    CHECK(Approx(AnalyticFunction({x1}, cos(x1)*x1).eval(M_PI)) == -1*M_PI);
    //.def("__rmul__", {}(const ScalarExpr& e1, const ScalarVar& e2);
    CHECK(Approx(AnalyticFunction({x1}, x1*cos(x1)).eval(M_PI)) == -1*M_PI);
    //.def("__mul__",  {}(const ScalarExpr& e1, const Interval& e2);
    CHECK(Approx(AnalyticFunction({x1}, cos(x1)*Interval(10.)).eval(M_PI),1e-9) == -Interval(10));
    //.def("__rmul__", {}(const ScalarExpr& e1, const Interval& e2);
    CHECK(Approx(AnalyticFunction({x1}, Interval(10.)*cos(x1)).eval(M_PI),1e-9) == -10);
    //.def("__mul__",  {}(const ScalarExpr& e1, const VectorExpr& e2);
    CHECK(Approx(AnalyticFunction({v1,v2}, cos(v1[0])*(v2+v2)).eval(Vector({M_PI,-1}),Vector({2,3})),1e-9) == IntervalVector({{-4},{-6}}));
    //.def("__truediv__",  {}(const ScalarExpr& e1, const ScalarExpr& e2);
    CHECK(Approx(AnalyticFunction({x1}, cos(x1)/cos(x1)).eval(Interval(0.))) == Interval(1.));
    //.def("__truediv__",  {}(const ScalarExpr& e1, const ScalarVar& e2);
    CHECK(Approx(AnalyticFunction({x1}, cos(x1)/x1).eval(M_PI)) == -1/M_PI);
    //.def("__rtruediv__", {}(const ScalarExpr& e1, const ScalarVar& e2);
    CHECK(Approx(AnalyticFunction({x1}, x1/cos(x1)).eval(M_PI)) == -M_PI);
    //.def("__truediv__",  {}(const ScalarExpr& e1, const Interval& e2);
    CHECK(Approx(AnalyticFunction({x1}, cos(x1)/Interval(4.)).eval(M_PI)) == -1./4);
    //.def("__rtruediv__", {}(const ScalarExpr& e1, const Interval& e2);
    CHECK(Approx(AnalyticFunction({x1}, 4./cos(x1)).eval(M_PI)) == -4);

    // ======> VectorExpr

    //.def("__pos__",  {}(const VectorExpr& e1);
    CHECK(Approx(AnalyticFunction({v1}, +(v1+v1)).eval(IntervalVector({{0.},{-oo,5}}))) == IntervalVector({{0.},{-oo,10}}));
    //.def(py::self + py::self);
    CHECK(Approx(AnalyticFunction({v1}, v1+v1).eval(IntervalVector({{0.},{-oo,5}}))) == IntervalVector({{0.},{-oo,10}}));
    //.def("__radd__", {}(const VectorExpr& e1, const VectorVar& e2);
    CHECK(Approx(AnalyticFunction({v1}, (v1+v1)+v1).eval(IntervalVector({{0.},{-oo,5}}))) == IntervalVector({{0.},{-oo,15}}));
    //.def("__radd__", {}(const VectorExpr& e1, const IntervalVector& e2);
    CHECK(Approx(AnalyticFunction({v1}, v1+(v1+v1)).eval(IntervalVector({{0.},{-oo,5}}))) == IntervalVector({{0.},{-oo,15}}));
    //.def(- py::self);
    CHECK(Approx(AnalyticFunction({v1}, -(v1+v1)).eval(IntervalVector({{0.},{-oo,5}}))) == -IntervalVector({{0.},{-oo,10}}));
    //.def(py::self - py::self);
    CHECK(Approx(AnalyticFunction({v1,v2}, (v1-v2)).eval(IntervalVector({{2},{3}}),Vector({1,5}))) == IntervalVector({{1},{-2}}));
    //.def("__sub__",  {}(const VectorExpr& e1, const VectorVar& e2);
    CHECK(Approx(AnalyticFunction({v1,v2}, (v1-v2)-v1).eval(IntervalVector({{2},{3}}),Vector({1,5}))) == IntervalVector({{-1},{-5}}));
    //.def("__rsub__", {}(const VectorExpr& e1, const VectorVar& e2);
    CHECK(Approx(AnalyticFunction({v1,v2}, v2-(v1-v2)).eval(IntervalVector({{2},{3}}),Vector({1,5}))) == IntervalVector({{0},{7}}));
    //.def("__sub__",  {}(const VectorExpr& e1, const IntervalVector& e2);
    CHECK(Approx(AnalyticFunction({v1,v2}, (v1-v2)-IntervalVector({{2},{3}})).eval(IntervalVector({{2},{3}}),Vector({1,5}))) == IntervalVector({{-1},{-5}}));
    //.def("__rsub__", {}(const VectorExpr& e1, const IntervalVector& e2);
    CHECK(Approx(AnalyticFunction({v1,v2}, Vector({1,5})-(v1-v2)).eval(IntervalVector({{2},{3}}),Vector({1,5}))) == IntervalVector({{0},{7}}));
    //.def("__rmul__", {}(const VectorExpr& e1, const ScalarExpr& e2);
    CHECK(Approx(AnalyticFunction({v1,v2}, cos(v1[0])*(v2+v2)).eval(Vector({M_PI,-1}),Vector({2,3})),1e-9) == IntervalVector({{-4},{-6}}));
    //.def("__rmul__", {}(const VectorExpr& e1, const ScalarVar& e2);
    CHECK(Approx(AnalyticFunction({x1}, x1*vec(3*x1,2*x1)).eval(3),1e-9) == IntervalVector({{27},{18}}));
  }

  {
    VectorVar x(2);
    AnalyticFunction f({x}, x[0]*(x[0]+x[1])+sqr(x[1]));
    CHECK(f.diff(Vector({2,3}))(0,0) == 7);
    CHECK(f.diff(Vector({2,3}))(0,1) == 8);
  }

  { // int values
    ScalarVar x, y;
    AnalyticFunction f({x,y}, x*(x+y)+sqr(y));
    CHECK(f.diff(2,3)(0,0) == 7);
    CHECK(f.diff(2,3)(0,1) == 8);
  }

  { // double values
    ScalarVar x, y;
    AnalyticFunction f({x,y}, x*(x+y)+sqr(y));
    CHECK(f.diff(2.,3.)(0,0) == 7);
    CHECK(f.diff(2.,3.)(0,1) == 8);
  }

  { // Interval values
    ScalarVar x, y;
    AnalyticFunction f({x,y}, x*(x+y)+sqr(y));
    CHECK(f.diff(Interval(2.),Interval(3.))(0,0) == 7);
    CHECK(f.diff(Interval(2.),Interval(3.))(0,1) == 8);
  }

}