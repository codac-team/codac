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

TEST_CASE("BoolInterval class - manual")
{
  /*
    // [boolinterval-class-1-beg]
    BoolInterval::FALSE;   // certainly false
    BoolInterval::TRUE;    // certainly true
    BoolInterval::UNKNOWN; // undetermined
    BoolInterval::EMPTY;   // inconsistent / impossible
    // [boolinterval-class-1-end]

    // [boolinterval-class-2-beg]
    BoolInterval::UNKNOWN == BoolInterval::TRUE | BoolInterval::FALSE
    BoolInterval::EMPTY   == BoolInterval::TRUE & BoolInterval::FALSE
    // [boolinterval-class-2-end]
  */
}

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
    Interval w = y & z;      // w =  [1, 1]
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

TEST_CASE("IntervalVector class - manual")
{
  {
    // [intervalvector-class-1-beg]
    // Default box: [-oo,oo]^n (Interval default constructor)
    IntervalVector x(3);

    // Cube [-1,3]^2
    IntervalVector y = IntervalVector::constant(2,{-1,3});

    // From a list of bounds (initializer-list style)
    IntervalVector z{{3,4},{4,6}}; // [3,4]×[4,6]

    // From a point (degenerate intervals)
    Vector p({0.42,0.42,0.42});
    IntervalVector bp(p); // [0.42,0.42]^3
    // [intervalvector-class-1-end]
  }
  
  {
    // [intervalvector-class-2-beg]
    IntervalVector x = IntervalVector::constant(2,{-1,3}); // [-1,3]^2
    x[1] = Interval(0,10); // [-1,3]×[0,10]

    // Accessing components
    const Interval& x0 = x[0];

    // Resize: new components are default-initialized ([-oo,oo])
    x.resize(4); // x == [-1,3]×[0,10]×[-oo,oo]×[-oo,oo]

    // Subvector / segment extraction
    IntervalVector s = x.subvector(1,2); // [0,10]×[-oo,oo]
    // [intervalvector-class-2-end]

    (void)x0; // avoid warning on unused variable
  }
  
  {
    // [intervalvector-class-3-beg]
    IntervalVector x{{0,2},{-1,3}};

    Index n   = x.size();
    Vector lo = x.lb();
    Vector hi = x.ub();
    Vector m  = x.mid();
    Vector d  = x.diam();
    // [intervalvector-class-3-end]

    (void)n; // avoid warning on unused variable
  }

  {
    // [intervalvector-class-4-beg]
    IntervalVector x{{0,1},{2,3}};
    IntervalVector y{{-0.5,2},{1,4}};

    assert(x.intersects(y));
    assert(x.is_subset(y));
    // [intervalvector-class-4-end]
  }
  
  {
    // [intervalvector-class-5-beg]
    IntervalVector x{{0,1},{2,3}};
    IntervalVector y{{1,2},{0,1}};

    IntervalVector z1 = x+y;
    IntervalVector z2 = 2.*x;
    IntervalVector z3 = x/2.;
    // [intervalvector-class-5-end]
  }
}