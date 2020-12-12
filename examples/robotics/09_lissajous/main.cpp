/** 
 *  tubex-lib - Examples
 *  A robot following a Lissajous trajectory
 * ----------------------------------------------------------------------------
 *
 *  \brief      Example from the paper "Guaranteed Computation of Robot Trajectories"
 *              Simon Rohou, Luc Jaulin, Lyudmila Mihaylova, Fabrice Le Bars, Sandor M. Veres
 *
 *  \date       2016
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
  /* =========== PARAMETERS =========== */

    Interval tdomain(0., 6.);
    float timestep = 0.001;

  /* =========== INITIALIZATION =========== */

    // Creating tubes over the [0,6] tdomain with some timestep:
    TubeVector x(tdomain, timestep, 6);
    x &= IntervalVector(6, Interval(-999.,999.)); // todo: remove this
    x[4] = Tube(x[5], TFunction("-10*cos(t)+[-0.001,0.001]"));

    // Initial conditions:
    IntervalVector x0(x.size());
    x0[0] = Interval(10.).inflate(0.2);
    x0[1] = Interval(0.).inflate(0.2);
    x0[2] = 0.;
    x0[3] = 10.;
    x.set(x0, 0.);

  /* =========== CREATING CONTRACTORS =========== */

    CtcDeriv ctc_deriv;
    CtcEval ctc_eval;
    ctc_eval.enable_time_propag(false); // faster use
    CtcFunction ctc_f(
      Function("x", "y", "xdot", "ydot", "xddot", "yddot",
               "-0.4*xddot*xdot - yddot"));

  /* =========== PROPAGATION =========== */

    int i = 0;
    double volume_x;
    do
    {
      volume_x = x.volume();

      // Contractors
      ctc_deriv.contract(x[3], x[5]);
      ctc_deriv.contract(x[2], x[4]);
      ctc_deriv.contract(x[1], x[3]);
      ctc_deriv.contract(x[0], x[2]);

      // Further constraints that could be considered
      if(false)
      {
        Interval t, y;

        t = x[1](M_PI/2.); y = 3.*M_PI/2.;
        ctc_eval.contract(t, y, x[1], x[3]);

        t = x[1](3.*M_PI/2.); y = M_PI/2.;
        ctc_eval.contract(t, y, x[1], x[3]);

        t = x[0](M_PI/2.); y = 3.*M_PI/2.;
        ctc_eval.contract(t, y, x[0], x[2]);

        t = x[0](3.*M_PI/2.); y = M_PI/2.;
        ctc_eval.contract(t, y, x[0], x[2]);
        // todo: ctc_periodic.contract(x[1], M_PI);
      }

      ctc_f.contract(x);

      i++;
    } while(volume_x != x.volume());

  /* =========== GRAPHICS =========== */

    TrajectoryVector x_truth(tdomain, TFunction("(10*cos(t);5*sin(2*t))"));
    x_truth.resize(x.size());

    vibes::beginDrawing();

    VIBesFigTubeVector fig_x("x", 0, 1); // first two components
    fig_x.add_tube(&x, "x");
    fig_x.add_trajectory(&x_truth, "x*");
    fig_x.set_properties(100, 100, 600, 300);
    fig_x.show();

    VIBesFigMap fig_map("Map");
    fig_map.set_properties(50, 50, 800, 500);
    fig_map.add_tube(&x, "x", 0, 1);
    fig_map.add_trajectory(&x_truth, "x*", 0, 1, "white");
    fig_map.show(1.2);

    vibes::endDrawing();

    
  // Checking if this example still works:
  return fabs(x.volume() - 10.6313) < 1e-2 ? EXIT_SUCCESS : EXIT_FAILURE;
}