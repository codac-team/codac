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

void displayTube(Tube* tube, const string& name, int x, int y);

int main(int argc, char *argv[])
{
  /* =========== PARAMETERS =========== */

    Interval domain(0,10);
    double timestep = 0.01;

  /* =========== INITIALIZATION =========== */

    vibes::beginDrawing();
    vibes::axisAuto();

    // Creating tubes over the [0,10] domain with some timestep:
    Tube tube_x(domain, timestep,
                Function("t", "(t-5)^2 + [-0.5,0.5]"));
    Tube tube_y(domain, timestep,
                Function("t", "-4*cos(t-5) + [-0.5,0.5] + 0.1*(t-3.3)^2*[-2,2]"));

    Tube tube_a = tube_x + tube_y;
    Tube tube_b = sin(tube_x);
    Tube tube_c = tube_x.primitive();

  /* =========== GRAPHICS =========== */

    displayTube(&tube_x, "Example tube [x](·)", 200, 100);
    displayTube(&tube_y, "Example tube [y](·)", 300, 200);
    displayTube(&tube_a, "Example tube [a](·)", 400, 300);
    displayTube(&tube_b, "Example tube [b](·)", 500, 400);
    displayTube(&tube_c, "Example tube [c](·)", 600, 500);

  /* =========== END =========== */

    vibes::endDrawing();

  // Checking if this example is still working:
  return (fabs(tube_x.volume() - 10.512) < 1e-2
       && fabs(tube_y.volume() - 55.008) < 1e-2
       && fabs(tube_a.volume() - 65.519) < 1e-2
       && fabs(tube_b.volume() - 6.8414) < 1e-2
       && fabs(tube_c.volume() - 53.446) < 1e-2) ? EXIT_SUCCESS : EXIT_FAILURE;
}

void displayTube(Tube* tube, const string& name, int x, int y)
{
  VibesFigure_Tube figtube(name, tube);
  figtube.setProperties(x, y, 700, 350);
  figtube.setColors("#B9B9B9[#B9B9B9]");
  figtube.show();
}