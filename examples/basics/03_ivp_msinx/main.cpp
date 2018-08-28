/* ============================================================================
 *  tubex-lib - Guaranteed integration example: xdot = -sin(x)
 *
 *  Example from the paper "Guaranteed Computation of Robot Trajectories"
 *  Simon Rohou, Luc Jaulin, Lyudmila Mihaylova, Fabrice Le Bars, Sandor M. Veres
 *
 *  Here we consider the following initial value problem:
 *    xdot = -sin(x)
 *    x0   = 1
 *
 *  This code will show successive contractions of tube [x](·): first five
 *  computations and final result when reaching a fix point. Light gray
 *  areas represent the tube part that has been contracted during
 *  the contraction step.
 *
 *  Note: example replaced by the Lissajous example in the paper
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

int main(int argc, char *argv[])
{
  /* =========== PARAMETERS =========== */

    double x0 = 1.;
    Interval domain(0,10);
    double timestep = 0.01;

  /* =========== INITIALIZATION =========== */

    // Creating the tube x over the [0,10] domain with some timestep:
    Tube x(domain, timestep);
    // Creating intermediate tubes based on x (same slicing)
    Tube a(x), b(x);

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    Trajectory truth(domain, tubex::Function("2.*atan(exp(-t)*tan(0.5))"));
    VibesFigure_Tube fig_x("x", &x, &truth);
    fig_x.set_properties(100, 100, 600, 600);
    fig_x.show();

  /* =========== INTERVAL INTEGRATION =========== */

    int i = 0;
    double volume_x;
    do
    {
      volume_x = x.volume(); // check tube's volume to detect a fixpoint

      // Trivial contractors based on elementary constraints:
      a &= sin(x);
      b &= a.primitive();
      x &= x0 - b;

      i++;
      
    } while(volume_x != x.volume()); // up to the fixpoint
    cout << i << " iterations" << endl;

  /* =========== END =========== */

    fig_x.show();
    vibes::endDrawing();


  // Checking if this example still works:
  return (volume_x <= 21.176) ? EXIT_SUCCESS : EXIT_FAILURE;
}