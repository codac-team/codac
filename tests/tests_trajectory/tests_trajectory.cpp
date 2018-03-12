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
using namespace std;

TEST_CASE("Trajectory base", "[traj]")
{
  SECTION("Evaluation")
  {
    map<double,double> map_values;
    for(double t = 0. ; t < 10. ; t++)
      map_values[t] = t;

    Trajectory traj(map_values);

    REQUIRE(isnan(traj[-0.1]));
    REQUIRE(Approx(traj[0.0]) == 0.0);
    REQUIRE(Approx(traj[0.2]) == 0.2);
    REQUIRE(Approx(traj[8.2]) == 8.2);
    REQUIRE(Approx(traj[9.0]) == 9.0);
    REQUIRE(isnan(traj[9.1]));
  }
}