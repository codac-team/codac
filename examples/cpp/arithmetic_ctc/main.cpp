/* ============================================================================
 *  tube-lib - Arithmetic on tubes involving local contractions: simple example
 *
 *  Example from the paper "Using Tubes to Solve Dynamical Systems"
 *  Simon Rohou, Luc Jaulin, Lyudmila Mihaylova, Fabrice Le Bars, Sandor M. Veres
 *
 *  Here we consider the following tubes:
 *    [x](·)
 *    [y](·), [ydot](·)
 *    [a](·) = [x](·) + [y](·)
 *    [b](·) = sin([x](·))
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

void displayTube(map<Tube*,VibesFigure_Tube*> &map_graphics, Tube *tube, const string& name = "", int x = 0, int y = 0);

void constraintNetwork(Tube& x, Tube& y, Tube& ydot, Tube& a, Tube& b, Tube& c)
{
  y.ctcFwdBwd(ydot);
  a &= x + y;
  b &= atan(y);
  c &= 2*sin(0.5*a) + sqr(b*2);
}

int main(int argc, char *argv[])
{
  /* =========== PARAMETERS =========== */

    Interval domain(0,10);
    double timestep = 0.01;

  /* =========== INITIALIZATION =========== */

    // Creating tubes over the [0,10] domain with some timestep:
    Tube ydot(domain, timestep), a(domain, timestep), b(domain, timestep), c(domain, timestep);
    Tube x(domain, timestep, Function("t", "(t-5)^2 + [-0.5,0.5]"));
    Tube y(domain, timestep, Function("t", "-4*cos(t-5) + [-0.5,0.5] + 0.1*(t-3.3)^2*[-2,2]"));

    // Applying constraints with contractors on tubes
    constraintNetwork(x, y, ydot, a, b, c);

    // Specifying ydot
    ydot &= Tube(domain, timestep, Function("t", "4*sin(t-5) + 0.2*(t-3.3)*[-0.5,0.5]"));

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing(); vibes::axisAuto();
    map<Tube*,VibesFigure_Tube*> map_graphics;
    displayTube(map_graphics, &x, "Tube [x](·)", 300, 200);
    displayTube(map_graphics, &y, "Tube [y](·)", 400, 300);
    displayTube(map_graphics, &a, "Tube [a](·)", 500, 400);
    displayTube(map_graphics, &b, "Tube [b](·)", 600, 500);
    displayTube(map_graphics, &c, "Tube [c](·)", 700, 600);

  /* =========== MEASUREMENT =========== */

    // Applying a local contraction on y tube
    IntervalVector measurement(2);
    measurement[0] = Interval(7.,7.2); // [t]
    measurement[1] = Interval(1.,1.8); // [y]
    y.ctcIn(ydot, measurement[1], measurement[0]);

  /* =========== PROPAGATION (CSP) =========== */

    // Applying constraints with contractors on tubes
    constraintNetwork(x, y, ydot, a, b, c);

  /* =========== GRAPHICS =========== */

    displayTube(map_graphics, &x);
    displayTube(map_graphics, &y);
    vibes::drawBox(measurement, "blue", vibesParams("figure", "Tube [y](·)"));
    displayTube(map_graphics, &a);
    displayTube(map_graphics, &b);
    displayTube(map_graphics, &c);
    vibes::endDrawing();

  /* =========== END =========== */

    // Deleting pointers to graphical tools
    for(auto it = map_graphics.begin(); it != map_graphics.end(); ++it)
      delete it->second;

  return EXIT_SUCCESS;
}

void displayTube(map<Tube*,VibesFigure_Tube*> &map_graphics, Tube *tube, const string& name, int x, int y)
{
  if(map_graphics.find(tube) == map_graphics.end())
  {
    VibesFigure_Tube *figtube = new VibesFigure_Tube(name, tube);
    figtube->setProperties(x, y, 700, 350);
    string fg = "#A2A2A2", bg = "#D2D2D2";
    figtube->setColors(fg + "[" + fg + "]", fg + "[" + fg + "]", bg + "[" + bg + "]");
    map_graphics[tube] = figtube;
  }

  map_graphics[tube]->show();
}