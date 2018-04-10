#include "../catch/catch.hpp"
#include "../catch/catch_interval.hpp"
#include "tests.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

TEST_CASE("Tube definition")
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

  SECTION("Tube class - 2 Trajectory")
  {
    Trajectory traj_lb(Function("t", "t^2"), Interval(-1.,10.));
    Trajectory traj_ub(Function("t", "t^2-2"), Interval(-1.,10.));
    
    Tube tube_1slice(traj_lb, traj_ub);
    CHECK(tube_1slice.nbSlices() == 1);
    CHECK(tube_1slice.domain() == Interval(-1.,10.));
    CHECK(tube_1slice.codomain() == Interval(-2.,100.));
    CHECK(Interval(28.25,30.25).is_subset(tube_1slice[5.5]));
    CHECK(tube_1slice.maxThickness() > 2.);
    
    Tube tube_100slices(traj_lb, traj_ub, traj_ub.domain().diam() / 100.);
    CHECK(tube_100slices.nbSlices() == 100);
    CHECK(tube_100slices.domain() == Interval(-1.,10.));
    CHECK(tube_100slices.codomain() == Interval(-2.,100.));
    CHECK(Interval(28.25,30.25).is_subset(tube_100slices[5.5]));
    CHECK(tube_100slices[5.5].is_subset(Interval(28.,32.)));
    CHECK(tube_100slices.maxThickness() > 2.);
  }

  SECTION("Tube class - 1 Trajectory")
  {
    Trajectory traj1(Function("t", "(t^2)*(-3+t*(1+2*(t^2)))"), Interval(0.,1.1));
    Trajectory traj2(Function("t", "2*(t^5)+(t^3)-3*(t^2)"), Interval(0.,1.1));
    
    Tube tube0(Interval(0.,1.1), 0.1, Interval::EMPTY_SET);
    CHECK(tube0.nbSlices() == 12);
    CHECK(tube0.codomain() == Interval::EMPTY_SET);
    tube0 |= traj2;

    Interval t = tube0.getLastSlice()->prevSlice()->domain(); //(1.0,1.1);
    Interval slice_codomain = 2*pow(t,5)+pow(t,3)-3*pow(t,2);
    CHECK(tube0.getLastSlice()->prevSlice()->codomain() == slice_codomain);
    CHECK(tube0.codomain().ub() == slice_codomain.ub());

    Tube tube1(traj1, 0., 0.1);
    Tube tube2(traj2, 0., 0.1);
    CHECK(tube2 == tube0);
    CHECK(tube1.isSubset(tube2));
    CHECK(tube1.isStrictSubset(tube2));
  }
}
