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

TEST_CASE("Trajectory base", "[traj]")
{
  SECTION("Evaluation")
  {
    map<double,double> map_values;
    for(double t = 0. ; t < 10. ; t++)
      map_values[t] = t;

    Trajectory traj(map_values);
    REQUIRE(std::isnan(traj[-0.1]));
    REQUIRE(Approx(traj[0.0]) == 0.0);
    REQUIRE(Approx(traj[0.2]) == 0.2);
    REQUIRE(Approx(traj[8.2]) == 8.2);
    REQUIRE(Approx(traj[9.0]) == 9.0);
    REQUIRE(std::isnan(traj[9.1]));
  }

  SECTION("Update")
  {
    Trajectory traj;
    traj.set(-4.,3.);
    REQUIRE(ApproxIntv(traj.domain()) == Interval(-4.));
    traj.set(-2.,4.);
    REQUIRE(ApproxIntv(traj.domain()) == Interval(-4.,-2.));
    REQUIRE(std::isnan(traj[-5.]));
    REQUIRE(Approx(traj[-3.]) == 3.5);
  }

  SECTION("Domain")
  {
    map<double,double> map_values;
    for(double t = 0. ; t < 10. ; t++)
      map_values[t] = t;

    Trajectory traj(map_values);

    REQUIRE(ApproxIntv(traj.domain()) == Interval(0.,9.));
    traj.truncateDomain(Interval(-3.,5.));
    REQUIRE(ApproxIntv(traj.domain()) == Interval(0.,5.));
  }

  SECTION("Domain shift")
  {
    map<double,double> map_values;
    for(double t = 0. ; t < 10. ; t++)
      map_values[t] = t;

    Trajectory traj(map_values);

    REQUIRE(ApproxIntv(traj.domain()) == Interval(0.,9.));
    REQUIRE(Approx(traj[5.2]) == 5.2);
    traj.shiftDomain(2.);
    REQUIRE(ApproxIntv(traj.domain()) == Interval(-2.,7.));
    REQUIRE(Approx(traj[5.2]) == 7.2);
    REQUIRE(Approx(traj[3.2]) == 5.2);
  }
}