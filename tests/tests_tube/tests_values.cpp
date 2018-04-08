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

    CHECK(tube.nbSlices() == 46);
    CHECK(tube[7] == Interval(-11,-7));
    CHECK(tube[3.5] == Interval(-4,4));
    CHECK(tube[17.5] == Interval(2,7));
    CHECK(tube[32.5] == Interval(2,5));
    CHECK(tube[17.0] == Interval(2,6)); // t belongs to two slices
    CHECK(tube[16.9999999] == Interval(0,6));
    CHECK(tube[17.0000001] == Interval(2,7));
    //CHECK(tube[Interval(9.5,17.5)] == Interval(-9,7));
    //CHECK(tube[Interval(16.1,37.9)] == Interval(0,13));
    //CHECK(tube[Interval(37.4,45.6)] == Interval(-1,6));
    //CHECK(tube[Interval(14.0,27.0)] == Interval(-4,13));
    //CHECK(tube[Interval(19.2,35.7)] == Interval(1,13));
    //CHECK(tube[Interval(15.0,16.0)] == Interval(-2,4));
    //CHECK(tube[Interval(0.)] == Interval(4,8));
    //CHECK(tube[Interval(5.)] == Interval(-7,-5));
    //CHECK(tube[Interval(24.)] == Interval(8,12));
    //CHECK(tube[Interval(46.)] == Interval(-1,3));
    //CHECK(tube.codomain() == Interval(-11,13));
  }

  SECTION("Setting values over an interval domain")
  {
    Tube tube(Interval(0.,10.));

    // Unbounded interval domain
    tube.set(Interval(6.,10.));
    CHECK(tube.nbSlices() == 1);
    CHECK(tube.getSlice(0)->domain() == Interval(0.,10.));
    CHECK(tube.getSlice(0)->codomain() == Interval(6.,10.));

    // Bounded interval domain
    tube.set(Interval(2.,4.), Interval(2.,3.));
    CHECK(tube.nbSlices() == 3);
    CHECK(tube.sliceDomain(0) == Interval(0.,2.));
    CHECK(tube[0] == Interval::ALL_REALS);
    CHECK(tube.sliceDomain(1) == Interval(2.,3.));
    CHECK(tube[1] == Interval(2.,4.));
    CHECK(tube.sliceDomain(2) == Interval(3.,10.));
    CHECK(tube[2] == Interval::ALL_REALS);

    // Gates, slices intersection
    tube.set(Interval(1.,3.), Interval(0.,2.));
    CHECK(tube.nbSlices() == 3);
    CHECK(tube[0] == Interval(1.,3.));
    CHECK(tube[0.] == Interval(1.,3.));
    CHECK(tube[2.] == Interval(2.,3.));
    CHECK(tube[2.01] == Interval(2.,4.));
    CHECK(tube[3.] == Interval(2.,4.));
    tube.set(Interval(9.,10.), Interval(3.,4.));
    CHECK(tube[3.] == Interval::EMPTY_SET);
  }

  SECTION("Test tube4")
  {
    /*Tube tube = tubeTest4();

    //CHECK(tube.image() == Interval(-1.5,2));
    CHECK(tube[0] == Interval(1.,2.));
    CHECK(tube[11] == Interval(-1.5,-0.5));
    CHECK(tube[11.0] == Interval(-1.0,-0.5));
    CHECK(tube[1.5] == Interval(1.,2.));*/
  }
}
