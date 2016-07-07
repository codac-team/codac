/* ============================================================================
 *  tube-lib - Guaranteed integration example: xdot = -sin(x)
 *
 *  Example from the paper "Guaranteed Computation of Robots Trajectories"
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
 * ============================================================================
 *  Copyright : Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the MIT license. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2016
 * ---------------------------------------------------------------------------- */

#include "Tube.h"
#include "VibesFigure_Tube.h"

using namespace std;
using namespace ibex;

int main(int argc, char *argv[])
{
  /* =========== PARAMETERS =========== */

    double x0 = 1.;
    Interval domain(0,10);
    double integration_timestep = 0.01;

  /* =========== INITIALIZATION =========== */

    // Creating tubes over the [0,10] domain with some timestep:
    Tube tube_a(domain, integration_timestep);
    Tube tube_b(domain, integration_timestep);
     // Initialization with [-1,1] values forall t:
    Tube tube_x(domain, integration_timestep, Interval(-1,1));

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    vibes::axisAuto();

    VibesFigure_Tube figtube_x("Tube x", &tube_x);
    figtube_x.setProperties(200, 050, 700, 700);
    figtube_x.setColors("#B2B2B2[#B2B2B2]");

  /* =========== INTEGRATION (intervals) =========== */

    int i = 0;
    double volume_x;
    do
    {
      cout << "Step " << i << "... \tpress ENTER to continue" << flush;
      volume_x = tube_x.volume(); // check tube's volume to detect a fixpoint

      // Contractors based on elementary constraints:
      tube_a &= sin(tube_x);
      tube_b &= tube_a.primitive();
      tube_x &= x0 - tube_b;

      figtube_x.show(); // displaying the tube
      i++;
      
      if(argc == 1) cin.ignore(); // press ENTER to continue
    } while(volume_x != tube_x.volume()); // up to the fixpoint

  /* =========== END =========== */

    vibes::endDrawing();

  // Checking if this example is still working:
  return (volume_x <= 9.15) ? EXIT_SUCCESS : EXIT_FAILURE;
}