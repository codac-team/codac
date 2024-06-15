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

TEST_CASE("orientation")
{
  // Align points
  CHECK(orientation({0.,0.},{5.,0.},{8.,0.}) == BoolInterval::UNKNOWN);
  CHECK(orientation({0.,0.},{5.,0.},{-2.,0.}) == BoolInterval::UNKNOWN);
  CHECK(orientation({0.,0.},{-2.,0.},{5.,0.}) == BoolInterval::UNKNOWN);
  CHECK(orientation({0.,0.},{0.,0.},{5.,0.}) == BoolInterval::UNKNOWN);
  CHECK(orientation({0.,0.},{0.,0.},{0.,0.}) == BoolInterval::UNKNOWN);

  // Clockwise points
  CHECK(orientation({0.,0.},{0.,-5.},{-2.,-5.}) == BoolInterval::TRUE);
  CHECK(orientation({-999.,0.1},{0.,0.},{-999.,-0.1}) == BoolInterval::TRUE);

  // Anticlockwise points
  CHECK(orientation({0.,0.},{0.,-5.},{2.,-5.}) == BoolInterval::FALSE);
  CHECK(orientation({999.,0.1},{0.,0.},{999.,-0.1}) == BoolInterval::FALSE);
}

TEST_CASE("in_polygon")
{
  CHECK(in_polygon({{3.1},{3}}, {{{3},{-1}},{{3},{4}},{{5},{6}},{{-1},{1}}}) == BoolInterval::FALSE);
  CHECK(in_polygon({{2.9},{3}}, {{{3},{-1}},{{3},{4}},{{5},{6}},{{-1},{1}}}) == BoolInterval::TRUE);
  CHECK(in_polygon({{3},{3}}, {{{3},{-1}},{{3},{4}},{{5},{6}},{{-1},{1}}}) == BoolInterval::UNKNOWN);
}