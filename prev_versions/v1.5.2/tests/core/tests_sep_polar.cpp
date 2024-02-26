#include <cstdio>
#include "catch_interval.hpp"
#include "ibex_Sep.h"
#include "codac_SepPolarXY.h"
#include <algorithm> // std::reverse

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;

TEST_CASE("SepPolarXY")
{

  SECTION("test_sepPolar")
  {
    SepPolarXY sep(Interval(1, 2), Interval(0.1, 2.1));
  }

  SECTION("test_SepPolarXY_01")
  {

    Interval x(-0x1.9032ae28d8664p+4, -0x1.9032ae28d8664p+4);
    Interval y(-0x1.68d6cf2e8c8b8p+0, -0x1.68d6cf2e8c8b8p+0);
    Interval rho(25.0607, 25.1231);
    Interval theta(-3.14159, -2.79253);
    SepPolarXY S(rho, theta);
    IntervalVector xin({x, y});
    IntervalVector xout({x, y});

    S.separate(xin, xout);
    CHECK(xout.is_empty());
    CHECK(xin == IntervalVector({x, y}));
  }

  SECTION("test_SepPolarXY_02")
  {
    Interval rho(0x1.b9adea897635fp+2, 0x1.c01450efdc9c5p+2);
    Interval theta(-0x1.95b92733b853ep+2, -0x1.690af69de9cacp+2);
    SepPolarXY S(rho, theta);

    IntervalVector X(2);
    X[0] = Interval(0x1.be002da3cfcc6p+2, 0x1.be002da3cfcc6p+2);
    X[1] = Interval(-0x1.32b8f5d87284bp-2, -0x1.32b8f5d87284bp-2);
    IntervalVector xin(X), xout(X);
    S.separate(xin, xout);

    CHECK(!(xin.is_empty() && xout.is_empty()));
  }
}
