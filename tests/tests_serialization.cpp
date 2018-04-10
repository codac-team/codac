#include "tests.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

TEST_CASE("serialization/deserialization of Tube")
{
  SECTION("No gates")
  {
    Tube tube1 = tubeTest1();
    string filename = "test_serialization.tube";
    tube1.serialize(filename);

    Tube tube2("test_serialization.tube");
    CHECK(tube1 == tube2);
  }

  SECTION("With gates")
  {
    Tube tube1 = tubeTest1();

    CHECK(tube1[0.] == Interval(4.,8.));
    CHECK(tube1[46.] == Interval(-1.,3.));
    CHECK(tube1[3.] == Interval(1.,4.));

    // todo: test a set value at 3.9

    tube1.set(Interval(2.,3.), 3.);
    tube1.set(Interval(7.), 0.);
    tube1.set(Interval::EMPTY_SET, 46.);

    CHECK(tube1[0.] == Interval(7.));
    CHECK(tube1[46.] == Interval::EMPTY_SET);
    CHECK(tube1[3.] == Interval(2.,3.));

    string filename = "test_serialization.tube";
    tube1.serialize(filename);
    Tube tube2("test_serialization.tube");
    CHECK(tube1 == tube2);

    CHECK(tube2[0.] == Interval(7.));
    CHECK(tube2[46.] == Interval::EMPTY_SET);
    CHECK(tube2[3.] == Interval(2.,3.));
  }
}