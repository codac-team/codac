/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_CtcInter.h>
#include <codac2_CtcWrapper.h>

using namespace std;
using namespace codac2;

TEST_CASE("CtcInter")
{
  {
    CtcWrapper c1(IntervalVector({{-10,10},{-2,2}}));
    CtcWrapper c2(IntervalVector({{-12,2},{0,4}}));

    auto c3 = c1 & c2;
    IntervalVector x(2);

    x = IntervalVector({{-oo,oo},{-oo,oo}});
    c3.contract(x);
    CHECK(x == IntervalVector({{-10,2},{0,2}}));

    x = IntervalVector({{-oo,0},{-oo,0}});
    c3.contract(x);
    CHECK(x == IntervalVector({{-10,0},{0,0}}));

    x = IntervalVector({{0,oo},{0,oo}});
    c3.contract(x);
    CHECK(x == IntervalVector({{0,2},{0,2}}));

    x = IntervalVector({{0,0},{0,0}});
    c3.contract(x);
    CHECK(x == IntervalVector({{0,0},{0,0}}));
  }
}