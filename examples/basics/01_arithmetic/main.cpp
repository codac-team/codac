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
    double timestep = 0.01;

    // Creating tubes over the [0,10] temporal domain with some timestep:
    Tube tube_x(domain, timestep,
                tubex::Function("(t-5)^2 + [-0.5,0.5]"));
    Tube tube_y(domain, timestep,
                tubex::Function("-4*cos(t-5) + [-0.5,0.5] + 0.1*(t-3.3)^2*[-2,2]"));

  /* =========== ARITHMETIC =========== */

    Tube tube_a = tube_x + tube_y;
    Tube tube_b = sin(tube_x);
    Tube tube_c = tube_x.primitive();
    Tube tube_d = abs(tube_y);

  /* =========== GRAPHICS =========== */
    
    VibesFigure_Tube::draw("Tube [x](·)", &tube_x, 100, 100);
    VibesFigure_Tube::draw("Tube [y](·)", &tube_y, 200, 200);
    VibesFigure_Tube::draw("Tube [a](·)", &tube_a, 300, 300);
    VibesFigure_Tube::draw("Tube [b](·)", &tube_b, 400, 400);
    VibesFigure_Tube::draw("Tube [c](·)", &tube_c, 500, 500);
    VibesFigure_Tube::draw("Tube [d](·)", &tube_d, 600, 600);
    VibesFigure_Tube::end_drawing();


  // Checking if this example is still working:
  return (fabs(tube_x.volume() - 10.5) < 1e-2
       && fabs(tube_y.volume() - 55.267) < 1e-2
       && fabs(tube_a.volume() - 65.767) < 1e-2
       && fabs(tube_b.volume() - 6.8308) < 1e-2
       && fabs(tube_c.volume() - 53.338) < 1e-2) ? EXIT_SUCCESS : EXIT_FAILURE;
}