/** 
 *  tubex-lib - Examples
 *  Graphics features
 * ----------------------------------------------------------------------------
 *
 *  \date       2019
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <tubex.h>

using namespace std;
using namespace tubex;

int main()
{
  /* =========== INITIALIZATION =========== */

    Tube xdot(Interval(0.,5.), 0.05, TFunction("cos(t)+[-0.10,0.10]"));
    Tube x = xdot.primitive(0.);
    CtcEval ctc_eval;
    Interval t(3.125), y(sin(t));
    ctc_eval.contract(t, y, x, xdot);

    Trajectory thicknesses = x.diam(false);
    Trajectory thicknesses_dot = x.diam(xdot);

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    VIBesFigTube fig_x("x");
    fig_x.set_properties(100, 100, 600, 300);
    fig_x.add_tube(&x, "x");
    fig_x.add_trajectory(&thicknesses, "thicknesses", "red");
    fig_x.add_trajectory(&thicknesses_dot, "thicknessesdot", "green");
    fig_x.set_tube_derivative(&x, &xdot);
    fig_x.show(true);

    vibes::endDrawing();

  return EXIT_SUCCESS; // todo: check the final output
}