#include "../catch/catch.hpp"
#include "../catch/catch_interval.hpp"
#include "tubex.h"
#include "../tests_cases.hpp"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

TEST_CASE("Tube values", "[core]")
{
  SECTION("Test tube1")
  {
    Tube tube = tubeTest1();
    tube.set(Interval(-4,2), 14); // to test primitives pre-computation

    /*REQUIRE(tube.codomain() == Interval(-11,13));
    REQUIRE(tube(7) == Interval(-11,-7));
    REQUIRE(tube(3.5) == Interval(-4,4));
    REQUIRE(tube(17.5) == Interval(2,7));
    REQUIRE(tube(32.5) == Interval(2,5));
    REQUIRE(tube(17.0) == Interval(2,6)); // t belongs to two slices
    REQUIRE(tube(16.9999999) == Interval(0,6));
    REQUIRE(tube(17.0000001) == Interval(2,7));
    REQUIRE(tube(Interval(9.5,17.5)) == Interval(-9,7));
    REQUIRE(tube(Interval(16.1,37.9)) == Interval(0,13));
    REQUIRE(tube(Interval(37.4,45.6)) == Interval(-1,6));
    REQUIRE(tube(Interval(14.0,27.0)) == Interval(-4,13));
    REQUIRE(tube(Interval(19.2,35.7)) == Interval(1,13));
    REQUIRE(tube(Interval(15.0,16.0)) == Interval(-2,4));
    REQUIRE(tube(Interval(0.)) == Interval(4,8));
    REQUIRE(tube(Interval(5.)) == Interval(-7,-5));
    REQUIRE(tube(Interval(24.)) == Interval(8,12));
    REQUIRE(tube(Interval(46.)) == Interval(-1,3));*/
  }

  SECTION("Test tube4")
  {
    /*Tube tube = tubeTest4();

    REQUIRE(tube.image() == Interval(-1.5,2));
    REQUIRE(tube[0] == Interval(1,2));
    REQUIRE(tube[11] == Interval(-1.5,-0.5));
    REQUIRE(tube[11.0] == Interval(-1,-0.5));
    REQUIRE(tube[1.5] == Interval(1,2));*/
  }
}
