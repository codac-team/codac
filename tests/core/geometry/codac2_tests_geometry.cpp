/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_geometry.h>

using namespace std;
using namespace codac2;

TEST_CASE("geometry")
{
  // Align points
  CHECK(orientation({0.,0.},{5.,0.},{8.,0.}) == BoolInterval::UNKNOWN);
  CHECK(orientation({0.,0.},{5.,0.},{-2.,0.}) == BoolInterval::UNKNOWN);
  CHECK(orientation({0.,0.},{-2.,0.},{5.,0.}) == BoolInterval::UNKNOWN);
  CHECK(orientation({0.,0.},{0.,0.},{5.,0.}) == BoolInterval::UNKNOWN);
  CHECK(orientation({0.,0.},{0.,0.},{0.,0.}) == BoolInterval::UNKNOWN);

  // Clockwise points
  CHECK(orientation({0.,0.},{0.,-5.},{-2.,-5.}) == BoolInterval::YES);
  CHECK(orientation({-999.,0.1},{0.,0.},{-999.,-0.1}) == BoolInterval::YES);

  // Anticlockwise points
  CHECK(orientation({0.,0.},{0.,-5.},{2.,-5.}) == BoolInterval::NO);
  CHECK(orientation({999.,0.1},{0.,0.},{999.,-0.1}) == BoolInterval::NO);
}