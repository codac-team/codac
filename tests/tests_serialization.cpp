#include "tests.h"
#include <cstdio>

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
    string filename = "test_serialization_nogates.tube";
    tube1.serialize(filename);

    Trajectory traj;
    vector<Trajectory> v_trajs;
    CHECK_THROWS(Tube tube2(filename, traj););
    CHECK_THROWS(Tube tube3(filename, v_trajs););
    Tube tube4(filename);
    remove(filename.c_str());
    CHECK(tube1 == tube4);
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

    string filename = "test_serialization_gates.tube";
    tube1.serialize(filename);
    Tube tube2(filename);
    remove(filename.c_str());
    CHECK(tube1 == tube2);

    CHECK(tube2[0.] == Interval(7.));
    CHECK(tube2[46.] == Interval::EMPTY_SET);
    CHECK(tube2[3.] == Interval(2.,3.));
  }

  SECTION("With trajectories")
  {
    Tube tube1 = tubeTest1();
    Trajectory traj1, traj2, traj3;
    for(int i = 0 ; i < tube1.nbSlices() ; i++)
    {
      traj1.set(tube1.getSlice(i)->domain().mid(), tube1[i].mid());
      traj2.set(tube1.getSlice(i)->domain().mid(), tube1[i].lb());
      traj3.set(tube1.getSlice(i)->domain().mid(), tube1[i].ub());
    }

    string filename = "test_serialization_traj.tube";
    tube1.serialize(filename, traj1);
    Trajectory traj4;
    Tube tube2(filename, traj4);
    remove(filename.c_str());
    CHECK(tube1 == tube2);
    CHECK(traj1 == traj4);

    vector<Trajectory> v_trajs;
    v_trajs.push_back(traj1);
    v_trajs.push_back(traj2);
    v_trajs.push_back(traj3);

    filename = "test_serialization_trajs.tube";
    tube1.serialize(filename, v_trajs);
    vector<Trajectory> v_trajs_2;
    Tube tube3(filename, v_trajs_2);
    remove(filename.c_str());
    CHECK(v_trajs.size() == v_trajs_2.size());
    CHECK(tube1 == tube3);
    for(int i = 0 ; i < v_trajs.size() ; i++)
      CHECK(v_trajs[i] == v_trajs_2[i]);
  }
}