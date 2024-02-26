/** 
 *  Codac - Examples
 *  Proving the existence of loops in robot trajectories
 * ----------------------------------------------------------------------------
 *
 *  \brief      Example from the papers:
 *              "Proving the existence of loops in robot trajectories"
 *              Simon Rohou, Peter Franek, Clément Aubry, Luc Jaulin
 *
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <codac.h>
#include <codac-rob.h>

using namespace std;
using namespace codac;


int main(int argc, char** argv)
{
  Tube::enable_syntheses(); // faster integral computations

  /* =========== LOADING DATA =========== */

    float dt = 0.01;
    Interval tdomain(-1., 10.);
    TrajectoryVector x_truth(tdomain, TFunction("(10*cos(t)+t;5*sin(2*t)+t)"));
    TubeVector x(tdomain, dt, 2);
    TubeVector v(tdomain, dt, TFunction("(-10*sin(t)+1+[-0.2,0.2];10*cos(2*t)+1+[-0.2,0.2])"));

    x.set(x_truth(tdomain.lb()), tdomain.lb());

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, v); // consistency between x and v

  /* =========== LOOPS DETECTION =========== */

    clock_t t_start = clock();

    TPlane tplane(x.tdomain());
    tplane.compute_detections(dt*2., x, v);
    tplane.compute_proofs(x, v);

    printf("t-plane computation time: %.2fs\n", (double)(clock() - t_start)/CLOCKS_PER_SEC);

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();

    VIBesFigPaving fig_tplane("t-plane", &tplane);
    fig_tplane.set_properties(550, 150, 500, 500);
    fig_tplane.show();

    ColorMap colormap;
    colormap.add_color_point(make_rgb((float)0.9, 0.9, 0.9), 0.);
    colormap.add_color_point(make_rgb((float)0., 0., 0.), 0.5);
    colormap.add_color_point(make_rgb((float)0.28, 0.51, 0.), 1.);

    Trajectory traj_colormap = tplane.traj_loops_summary();

    /*VIBesFigTube fig_colors("Colors");
    fig_colors.set_properties(450, 50, 800, 400);
    fig_colors.add_trajectory(&traj_colormap, "c");
    fig_colors.show();*/

    VIBesFigMap fig_map("Map");
    fig_map.set_properties(450, 50, 800, 800);
    fig_map.add_tube(&x, "x", 0, 1);
    fig_map.add_trajectory(&x_truth, "x*", 0, 1);
    fig_map.set_trajectory_color(&x_truth, colormap, &traj_colormap);
    fig_map.show(1.);

    vector<IntervalVector> v_loops;

    v_loops = tplane.detected_loops();
    for(size_t i = 0 ; i < v_loops.size() ; i++)
      fig_tplane.draw_box(v_loops[i], "red");

    v_loops = tplane.proven_loops();
    for(size_t i = 0 ; i < v_loops.size() ; i++)
      fig_tplane.draw_box(v_loops[i], "green");

    vibes::endDrawing();

  /* =========== ENDING =========== */

  // Checking if this example still works:
  return (tplane.nb_loops_detections() == 5 && tplane.nb_loops_proofs() == 4) ? EXIT_SUCCESS : EXIT_FAILURE;
}
