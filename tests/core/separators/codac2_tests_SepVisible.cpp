/** * Codac tests - Visibility Separator
 * ----------------------------------------------------------------------------
 * \date       2026
 * \author     Quentin Brateau
 * \copyright  Copyright 2024 Codac Team
 * \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_SepVisible.h>

using namespace codac2;

TEST_CASE("SepVisible - Space Partitioning")
{
  Vector a({0.0, 0.0});
  Segment s({1.0, 1.0}, {1.0, -1.0}); // Vertical wall at x=1
  SepVisible sep(a, s);

  SECTION("Consistency check: in | out should cover boundary")
  {
    IntervalVector x({{0.0, 2.0}, {-2.0, 2.0}});
    BoxPair res = sep.separate(x);
    
    // The union of the contracted boxes should ideally cover the original box 
    // (minus the parts definitively removed by contractors)
    CHECK(!res.inner.is_empty());  // Some parts are hidden (x > 1)
    CHECK(!res.outer.is_empty()); // Some parts are visible (x < 1 or y > cone)
  }

  SECTION("Corner case: Box exactly on the observation point")
  {
    // A point at the source is always visible (or at least not hidden by the obstacle)
    IntervalVector x({{0.0, 0.0}, {0.0, 0.0}});
    BoxPair res = sep.separate(x);
    
    CHECK(res.inner.is_empty());    // Not hidden
    CHECK(res.outer == x);         // Visible
  }
}