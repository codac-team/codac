/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_SepCartProd.h>
#include <codac2_SepInverse.h>

using namespace std;
using namespace codac2;

TEST_CASE("SepCartProd")
{
  {
    VectorVar v(1);
    SepInverse s1(AnalyticFunction({v}, v), IntervalVector({{-1,1}}));
    SepInverse s2(AnalyticFunction({v}, v), IntervalVector({{5,6}}));

    {
      IntervalVector x({{0,8},{5.5,oo}});
      SepCartProd s3(s1,s2);
      auto xs = s3.separate(x);
      CHECK(xs.inner == IntervalVector({{1,8},{6,oo}}));
      CHECK(xs.outer == IntervalVector({{0,1},{5.5,6}}));
    }

    {
      IntervalVector x({{0,8},{5.5,oo}});
      auto s4 = cart_prod(s1,s2);
      auto xs = s4.separate(x);
      CHECK(xs.inner == IntervalVector({{1,8},{6,oo}}));
      CHECK(xs.outer == IntervalVector({{0,1},{5.5,6}}));
    }
  }
}