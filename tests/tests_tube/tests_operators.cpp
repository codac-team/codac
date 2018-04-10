#include "../catch/catch.hpp"
#include "../catch/catch_interval.hpp"
#include "tests.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

TEST_CASE("Operators", "[core]")
{
  SECTION("Test tube1")
  {
    Tube tube = tubeTest1();
    tube.set(Interval(-4,2), 14); // to test primitives pre-computation

    CHECK(tube.nbSlices() == 46);
  }
}