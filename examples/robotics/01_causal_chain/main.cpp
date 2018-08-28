/* ============================================================================
 *  tubex-lib - Arithmetic on tubes: Causal chain example
 *
 *  Example from the paper "Guaranteed Computation of Robot Trajectories"
 *  Simon Rohou, Luc Jaulin, Lyudmila Mihaylova, Fabrice Le Bars, Sandor M. Veres
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2016
 * ---------------------------------------------------------------------------- */

#include "tubex.h"
#include "tubex-robotics.h"

using namespace std;
using namespace ibex;
using namespace tubex;

#define FINAL_CONDITION 0

int main()
{
  /* =========== INITIALIZATION =========== */

    Interval domain(0., 14.);
    float timestep = 0.001;

    // Creating a tube over the [0,14] domain with some timestep:
    TubeVector x(domain, timestep, 3);

    // Initial condition:
    IntervalVector x0(x.size());
    x0[0] = Interval(0.).inflate(1.);
    x0[1] = Interval(0.).inflate(1.);
    x0[2] = Interval((-6./5.)*M_PI).inflate(0.02);
    x.set(x0, 0.);

    if(FINAL_CONDITION)
    {
      // Final condition:
      IntervalVector xf(x.size());
      xf[0] = Interval(53.9,55.9);
      xf[1] = Interval(6.9,8.9);
      xf[2] = Interval(-2.36,-2.32);
      x.set(xf, 14.);
    }

  /* =========== PROPAGATION (CONTRACTORS) =========== */

    tubex::Function f("x", "y", "theta",
                      "(10.*cos(theta) ;\
                        10.*sin(theta) ;\
                        -cos((t+33)/5)+[-0.02,0.02])");

    CtcDeriv ctc_deriv;

    ctc_deriv.contract(x, f.eval_vector(x));
    ctc_deriv.contract(x, f.eval_vector(x));

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();

    VibesFigure_Map fig_map("Map");
    fig_map.set_properties(50, 50, 550, 350);
    fig_map.add_tubevector(&x, "x", 0, 1);
    fig_map.show();

    if(!FINAL_CONDITION)
    {
      // Tubex result
      fig_map.draw_box(x(x.domain().ub()).subvector(0,1), "#008000");

      // CAPD result
      IntervalVector capd_box(2);
      capd_box[0] = Interval(38.1545777835, 71.7221807908);
      capd_box[1] = Interval(-11.4056594783, 27.2737690279);
      fig_map.draw_box(capd_box, "#DC4F2C");
    }

    vibes::endDrawing();


  // Checking if this example is still working:
  return (FINAL_CONDITION && fabs(x.volume() - 143.027) < 1e-2)
      || (!FINAL_CONDITION && fabs(x.volume() - 353.406)) ? EXIT_SUCCESS : EXIT_FAILURE;
}