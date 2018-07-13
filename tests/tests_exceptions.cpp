#include "tests.h"
#include <utility>
#include "tubex_EmptyException.h"
#include "tubex_SamplingException.h"
#include "tubex_DomainException.h"
#include "tubex_DimensionException.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

TEST_CASE("Exceptions")
{
  SECTION("DomainException")
  {
    Tube tube1(Interval(0.,1.)), tube2(Interval(0.,1.4));
    CHECK_THROWS(DomainException::check(tube1, tube2));
    CHECK_THROWS(DomainException::check(tube1, -3.));
    CHECK_THROWS(DomainException::check(tube1, 2));
    CHECK_THROWS(DomainException::check(tube1, Interval(-1.,5.)));

    TubeSlice slice1(Interval(0.,1.)), slice2(Interval(0.,1.4));
    CHECK_THROWS(DomainException::check(slice1, slice2));
    CHECK_THROWS(DomainException::check(slice1, -3.));
    CHECK_THROWS(DomainException::check(slice1, 2));
    CHECK_THROWS(DomainException::check(slice1, Interval(-1.,5.)));
  }

  SECTION("EmptyException")
  {
    Tube tube(Interval(0.,1.), Interval::EMPTY_SET);
    CHECK_THROWS(EmptyException::check(tube));

    TubeSlice slice(Interval(0.,1.), IntervalVector(1, Interval::EMPTY_SET));
    CHECK_THROWS(EmptyException::check(slice));
  }

  SECTION("SamplingException")
  {
    Tube tube1(Interval(0.,1.), 1.), tube2(Interval(0.,1.), 4.), tube3(Interval(0.,1.), 0.5);
    CHECK(tube1.nb_slices() == tube2.nb_slices());
    CHECK_THROWS(SamplingException::check(tube1, tube3));
  }
}