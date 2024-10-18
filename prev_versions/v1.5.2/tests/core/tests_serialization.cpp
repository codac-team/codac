#include <cstdio>
#include "codac_serialize_trajectories.h"
#include "codac_serialize_tubes.h"
#include "catch_interval.hpp"
#include "tests_predefined_tubes.h"

// Using #define so that we can access protected methods
// of the class for tests purposes
#define protected public
#include "codac_TrajectoryVector.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;

TEST_CASE("serialization/deserialization of Tube")
{
  SECTION("Trajectories only")
  {
    string filename = "test_traj.tube";
    Trajectory traj1;
    traj1.set(0., 3.);
    traj1.set(1., 4.);

    ofstream obin_file(filename.c_str(), ios::out | ios::binary);
    serialize_Trajectory(obin_file, traj1);
    obin_file.close();

    Trajectory *traj2;
    ifstream ibin_file(filename.c_str(), ios::in | ios::binary);
    deserialize_Trajectory(ibin_file, traj2);
    remove(filename.c_str());

    CHECK(traj1 == *traj2);
    CHECK(traj1.tdomain() == traj2->tdomain());
    CHECK(traj2->tdomain() == Interval(3.,4.));

    ibin_file.close();
  }

  SECTION("Tubes only")
  {
    string filename = "test_tube.tube";
    Tube tube1(Interval(0.,11.), 0.02);

    ofstream obin_file(filename.c_str(), ios::out | ios::binary);
    serialize_Tube(obin_file, tube1);
    obin_file.close();

    Tube *tube2;
    ifstream ibin_file(filename.c_str(), ios::in | ios::binary);
    deserialize_Tube(ibin_file, tube2);
    remove(filename.c_str());

    CHECK(tube1 == *tube2);
    CHECK(tube1.tdomain() == tube2->tdomain());
    CHECK(tube2->tdomain() == Interval(0.,11.));

    ibin_file.close();
  }

  SECTION("No gates")
  {
    Tube tube1 = tube_test_1();
    string filename = "test_serialization_nogates.tube";
    tube1.serialize(filename);

    Trajectory *traj;
    CHECK_THROWS(Tube tube2(filename, traj););

    Tube tube4(filename);
    remove(filename.c_str());
    CHECK(tube1 == tube4);
  }

  SECTION("With gates")
  {
    Tube tube1 = tube_test_1();

    CHECK(tube1(0.) == Interval(4.,8.));
    CHECK(tube1(46.) == Interval(-1.,3.));
    CHECK(tube1(3.) == Interval(1.,4.));

    // todo: test a set value at 3.9

    tube1.set(Interval(2.,3.), 3.);
    tube1.set(Interval(7.), 0.);
    tube1.set(Interval::EMPTY_SET, 46.);

    CHECK(tube1(0.) == Interval(7.));
    CHECK(tube1(46.) == Interval::EMPTY_SET);
    CHECK(tube1(3.) == Interval(2.,3.));

    string filename = "test_serialization_gates.tube";
    tube1.serialize(filename);
    Tube tube2(filename);
    remove(filename.c_str());
    CHECK(tube1 == tube2);

    CHECK(tube2(0.) == Interval(7.));
    CHECK(tube2(46.) == Interval::EMPTY_SET);
    CHECK(tube2(3.) == Interval(2.,3.));
  }

  SECTION("With trajectories")
  {
    Tube tube1 = tube_test_1();
    Trajectory traj1, traj2, traj3;
    for(int i = 0 ; i < tube1.nb_slices() ; i++)
    {
      traj1.set(tube1(i).mid(), tube1.slice(i)->tdomain().mid());
      traj2.set(tube1(i).lb(), tube1.slice(i)->tdomain().mid());
      traj3.set(tube1(i).ub(), tube1.slice(i)->tdomain().mid());
    }

    string filename = "test_serialization_traj.tube";
    tube1.serialize(filename, traj1);
    Trajectory *traj4;
    Tube tube2(filename, traj4);
    remove(filename.c_str());
    CHECK(tube1 == tube2);
    CHECK(traj1 == *traj4);
  }

  SECTION("Vector case, no gates")
  {
    TubeVector tube1(Interval(0.,10.), 10);
    string filename = "test_serialization_nogates.tube";
    tube1.serialize(filename);

    TrajectoryVector *traj;
    CHECK_THROWS(TubeVector tube2(filename, traj););
    TubeVector tube4(filename);
    remove(filename.c_str());
    CHECK(tube1 == tube4);
  }

  SECTION("Vector case, with gates")
  {
    TubeVector tube1(Interval(0.,46.), 10);

    tube1.set(IntervalVector(10, Interval(2.,3.)), 0.);
    tube1.set(IntervalVector(10, Interval(7.)), 3.);
    tube1.set(IntervalVector(10, Interval::EMPTY_SET), 46.);

    CHECK(tube1(0.) == IntervalVector(10, Interval(2.,3.)));
    CHECK(tube1(3.) == IntervalVector(10, Interval(7.)));
    CHECK(tube1(46.) == IntervalVector(10, Interval::EMPTY_SET));

    string filename = "test_serialization_gates.tube";
    tube1.serialize(filename);
    TubeVector tube2(filename);
    remove(filename.c_str());
    CHECK(tube1 == tube2);

    CHECK(tube2(0.) == IntervalVector(10, Interval(2.,3.)));
    CHECK(tube2(3.) == IntervalVector(10, Interval(7.)));
    CHECK(tube2(46.) == IntervalVector(10, Interval::EMPTY_SET));
  }

  SECTION("Vector case, with trajectories")
  {
    TubeVector tube1(Interval(0.,46.), 1., 10);
    TrajectoryVector traj1(10), traj2(10), traj3(10);

    for(int k = 0 ; k < tube1.nb_slices() ; k++)
    {
      for(int i = 0 ; i < 10 ; i++)
      {
        traj1[i].set(tube1[i](k).mid(), tube1[i].slice(k)->tdomain().mid());
        traj2[i].set(tube1[i](k).lb(), tube1[i].slice(k)->tdomain().mid());
        traj3[i].set(tube1[i](k).ub(), tube1[i].slice(k)->tdomain().mid());
      }
    }

    string filename = "test_serialization_traj.tube";
    tube1.serialize(filename, traj1);
    TrajectoryVector *traj4;
    TubeVector tube2(filename, traj4);
    remove(filename.c_str());
    CHECK(tube1 == tube2);
    CHECK(traj1 == *traj4);
  }
}

bool test_serialization(const Tube& tube1)
{
  string filename = "test_serialization.tube";

  Trajectory traj_test1, *traj_test2;

  for(int i = 0 ; i < tube1.nb_slices() ; i++)
    traj_test1.set(tube1(i).is_unbounded() | tube1(i).is_empty() ? 1. : tube1(i).mid(),
                   tube1.slice(i)->tdomain().mid());

  tube1.serialize(filename, traj_test1); // serialization

  Tube tube2(filename, traj_test2); // deserialization
  remove(filename.c_str());

  bool equality = tube1 == tube2;
  equality &= traj_test1 == *traj_test2;

  return equality;
}

TEST_CASE("(de)serializations on bounded tubes", "[core]")
{
  SECTION("Test tube1")
  {
    CHECK(test_serialization(tube_test_1()));
  }

  SECTION("Test tube1(01)")
  {
    CHECK(test_serialization(tube_test_1_01()));    
  }

  SECTION("Test tube2")
  {
    CHECK(test_serialization(tube_test2()));
  }

  SECTION("Test tube3")
  {
    CHECK(test_serialization(tube_test3()));
  }

  SECTION("Test tube4")
  {
    CHECK(test_serialization(tube_test4()));
  }

  SECTION("Test tube4(05)")
  {
    CHECK(test_serialization(tube_test4_05()));
  }
}

TEST_CASE("(de)serializations on unbounded tubes", "[core]")
{
  SECTION("Test POS_REALS")
  {
    Tube tube = tube_test_1();
    tube.set(Interval::POS_REALS, tube.nb_slices() / 2);
    CHECK(test_serialization(tube));
    tube.set(Interval::POS_REALS);
    CHECK(test_serialization(tube));
  }

  SECTION("Test NEG_REALS")
  {
    Tube tube = tube_test2();
    tube.set(Interval::NEG_REALS, 5);
    CHECK(test_serialization(tube));
    tube.set(Interval::NEG_REALS);
    CHECK(test_serialization(tube));
  }

  SECTION("Test [-oo,5]")
  {
    Tube tube = tube_test2();
    tube.set(Interval::NEG_REALS | 5., 5);
    CHECK(test_serialization(tube));
    tube.set(Interval::NEG_REALS | 5.);
    CHECK(test_serialization(tube));
  }

  SECTION("Test [-5,oo]")
  {
    Tube tube = tube_test2();
    tube.set(Interval::POS_REALS | -5., 5);
    CHECK(test_serialization(tube));
    tube.set(Interval::POS_REALS | -5.);
    CHECK(test_serialization(tube));
  }

  SECTION("Test ALL_REALS")
  {
    Tube tube = tube_test3();
    tube.set(Interval::ALL_REALS, 1);
    CHECK(test_serialization(tube));
    tube.set(Interval::ALL_REALS);
    CHECK(test_serialization(tube));
  }

  SECTION("Test EMPTY_SET")
  {
    Tube tube = tube_test4();
    tube.set(Interval::EMPTY_SET, 0);
    tube.set(Interval::EMPTY_SET, 8);
    CHECK(test_serialization(tube));
    tube.set(Interval::EMPTY_SET);
    CHECK(test_serialization(tube));
  }
}