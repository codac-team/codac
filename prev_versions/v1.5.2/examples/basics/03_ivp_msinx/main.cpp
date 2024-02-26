/** 
 *  Codac - Examples
 *  Guaranteed integration: xdot = -sin(x), x(0)=1
 * ----------------------------------------------------------------------------
 *
 *  \brief      Here we consider the following initial value problem:
 *                xdot = -sin(x)
 *                x0   = 1
 *            
 *              This IVP problem is solved using contractors and a
 *              fixed point resolution method.
 *
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <codac.h>

using namespace std;
using namespace codac;

int main(int argc, char *argv[])
{
  /* =========== INITIALIZATION =========== */

    // Tubes parameters
    Interval tdomain(0,10); // temporal definition domain
    double timestep = 0.01;

    // Creating the tubes (x, xdot) over the [0,10] tdomain with some timestep:
    TubeVector x(tdomain, timestep, 2);
    x[0].set(1., 0.); // setting initial condition x(0)=1

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    Trajectory truth(tdomain, TFunction("2.*atan(exp(-t)*tan(0.5))"));
    VIBesFigTube fig_x("x", &x[0], &truth);
    fig_x.set_properties(100, 100, 600, 600);

  /* =========== INTERVAL INTEGRATION =========== */

    // Defining contractors

    CtcFunction ctc_f(Function("x", "xdot", "xdot+sin(x)")); // algebraic contractor
    x[1] &= Interval(-1.,1.); // xdot=-sin(..)
    CtcDeriv ctc_deriv; // differential contractor

    // Calling contractors (iterative resolution)

    int i = 0;
    double volume_x;
    do
    {
      volume_x = x[0].volume(); // check tube's volume to detect a fixpoint
      ctc_f.contract(x[0], x[1]);
      ctc_deriv.contract(x[0], x[1]);
      i++;
    } while(volume_x != x[0].volume()); // up to the fixpoint
    cout << i << " iterations" << endl;

  /* =========== ENDING =========== */

    fig_x.show();
    vibes::endDrawing();


  // Checking if this example still works:
  return (volume_x <= 21.176) ? EXIT_SUCCESS : EXIT_FAILURE;
}