/* ============================================================================
 *  tubex-lib - Arithmetic on tubes: simple example
 *
 *  Example from the paper "Guaranteed Computation of Robot Trajectories"
 *  Simon Rohou, Luc Jaulin, Lyudmila Mihaylova, Fabrice Le Bars, Sandor M. Veres
 *
 *  Here we consider the following tubes:
 *    [x](·)
 *    [y](·)
 *    [a](·) = [x](·) + [y](·)
 *    [b](·) = sin([x](·))
 *    [c](·) = integ([x](tau)dtau)
 *    [d](·) = |[y](·)|
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

int main()
{
  /* =========== INITIALIZATION =========== */
  
    Interval domain(0,10);
    TubeVector x(domain, 6);

    double timestep = 0.01;
    x[0] = Tube(domain, timestep, // tube [x](·)
                tubex::Function("(t-5)^2 + [-0.5,0.5]"));
    x[1] = Tube(domain, timestep, // tube [y](·)
                tubex::Function("-4*cos(t-5) + [-0.5,0.5] + 0.1*(t-3.3)^2*[-2,2]"));

  /* =========== ARITHMETIC =========== */

    x[2] = x[0] + x[1];      // tube [a](·)
    x[3] = sin(x[0]);        // tube [b](·)
    x[4] = x[0].primitive(); // tube [c](·)
    x[5] = abs(x[1]);        // tube [d](·)

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    VIBesFigure_TubeVector fig_x("x", 4, 4);
    fig_x.set_properties(100, 100, 600, 300);
    fig_x.add_tubevector(&x, "x");
    fig_x.show();
    vibes::endDrawing();
    

  // Checking if this example still works:
  return (fabs(x.volume() - 233.765) < 1e-2) ? EXIT_SUCCESS : EXIT_FAILURE;
}