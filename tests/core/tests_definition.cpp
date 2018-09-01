#include "tests.h"
#include "tubex_Function.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

TEST_CASE("Tube definition")
{
  SECTION("Slice class")
  {
    Slice tubeslice(Interval(0.,1.), Interval(-1.,1.));
    CHECK(tubeslice.domain() == Interval(0.,1.));
    CHECK(tubeslice.codomain() == Interval(-1.,1.));

    Slice tubeslice_copy1 = tubeslice;
    CHECK(tubeslice_copy1.domain() == Interval(0.,1.));
    CHECK(tubeslice_copy1.codomain() == Interval(-1.,1.));
    CHECK(tubeslice_copy1 == tubeslice);

    Slice tubeslice_copy2(tubeslice);
    CHECK(tubeslice_copy2.domain() == Interval(0.,1.));
    CHECK(tubeslice_copy2.codomain() == Interval(-1.,1.));
    CHECK(tubeslice_copy2 == tubeslice);

    // todo: find a way to catch assert abort: CHECK_THROWS(Slice(Interval::ALL_REALS));
    // todo: find a way to catch assert abort: CHECK_THROWS(Slice(Interval::EMPTY_SET));
  }

  SECTION("Tube class")
  {
    Tube tube(Interval(0.,1.), Interval(-1.,1.));
    CHECK(tube.domain() == Interval(0.,1.));
    CHECK(tube.codomain() == Interval(-1.,1.));

    Tube tube_copy1 = tube;
    CHECK(tube_copy1.domain() == Interval(0.,1.));
    CHECK(tube_copy1.codomain() == Interval(-1.,1.));
    CHECK(tube_copy1 == tube);

    Tube tube_copy2(tube);
    CHECK(tube_copy2.domain() == Interval(0.,1.));
    CHECK(tube_copy2.codomain() == Interval(-1.,1.));
    CHECK(tube_copy2 == tube);

    // todo: find a way to catch assert abort: CHECK_THROWS(Tube(Interval::ALL_REALS));
    // todo: find a way to catch assert abort: CHECK_THROWS(Tube(Interval::EMPTY_SET));
    // todo: find a way to catch assert abort: CHECK_THROWS(Tube(Interval(5.9))); // degenerate domain
  }

  SECTION("TubeVector class")
  {
    TubeVector tube(Interval(0.,1.), Interval(-1.,1.));
    CHECK(tube.domain() == Interval(0.,1.));
    CHECK(tube.codomain() == Interval(-1.,1.));

    TubeVector tube_copy1 = tube;
    CHECK(tube_copy1.domain() == Interval(0.,1.));
    CHECK(tube_copy1.codomain() == Interval(-1.,1.));
    CHECK(tube_copy1 == tube);

    TubeVector tube_copy2(tube);
    CHECK(tube_copy2.domain() == Interval(0.,1.));
    CHECK(tube_copy2.codomain() == Interval(-1.,1.));
    CHECK(tube_copy2 == tube);

    // todo: find a way to catch assert abort: CHECK_THROWS(TubeVector(Interval::ALL_REALS));
    // todo: find a way to catch assert abort: CHECK_THROWS(TubeVector(Interval::EMPTY_SET));
    // todo: find a way to catch assert abort: CHECK_THROWS(TubeVector(Interval(5.9))); // degenerate domain
  }

  SECTION("Tube class")
  {
    Tube tube_a(Interval(0.,12.), 1.);
    CHECK(tube_a.nb_slices() == 12);
    CHECK(tube_a.domain() == Interval(0.,12.));

    Tube tube_b(Interval(0.,12.), 0.5, Interval(-3.,-2.));
    CHECK(tube_b.nb_slices() == 24);
    CHECK(tube_b.domain() == Interval(0.,12.));
    CHECK(tube_b(3) == Interval(-3.,-2.));
    CHECK(tube_b.codomain() == Interval(-3.,-2.));

    Tube tube_c(Interval(0.,4.), 1.1);
    CHECK(tube_c.nb_slices() == 4);
    //CHECK(tube_c.get_slice(0)->tube_reference() == &tube_c);
    //CHECK(tube_c.get_slice(1)->tube_reference() == &tube_c);
    //CHECK(tube_c.get_slice(2)->tube_reference() == &tube_c);
    //CHECK(tube_c.get_slice(3)->tube_reference() == &tube_c);
    CHECK(ApproxIntv(tube_c.get_slice(0)->domain()) == Interval(0.,1.1));
    CHECK(ApproxIntv(tube_c.get_slice(1)->domain()) == Interval(1.1,2.2));
    CHECK(ApproxIntv(tube_c.get_slice(2)->domain()) == Interval(2.2,3.3));
    CHECK(ApproxIntv(tube_c.get_slice(3)->domain()) == Interval(3.3,4.));

    Tube tube_d(tube_c);
    CHECK(tube_d == tube_c);
    CHECK(tube_d.codomain() == tube_c.codomain());
    CHECK(tube_d.nb_slices() == tube_c.nb_slices());
    //CHECK(tube_d.get_slice(0)->tube_reference() == &tube_d);
    //CHECK(tube_d.get_slice(0)->tube_reference() == tube_d.get_slice(1)->tube_reference());
    //CHECK(tube_d.get_slice(1)->tube_reference() == &tube_d);
    //CHECK(tube_d.get_slice(2)->tube_reference() == &tube_d);
    //CHECK(tube_d.get_slice(3)->tube_reference() == &tube_d);

    Tube tube_e(tube_c, Interval(1.,2.));
    //CHECK(tube_e.get_slice(0)->tube_reference() == &tube_e);
    //CHECK(tube_e.get_slice(1)->tube_reference() == &tube_e);
    //CHECK(tube_e.get_slice(2)->tube_reference() == &tube_e);
    //CHECK(tube_e.get_slice(3)->tube_reference() == &tube_e);
    CHECK(tube_e.codomain() == Interval(1.,2.));
    CHECK(tube_e.nb_slices() == tube_c.nb_slices());
    CHECK(tube_e.get_slice(0)->codomain() == Interval(1.,2.));
    CHECK(tube_e.get_slice(1)->codomain() == Interval(1.,2.));
    CHECK(tube_e.get_slice(2)->codomain() == Interval(1.,2.));
    CHECK(tube_e.get_slice(3)->codomain() == Interval(1.,2.));

    // todo: find a way to catch assert abort: CHECK_THROWS(Tube tube_f(Interval(0.,12.), -1.););

    Tube tube_f(Interval(0.,12.), 0.);
    CHECK(tube_f.domain() == Interval(0.,12.));
    CHECK(tube_f.nb_slices() == 1);
  }

  SECTION("Tube class - Function")
  {
    Tube tube1(Interval(-1.,10.), 0.01, tubex::Function("t^2"));
    Trajectory traj1_inside(Interval(-1.,10.), tubex::Function("t^2"));
    Trajectory traj1_outside(Interval(-1.,10.), tubex::Function("t^2+2"));

    CHECK(tube1.contains(traj1_inside));
    CHECK_FALSE(tube1.contains(traj1_outside));

    Tube tube2(Interval(-1.,10.), 0.01, tubex::Function("t^2+[-1,1]"));
    Trajectory traj3_lb(Interval(-1.,10.), tubex::Function("(t^2)-1"));
    Trajectory traj3_ub(Interval(-1.,10.), tubex::Function("(t^2)+1"));
    Tube tube3(traj3_lb, traj3_ub, 0.01);
    CHECK(tube2.nb_slices() == tube3.nb_slices());
    CHECK(tube2.domain() == tube3.domain());
    CHECK(tube2.codomain() == tube3.codomain());
    CHECK(tube2.volume() == tube3.volume());
    for(int i = 0 ; i < tube2.nb_slices() ; i++)
      CHECK(tube2(i) == tube3(i));
  }

  SECTION("Tube class - Function (thick slices)")
  {
    Tube tube(Interval(-1.,1.), 0.667, tubex::Function("t^2"));
    CHECK(tube.nb_slices() == 3);
    CHECK(tube(0) == Interval(pow(-1. + 0.667, 2), 1.));
    CHECK(tube(-1.) == Interval(1.));
    CHECK(tube(1.) == Interval(1.));
    CHECK(tube(-0.9) == tube(0));
    CHECK(ApproxIntv(tube(1)) == Interval(0., pow(tube.get_slice(1)->domain().ub(), 2)));
  }

  SECTION("Tube class - 2 Trajectory")
  {
    Trajectory traj_lb(Interval(-1.,10.), tubex::Function("t^2"));
    Trajectory traj_ub(Interval(-1.,10.), tubex::Function("t^2-2"));
    
    Tube tube_1slice(traj_lb, traj_ub, traj_lb.domain().diam());
    CHECK(tube_1slice.nb_slices() == 1);
    CHECK(tube_1slice.domain() == Interval(-1.,10.));
    CHECK(tube_1slice.codomain() == Interval(-2.,100.));
    CHECK(Interval(28.25,30.25).is_subset(tube_1slice(5.5)));
    CHECK(tube_1slice.max_thickness() > 2.);
    
    Tube tube_100slices(traj_lb, traj_ub, traj_ub.domain().diam() / 100.);
    CHECK(tube_100slices.nb_slices() == 100);
    CHECK(tube_100slices.domain() == Interval(-1.,10.));
    CHECK(tube_100slices.codomain() == Interval(-2.,100.));
    CHECK(Interval(28.25,30.25).is_subset(tube_100slices(5.5)));
    CHECK(tube_100slices(5.5).is_subset(Interval(28.,32.)));
    CHECK(tube_100slices.max_thickness() > 2.);
  }

  SECTION("Tube class - 1 Trajectory")
  {
    Trajectory traj1(Interval(0.,1.1), tubex::Function("(t^2)*(-3+t*(1+2*(t^2)))"));
    Trajectory traj2(Interval(0.,1.1), tubex::Function("2*(t^5)+(t^3)-3*(t^2)"));
    
    Tube tube0(Interval(0.,1.1), 0.1, Interval::EMPTY_SET);
    CHECK(tube0.nb_slices() == 12);
    CHECK(tube0.codomain() == Interval::EMPTY_SET);
    tube0 |= traj2;

    Interval t = tube0.get_last_slice()->prev_slice()->domain(); //(1.0,1.1);
    Interval slice_codomain = 2*pow(t,5)+pow(t,3)-3*pow(t,2);
    CHECK(tube0.get_last_slice()->prev_slice()->codomain() == slice_codomain);
    CHECK(tube0.codomain().ub() == slice_codomain.ub());

    Tube tube2(traj2, 0.1);
    CHECK(tube2(0.) == tube0(0.));
    CHECK(ApproxIntv(tube2(1.1)) == tube0(1.1));
    CHECK(ApproxTube(tube2) == tube0);
    Tube tube1 = tube2;
    tube1.inflate(0.001);
    CHECK(tube2.is_subset(tube1));
    CHECK(tube2.is_strict_subset(tube1));
  }
}
