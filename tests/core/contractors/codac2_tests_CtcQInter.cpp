/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_CtcQInter.h>
#include <codac2_CtcInter.h>
#include <codac2_CtcWrapper.h>

using namespace std;
using namespace codac2;

TEST_CASE("CtcQInter")
{
  {
    CtcWrapper c1(IntervalVector({{-10,10},{-2,2}}));
    CtcWrapper c2(IntervalVector({{-12,2},{0,4}}));

    CtcInter c3({c1,c2});

    // Testing constructors
    CtcQInter c4(1, {c1,c2}); // works if c1,c2 of same types
    CtcQInter c5(1, c1,c2,c3); // different types
    CHECK(c4.nb() == 2);
    CHECK(c5.nb() == 3);
  }
}