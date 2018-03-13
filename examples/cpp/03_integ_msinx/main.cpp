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

    // Creating tubes over the [0,10] domain with some timestep:
    Tube a(domain, timestep);
    Tube b(domain, timestep);
     // Initialization with [-1,1] values forall t:
    Tube x(domain, timestep, Interval(-1,1));

  /* =========== GRAPHICS =========== */

    VibesFigure_Tube::draw("Tube [x](·)", &x, 200, 50);

  /* =========== INTERVAL INTEGRATION =========== */

    int i = 0;
    double volume_x;
    do
    {
      cout << "Step " << i << "... \tpress ENTER to continue" << flush;
      volume_x = x.volume(); // check tube's volume to detect a fixpoint

      // Contractors based on elementary constraints:
      a &= sin(x);
      b &= a.primitive();
      x &= x0 - b;

      VibesFigure_Tube::draw("Tube [x](·)");
      i++;
      
      if(argc == 1) cin.ignore(); // press ENTER to continue
    } while(volume_x != x.volume()); // up to the fixpoint

  /* =========== END =========== */

    VibesFigure_Tube::endDrawing();

  // Checking if this example is still working:
  return (volume_x <= 9.15) ? EXIT_SUCCESS : EXIT_FAILURE;
}