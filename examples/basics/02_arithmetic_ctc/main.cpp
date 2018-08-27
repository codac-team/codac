/* ============================================================================
 *  tubex-lib - Arithmetic on tubes involving local contractions: simple example
 *
 *  Here we consider the following tubes:
 *    [x](·)
 *    [y](·), [ydot](·)
 *    [a](·) = [x](·) + [y](·)
 *    [b](·) = sin([x](·))
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

using namespace std;
using namespace ibex;
using namespace tubex;

void contract(TubeVector& x)
{
  CtcDeriv ctc_deriv;
  ctc_deriv.contract(x[1], Tube(x[1], tubex::Function("4*sin(t-5) + (t-3.3)*[-0.1,0.1]")));
  x[2] &= x[0] + x[1];
  x[3] &= atan(x[1]);
  x[4] &= 2*sin(0.5*x[2]) + sqr(x[3]*2);
}

int main()
{
  /* =========== PARAMETERS =========== */

    Interval domain(0,10);
    double timestep = 0.01;
    TubeVector x(domain, timestep, 5);

    // An evaluation will be made at t=7., so we sample the vector beforehand,
    x.sample(7.); // so that all components share the same slicing

    x[0] = Tube(x[0], tubex::Function("(t-5)^2 + [-0.5,0.5]"));
    x[1] = Tube(x[1], tubex::Function("[-0.5,0.5] - 4*cos(t-5) + [-0.2,0.2]*(t-3.3)^2"));
    contract(x); // applying constraints with contractors on tubes, before the evaluation

    x[1].set(Interval(1.,1.8), 7.); // local evaluation on x[1]

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    VibesFigure_TubeVector fig_x("x", &x);
    fig_x.set_properties(100, 100, 600, 250);
    fig_x.show();

  /* =========== PROPAGATION (CONTRACTORS) =========== */

    contract(x); // applying constraints with contractors on tubes
    fig_x.show();
    vibes::endDrawing();


  // Checking if this example is still working:
  return (fabs(x.volume() - 10.5) < 1e-2) ? EXIT_SUCCESS : EXIT_FAILURE;
}