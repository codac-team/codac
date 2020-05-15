/** 
 *  tubex-lib - Examples
 *  How to compute a paving from a tube
 * ----------------------------------------------------------------------------
 *
 *  \brief      Simple example
 *  \date       2019
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <tubex.h>
#include <tubex-rob.h>

using namespace std;
using namespace tubex;


int main()
{
  /* =========== TUBE =========== */

    Interval tdomain(0., 2.*M_PI);
    TrajectoryVector traj_bound_1(tdomain, TFunction("(2*sin(t)-0.2;sin(2*t)-0.2)"));
    TrajectoryVector traj_bound_2(tdomain, TFunction("(2*sin(t)-0.2;sin(2*t)+0.2)"));
    TrajectoryVector traj_bound_3(tdomain, TFunction("(2*sin(t)+0.2;sin(2*t)-0.2)"));
    TrajectoryVector traj_bound_4(tdomain, TFunction("(2*sin(t)+0.2;sin(2*t)+0.2)"));

    TubeVector x(traj_bound_1, 0.1);
    x |= traj_bound_1;
    x |= traj_bound_2;
    x |= traj_bound_3;

    Tube::enable_syntheses(); // faster integral computations


  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();

    VIBesFigMap fig_map("Map");
    fig_map.set_properties(50, 50, 500, 300);
    fig_map.add_tube(&x, "x", 0, 1);
    fig_map.add_trajectory(&traj_bound_1, "bound1", 0, 1);
    fig_map.add_trajectory(&traj_bound_2, "bound2", 0, 1);
    fig_map.add_trajectory(&traj_bound_3, "bound3", 0, 1);
    fig_map.add_trajectory(&traj_bound_4, "bound4", 0, 1);
    fig_map.show(0.);


  /* =========== PAVING =========== */

    TubePaving pav(fig_map.view_box());
    pav.compute(0.08, x);


  /* =========== GRAPHICS =========== */

    VIBesFigPaving fig_pav("Tube paving", &pav);
    fig_pav.set_properties(50, 400, 500, 300);
    fig_pav.axis_limits(fig_map.view_box());
    fig_pav.show();
    vibes::endDrawing();


  /* =========== ENDING =========== */

  // Checking if this example still works:
  return (fabs(x.volume() - 6.6203) < 1e-2) ? EXIT_SUCCESS : EXIT_FAILURE;
}