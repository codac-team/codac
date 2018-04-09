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
    Tube tube_a(Interval(0.,12.), 1.);
    CHECK(tube_a.nbSlices() == 12);
    CHECK(tube_a.domain() == Interval(0.,12.));
    CHECK(tube_a.getFirstTubeNode()->domain() == Interval(0.,6.));
    CHECK(tube_a.getFirstTubeNode()->nbSlices() == 6);
    CHECK(tube_a.getSecondTubeNode()->domain() == Interval(6.,12.));
    CHECK(tube_a.getSecondTubeNode()->nbSlices() == 6);
    CHECK(((TubeTree*)tube_a.getFirstTubeNode())->getFirstTubeNode()->domain() == Interval(0.,3.));
    CHECK(((TubeTree*)tube_a.getFirstTubeNode())->getFirstTubeNode()->nbSlices() == 3);
    CHECK(((TubeTree*)tube_a.getFirstTubeNode())->getSecondTubeNode()->domain() == Interval(3.,6.));
    CHECK(((TubeTree*)tube_a.getFirstTubeNode())->getSecondTubeNode()->nbSlices() == 3);

    Tube tube_b(Interval(0.,12.), 0.5, Interval(-3.,-2.));
    CHECK(tube_b.nbSlices() == 24);
    CHECK(tube_b.domain() == Interval(0.,12.));
    CHECK(tube_b[3] == Interval(-3.,-2.));
    CHECK(tube_b.codomain() == Interval(-3.,-2.));

    Tube tube_c(Interval(0.,4.), 1.1);
    CHECK(tube_c.nbSlices() == 4);
    CHECK(ApproxIntv(tube_c.getSlice(0)->domain()) == Interval(0.,1.1));
    CHECK(ApproxIntv(tube_c.getSlice(1)->domain()) == Interval(1.1,2.2));
    CHECK(ApproxIntv(tube_c.getSlice(2)->domain()) == Interval(2.2,3.3));
    CHECK(ApproxIntv(tube_c.getSlice(3)->domain()) == Interval(3.3,4.));

    Tube tube_d(tube_c);
    CHECK(tube_d.codomain() == tube_c.codomain());
    CHECK(tube_d.nbSlices() == tube_c.nbSlices());

    Tube tube_e(tube_c, Interval(1.,2.));
    CHECK(tube_e.codomain() == Interval(1.,2.));
    CHECK(tube_e.nbSlices() == tube_c.nbSlices());

    CHECK_THROWS(Tube tube_f(Interval(0.,12.), -1.););

    Tube tube_f(Interval(0.,12.), 0.);
    CHECK(tube_f.domain() == Interval(0.,12.));
    CHECK(tube_f.nbSlices() == 1);
  }

  SECTION("Tube class - Trajectory")
  {
    // todo...
  }
}
