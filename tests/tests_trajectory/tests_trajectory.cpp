/* ============================================================================
 *  tubex-lib - Trajectory tests
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#define CATCH_CONFIG_MAIN
#include "../catch/catch.hpp"
#include "../catch/catch_interval.hpp"
#include "tubex_Trajectory.h"

using namespace Catch;
using namespace Detail;
using namespace tubex;
using namespace ibex;
using namespace std;

TEST_CASE("Trajectory base")
{
  SECTION("Evaluation")
  {
    map<double,double> map_values;
    for(double t = 0. ; t < 10. ; t++)
      map_values[t] = t;

    Trajectory traj1(map_values);
    CHECK_THROWS(traj1[-0.1];);
    CHECK(Approx(traj1[0.0]) == 0.0);
    CHECK(Approx(traj1[0.2]) == 0.2);
    CHECK(Approx(traj1[8.2]) == 8.2);
    CHECK(Approx(traj1[9.0]) == 9.0);
    CHECK_THROWS(traj1[9.1];);

    Trajectory traj2;
    traj2.set(0., 1.);
    traj2.set(2., -1.);
    traj2.set(6., 1.);
    traj2.set(7., 2.);
    traj2.set(8., 4.);
    traj2.set(10., 4.);
    traj2.set(11., -2.);
    traj2.set(14., 1.);
    CHECK(traj2.domain() == Interval(0.,14.));
    CHECK(traj2.codomain() == Interval(-2.,4.));
    CHECK_THROWS(traj2[-0.1];);
    CHECK(traj2[0.] == 1.);
    CHECK(traj2[1.] == 0.);
    CHECK(traj2[4.] == 0.);
    CHECK(traj2[9.] == 4.);
    CHECK(traj2[13.] == 0.);
    CHECK(traj2[14.] == 1.);
    CHECK(traj2[Interval(2.,9.)] == Interval(-1.,4.));
    CHECK(traj2[Interval(3.,12.)] == Interval(-2.,4.));
    CHECK(traj2[traj2.domain()] == Interval(-2.,4.));
  }

  SECTION("Update")
  {
    Trajectory traj;
    traj.set(-4.,3.);
    CHECK(ApproxIntv(traj.domain()) == Interval(-4.));
    traj.set(-2.,4.);
    CHECK(ApproxIntv(traj.domain()) == Interval(-4.,-2.));
    CHECK_THROWS(traj[-5.];);
    CHECK(Approx(traj[-3.]) == 3.5);
  }

  SECTION("Domain")
  {
    map<double,double> map_values;
    for(double t = 0. ; t < 10. ; t++)
      map_values[t] = t;

    Trajectory traj1(map_values);

    CHECK(ApproxIntv(traj1.domain()) == Interval(0.,9.));
    CHECK(ApproxIntv(traj1.codomain()) == Interval(0.,9.));
    traj1.truncateDomain(Interval(-3.,5.));
    CHECK(ApproxIntv(traj1.domain()) == Interval(0.,5.));
    CHECK(ApproxIntv(traj1.codomain()) == Interval(0.,5.));

    Trajectory traj2(Function("t", "t^2"), Interval(-1.,10.));
    CHECK(traj2.domain() == Interval(-1.,10.));
    CHECK(traj2.codomain() == Interval(0.,100.));
    CHECK(Approx(traj2[5.3]) == 28.09);
  }

  SECTION("Domain shift")
  {
    map<double,double> map_values;
    for(double t = 0. ; t < 10. ; t++)
      map_values[t] = t;

    Trajectory traj(map_values);

    CHECK(ApproxIntv(traj.domain()) == Interval(0.,9.));
    CHECK(Approx(traj[5.2]) == 5.2);
    traj.shiftDomain(2.);
    CHECK(ApproxIntv(traj.domain()) == Interval(-2.,7.));
    CHECK(Approx(traj[5.2]) == 7.2);
    CHECK(Approx(traj[3.2]) == 5.2);
  }
}