#include "tests.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

TEST_CASE("Operators")
{
  SECTION("Test tube1|tube2")
  {
    Tube tube1 = tubeTest1();
    tube1.set(Interval(-4,2), 14); // to test primitives pre-computation
    Tube tube2 = tubeTest2();
    tube1 |= tube2;

    CHECK_THROWS(tube1 |= tubeTest4_05(););
    CHECK(tube1.codomain() == Interval(-11,13));
    CHECK(tube1[0] == Interval(-2,8));
    CHECK(tube1[5.5] == Interval(-9,6));
    CHECK(tube1[11] == Interval(-7,3));
    CHECK(tube1[21.3] == Interval(2,11));
    CHECK(tube1[45] == Interval(-1,3));
    CHECK(tube1[Interval(5,10)] == Interval(-11,6));
    CHECK(tube1[Interval(15,20)] == Interval(-3,9));
    CHECK(tube1[Interval(8.2,39.9)] == Interval(-10,13));

    // Gates
    tube1 = tubeTest1();
    tube2 = tubeTest2();

    tube1.set(Interval(4.), 0.);
    tube2.set(Interval(-1.,0.), 0.);
    tube1 |= tube2;
    CHECK(tube1[0.] == Interval(-1.,4.));
    CHECK(tube1[0] == Interval(-2.,8.));

    tube1.set(Interval(0.,2.), 40.);
    tube2.set(Interval(-3.), 40.);
    tube1 |= tube2;
    CHECK(tube1[40.] == Interval(-3.,2.));
    CHECK(tube1[40] == Interval(-3.,4.));
  }

  SECTION("Test tube1|traj2")
  {
    // todo
  }

  SECTION("Test tube1&tube2")
  {
    Tube tube1 = tubeTest1();
    tube1.set(Interval(-4,2), 14); // to test primitives pre-computation
    Tube tube2 = tubeTest2();
    tube1 &= tube2;

    CHECK_THROWS(tube1 &= tubeTest4_05(););
    CHECK(tube1.codomain() == Interval(-2,4));
    CHECK(tube1[0] == Interval::EMPTY_SET);
    CHECK(tube1[2.5] == Interval(1,3));
    CHECK(tube1[15.0] == Interval(-2,0));
    CHECK(tube1[44.5] == Interval(0,2));
    CHECK(tube1[39] == Interval(-1));
    CHECK(tube1[Interval(0,6)] == Interval(1,4));
    CHECK(tube1[Interval(11,19)] == Interval(-2,1));
    CHECK(tube1[Interval(39.5,40.3)] == Interval(-1));

    // Gates
    tube1 = tubeTest1();
    tube2 = tubeTest2();

    tube1.set(Interval(4.), 0.);
    tube2.set(Interval(-1.,0.), 0.);
    tube1 &= tube2;
    CHECK(tube1[0.] == Interval::EMPTY_SET);
    CHECK(tube1[0] == Interval::EMPTY_SET);

    tube1.set(Interval(0.,2.), 40.);
    tube2.set(Interval(1.), 40.);
    tube1 &= tube2;
    CHECK(tube1[40.] == Interval(1.));
    CHECK(tube1[40] == Interval(-1.));
  }

  SECTION("Test tube1&traj2")
  {
    // todo
  }
}