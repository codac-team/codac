/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_Interval.h>
#include <codac2_Approx.h>

using namespace std;
using namespace codac2;

TEST_CASE("Interval class - manual")
{
  #if 0
  {
    // [interval-class-1-beg]
    Interval x(3, 5);                           // [3.0, 5.0]
    Interval x(4.2);                            // [4.2, 4.2]
    Interval x;                                 // [-∞, ∞]
    Interval x(-oo, -10);                       // [-∞, -10]
    Interval x(std::array<double,2>{2.0, -3.0}); // [-3.0, 2.0]
    Interval x(std::array<double,1>{3.14});     // [3.14, 3.14]
    // [interval-class-1-end]
  }
  #endif

  #if 0
  {
    // [interval-class-2-beg]
    Interval x;                                 // [-∞,∞] (default value)
    Interval x = Interval::empty();             // ∅
    Interval x = Interval::pi();                // [π]
    Interval x = Interval::two_pi();            // [2π]
    Interval x = Interval::half_pi();           // [π/2]
    Interval x = Interval::zero();              // [0]
    Interval x = Interval::one();               // [1]
    // [interval-class-2-end]
  }
  #endif

  {
    // [interval-class-3-beg]
    Interval x = Interval::pi();                // [π]
    // x = [3.141592653589793, 3.141592653589794]
    // [interval-class-3-end]
  }

  {
    Interval x;
    // [interval-class-4-beg]
    x.lb();     // lower bound
    x.ub();     // upper bound
    x.mid();    // midpoint
    x.rad();    // radius
    x.diam();   // diameter
    x.mag();    // magnitude
    x.mig();    // mignitude
    x.smag();   // signed magnitude
    x.smig();   // signed mignitude
    x.size();   // dimension (always 1)
    // [interval-class-4-end]
  }

  {
    // [interval-class-5-beg]
    Interval x(2,3);
    Interval y(1,2);
    Interval z = x + y;    // [3, 5]
    z = x - 1;             // [1, 2]
    z = 2 * x;             // [4, 6]
    z = x / y;             // [1, 3]
    // [interval-class-5-end]
    CHECK((x + y) == Interval(3,5));
    CHECK((x - 1) == Interval(1,2));
    CHECK((2 * x) == Interval(4,6));
    CHECK((x / y) == Interval(1,3));
  }

  {
    // [interval-class-6-beg]
    Interval x = Interval::half_pi();
    x |= 0;                  // x = [0, π/2]
    Interval y = sin(x);     // y = [0, 1]
    Interval z = exp(x);     // z = [1, e^(π/2)]
    // [interval-class-6-end]
    CHECK(Approx(x) == Interval(0,PI/2));
    CHECK(Approx(y) == Interval(0,1));
    CHECK(Approx(z) == Interval(1, std::exp(PI/2)));
  }

  {
    // [interval-class-6b-beg]
    Interval x = 2.5_i;  // [2.5, 2.5]
    // [interval-class-6b-end]
  }

  {
    // [interval-class-7-beg]
    double x = prev_float(1.0);
    // x = 0.9999999999999999
    // [interval-class-7-end]
    CHECK(Approx<double>(x) == 0.9999999999999999);
    CHECK(x != 1.);
  }
}