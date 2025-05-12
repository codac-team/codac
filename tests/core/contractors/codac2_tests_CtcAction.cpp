/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_CtcAction.h>
#include <codac2_CtcInverse.h>
#include <codac2_Wrapper.h>

using namespace std;
using namespace codac2;

TEST_CASE("CtcAction")
{
  {
    VectorVar x(2);
    CtcInverse c1(AnalyticFunction({x}, x-IntervalVector({{1,5},{8,6}})), IntervalVector({0.,0.}));

    IntervalVector b(2);
    c1.contract(b);
    CHECK(b == IntervalVector({{1,5},{8,6}}));

    CtcAction c2(c1, {2,-1});
    b = IntervalVector(2);
    c2.contract(b);
    CHECK(b == IntervalVector({{8,6},{-5,1}}));
  }
}