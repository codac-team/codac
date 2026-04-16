/** * Codac tests - Visibility Contractors
 * ----------------------------------------------------------------------------
 * \date       2026
 * \author     Quentin Brateau
 * \copyright  Copyright 2026 Codac Team
 * \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_CtcVisible.h>
#include <codac2_Vector.h>

using namespace codac2;

TEST_CASE("CtcVisible & CtcNoVisible - Point/Segment visibility")
{
  // Observer at origin, obstacle horizontal from (2, -1) to (2, 1)
  Vector a({0.0, 0.0});
  Segment s({2.0, -1.0}, {2.0, 1.0});
  
  CtcVisible ctc_vis(a, s);
  CtcNoVisible ctc_no_vis(a, s);

  SECTION("Box fully in visible zone (in front of obstacle)")
  {
    IntervalVector x({{0.5, 1.5}, {-0.5, 0.5}});
    IntervalVector x_orig = x;

    ctc_vis.contract(x);
    CHECK(x == x_orig); // Should not be contracted

    ctc_no_vis.contract(x_orig);
    CHECK(x_orig.is_empty()); // Cannot be hidden if in front
  }

  SECTION("Box fully in hidden zone (shadow)")
  {
    IntervalVector x({{3.0, 4.0}, {0.1, 0.4}});
    IntervalVector x_orig = x;

    ctc_no_vis.contract(x_orig);
    CHECK(x == x_orig); // Fully hidden

    ctc_vis.contract(x);
    CHECK(x.is_empty()); // Fully in shadow -> not visible
  }

  SECTION("Box behind the observer")
  {
    IntervalVector x({{-2.0, -1.0}, {-1.0, -0.2}});
    IntervalVector x_orig = x;

    ctc_vis.contract(x);
    CHECK(x == x_orig); // Visible (obstacle is far away in the other direction)

    ctc_no_vis.contract(x_orig);
    CHECK(x_orig.is_empty());
  }

  SECTION("Box on the side (outside the angular cone)")
  {
    IntervalVector x({{1.0, 4.0}, {2.5, 4.0}}); // High above the obstacle
    IntervalVector x_orig = x;

    ctc_vis.contract(x);
    CHECK(x == x_orig); 

    ctc_no_vis.contract(x_orig);
    CHECK(x_orig.is_empty());
  }
}