#include "tests.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

TEST_CASE("Polygon")
{
  SECTION("Polygon from IntervalVector")
  {
    IntervalVector iv(2);
    iv[0] = Interval(-1.,5.);
    iv[1] = Interval(10.,11.);

    Polygon p(iv);
    CHECK(p.nbPoints() == 5);
    CHECK(p.box() == iv);
    Polygon p_convex = p;
    p_convex.makeConvex();
    CHECK(p_convex.nbPoints() == 5);
    CHECK(p == p_convex);
    CHECK_FALSE(p != p_convex);
  }

  SECTION("Polygon from IntervalVector (unbounded case)")
  {
    IntervalVector iv(2);
    iv[0] = Interval(-1.,5.);
    CHECK_THROWS(Polygon p(iv););
  }
}