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

TEST_CASE("AnalyticFunction - manual")
{
  {
    // [1-beg]
    ScalarVar x1, x2;
    VectorVar v(3);

    // Example of scalar function: from R to R
    AnalyticFunction f1({x1}, x1*cos(x1));

    // Example of vectorial function: from R to R²
    AnalyticFunction f2({x1}, { x1*cos(x1), x1*sin(x1) });

    // Example of vectorial function: from R³ to R²
    AnalyticFunction f3({v}, Interval(-1,1)*v);

    // Example of multivariate vectorial function: from R×R to R³
    AnalyticFunction f4({x1,x2}, { x1+x2, Interval(0,1)*exp(x1), x2^(1+x1) });
    // [1-end]


    // [2-beg]
    // Example of function: from R³ to R²
    AnalyticFunction f5({v}, (v[0]*v).subvector(1,2));

    // Example of scalar function: from R^(2x2) to R
    MatrixVar M(2,2);
    AnalyticFunction f6({M}, M(0,0)*M(1,1)-M(1,0)*M(0,1));
    // [2-end]

    CHECK(f5.eval(Vector({5,6,7})) == Vector({30,35}));
    CHECK(f6.eval(Matrix({{1,2},{3,4}})) == -2);


    /*
    // [3-beg]
    auto create_f()
    {
      ScalarVar x;
      return AnalyticFunction({x}, x*cos(x));
    }

    auto f = create_f(); // x is no longer useful here
    // [3-end]
    */

    CHECK(Approx([]() {
      ScalarVar x;
      return AnalyticFunction({x}, x*cos(x));
      }().eval(PI)) == -PI);


    // [4-beg]
    AnalyticFunction f({x1}, x1*cos(x1));
    AnalyticFunction g({x1}, { f(2*x1), x1*sin(x1) });
    AnalyticFunction h({x1}, g(x1)[0]); // output is 2*x*cos(2*x)
    // [4-end]

    CHECK(Approx(h.eval(PI)) == 2*PI);
  }

  {
    // [4b-beg]
    ScalarVar a;
    VectorVar v(2);

    AnalyticFunction Rot({a}, mat(vec(cos(a),sin(a)),vec(-sin(a),cos(a))));
    AnalyticFunction f({v}, Rot(PI/4)*v);

    IntervalVector y = f.eval(Vector({1,1})); // y == [ [-2.22045e-16, 4.4409e-16] ; [1.41421, 1.41422] ]
    // [4b-end]

    CHECK(Approx(y,1e-5) == Vector({0,1.41421}));
  }

  {
    // [5-beg]
    ScalarVar x1; // scalar argument
    AnalyticFunction f1({x1}, x1*cos(x1));

    Interval y1 = f1.eval(0.);
    y1 = f1.eval(PI);
    y1 = f1.eval(Interval(0,1));

    ScalarVar x2;
    AnalyticFunction f2({x1,x2}, x1^x2); // example of multivariate function

    Interval y2 = f2.eval(Interval(2,3), 2);

    VectorVar v(3), w(3); // vector arguments
    AnalyticFunction f3({v,w}, v-w); // example of vectorial function

    IntervalVector y3 = f3.eval(Vector({5,4,3}), IntervalVector({{3,oo},{2},{1,2}}));
    // [5-end]

    CHECK(f1.eval(0.) == 0);
    CHECK(Approx(f1.eval(PI),1e-6) == Interval(-PI,-PI));
    CHECK(Approx(f1.eval(Interval(0,1)),1e-6) == Interval(0, 0.938792));
    CHECK(y2 == Interval(4,9));
    CHECK(y3 == IntervalVector({{-oo,2},2,{1,2}}));


    // [6-beg]
    f1.eval(EvalMode::NATURAL, Interval(0,1));
    f1.eval(EvalMode::CENTERED, Interval(0,1));

    f1.eval(EvalMode::NATURAL | EvalMode::CENTERED, Interval(0,1));
    // which is equivalent to: f1.eval(Interval(0,1));
    // [6-end]

    CHECK(Approx(f1.eval(EvalMode::NATURAL, Interval(0,1)), 1e-6) == Interval(0,1));
    CHECK(Approx(f1.eval(EvalMode::CENTERED, Interval(0,1)), 1e-6) == Interval(-0.0612088, 0.938792));
    CHECK(Approx(f1.eval(EvalMode::NATURAL | EvalMode::CENTERED, Interval(0,1)), 1e-6) == Interval(0, 0.938792));
  }

  {
    // [7-beg]
    ScalarVar x1;
    AnalyticFunction f1({x1}, x1*cos(x1));
    IntervalMatrix J1 = f1.diff(Interval(0,PI/2));
    // J1 = intv. matrix 1x1: [[ [-(PI/2),1] ]]

    ScalarVar x2;
    AnalyticFunction f2({x1,x2}, x1^x2); // example of multivariate function
    IntervalMatrix J2 = f2.diff(2.,Interval(2,3));
    // J2 = intv. matrix 1x2: [[ [4,12], [2.77258,5.54518] ]]

    VectorVar v(3);
    AnalyticFunction f3({v}, { // vectorial function
      v[0]-(v[1]^2),
      Interval(-1,0)*v[2]
    });
    IntervalMatrix J3 = f3.diff(Vector({5,8,10}));
    // J3 = intv. matrix 2x3: [[ 1,-16,0 ],[ 0,0,[-1,0] ]]
    // [7-end]

    CHECK(Approx(J1) == IntervalMatrix({{{-(PI/2),1}}}));
    CHECK(Approx(J2,1e-6) == IntervalMatrix({{{4,12},{2.7725887222,5.5451774445}}}));
    CHECK(J3 == IntervalMatrix({{1,-16,0},{0,0,{-1,0}}}));

    AnalyticFunction f = f3;

    // [8-beg]
    int n = f.input_size();
    int m = f.output_size();
    // [8-end]

    CHECK(n == 3);
    CHECK(m == 2);

    // [9-beg]
    // Forward evaluation
    Interval y = CosOp::fwd({0,PI/2}); // y = [0,1]

    // Backward evaluation
    Interval x(0,PI); // prior value of [x]
    CosOp::bwd({0,0.5}, x); // [x] is contracted to [PI/3,PI/2]
    // [9-end]

    CHECK(Approx(y) == Interval(0,1));
    CHECK(Approx(x) == Interval(PI/3,PI/2));
  }
}
