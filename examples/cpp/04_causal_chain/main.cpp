/* ============================================================================
 *  tubex-lib - Arithmetic on tubes: Causal chain example
 *
 *  Example from the paper "Guaranteed Computation of Robot Trajectories"
 *  Simon Rohou, Luc Jaulin, Lyudmila Mihaylova, Fabrice Le Bars, Sandor M. Veres
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

#define BACKWARD_EXAMPLE 1

void displayCausalMap(const Tube& x, const Tube& y, int fig_x, int fig_y);

int main(int argc, char *argv[])
{
  /* =========== PARAMETERS =========== */

    Interval domain(0,14);
    double timestep = 0.001;
    float speed = 10.;

  /* =========== INITIALIZATION =========== */

    // Creating a tube over the [0,14] domain with some timestep:
    Tube u(domain, timestep, Function("t", "-cos((t+33)/5)+[-0.02,0.02]"));
    // Initial conditions:
    Interval x0 = Interval(0.).inflate(1.), y0 = x0;
    Interval theta0 = Interval((-6./5.)*M_PI).inflate(0.02);

  /* =========== CONSTRAINT NETWORK =========== */

    Tube theta = u.primitive(theta0);
    Tube xdot = speed * cos(theta);
    Tube ydot = speed * sin(theta);
    Tube x = xdot.primitive(x0);
    Tube y = ydot.primitive(y0);

  #if BACKWARD_EXAMPLE

    /* =========== BACKWARD PROPAGATION =========== */

      theta.ctcEval(u, 14., Interval(-2.36,-2.32));
      x.ctcEval(xdot, 14., Interval(53.9,55.9));
      y.ctcEval(ydot, 14., Interval(6.9,8.9));

    /* =========== CONSTRAINT NETWORK =========== */

      xdot &= speed * cos(theta);
      ydot &= speed * sin(theta);
      x.ctcFwdBwd(xdot);
      y.ctcFwdBwd(ydot);

  #endif

  /* =========== GRAPHICS =========== */

    VibesFigure_Tube::draw("Tube [x](·)", &x, 100, 100);
    VibesFigure_Tube::draw("Tube [y](·)", &y, 150, 150);
    displayCausalMap(x, y, 200, 200);

    #if !BACKWARD_EXAMPLE

      // Tubes result
      IntervalVector last_slice(2);
      last_slice[0] = x[x.size() - 1];
      last_slice[1] = y[y.size() - 1];
      vibes::drawBox(last_slice, "#008000", vibesParams("figure", "Map (top view): [x](·)x[y](·)"));

      // CAPD result
      IntervalVector capd_box(2);
      capd_box[0] = Interval(38.1545777835, 71.7221807908);
      capd_box[1] = Interval(-11.4056594783, 27.2737690279);
      vibes::drawBox(capd_box, "#DC4F2C", vibesParams("figure", "Map (top view): [x](·)x[y](·)"));

    #endif

  /* =========== END =========== */

    VibesFigure_Tube::endDrawing();

  // Checking if this example is still working:
  return (BACKWARD_EXAMPLE && fabs(x.volume() - 65.5594) < 1e-2 && fabs(y.volume() - 74.9425) < 1e-2)
      || (!BACKWARD_EXAMPLE && fabs(x.volume() - 158.839) < 1e-2 && fabs(y.volume() - 190.126) < 1e-2)
        ? EXIT_SUCCESS : EXIT_FAILURE;
}

void displayCausalMap(const Tube& x, const Tube& y, int fig_x, int fig_y)
{
  const string fig_name = "Map (top view): [x](·)x[y](·)";
  const int slices_number_to_display = 500;

  vibes::newFigure(fig_name);
  vibes::setFigureProperties(
            vibesParams("figure", fig_name, "x", fig_x, "y", fig_y, "width", 900, "height", 600));
  vibes::axisLimits(-20, 100, -40, 40);

  // Robot's tubes projection
  int startpoint;
  for(int i = 0 ; i < x.size() ; i += max((int)(x.size() / slices_number_to_display), 1))
    startpoint = i;

  for(int i = startpoint ; i >= 0; i -= max((int)(x.size() / slices_number_to_display), 1))
  {
    Interval intv_x = x[i];
    Interval intv_y = y[i];
    if(!intv_x.is_unbounded() && !intv_y.is_unbounded())
      vibes::drawBox(intv_x.lb(), intv_x.ub(), intv_y.lb(), intv_y.ub(),
                     "lightGray[white]", vibesParams("figure", fig_name));
  }
}