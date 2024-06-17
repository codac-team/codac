/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_SepPolygon.h>

using namespace std;
using namespace codac2;

TEST_CASE("SepPolygon")
{
  SepPolygon s({{{3},{-1}},{{3},{4}},{{5},{6}},{{-1},{1}}});

  IntervalVector x(2);
  auto xs = s.separate(IntervalVector(2));
  CHECK(xs.inner == IntervalVector(2));
  CHECK(xs.outer == IntervalVector({{-1,5},{-1,6}}));

  x = IntervalVector({{3.02,3.16},{2.5,3.2}}); // possible bug
  xs = s.separate(x);
  CHECK(xs.inner == x);
  CHECK(xs.outer.is_empty());
}