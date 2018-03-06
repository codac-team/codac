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

#include "tubex_Tube.h"
#include "tubex_VibesFigure_Tube.h"

using namespace std;
using namespace ibex;
using namespace tubex;

void constraintNetwork(Tube& x, Tube& y, Tube& ydot, Tube& a, Tube& p, Tube& q)
{
  y.ctcFwdBwd(ydot);
  a &= x + y;
  p &= atan(y);
  q &= 2*sin(0.5*a) + sqr(p*2);
}

int main(int argc, char *argv[])
{
  /* =========== PARAMETERS =========== */

    Interval domain(0,10);
    double timestep = 0.01;

  /* =========== INITIALIZATION =========== */

    // Creating tubes over the [0,10] domain with some timestep:
    Tube ydot(domain, timestep), a(domain, timestep), p(domain, timestep), q(domain, timestep);
    Tube x(domain, timestep, Function("t", "(t-5)^2 + [-0.5,0.5]"));
    Tube y(domain, timestep, Function("t", "[-0.5,0.5] - 4*cos(t-5) + [-0.2,0.2]*(t-3.3)^2"));

    // Applying constraints with contractors on tubes
    constraintNetwork(x, y, ydot, a, p, q);

    // Specifying ydot
    ydot &= Tube(domain, timestep, Function("t", "4*sin(t-5) + (t-3.3)*[-0.1,0.1]"));

  /* =========== GRAPHICS =========== */

    VibesFigure_Tube::show(&x, "Tube [x](·)", 300, 200);
    VibesFigure_Tube::show(&y, "Tube [y](·)", 400, 300);
    VibesFigure_Tube::show(&a, "Tube [a](·)", 500, 400);
    VibesFigure_Tube::show(&p, "Tube [p](·)", 600, 500);
    VibesFigure_Tube::show(&q, "Tube [q](·)", 700, 600);

  /* =========== MEASUREMENT =========== */

    // Applying a local contraction on y tube
    IntervalVector measurement(2);
    measurement[0] = Interval(7.,7.2); // [t]
    measurement[1] = Interval(1.,1.8); // [y]
    y.ctcEval(ydot, measurement[0], measurement[1]);

  /* =========== PROPAGATION (CSP) =========== */

    // Applying constraints with contractors on tubes
    constraintNetwork(x, y, ydot, a, p, q);

  /* =========== GRAPHICS =========== */

    VibesFigure_Tube::show(&x);
    VibesFigure_Tube::show(&y);
    vibes::drawBox(measurement, "blue", vibesParams("figure", "Tube [y](·)"));
    VibesFigure_Tube::show(&a);
    VibesFigure_Tube::show(&p);
    VibesFigure_Tube::show(&q);
    VibesFigure_Tube::endDrawing();

  // Checking if this example is still working:
  return (fabs(x.volume() - 10.511) < 1e-2
       && fabs(y.volume() - 18.033) < 1e-2
       && fabs(a.volume() - 28.544) < 1e-2
       && fabs(p.volume() - 5.4136) < 1e-2
       && fabs(q.volume() - 41.736) < 1e-2) ? EXIT_SUCCESS : EXIT_FAILURE;
}