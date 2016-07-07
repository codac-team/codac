/* ============================================================================
 *  tube-lib - Arithmetics on tubes: simple example
 *
 *  Example from the paper "Guaranteed Computation of Robots Trajectories"
 *  Simon Rohou, Luc Jaulin, Lyudmila Mihaylova, Fabrice Le Bars, Sandor M. Veres
 *
 *  Here we consider the following tubes:
 *    [x](·)
 *    [y](·)
 *    [a](·) = [x](·) + [y](·)
 *    [b](·) = sin([x](·))
 *    [c](·) = integ([x](tau)dtau)
 * ============================================================================
 *  Copyright : Copyright 2016 Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the Apache License, Version 2.0. See the file LICENSE.
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

    Interval domain(0,10);
    double integration_timestep = 0.01;

  /* =========== INITIALIZATION =========== */

    vibes::beginDrawing();
    vibes::axisAuto();

    // Creating tubes over the [0,10] domain with some timestep:
    Tube tube_x(domain, integration_timestep);
    Tube tube_y(domain, integration_timestep);

    // Creating tubes [x](·) and [y](·) with some data
    for(int i = 0 ; i < tube_x.size() ; i++)
    {
      // Setting [value] for the ith slice:
      Interval value;

      value = pow(tube_x.getT(i) - domain.ub() / 2., 2) + Interval(-0.5,0.5);
      tube_x.setY(value, i);

      value = 4. * -cos(tube_y.getT(i) - domain.ub() / 2.)
              + Interval(-0.5,0.5)
              + 0.1 * pow(tube_x.getT(i) - domain.ub() / 3., 2) * Interval(-2,2);
      tube_y.setY(value, i);
    }

    Tube tube_a = tube_x + tube_y;
    Tube tube_b = sin(tube_x);
    Tube tube_c = tube_x.primitive();

  /* =========== GRAPHICS =========== */

    int w = 700, h = 350;

    string color = "#B9B9B9[#B9B9B9]";

    VibesFigure_Tube figtube_x("example_tube_x", &tube_x);
    figtube_x.setProperties(200, 100, w, h);
    figtube_x.setColors(color);
    figtube_x.show();

    VibesFigure_Tube figtube_y("example_tube_y", &tube_y);
    figtube_y.setProperties(300, 200, w, h);
    figtube_y.setColors(color);
    figtube_y.show();

    VibesFigure_Tube figtube_a("example_tube_a", &tube_a);
    figtube_a.setProperties(400, 300, w, h);
    figtube_a.setColors(color);
    figtube_a.show();

    VibesFigure_Tube figtube_b("example_tube_b", &tube_b);
    figtube_b.setProperties(500, 400, w, h);
    figtube_b.setColors(color);
    figtube_b.show();

    VibesFigure_Tube figtube_c("example_tube_c", &tube_c);
    figtube_c.setProperties(600, 500, w, h);
    figtube_c.setColors(color);
    figtube_c.show();

  /* =========== END =========== */

    vibes::endDrawing();

  // Checking if this example is still working:
  return (fabs(tube_x.volume() - 10.512) < 1e-2
       && fabs(tube_y.volume() - 55.008) < 1e-2
       && fabs(tube_a.volume() - 65.519) < 1e-2
       && fabs(tube_b.volume() - 6.8414) < 1e-2
       && fabs(tube_c.volume() - 53.446) < 1e-2) ? EXIT_SUCCESS : EXIT_FAILURE;
}