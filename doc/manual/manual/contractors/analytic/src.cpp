/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Simon Rohou
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_CtcInverse.h>
#include <codac2_SepInverse.h>
#include <codac2_CtcInverseNotIn.h>
#include <codac2_Approx.h>
#include <codac2_Figure2D.h>

using namespace std;
using namespace codac2;

TEST_CASE("CtcInverse - manual")
{
  {
    // [ctcinv-1-beg]
    // Example of Himmelblau's function
    double a = 11, b = 7;
    VectorVar x(2);
    AnalyticFunction f({x}, sqr(sqr(x[0])+x[1]-a)+sqr(x[0]+sqr(x[1])-b));
    CtcInverse c(f, 50);
    // [ctcinv-1-end]

    // [ctcinv-2-beg]
    DefaultFigure::pave({{-6,6},{-6,6}}, c, 1e-2);
    // [ctcinv-2-end]

    // [ctcinv-3-beg]
    SepInverse s(f, {0,50});
    DefaultFigure::pave({{-6,6},{-6,6}}, s, 1e-2);
    // [ctcinv-3-end]

    // [ctcinv-4-beg]
    auto cu = CtcInverse(f,50) | CtcInverse(f,150) | CtcInverse(f,250);
    DefaultFigure::pave({{-6,6},{-6,6}}, cu, 1e-2);
    // [ctcinv-4-end]
  }

  {
    // [ctcinv-5-beg]
    VectorVar x(2);
    AnalyticFunction f({x}, x[0]);

    // Enforce the first component not in [0,1]
    CtcInverseNotIn c(f, {0,1});

    IntervalVector y({{0.5,3},{-1,1}});
    c.contract(y); // {{1,3},{-1,1}}
    // Only the first component is constrained by the not-in condition
    // [ctcinv-5-end]

    CHECK(y == IntervalVector({{1,3},{-1,1}}));
  }

  {
    // [ctcinv-6-beg]
    VectorVar x(2);
    AnalyticFunction f({x}, x[0]-x[1]);
    CtcInverse c(f, 0);
    // c.fnc().input_size() == 2
    // c.fnc().output_size() == 1
    // [ctcinv-6-end]

    CHECK(c.fnc().input_size() == 2);
    CHECK(c.fnc().output_size() == 1);
  }
}