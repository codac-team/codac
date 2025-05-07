/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_CtcCartProd.h>
#include <codac2_CtcInverse.h>

using namespace std;
using namespace codac2;

TEST_CASE("CtcCartProd")
{
  {
    VectorVar v(1);
    CtcInverse c1(AnalyticFunction({v}, v), IntervalVector({{-1,1}}));
    CtcInverse c2(AnalyticFunction({v}, v), IntervalVector({{5,6}}));

    IntervalVector a(1),b(1);
    c1.contract(a);
    CHECK(a == IntervalVector({{-1,1}}));
    c2.contract(b);
    CHECK(b == IntervalVector({{5,6}}));

    {
      IntervalVector x(2);
      CtcCartProd c3(c1,c2);
      c3.contract(x);
      CHECK(x == IntervalVector({{-1,1},{5,6}}));
    }

    {
      IntervalVector x(2);
      auto c4 = cart_prod(c1,c2);
      c4.contract(x);
      CHECK(x == IntervalVector({{-1,1},{5,6}}));
    }
  }
}