/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_CtcUnion.h>
#include <codac2_CtcInter.h>
#include <codac2_CtcWrapper.h>

using namespace std;
using namespace codac2;

TEST_CASE("CtcUnion")
{
  {
    CtcWrapper c1(IntervalVector({{-10,10},{-2,2}}));
    CtcWrapper c2(IntervalVector({{-12,2},{0,4}}));

    auto c3 = c1 & c2;

    // Testing constructors
    CtcUnion test_construct_1(c1,c2);
    CtcUnion test_construct_2({c1,c2}); // works if c1,c2 of same types
    CtcUnion test_construct_3(c1,c2,c3); // different types
  }
}