#include <cstdio>
#include "catch_interval.hpp"
#include "codac_CtcBox.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;


TEST_CASE("CtcBox")
{
  SECTION("Test CtcBox 1D")
  {
    // 1D IntervalVector
    IntervalVector b1{{0, 1}};

    // CtcBox
    CtcBox c(b1);

    // Contraction test
    IntervalVector x1{{-3, 4}};
    IntervalVector x2{{-1, 0.25}};

    // Testing x1
    c.contract(x1);
    CHECK(x1[0] == Interval(0, 1));

    // Testing x2
    c.contract(x2);
    CHECK(x2[0] == Interval(0, 0.25));
  }

  SECTION("Test CtcBox 2D")
  {
    // 2D IntervalVector
    IntervalVector b1{{1, 2}, {3, 4}};

    // CtcBox
    CtcBox c(b1);

    // Contraction test
    IntervalVector x1{{-3, 4}, {2, 5}};
    IntervalVector x2{{1.5, 3}, {2, 3.5}};

    // Testing x1
    c.contract(x1);
    CHECK(x1 == b1);

    // Testing x2
    c.contract(x2);
    CHECK(x2[0] == Interval(1.5, 2));
    CHECK(x2[1] == Interval(3, 3.5));
  }
}