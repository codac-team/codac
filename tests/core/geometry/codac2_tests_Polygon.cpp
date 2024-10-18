/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_Polygon.h>

using namespace std;
using namespace codac2;

TEST_CASE("Polygon")
{
  CHECK(Polygon({{3,-1},{3,4},{5,6},{-1,1}}).contains({{3.1},{3}}) == BoolInterval::FALSE);
  CHECK(Polygon({{3,-1},{3,4},{5,6},{-1,1}}).contains({{2.9},{3}}) == BoolInterval::TRUE);
  CHECK(Polygon({{3,-1},{3,4},{5,6},{-1,1}}).contains({{3},{3}}) == BoolInterval::UNKNOWN);
}