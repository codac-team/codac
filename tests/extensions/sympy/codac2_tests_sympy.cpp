/**
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Simon Rohou
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_sympy.h>

using namespace codac2;

TEST_CASE("Sympy")
{
  {
    ScalarVar x;
    AnalyticFunction f({x}, cos(x)*x + sin(x));
    auto df = sympy_diff(f);
    auto dfx = sympy_diff(f, x);

    CHECK(sympy_equal(df, AnalyticFunction({x}, 2.*cos(x) - x*sin(x))));
    CHECK(sympy_equal(dfx, AnalyticFunction({x}, 2.*cos(x) - x*sin(x))));
  }

  {
    ScalarVar x;
    ScalarVar y;
    AnalyticFunction f({x,y}, x*y + sin(x));

    auto dfdx = sympy_partial_diff(f, x);
    auto dfdy = sympy_partial_diff(f, y);

    CHECK(sympy_equal(dfdx, AnalyticFunction({x,y}, y + cos(x))));
    CHECK(sympy_equal(dfdy, AnalyticFunction({x,y}, x)));
  }

  {
    VectorVar v(2);
    AnalyticFunction g({v}, v[0]*v[1] + sin(v[0]));

    auto dg_dv0 = sympy_partial_diff(g, v[0]);
    auto dg_dv1 = sympy_partial_diff(g, v[1]);
    auto grad_g = sympy_gradient(g);

    CHECK(sympy_equal(dg_dv0, AnalyticFunction({v}, v[1] + cos(v[0]))));
    CHECK(sympy_equal(dg_dv1, AnalyticFunction({v}, v[0])));
    CHECK(sympy_equal(grad_g, AnalyticFunction({v}, vec(v[1] + cos(v[0]), v[0]))));
  }

  {
    VectorVar v(2);
    AnalyticFunction f({v}, {
      v[0]*v[1] + sin(v[0]),
      sqr(v[0]) + cos(v[1])
    });

    auto J = sympy_diff(f);

    CHECK(sympy_equal(
      J,
      AnalyticFunction(
        {v},
        mat(
          vec(v[1] + cos(v[0]), 2.*v[0]),
          vec(v[0], -sin(v[1]))
        ))));
  }

  {
    ScalarVar x;
    AnalyticFunction f({x}, sqr(sin(x)) + sqr(cos(x)));
    auto fs = sympy_simplify(f);
    auto dfs = sympy_diff(fs);

    CHECK(sympy_equal(fs, AnalyticFunction({x}, 1.)));
    CHECK(sympy_equal(dfs, AnalyticFunction({x}, 0.)));
  }

  {
    VectorVar v(2);
    AnalyticFunction f({v}, {
      v[0] + v[0],
      sqr(sin(v[1])) + sqr(cos(v[1]))
    });

    auto fs = sympy_simplify(f);

    CHECK(sympy_equal(fs, AnalyticFunction({v}, {
      2.*v[0],
      1.
    })));
  }

  {
    ScalarVar x;
    AnalyticFunction f({x}, sin(x));

    auto d3f = sympy_diff(f, x, 3);
    auto d0f = sympy_diff(f, x, 0);

    CHECK(sympy_equal(d3f, AnalyticFunction({x}, -cos(x))));
    CHECK(sympy_equal(d0f, f));
  }

  {
    ScalarVar x;
    ScalarVar y;
    AnalyticFunction f({x,y}, x*y + sin(x) + sqr(y));

    auto H = sympy_hessian(f);

    CHECK(sympy_equal(
      H,
      AnalyticFunction(
        {x,y},
        mat(
          vec(-sin(x), 1.),
          vec(1., 2.)
        ))));
  }

  {
    ScalarVar x;
    AnalyticFunction f({x}, 1. / (1. - x));
    auto p = sympy_series(f, x, 0.0, 3);

    CHECK(sympy_equal(p, AnalyticFunction({x}, 1. + x + sqr(x) + x*sqr(x))));
  }

  {
    ScalarVar x;
    ScalarVar y;
    AnalyticFunction f({x,y}, y + 1. / (1. - x));
    auto p = sympy_series(f, x, 0.0, 2);

    CHECK(sympy_equal(p, AnalyticFunction({x,y}, y + 1. + x + sqr(x))));
  }

  {
    ScalarVar x;
    AnalyticFunction f({x}, 9.*pow(x,4) + 8.*pow(x,3) + 7.*sqr(x) + 6.*x + 5.);
    auto h = sympy_horner(f);

    CHECK(sympy_equal(h, f));
  }

  {
    VectorVar v(2);
    AnalyticFunction f({v}, {
      3.*pow(v[0],3) + 2.*sqr(v[0]) + v[0] + 1.,
      v[1]*(v[1] + 1.) + 2.
    });

    auto h = sympy_horner(f);

    CHECK(sympy_equal(h, f));
  }

  {
    MatrixVar A(2,2);
    AnalyticFunction f({A}, mat(
      vec(A(0,0)*A(0,0) + 2.*A(0,0)*A(0,1) + 1., A(1,0) + 3.*A(1,1)),
      vec(A(0,0) - A(0,1), A(1,1)*A(1,1) + A(1,1) + 2.)
    ));
    auto h = sympy_horner(f);

    CHECK(sympy_equal(h, f));
  }
  
  {
    ScalarVar x;

    auto dtan = sympy_diff(AnalyticFunction({x}, tan(x)));
    auto dasin = sympy_diff(AnalyticFunction({x}, asin(x)));
    auto dacos = sympy_diff(AnalyticFunction({x}, acos(x)));
    auto datan = sympy_diff(AnalyticFunction({x}, atan(x)));
    auto dsinh = sympy_diff(AnalyticFunction({x}, sinh(x)));
    auto dcosh = sympy_diff(AnalyticFunction({x}, cosh(x)));
    auto dtanh = sympy_diff(AnalyticFunction({x}, tanh(x)));

    CHECK(sympy_equal(dtan,  AnalyticFunction({x}, 1. / sqr(cos(x)))));
    CHECK(sympy_equal(dasin, AnalyticFunction({x}, 1. / sqrt(1. - sqr(x)))));
    CHECK(sympy_equal(dacos, AnalyticFunction({x}, -1. / sqrt(1. - sqr(x)))));
    CHECK(sympy_equal(datan, AnalyticFunction({x}, 1. / (1. + sqr(x)))));
    CHECK(sympy_equal(dsinh, AnalyticFunction({x}, cosh(x))));
    CHECK(sympy_equal(dcosh, AnalyticFunction({x}, sinh(x))));
    CHECK(sympy_equal(dtanh, AnalyticFunction({x}, 1. - sqr(tanh(x)))));
  }

  {
    ScalarVar x;
    AnalyticFunction f({x}, abs(x));
    auto df = sympy_diff(f);

    CHECK(sympy_equal(df, AnalyticFunction({x}, sign(x))));
  }

  {
    ScalarVar y;
    ScalarVar x;
    AnalyticFunction f({y,x}, atan2(y, x));

    auto dfdy = sympy_partial_diff(f, y);
    auto dfdx = sympy_partial_diff(f, x);

    CHECK(sympy_equal(dfdy, AnalyticFunction({y,x}, x / (sqr(x) + sqr(y)))));
    CHECK(sympy_equal(dfdx, AnalyticFunction({y,x}, -y / (sqr(x) + sqr(y)))));
  }


  {
    MatrixVar A(2,2);
    AnalyticFunction f({A}, A(0,0)*A(1,1) + sin(A(0,1)) + A(1,0));

    auto dfdA00 = sympy_partial_diff(f, A(0,0));
    auto dfdA01 = sympy_partial_diff(f, A(0,1));
    auto dfdA10 = sympy_partial_diff(f, A(1,0));
    auto dfdA11 = sympy_partial_diff(f, A(1,1));
    auto grad_f = sympy_gradient(f);

    CHECK(sympy_equal(dfdA00, AnalyticFunction({A}, A(1,1))));
    CHECK(sympy_equal(dfdA01, AnalyticFunction({A}, cos(A(0,1)))));
    CHECK(sympy_equal(dfdA10, AnalyticFunction({A}, 1.)));
    CHECK(sympy_equal(dfdA11, AnalyticFunction({A}, A(0,0))));
    CHECK(sympy_equal(
      grad_f,
      AnalyticFunction({A}, vec(A(1,1), cos(A(0,1)), 1., A(0,0)))));
  }

  {
    MatrixVar A(2,2);
    AnalyticFunction f({A}, {
      A(0,0) + A(1,1),
      A(0,1)*A(1,0)
    });

    auto J = sympy_diff(f);

    CHECK(sympy_equal(
      J,
      AnalyticFunction(
        {A},
        mat(
          vec(1., 0.),
          vec(0., A(1,0)),
          vec(0., A(0,1)),
          vec(1., 0.)))));
  }

  {
    ScalarVar x;
    ScalarVar y;
    VectorVar v(2);
    MatrixVar A(2,1);

    CHECK(sympy_equal(
      AnalyticFunction({x,y}, x + 2.*y),
      AnalyticFunction({v}, v[0] + 2.*v[1])));

    CHECK(sympy_equal(
      AnalyticFunction({x,y}, vec(x+y, x-y)),
      AnalyticFunction({v}, vec(v[0] + v[1], v[0] - v[1]))));

    CHECK(sympy_equal(
      AnalyticFunction({x,y}, mat(vec(x+y, x-y))),
      AnalyticFunction({A}, mat(vec(A(0,0) + A(1,0), A(0,0) - A(1,0))))));
  }

}
