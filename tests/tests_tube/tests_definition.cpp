#include "../catch/catch.hpp"
#include "../catch/catch_interval.hpp"
#include "tubex.h"
#include "tubex_TubeSlice.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

TEST_CASE("Tube definition", "[core]")
{
  SECTION("TubeSlice class")
  {
    TubeSlice tubeslice(Interval(0.,1.), Interval(-1.,1.));
    CHECK(tubeslice.domain() == Interval(0.,1.));
    CHECK(tubeslice.codomain() == Interval(-1.,1.));

    TubeSlice tubeslice_copy1 = tubeslice;
    CHECK(tubeslice_copy1.domain() == Interval(0.,1.));
    CHECK(tubeslice_copy1.codomain() == Interval(-1.,1.));

    TubeSlice tubeslice_copy2(tubeslice);
    CHECK(tubeslice_copy2.domain() == Interval(0.,1.));
    CHECK(tubeslice_copy2.codomain() == Interval(-1.,1.));

    CHECK_THROWS(TubeSlice(Interval::ALL_REALS));
    CHECK_THROWS(TubeSlice(Interval::EMPTY_SET));
  }

  SECTION("TubeTree class")
  {
    TubeTree tubetree(Interval(0.,1.), Interval(-1.,1.));
    CHECK(tubetree.domain() == Interval(0.,1.));
    CHECK(tubetree.codomain() == Interval(-1.,1.));

    TubeTree tubetree_copy1 = tubetree;
    CHECK(tubetree_copy1.domain() == Interval(0.,1.));
    CHECK(tubetree_copy1.codomain() == Interval(-1.,1.));

    TubeTree tubetree_copy2(tubetree);
    CHECK(tubetree_copy2.domain() == Interval(0.,1.));
    CHECK(tubetree_copy2.codomain() == Interval(-1.,1.));

    CHECK_THROWS(TubeTree(Interval::ALL_REALS));
    CHECK_THROWS(TubeTree(Interval::EMPTY_SET));
  }

  SECTION("Tube class")
  {
    // todo
  }
}
