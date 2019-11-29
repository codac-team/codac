/** 
 *  tubex-lib - Examples
 *  Arithmetic on tubes: simple example
 * ----------------------------------------------------------------------------
 *
 *  \brief      Example from the paper "Guaranteed Computation of Robot Trajectories"
 *              Simon Rohou, Luc Jaulin, Lyudmila Mihaylova, Fabrice Le Bars, Sandor M. Veres
 *
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

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
    VIBesFigTubeVector fig_x("x", 0, 4);
    fig_x.set_properties(100, 100, 600, 300);
    fig_x.add_tubevector(&x, "x");
    fig_x.show();
    vibes::endDrawing();
    

  // Checking if this example still works:
  return (fabs(x.volume() - 233.765) < 1e-2) ? EXIT_SUCCESS : EXIT_FAILURE;
}