/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
// [sympy-1-beg]
#include <codac-sympy.h>
// [sympy-1-end]

using namespace std;
using namespace codac2;

TEST_CASE("Sympy extension - manual")
{
  {
    // [sympy-2-beg]
    ScalarVar x;
    AnalyticFunction f({x}, sqr(sin(x)) + sqr(cos(x)));

    auto fs = sympy_simplify(f);
    assert(sympy_equal(fs, AnalyticFunction({x}, 1.)));
    // [sympy-2-end]
  }

  {
    // [sympy-3-beg]
    ScalarVar x;
    AnalyticFunction f({x}, x*(x + 1.) - sqr(x));

    auto fs = sympy_simplify(f);
    assert(sympy_equal(fs, AnalyticFunction({x}, x)));
    // [sympy-3-end]
  }

  {
    // [sympy-4-beg]
    ScalarVar x,y;
    AnalyticFunction f({x,y}, x*y + sin(x));

    auto dfdx = sympy_partial_diff(f,x);
    auto dfdy = sympy_partial_diff(f,y);

    assert(sympy_equal(dfdx, AnalyticFunction({x,y}, y + cos(x))));
    assert(sympy_equal(dfdy, AnalyticFunction({x,y}, x)));
    // [sympy-4-end]
  }

  {
    // [sympy-5-beg]
    VectorVar v(2);
    AnalyticFunction g({v}, v[0]*v[1] + sin(v[0]));

    auto dg_dv0 = sympy_partial_diff(g,v[0]);
    auto dg_dv1 = sympy_partial_diff(g,v[1]);

    assert(sympy_equal(dg_dv0, AnalyticFunction({v}, v[1] + cos(v[0]))));
    assert(sympy_equal(dg_dv1, AnalyticFunction({v}, v[0])));
    // [sympy-5-end]
  }

  {
    // [sympy-6-beg]
    ScalarVar x;
    AnalyticFunction f({x}, cos(x)*x + sin(x));

    auto df = sympy_diff(f);
    auto d3f = sympy_diff(f, x, 3);

    assert(sympy_equal(df, AnalyticFunction({x}, 2.*cos(x) - x*sin(x))));
    assert(sympy_equal(d3f, AnalyticFunction({x}, x*sin(x) - 4.*cos(x))));
    // [sympy-6-end]
  }

  {
    // [sympy-7-beg]
    VectorVar v(2);
    AnalyticFunction g({v}, v[0]*v[1] + sin(v[0]));

    auto grad_g = sympy_gradient(g);

    assert(sympy_equal(
      grad_g,
      AnalyticFunction({v}, vec(v[1] + cos(v[0]), v[0]))));
    // [sympy-7-end]
  }

  {
    // [sympy-8-beg]
    ScalarVar x,y;
    AnalyticFunction f({x,y}, x*y + sin(x) + sqr(y));

    auto H = sympy_hessian(f);

    assert(sympy_equal(
      H,
      AnalyticFunction(
        {x,y},
        mat(
          vec(-sin(x), 1.),
          vec(1., 2.)
      ))));
    // [sympy-8-end]
  }

  {
    // [sympy-9-beg]
    VectorVar v(2);

    AnalyticFunction f({v}, {
      v[0]*v[1] + sin(v[0]),
      sqr(v[0]) + cos(v[1])
    });

    auto J = sympy_diff(f);

    assert(sympy_equal(
      J,
      AnalyticFunction(
        {v},
        mat(
          vec(v[1] + cos(v[0]), 2.*v[0]),
          vec(v[0], -sin(v[1]))
        ))));
    // [sympy-9-end]
  }

  {
    // [sympy-10-beg]
    ScalarVar x,y;
    VectorVar v(2);

    auto f = AnalyticFunction({x,y}, x + 2.*y);
    auto g = AnalyticFunction({v}, v[0] + 2.*v[1]);

    assert(sympy_equal(f, g));
    // [sympy-10-end]
  }

  {
    // [sympy-11-beg]
    ScalarVar x;
    AnalyticFunction f({x}, 1. / (1. - x));

    auto p = sympy_series(f, x, 0.0, 3);

    assert(sympy_equal(p, AnalyticFunction({x}, 1. + x + sqr(x) + x*sqr(x))));
    // [sympy-11-end]
  }

  {
    // [sympy-12-beg]
    ScalarVar x,y;
    AnalyticFunction f({x,y}, y + 1. / (1. - x));

    auto p = sympy_series(f, x, 0.0, 2);

    assert(sympy_equal(p, AnalyticFunction({x,y}, y + 1. + x + sqr(x))));
    // [sympy-12-end]
  }

  {
    // [sympy-13-beg]
    ScalarVar x;
    AnalyticFunction p({x}, 2.*pow(x,5) + pow(x,3) - 3.*sqr(x));

    auto h = sympy_horner(p);

    assert(sympy_equal(h, AnalyticFunction({x}, sqr(x)*(-3+x*(1+2*sqr(x))))));
    // [sympy-13-end]
  }
}