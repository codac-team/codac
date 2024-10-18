/** 
 *  Codac - Examples
 *  Robot evolution equations: causal chain
 * ----------------------------------------------------------------------------
 *
 *  \brief      Example from the paper "Guaranteed Computation of Robot Trajectories"
 *              Simon Rohou, Luc Jaulin, Lyudmila Mihaylova, Fabrice Le Bars, Sandor M. Veres
 *
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <codac.h>
#include <codac-rob.h>

using namespace std;
using namespace codac;

#define FINAL_CONDITION 0

int main()
{
  /* =========== INITIALIZATION =========== */

    clock_t t_start = clock();
    float dt = 0.01;
    Interval tdomain(0., 14.);

    // Tube state vector
      TubeVector x(tdomain, dt, 3);

    // Initial state x0
      Vector x0(3, 0.);
      x0[2] = (-6./5.)*M_PI;

    // Bounded initial state x0 (with uncertainties)
      IntervalVector ix0(x0);
      ix0[0].inflate(1.);
      ix0[1].inflate(1.);
      ix0[2].inflate(0.02);
      x.set(ix0, 0.); // tube state vector with initial conditions

    // Computing an approximation of the actual state trajectory
      Trajectory traj_phidot(tdomain, TFunction("-cos((t+33)/5)"), dt);

      TrajectoryVector traj_state(3); // state equations
      traj_state[2] = traj_phidot.primitive(x0[2]);
      traj_state[0] = (10.*cos(traj_state[2])).primitive();
      traj_state[1] = (10.*sin(traj_state[2])).primitive();


    if(FINAL_CONDITION)
    {
      // Final condition:
      IntervalVector ixf(x.size());
      ixf[0] = Interval(53.9,55.9);
      ixf[1] = Interval(6.9,8.9);
      ixf[2] = Interval(-2.36,-2.32);
      x.set(ixf, 14.);
    }

  /* =========== PROPAGATION (CONTRACTORS) =========== */

    TFunction f("x", "y", "theta",
                      "(10.*cos(theta) ;\
                        10.*sin(theta) ;\
                        -cos((t+33)/5)+[-0.02,0.02])");

    CtcDeriv ctc_deriv;

    ctc_deriv.contract(x, f.eval_vector(x));
    ctc_deriv.contract(x, f.eval_vector(x));

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();

    VIBesFigMap fig_map("Map");
    fig_map.set_properties(50, 50, 550, 350);
    fig_map.add_tube(&x, "x", 0, 1);
    fig_map.add_trajectory(&traj_state, "x*", 0, 1, 2, "white");
    fig_map.show(2.);

    if(!FINAL_CONDITION)
    {
      // Codac result
      fig_map.draw_box(x(x.tdomain().ub()).subvector(0,1), "#008000");

      // CAPD result
      IntervalVector capd_box(2);
      capd_box[0] = Interval(38.1545777835, 71.7221807908);
      capd_box[1] = Interval(-11.4056594783, 27.2737690279);
      fig_map.draw_box(capd_box, "#DC4F2C");
    }

    vibes::endDrawing();


  printf("Time taken: %.2fs\n", (double)(clock() - t_start)/CLOCKS_PER_SEC);
  // Checking if this example still works:
  return ((FINAL_CONDITION && fabs(x.volume() - 143.027) < 1e-2)
       || (!FINAL_CONDITION && fabs(x.volume() - 353.406)))
        && x.contains(traj_state) == BoolInterval::YES ? EXIT_SUCCESS : EXIT_FAILURE;
}