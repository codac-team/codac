/** 
 *  tubex-lib - Examples
 *  Proving the existence of loops in robot trajectories
 * ----------------------------------------------------------------------------
 *
 *  \brief      Example from the papers:
 *              "Proving the existence of loops in robot trajectories"
 *              Simon Rohou, Peter Franek, Clément Aubry, Luc Jaulin
 *
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <tubex.h>
#include <tubex-rob.h>

using namespace std;
using namespace tubex;

TubeVector *x, *v;

IntervalVector f(const IntervalVector& input) // inclusion function
{
  return v->integral(input[0], input[1])
       & ((*x)(input[1]) - (*x)(input[0]));
}

int main(int argc, char** argv)
{
  Tube::enable_syntheses(); // faster integral computations

  /* =========== LOADING DATA =========== */

    float dt = 0.01;
    Interval tdomain(-1., 10.);
    TrajectoryVector x_truth(tdomain, TFunction("(10*cos(t)+t;5*sin(2*t)+t)"));
    x = new TubeVector(tdomain, dt, 2);
    v = new TubeVector(tdomain, dt, TFunction("(-10*sin(t)+1+[-0.2,0.2];10*cos(2*t)+1+[-0.2,0.2])"));

    x->set(x_truth(tdomain.lb()), tdomain.lb());

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(*x, *v);

  /* =========== LOOPS DETECTION =========== */

    clock_t t_start = clock();

    TPlane tplane;
    tplane.compute_detections(dt*2., *x, *v);
    tplane.compute_proofs(f);

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
    fig_map.add_tube(x, "x", 0, 1);
    fig_map.add_trajectory(&x_truth, "x*", 0, 1);
    fig_map.set_trajectory_color(&x_truth, colormap, &traj_colormap);
    fig_map.show(1.);

    vector<ConnectedSubset> v_loops;

    v_loops = tplane.get_detected_loops();
    for(size_t i = 0 ; i < v_loops.size() ; i++)
      fig_tplane.draw_box(v_loops[i].box(), "red");

    v_loops = tplane.get_proven_loops();
    for(size_t i = 0 ; i < v_loops.size() ; i++)
      fig_tplane.draw_box(v_loops[i].box(), "green");

    vibes::endDrawing();

  /* =========== ENDING =========== */

    delete x;
    delete v;

  // Checking if this example still works:
  return (tplane.nb_loops_detections() == 5 && tplane.nb_loops_proofs() == 4) ? EXIT_SUCCESS : EXIT_FAILURE;
}
