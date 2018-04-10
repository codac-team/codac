#include "../catch/catch.hpp"
#include "../catch/catch_interval.hpp"
#include "tests.h"
#include <utility>

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

TEST_CASE("Exceptions")
{
  SECTION("DomainException")
  {
    // todo: static void check(const TubeNode& x, int slice_index);
    // todo: static void check(const TubeNode& x, double t);
    // todo: static void check(const TubeNode& x, const ibex::Interval& intv_t);
  }

  SECTION("EmptyException")
  {
    /*Tube tube(Interval(0.,1.), Interval::EMPTY_SET);
    CHECK_THROWS(EmptyException::check(tube));

    TubeSlice slice(Interval(0.,1.), Interval::EMPTY_SET);
    CHECK_THROWS(EmptyException::check(slice));*/
  }

  SECTION("StructureException")
  {
    // todo: static void check(const TubeNode& x1, const TubeNode& x2);
    // todo: static void check(const TubeTree& x1, const TubeTree& x2);
  }
}