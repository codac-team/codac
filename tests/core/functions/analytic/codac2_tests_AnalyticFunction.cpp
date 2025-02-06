/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_AnalyticFunction.h>
#include <codac2_Approx.h>
#include <codac2_math.h>

using namespace std;
using namespace codac2;

TEST_CASE("AnalyticFunction")
{
  std::array<EvalMode,3> modes {
    EvalMode::NATURAL,
    EvalMode::CENTERED,
    EvalMode::NATURAL | EvalMode::NATURAL
  };

  for(const auto& m : modes) // try all available evaluation modes
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
        CHECK(Approx(f.eval(m, Interval(0))) == 0);
      }
      {
        AnalyticFunction f({x}, x+Interval(2));
        CHECK(Approx(f.eval(m, Interval(3))) == 5);
      }
      {
        AnalyticFunction f({x}, x+x);
        CHECK(Approx(f.eval(m, Interval(2))) == 4);
      }
      {
        AnalyticFunction f({x}, x+x+2);
        CHECK(Approx(f.eval(m, Interval(2))) == 6);
      }
      {
        AnalyticFunction f({x}, pow(x,2));
        CHECK(Approx(f.eval(m, Interval(3))) == 9);
      }
      {
        AnalyticFunction f({x}, cos(x));
        CHECK(Approx(f.eval(m, Interval(0))) == 1);
      }

      AnalyticFunction f({x}, vec(x,x));

      AnalyticFunction fvec({x}, vec(x,x));
      CHECK(Approx(f.eval(m, 1)) == IntervalVector({1,1}));
    }

    {
      CHECK(AnalyticFunction({}, +Interval(4,5)).eval(m) == Interval(4,5));
      CHECK(AnalyticFunction({}, +4).eval(m) == 4);
      CHECK(AnalyticFunction({}, +4.).eval(m) == 4.);
      CHECK(AnalyticFunction({}, Vector({2,9})).eval(m) == Vector({2,9}));
      CHECK(AnalyticFunction({}, IntervalVector(3)).eval(m) == IntervalVector({{-oo,oo},{-oo,oo},{-oo,oo}}));

      ScalarVar x1;
      ScalarVar x2;
      VectorVar v1(2);
      VectorVar v2(2);

      // ======> ScalarVar

      // .def("__pos__",  {}(const ScalarVar& e1);
      CHECK(AnalyticFunction({x1}, +x1).eval(m, 5.) == 5.);
      // .def("__add__",  {}(const ScalarVar& e1, const ScalarVar& e2);
      CHECK(AnalyticFunction({x1,x2}, x1+x2).eval(m, 5.,6.) == 11.);
      // .def("__add__",  {}(const ScalarVar& e1, const Interval& e2);
      CHECK(AnalyticFunction({x1}, x1+Interval(3)).eval(m, 5.) == 8.);
      // .def("__radd__", {}(const ScalarVar& e1, const Interval& e2);
      CHECK(AnalyticFunction({x1}, Interval(3)+x1).eval(m, 5.) == 8.);
      // .def("__neg__",  {}(const ScalarVar& e1);
      CHECK(AnalyticFunction({x2}, -x2).eval(m, 6.) == -6.);
      // .def("__sub__",  {}(const ScalarVar& e1, const ScalarVar& e2);
      CHECK(AnalyticFunction({x1,x2}, x1-x2).eval(m, 5.,6.) == -1.);
      // .def("__sub__",  {}(const ScalarVar& e1, const Interval& e2);
      CHECK(AnalyticFunction({x1}, x1-Interval(2.)).eval(m, 5.) == 3.);
      // .def("__rsub__", {}(const ScalarVar& e1, const Interval& e2);
      CHECK(AnalyticFunction({x1}, Interval(2.)-x1).eval(m, 5.) == -3.);
      // .def("__mul__",  {}(const ScalarVar& e1, const ScalarVar& e2);
      CHECK(AnalyticFunction({x1,x2}, x1*x2).eval(m, 5.,6.) == 30.);
      // .def("__mul__",  {}(const ScalarVar& e1, const Interval& e2);
      CHECK(AnalyticFunction({x1}, x1*Interval(6.)).eval(m, 5.) == 30.);
      // .def("__rmul__", {}(const ScalarVar& e1, const Interval& e2);
      CHECK(AnalyticFunction({x1}, Interval(6.)*x1).eval(m, 5.) == 30.);
      // .def("__mul__",  {}(const ScalarVar& e1, const VectorVar& e2);
      CHECK(AnalyticFunction({v1,v2}, v1[0]*v2).eval(m, Vector({5.,10.}),IntervalVector({3,3})) == Vector({15,15}));
      // .def("__mul__",  {}(const ScalarVar& e1, const IntervalVector& e2);
      CHECK(AnalyticFunction({x1}, x1*IntervalVector({{-2,3},{0,1}})).eval(EvalMode::CENTERED,5.) == IntervalVector({{-10,15},{0,5}}));
      // .def("__truediv__",  {}(const ScalarVar& e1, const ScalarVar& e2);
      CHECK(Approx(AnalyticFunction({x1,x2}, x1/x2).eval(m, 1.,10.)) == 0.1);
      // .def("__truediv__",  {}(const ScalarVar& e1, const Interval& e2);
      CHECK(Approx(AnalyticFunction({x1}, x1/Interval(10)).eval(m, 1.)) == 0.1);
      // .def("__rtruediv__", {}(const ScalarVar& e1, const Interval& e2);
      CHECK(Approx(AnalyticFunction({x1}, Interval(2.)/x1).eval(m, 10.)) == 0.2);

      // ======> VectorVar
      CHECK(-IntervalVector({-5.,-6.}) == Vector({5.,6.}));
      
      //.def("__pos__",  {}(const VectorVar& e1);
      CHECK(AnalyticFunction({v1}, +v1).eval(m, Vector({5.,6.})) == Vector({5.,6.}));
      //.def("__add__",  {}(const VectorVar& e1, const VectorVar& e2);
      CHECK(AnalyticFunction({v1,v2}, v1+v2).eval(m, Vector({5.,6.}),Vector({2.,3.})) == Vector({7.,9.}));
      //.def("__add__",  {}(const VectorVar& e1, const IntervalVector& e2);
      CHECK(AnalyticFunction({v1}, v1+IntervalVector({2,3})).eval(m, Vector({5.,6.})) == IntervalVector({7.,9.}));
      //.def("__radd__", {}(const VectorVar& e1, const IntervalVector& e2);
      CHECK(AnalyticFunction({v1}, IntervalVector({2,3})+v1).eval(m, Vector({5.,6.})) == IntervalVector({7.,9.}));
      //.def("__neg__",  {}(const VectorVar& e1);
      CHECK(AnalyticFunction({v1}, -v1).eval(m, Vector({5.,6.})) == -Vector({5.,6.}));
      //.def("__sub__",  {}(const VectorVar& e1, const VectorVar& e2);
      CHECK(AnalyticFunction({v1,v2}, v1-v2).eval(m, Vector({5.,6.}),Vector({2.,3.})) == Vector({3.,3.}));
      //.def("__sub__",  {}(const VectorVar& e1, const IntervalVector& e2);
      CHECK(AnalyticFunction({v1}, v1-IntervalVector({2,3})).eval(m, Vector({5.,6.})) == IntervalVector({3.,3.}));
      //.def("__rsub__", {}(const VectorVar& e1, const IntervalVector& e2);
      CHECK(AnalyticFunction({v1}, IntervalVector({2,3})-v1).eval(m, Vector({5.,6.})) == IntervalVector({-3.,-3.}));

      // ======> ScalarExpr
      
      //.def("__pos__",  {}(const ScalarExpr& e1);
      CHECK(Approx(AnalyticFunction({x1}, +cos(x1)).eval(m, Interval(0.))) == Interval(1.));
      //.def(py::self + py::self);
      CHECK(Approx(AnalyticFunction({x1}, cos(x1)+cos(x1)).eval(m, Interval(0.))) == Interval(2.));
      //.def("__add__",  {}(const ScalarExpr& e1, const ScalarVar& e2);
      CHECK(Approx(AnalyticFunction({x1}, cos(x1)+x1).eval(m, PI)) == PI-1);
      //.def("__radd__", {}(const ScalarExpr& e1, const ScalarVar& e2);
      CHECK(Approx(AnalyticFunction({x1}, x1+cos(x1)).eval(m, PI)) == PI-1);
      //.def("__add__",  {}(const ScalarExpr& e1, const Interval& e2);
      CHECK(Approx(AnalyticFunction({x1}, cos(x1)+Interval(10.)).eval(m, PI)) == Interval(9));
      //.def("__radd__", {}(const ScalarExpr& e1, const Interval& e2);
      CHECK(Approx(AnalyticFunction({x1}, 10+cos(x1)).eval(m, PI)) == Interval(9));
      //.def(- py::self);
      CHECK(Approx(AnalyticFunction({x1}, -cos(x1)).eval(m, Interval(0.))) == Interval(-1.));
      //.def(py::self - py::self);
      CHECK(Approx(AnalyticFunction({x1}, cos(x1)-cos(x1)).eval(m, Interval(0.))) == Interval(0.));
      //.def("__sub__",  {}(const ScalarExpr& e1, const ScalarVar& e2);
      CHECK(Approx(AnalyticFunction({x1}, cos(x1)-x1).eval(m, PI)) == -PI-1);
      //.def("__rsub__", {}(const ScalarExpr& e1, const ScalarVar& e2);
      CHECK(Approx(AnalyticFunction({x1}, x1-cos(x1)).eval(m, PI)) == PI+1);
      //.def("__sub__",  {}(const ScalarExpr& e1, const Interval& e2);
      CHECK(Approx(AnalyticFunction({x1}, cos(x1)-Interval(10.)).eval(m, PI)) == -Interval(11));
      //.def("__rsub__", {}(const ScalarExpr& e1, const Interval& e2);
      CHECK(Approx(AnalyticFunction({x1}, 10-cos(x1)).eval(m, PI)) == Interval(11));
      //.def(py::self * py::self);
      CHECK(Approx(AnalyticFunction({x1}, cos(x1)*cos(x1)).eval(m, Interval(0.))) == Interval(1.));
      //.def("__mul__",  {}(const ScalarExpr& e1, const ScalarVar& e2);
      CHECK(Approx(AnalyticFunction({x1}, cos(x1)*x1).eval(m, PI)) == -1*PI);
      //.def("__rmul__", {}(const ScalarExpr& e1, const ScalarVar& e2);
      CHECK(Approx(AnalyticFunction({x1}, x1*cos(x1)).eval(m, PI)) == -1*PI);
      //.def("__mul__",  {}(const ScalarExpr& e1, const Interval& e2);
      CHECK(Approx(AnalyticFunction({x1}, cos(x1)*Interval(10.)).eval(m, PI),1e-9) == -Interval(10));
      //.def("__rmul__", {}(const ScalarExpr& e1, const Interval& e2);
      CHECK(Approx(AnalyticFunction({x1}, Interval(10.)*cos(x1)).eval(m, PI),1e-9) == -10);
      //.def("__mul__",  {}(const ScalarExpr& e1, const VectorExpr& e2);
      CHECK(Approx(AnalyticFunction({v1,v2}, cos(v1[0])*(v2+v2)).eval(m, Vector({PI,-1}),Vector({2,3})),1e-9) == IntervalVector({-4,-6}));
      //.def("__truediv__",  {}(const ScalarExpr& e1, const ScalarExpr& e2);
      CHECK(Approx(AnalyticFunction({x1}, cos(x1)/cos(x1)).eval(m, Interval(0.))) == Interval(1.));
      //.def("__truediv__",  {}(const ScalarExpr& e1, const ScalarVar& e2);
      CHECK(Approx(AnalyticFunction({x1}, cos(x1)/x1).eval(m, PI)) == -1/PI);
      //.def("__rtruediv__", {}(const ScalarExpr& e1, const ScalarVar& e2);
      CHECK(Approx(AnalyticFunction({x1}, x1/cos(x1)).eval(m, PI)) == -PI);
      //.def("__truediv__",  {}(const ScalarExpr& e1, const Interval& e2);
      CHECK(Approx(AnalyticFunction({x1}, cos(x1)/Interval(4.)).eval(m, PI)) == -1./4);
      //.def("__rtruediv__", {}(const ScalarExpr& e1, const Interval& e2);
      CHECK(Approx(AnalyticFunction({x1}, 4./cos(x1)).eval(m, PI)) == -4);

      // ======> VectorExpr

      //.def("__pos__",  {}(const VectorExpr& e1);
      CHECK(Approx(AnalyticFunction({v1}, +(v1+v1)).eval(m, IntervalVector({{0.},{-oo,5}}))) == 
        (m == EvalMode::CENTERED ? IntervalVector({{0.},{-oo,oo}}) : IntervalVector({{0.},{-oo,10}})));
      //.def(py::self + py::self);
      CHECK(Approx(AnalyticFunction({v1}, v1+v1).eval(m, IntervalVector({{0.},{-oo,5}}))) == 
        (m == EvalMode::CENTERED ? IntervalVector({{0.},{-oo,oo}}) : IntervalVector({{0.},{-oo,10}})));
      //.def("__radd__", {}(const VectorExpr& e1, const VectorVar& e2);
      CHECK(Approx(AnalyticFunction({v1}, (v1+v1)+v1).eval(m, IntervalVector({{0.},{-oo,5}}))) == 
        (m == EvalMode::CENTERED ? IntervalVector({{0.},{-oo,oo}}) : IntervalVector({{0.},{-oo,15}})));
      //.def("__radd__", {}(const VectorExpr& e1, const IntervalVector& e2);
      CHECK(Approx(AnalyticFunction({v1}, v1+(v1+v1)).eval(m, IntervalVector({{0.},{-oo,5}}))) == 
        (m == EvalMode::CENTERED ? IntervalVector({{0.},{-oo,oo}}) : IntervalVector({{0.},{-oo,15}})));
      //.def(- py::self);
      CHECK(Approx(AnalyticFunction({v1}, -(v1+v1)).eval(m, IntervalVector({{0.},{-oo,5}}))) == 
        (m == EvalMode::CENTERED ? IntervalVector({{0.},{-oo,oo}}) : IntervalVector({{0.},{-10,oo}})));
      //.def(py::self - py::self);
      CHECK(Approx(AnalyticFunction({v1,v2}, (v1-v2)).eval(m, IntervalVector({2,3}),Vector({1,5}))) == IntervalVector({1,-2}));
      //.def("__sub__",  {}(const VectorExpr& e1, const VectorVar& e2);
      CHECK(Approx(AnalyticFunction({v1,v2}, (v1-v2)-v1).eval(m, IntervalVector({2,3}),Vector({1,5}))) == IntervalVector({-1,-5}));
      //.def("__rsub__", {}(const VectorExpr& e1, const VectorVar& e2);
      CHECK(Approx(AnalyticFunction({v1,v2}, v2-(v1-v2)).eval(m, IntervalVector({2,3}),Vector({1,5}))) == IntervalVector({0,7}));
      //.def("__sub__",  {}(const VectorExpr& e1, const IntervalVector& e2);
      CHECK(Approx(AnalyticFunction({v1,v2}, (v1-v2)-IntervalVector({2,3})).eval(m, IntervalVector({2,3}),Vector({1,5}))) == IntervalVector({-1,-5}));
      //.def("__rsub__", {}(const VectorExpr& e1, const IntervalVector& e2);
      CHECK(Approx(AnalyticFunction({v1,v2}, Vector({1,5})-(v1-v2)).eval(m, IntervalVector({2,3}),Vector({1,5}))) == IntervalVector({0,7}));
      //.def("__rmul__", {}(const VectorExpr& e1, const ScalarExpr& e2);
      CHECK(Approx(AnalyticFunction({v1,v2}, cos(v1[0])*(v2+v2)).eval(m, Vector({PI,-1}),Vector({2,3})),1e-9) == IntervalVector({-4,-6}));
      //.def("__rmul__", {}(const VectorExpr& e1, const ScalarVar& e2);
      CHECK(Approx(AnalyticFunction({x1}, x1*vec(3*x1,2*x1)).eval(m, 3),1e-9) == IntervalVector({27,18}));
    }
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

  {
    ScalarVar x;
    AnalyticFunction f({x}, x-x);
    CHECK(f.eval(EvalMode::NATURAL,Interval(-1,1)) == Interval(-2,2));
    CHECK(f.eval(EvalMode::CENTERED,Interval(-1,1)) == Interval(0));
    CHECK(f.eval(Interval(-1,1)) == Interval(0));
  }

  {
    // Scalar outputs
    {
      AnalyticFunction f1({}, (int)3);
      CHECK(f1.eval() == Interval(3));

      AnalyticFunction f2({}, (double)3.);
      CHECK(f2.eval() == Interval(3));

      AnalyticFunction f3({}, Interval(3.));
      CHECK(f3.eval() == Interval(3));

      ScalarVar x;
      AnalyticFunction f4({x}, x*x);
      CHECK(f4.eval(2.) == Interval(4));
    }

    // Vectorial outputs
    {
      AnalyticFunction f1({}, { (int)3 });
      CHECK(f1.eval() == IntervalVector({3}));

      AnalyticFunction f2({}, { (double)3. });
      CHECK(f2.eval() == IntervalVector({3}));

      AnalyticFunction f3({}, { Interval(3.) });
      CHECK(f3.eval() == IntervalVector({3}));

      ScalarVar x;
      AnalyticFunction f4({x}, { x*x });
      CHECK(f4.eval(2.) == IntervalVector({4}));

      AnalyticFunction f_2args({x}, { x*x,x*x });
      CHECK(f_2args.eval(1.) == IntervalVector::constant(2,{1}));
      AnalyticFunction f_3args({x}, { x,x*x,1 });
      CHECK(f_3args.eval(1.) == IntervalVector::constant(3,{1}));
      AnalyticFunction f_4args({x}, { x,x*x,1,x });
      CHECK(f_4args.eval(1.) == IntervalVector::constant(4,{1}));
      AnalyticFunction f_5args({x}, { x,x*x,1,x,x });
      CHECK(f_5args.eval(1.) == IntervalVector::constant(5,{1}));
      AnalyticFunction f_6args({x}, { x,x*x,1,x,x,1*x });
      CHECK(f_6args.eval(1.) == IntervalVector::constant(6,{1}));
      AnalyticFunction f_7args({x}, { x,x*x,1,x,x,1*x,x*x });
      CHECK(f_7args.eval(1.) == IntervalVector::constant(7,{1}));
      AnalyticFunction f_8args({x}, { x,x*x,1,x,x,1*x,x*x,1 });
      CHECK(f_8args.eval(1.) == IntervalVector::constant(8,{1}));
      AnalyticFunction f_9args({x}, { x,x*x,1,x,x,1*x,x*x,1,x });
      CHECK(f_9args.eval(1.) == IntervalVector::constant(9,{1}));
      AnalyticFunction f_10args({x}, { x,x*x,1,x,x,1*x,x*x,1,x,1*x });
      CHECK(f_10args.eval(1.) == IntervalVector::constant(10,{1}));
    }
  }

  // Subvector on variables
  {
    VectorVar p(2);
    VectorVar x(4);
    AnalyticFunction f({p}, p[0]*p[1]);
    AnalyticFunction g({x}, f(x.subvector(0,1)) + f(x.subvector(2,3)));

    IntervalVector a(4);

    a = IntervalVector({1,2,3,4});
    CHECK(g.eval(EvalMode::NATURAL,a) == 14);
    CHECK(g.eval(EvalMode::CENTERED,a) == 14);
    CHECK(g.eval(a) == 14);

    a = IntervalVector({0,2,5,4});
    CHECK(g.eval(EvalMode::NATURAL,a) == 20);
    CHECK(g.eval(EvalMode::CENTERED,a) == 20);
    CHECK(g.eval(a) == 20);
  }

  // Sign, floor, ceil, min, max
  {
    ScalarVar x1, x2;

    {
      AnalyticFunction f({x1,x2}, 2*max(x1,x2+1));
      CHECK(f.eval(0.,1.) == 4.);
      CHECK(f.eval(2.,1.) == 4.);
      CHECK(f.eval(3.,1.) == 6.);
    }

    {
      AnalyticFunction f({x1,x2}, 2*min(x1,x2+1));
      CHECK(f.eval(0.,1.) == 0.);
      CHECK(f.eval(2.,1.) == 4.);
      CHECK(f.eval(3.,1.) == 4.);
    }

    {
      AnalyticFunction f({x1}, 2*sign(x1+1));
      CHECK(f.eval(0.) == 2.);
      CHECK(sign(Interval::zero()) == Interval(-1,1));
      CHECK(sign(Interval(-0,0)) == Interval(-1,1));
      CHECK(f.eval(-1.) == Interval(-2,2));
      CHECK(f.eval(-2.) == -2.);
    }

    {
      AnalyticFunction f({x1}, 2*floor(x1));
      CHECK(f.eval(0.) == 0.);
      CHECK(f.eval(1.5) == 2.);
      CHECK(f.eval(-1.5) == -4.);
    }

    {
      AnalyticFunction f({x1}, 2*ceil(x1));
      CHECK(f.eval(0.) == 0.);
      CHECK(f.eval(1.5) == 4.);
      CHECK(f.eval(-1.5) == -2.);
    }
  }
}