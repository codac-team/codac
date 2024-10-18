#include "catch_interval.hpp"
#include "codac_TFunction.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;

TEST_CASE("Tube definition")
{
  SECTION("Slice class")
  {
    Slice tubeslice(Interval(0.,1.), Interval(-1.,1.));
    CHECK(tubeslice.tdomain() == Interval(0.,1.));
    CHECK(tubeslice.codomain() == Interval(-1.,1.));

    Slice tubeslice_copy1 = tubeslice;
    CHECK(tubeslice_copy1.tdomain() == Interval(0.,1.));
    CHECK(tubeslice_copy1.codomain() == Interval(-1.,1.));
    CHECK(tubeslice_copy1 == tubeslice);

    Slice tubeslice_copy2(tubeslice);
    CHECK(tubeslice_copy2.tdomain() == Interval(0.,1.));
    CHECK(tubeslice_copy2.codomain() == Interval(-1.,1.));
    CHECK(tubeslice_copy2 == tubeslice);

    // todo: find a way to catch assert abort: CHECK_THROWS(Slice(Interval::ALL_REALS));
    // todo: find a way to catch assert abort: CHECK_THROWS(Slice(Interval::EMPTY_SET));
  }

  SECTION("Tube class")
  {
    Tube tube(Interval(0.,1.), Interval(-1.,1.));
    CHECK(tube.tdomain() == Interval(0.,1.));
    CHECK(tube.codomain() == Interval(-1.,1.));

    Tube tube_copy1 = tube;
    CHECK(tube_copy1.tdomain() == Interval(0.,1.));
    CHECK(tube_copy1.codomain() == Interval(-1.,1.));
    CHECK(tube_copy1 == tube);

    Tube tube_copy2(tube);
    CHECK(tube_copy2.tdomain() == Interval(0.,1.));
    CHECK(tube_copy2.codomain() == Interval(-1.,1.));
    CHECK(tube_copy2 == tube);

    // todo: find a way to catch assert abort: CHECK_THROWS(Tube(Interval::ALL_REALS));
    // todo: find a way to catch assert abort: CHECK_THROWS(Tube(Interval::EMPTY_SET));
    // todo: find a way to catch assert abort: CHECK_THROWS(Tube(Interval(5.9))); // degenerate domain
  }

  SECTION("TubeVector class")
  {
    TubeVector tube(Interval(0.,1.), Interval(-1.,1.));
    CHECK(tube.tdomain() == Interval(0.,1.));
    CHECK(tube.codomain() == Interval(-1.,1.));

    TubeVector tube_copy1 = tube;
    CHECK(tube_copy1.tdomain() == Interval(0.,1.));
    CHECK(tube_copy1.codomain() == Interval(-1.,1.));
    CHECK(tube_copy1 == tube);

    TubeVector tube_copy2(tube);
    CHECK(tube_copy2.tdomain() == Interval(0.,1.));
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
    CHECK(tube_a.tdomain() == Interval(0.,12.));

    Tube tube_b(Interval(0.,12.), 0.5, Interval(-3.,-2.));
    CHECK(tube_b.nb_slices() == 24);
    CHECK(tube_b.tdomain() == Interval(0.,12.));
    CHECK(tube_b(3) == Interval(-3.,-2.));
    CHECK(tube_b.codomain() == Interval(-3.,-2.));

    Tube tube_c(Interval(0.,4.), 1.1);
    CHECK(tube_c.nb_slices() == 4);
    //CHECK(tube_c.slice(0)->tube_reference() == &tube_c);
    //CHECK(tube_c.slice(1)->tube_reference() == &tube_c);
    //CHECK(tube_c.slice(2)->tube_reference() == &tube_c);
    //CHECK(tube_c.slice(3)->tube_reference() == &tube_c);
    CHECK(ApproxIntv(tube_c.slice(0)->tdomain()) == Interval(0.,1.1));
    CHECK(ApproxIntv(tube_c.slice(1)->tdomain()) == Interval(1.1,2.2));
    CHECK(ApproxIntv(tube_c.slice(2)->tdomain()) == Interval(2.2,3.3));
    CHECK(ApproxIntv(tube_c.slice(3)->tdomain()) == Interval(3.3,4.));

    Tube tube_d(tube_c);
    CHECK(tube_d == tube_c);
    CHECK(tube_d.codomain() == tube_c.codomain());
    CHECK(tube_d.nb_slices() == tube_c.nb_slices());
    //CHECK(tube_d.slice(0)->tube_reference() == &tube_d);
    //CHECK(tube_d.slice(0)->tube_reference() == tube_d.slice(1)->tube_reference());
    //CHECK(tube_d.slice(1)->tube_reference() == &tube_d);
    //CHECK(tube_d.slice(2)->tube_reference() == &tube_d);
    //CHECK(tube_d.slice(3)->tube_reference() == &tube_d);

    Tube tube_e(tube_c);
    tube_e.set(Interval(1.,2.));
    //CHECK(tube_e.slice(0)->tube_reference() == &tube_e);
    //CHECK(tube_e.slice(1)->tube_reference() == &tube_e);
    //CHECK(tube_e.slice(2)->tube_reference() == &tube_e);
    //CHECK(tube_e.slice(3)->tube_reference() == &tube_e);
    CHECK(tube_e.codomain() == Interval(1.,2.));
    CHECK(tube_e.nb_slices() == tube_c.nb_slices());
    CHECK(tube_e.slice(0)->codomain() == Interval(1.,2.));
    CHECK(tube_e.slice(1)->codomain() == Interval(1.,2.));
    CHECK(tube_e.slice(2)->codomain() == Interval(1.,2.));
    CHECK(tube_e.slice(3)->codomain() == Interval(1.,2.));

    // todo: find a way to catch assert abort: CHECK_THROWS(Tube tube_f(Interval(0.,12.), -1.););

    Tube tube_f(Interval(0.,12.), 0.);
    CHECK(tube_f.tdomain() == Interval(0.,12.));
    CHECK(tube_f.nb_slices() == 1);
  }

  SECTION("Tube class - TFunction")
  {
    Tube tube1(Interval(-1.,10.), 0.01, TFunction("t^2"));
    Trajectory traj1_inside(Interval(-1.,10.), TFunction("t^2"));
    Trajectory traj1_outside(Interval(-1.,10.), TFunction("t^2+2"));

    CHECK(tube1.contains(traj1_inside) == YES);
    CHECK(tube1.contains(traj1_outside) == NO);

    Tube tube2(Interval(-1.,10.), 0.01, TFunction("t^2+[-1,1]"));
    Trajectory traj3_lb(Interval(-1.,10.), TFunction("(t^2)-1"));
    Trajectory traj3_ub(Interval(-1.,10.), TFunction("(t^2)+1"));
    Tube tube3(traj3_lb, traj3_ub, 0.01);
    CHECK(tube2.nb_slices() == tube3.nb_slices());
    CHECK(tube2.tdomain() == tube3.tdomain());
    CHECK(tube2.codomain() == tube3.codomain());
    CHECK(tube2.volume() == tube3.volume());
    for(int i = 0 ; i < tube2.nb_slices() ; i++)
      CHECK(tube2(i) == tube3(i));
  }

  SECTION("Tube class - TFunction (thick slices)")
  {
    Tube tube(Interval(-1.,1.), 0.667, TFunction("t^2"));
    CHECK(tube.nb_slices() == 3);
    CHECK(ApproxIntv(tube(0)) == Interval(pow(-1. + 0.667, 2), 1.));
    CHECK(tube(-1.) == Interval(1.));
    CHECK(tube(1.) == Interval(1.));
    CHECK(tube(-0.9) == tube(0));
    CHECK(ApproxIntv(tube(1)) == Interval(0., pow(tube.slice(1)->tdomain().ub(), 2)));
  }

  SECTION("Tube class - 2 Trajectory")
  {
    Trajectory traj_lb(Interval(-1.,10.), TFunction("t^2"));
    Trajectory traj_ub(Interval(-1.,10.), TFunction("t^2-2"));
    
    Tube tube_1slice(traj_lb, traj_ub, traj_lb.tdomain().diam());
    CHECK(tube_1slice.nb_slices() == 1);
    CHECK(tube_1slice.tdomain() == Interval(-1.,10.));
    CHECK(tube_1slice.codomain() == Interval(-2.,100.));
    CHECK(Interval(28.25,30.25).is_subset(tube_1slice(5.5)));
    CHECK(tube_1slice.max_diam() > 2.);
    
    Tube tube_100slices(traj_lb, traj_ub, traj_ub.tdomain().diam() / 100.);
    CHECK(tube_100slices.nb_slices() == 100);
    CHECK(tube_100slices.tdomain() == Interval(-1.,10.));
    CHECK(tube_100slices.codomain() == Interval(-2.,100.));
    CHECK(Interval(28.25,30.25).is_subset(tube_100slices(5.5)));
    CHECK(tube_100slices(5.5).is_subset(Interval(28.,32.)));
    CHECK(tube_100slices.max_diam() > 2.);
  }

  SECTION("Tube class - 1 Trajectory")
  {
    Trajectory traj1(Interval(0.,1.1), TFunction("(t^2)*(-3+t*(1+2*(t^2)))"));
    Trajectory traj2(Interval(0.,1.1), TFunction("2*(t^5)+(t^3)-3*(t^2)"));
    
    Tube tube0(Interval(0.,1.1), 0.1, Interval::EMPTY_SET);
    CHECK(tube0.nb_slices() == 11);
    CHECK(tube0.codomain() == Interval::EMPTY_SET);
    tube0 |= traj2;

    Interval t = tube0.last_slice()->tdomain(); //(1.0,1.1);
    Interval slice_codomain = 2*pow(t,5)+pow(t,3)-3*pow(t,2);
    CHECK(tube0.last_slice()->codomain() == slice_codomain);
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

  SECTION("Tube class - vector of boxes")
  {
    IntervalVector box1(2), box2(2), box3(2), box4(2);

    box1[0] = Interval(-1.,10.);
    box1[1] = Interval(3.,4.);

    box2[0] = Interval(10.,10.5);
    box2[1] = Interval(2.,7.);

    box3[0] = Interval(10.5,12.);
    box3[1] = Interval(5.,6.);

    box4[0] = Interval(12.,14.);
    box4[1] = Interval(5.5);

    vector<Interval> v_domains, v_codomains;

    v_domains.push_back(box1[0]);
    v_domains.push_back(box2[0]);
    v_domains.push_back(box3[0]);
    v_domains.push_back(box4[0]);

    v_codomains.push_back(box1[1]);
    v_codomains.push_back(box2[1]);
    v_codomains.push_back(box3[1]);
    v_codomains.push_back(box4[1]);

    Tube tube_from_boxes(v_domains, v_codomains);

    CHECK(tube_from_boxes.tdomain() == Interval(-1.,14.));
    CHECK(tube_from_boxes.codomain() == Interval(2.,7.));
    CHECK(tube_from_boxes.nb_slices() == 4);

    CHECK(tube_from_boxes(0) == Interval(3.,4.));
    CHECK(tube_from_boxes(1) == Interval(2.,7.));
    CHECK(tube_from_boxes(2) == Interval(5.,6.));
    CHECK(tube_from_boxes(3) == Interval(5.5));

    CHECK(tube_from_boxes.slice(0)->tdomain() == Interval(-1.,10.));
    CHECK(tube_from_boxes.slice(1)->tdomain() == Interval(10.,10.5));
    CHECK(tube_from_boxes.slice(2)->tdomain() == Interval(10.5,12.));
    CHECK(tube_from_boxes.slice(3)->tdomain() == Interval(12.,14.));
  }

  SECTION("Tube class - vector of boxes - n-dim case")
  {
    IntervalVector box1(3), box2(3), box3(3), box4(3);

    box1[0] = Interval(-1.,10.);
    box1[1] = Interval(3.,4.);
    box1[2] = Interval(0.,2.);

    box2[0] = Interval(10.,10.5);
    box2[1] = Interval(2.,7.);
    box2[2] = Interval(1.,3.);

    box3[0] = Interval(10.5,12.);
    box3[1] = Interval(5.,6.);
    box3[2] = Interval(2.,4.);

    box4[0] = Interval(12.,14.);
    box4[1] = Interval(5.5);
    box4[2] = Interval(3.,5.);

    vector<Interval> v_domains;
    vector<IntervalVector> v_codomains;

    v_domains.push_back(box1[0]);
    v_domains.push_back(box2[0]);
    v_domains.push_back(box3[0]);
    v_domains.push_back(box4[0]);

    v_codomains.push_back(box1.subvector(1,2));
    v_codomains.push_back(box2.subvector(1,2));
    v_codomains.push_back(box3.subvector(1,2));
    v_codomains.push_back(box4.subvector(1,2));

    TubeVector tube_from_boxes(v_domains, v_codomains);

    CHECK(tube_from_boxes.size() == 2);
    CHECK(tube_from_boxes.tdomain() == Interval(-1.,14.));
    CHECK(tube_from_boxes.codomain()[0] == Interval(2.,7.));
    CHECK(tube_from_boxes.codomain()[1] == Interval(0.,5.));
    CHECK(tube_from_boxes.nb_slices() == 4);

    CHECK(tube_from_boxes[0](0) == Interval(3.,4.));
    CHECK(tube_from_boxes[0](1) == Interval(2.,7.));
    CHECK(tube_from_boxes[0](2) == Interval(5.,6.));
    CHECK(tube_from_boxes[0](3) == Interval(5.5));

    CHECK(tube_from_boxes[0].slice(0)->tdomain() == Interval(-1.,10.));
    CHECK(tube_from_boxes[0].slice(1)->tdomain() == Interval(10.,10.5));
    CHECK(tube_from_boxes[0].slice(2)->tdomain() == Interval(10.5,12.));
    CHECK(tube_from_boxes[0].slice(3)->tdomain() == Interval(12.,14.));

    CHECK(tube_from_boxes[1](0) == Interval(0.,2.));
    CHECK(tube_from_boxes[1](1) == Interval(1.,3.));
    CHECK(tube_from_boxes[1](2) == Interval(2.,4.));
    CHECK(tube_from_boxes[1](3) == Interval(3.,5.));

    CHECK(tube_from_boxes[1].slice(0)->tdomain() == Interval(-1.,10.));
    CHECK(tube_from_boxes[1].slice(1)->tdomain() == Interval(10.,10.5));
    CHECK(tube_from_boxes[1].slice(2)->tdomain() == Interval(10.5,12.));
    CHECK(tube_from_boxes[1].slice(3)->tdomain() == Interval(12.,14.));
  }
}
