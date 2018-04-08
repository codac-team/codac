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

    CHECK(tube[ 0] == Interval(4,8));
    CHECK(tube[ 1] == Interval(2,7));
    CHECK(tube[ 2] == Interval(1,6));
    CHECK(tube[ 3] == Interval(-4,4));
    CHECK(tube[ 4] == Interval(-7,-1));
    CHECK(tube[ 5] == Interval(-9,-5));
    CHECK(tube[ 6] == Interval(-10,-6));
    CHECK(tube[ 7] == Interval(-11,-7));
    CHECK(tube[ 8] == Interval(-10,-6));
    CHECK(tube[ 9] == Interval(-9,-4));
    CHECK(tube[10] == Interval(-8,-5));
    CHECK(tube[11] == Interval(-7,-4));
    CHECK(tube[12] == Interval(-6,-2));
    CHECK(tube[13] == Interval(-5,-1));
    CHECK(tube[14] == Interval(-4,2));
    CHECK(tube[15] == Interval(-2,4));
    CHECK(tube[16] == Interval(0,6));
    CHECK(tube[17] == Interval(2,7));
    CHECK(tube[18] == Interval(4,8));
    CHECK(tube[19] == Interval(6,9));
    CHECK(tube[20] == Interval(7,10));
    CHECK(tube[21] == Interval(8,11));
    CHECK(tube[22] == Interval(9,12));
    CHECK(tube[23] == Interval(8,13));
    CHECK(tube[24] == Interval(7,12));
    CHECK(tube[25] == Interval(5,11));
    CHECK(tube[26] == Interval(3,10));
    CHECK(tube[27] == Interval(4,9));
    CHECK(tube[28] == Interval(5,8));
    CHECK(tube[29] == Interval(4,7));
    CHECK(tube[30] == Interval(3,6));
    CHECK(tube[31] == Interval(3,5));
    CHECK(tube[32] == Interval(2,5));
    CHECK(tube[33] == Interval(2,5));
    CHECK(tube[34] == Interval(1,5));
    CHECK(tube[35] == Interval(2,4));
    CHECK(tube[36] == Interval(1,4));
    CHECK(tube[37] == Interval(0,4));
    CHECK(tube[38] == Interval(-1,3));
    CHECK(tube[39] == Interval(-1,3));
    CHECK(tube[40] == Interval(-1,4));
    CHECK(tube[41] == Interval(0,5));
    CHECK(tube[42] == Interval(1,6));
    CHECK(tube[43] == Interval(0,5));
    CHECK(tube[44] == Interval(-1,4));
    CHECK(tube[45] == Interval(-1,3));

    CHECK(tube[7] == Interval(-11,-7));
    CHECK(tube[3.5] == Interval(-4,4));
    CHECK(tube[17.5] == Interval(2,7));
    CHECK(tube[32.5] == Interval(2,5));
    CHECK(tube[17.0] == Interval(2,6)); // t belongs to two slices
    CHECK(tube[16.9999999] == Interval(0,6));
    CHECK(tube[17.0000001] == Interval(2,7));
    CHECK(tube[Interval(9.5,17.5)] == Interval(-9,7));
    CHECK(tube[Interval(16.1,37.9)] == Interval(0,13));
    CHECK(tube[Interval(37.4,45.6)] == Interval(-1,6));
    CHECK(tube[Interval(14.0,27.0)] == Interval(-4,13));
    CHECK(tube[Interval(19.2,35.7)] == Interval(1,13));
    CHECK(tube[Interval(15.0,16.0)] == Interval(-2,4));
    CHECK(tube[Interval(15.2,15.3)] == Interval(-2,4));
    CHECK(tube[15.2] == Interval(-2,4));
    CHECK(tube[Interval(0.)] == Interval(4,8));
    CHECK(tube[Interval(5.)] == Interval(-7,-5));
    CHECK(tube[Interval(24.)] == Interval(8,12));
    CHECK(tube[Interval(46.)] == Interval(-1,3));
    CHECK(tube.codomain() == Interval(-11,13));
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
    CHECK(tube[0] == Interval(6.,10.));
    CHECK(tube.sliceDomain(1) == Interval(2.,3.));
    CHECK(tube[1] == Interval(2.,4.));
    CHECK(tube.sliceDomain(2) == Interval(3.,10.));
    CHECK(tube[2] == Interval(6.,10.));

    // Gates, slices intersection
    tube.set(Interval(1.,3.), Interval(0.,2.));
    CHECK(tube.nbSlices() == 3);
    CHECK(tube[0] == Interval(1.,3.));
    CHECK(tube[0.] == Interval(1.,3.));
    CHECK(tube[2.] == Interval(2.,3.));
    CHECK(tube[2.01] == Interval(2.,4.));
    CHECK(tube[3.] == Interval::EMPTY_SET);
    tube.set(Interval(4.,5.), Interval(3.,4.));
    CHECK(tube[3.] == Interval(4.));
  }

  SECTION("Setting values at some precise time input")
  {
    Tube tube(Interval(0.,10.));
    tube.set(Interval(3.,4.), 2.);
    CHECK(tube.nbSlices() == 2);
    CHECK(tube.sliceDomain(0) == Interval(0.,2.));
    CHECK(tube.sliceDomain(1) == Interval(2.,10.));
    CHECK(tube[0] == Interval::ALL_REALS);
    CHECK(tube[1] == Interval::ALL_REALS);
    CHECK(tube[2.] == Interval(3.,4.));
  }

  SECTION("Test tube4")
  {
    Tube tube = tubeTest4();
    CHECK(tube.codomain() == Interval(-1.5,2));
    CHECK(tube[0] == Interval(1.,2.));
    CHECK(tube[10.2] == Interval(0.5,1.0));
    CHECK(tube[11.0] == Interval::EMPTY_SET);
    CHECK(tube[1.5] == Interval(1.,2.));
  }
}