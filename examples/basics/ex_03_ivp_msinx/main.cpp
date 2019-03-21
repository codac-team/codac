/** 
 *  tubex-lib - Examples
 *  Guaranteed integration: xdot = -sin(x)
 * ----------------------------------------------------------------------------
 *
 *  \brief      Here we consider the following initial value problem:
 *                xdot = -sin(x)
 *                x0   = 1
 *            
 *              This code will show successive contractions of tube [x](·): first five
 *              computations and final result when reaching a fixed point. Light gray
 *              areas represent the tube part that has been contracted during
 *              the contraction step.
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
    VIBesFigTube fig_x("x", &x, &truth);
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