#include <cstdio>
#include "catch_interval.hpp"
#include "codac_SepBox.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;


TEST_CASE("SepBox")
{
  SECTION("Test SepBox 1D")
  {
    // 1D IntervalVector
    IntervalVector b1{{0, 1}};

    // CtcBox
    SepBox s(b1);

    // Contraction test
    IntervalVector x1{{0.5, 1.5}};
    IntervalVector x2{{-1, 2}};

    // Testing x1
    IntervalVector x1_in(x1);
    IntervalVector x1_out(x1);
    s.separate(x1_in, x1_out);
    CHECK(x1_in[0] == Interval(1, 1.5));
    CHECK(x1_out[0] == Interval(0.5, 1));

    // Testing x2
    IntervalVector x2_in(x2);
    IntervalVector x2_out(x2);
    s.separate(x2_in, x2_out);
    CHECK(x2_in[0] == Interval(-1, 2));
    CHECK(x2_out[0] == Interval(0, 1));
  }

  SECTION("Test SepBox 2D")
  {
    // 2D IntervalVector
    IntervalVector b1{{1, 2}, {3, 4}};

    // CtcBox
    SepBox s(b1);

    // Contraction test
    IntervalVector x1{{0, 5}, {0, 5}};
    IntervalVector x2{{1.25, 1.75}, {3.5, 4.5}};

    // Testing x1
    IntervalVector x1_in(x1);
    IntervalVector x1_out(x1);
    s.separate(x1_in, x1_out);
    CHECK(x1_in[0] == Interval(0, 5));
    CHECK(x1_in[1] == Interval(0, 5));
    CHECK(x1_out[0] == Interval(1, 2));
    CHECK(x1_out[1] == Interval(3, 4));

    // Testing x2
    IntervalVector x2_in(x2);
    IntervalVector x2_out(x2);
    s.separate(x2_in, x2_out);
    CHECK(x2_in[0] == Interval(1.25, 1.75));
    CHECK(x2_in[1] == Interval(4, 4.5));
    CHECK(x2_out[0] == Interval(1.25, 1.75));
    CHECK(x2_out[1] == Interval(3.5, 4));
  }
}