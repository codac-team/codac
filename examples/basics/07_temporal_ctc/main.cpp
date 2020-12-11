/** 
 *  tubex-lib - Examples
 *  Contractors on tubes: temporal contractors
 * ----------------------------------------------------------------------------
 *
 *  \brief      Comparison between the Lohner & Picard temporal contractors
 *
 *  \date       2020
 *  \author     Auguste Bourgois
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <tubex.h>

using namespace std;
using namespace tubex;

int main() // testcase
{
  double dt = 0.1; // time step of the tubes
  Interval tdomain(0, 5); // time domain of the tubes

  // Picard contractor
  TubeVector x(tdomain, dt, 1); // 1d tube vector
  x.set(IntervalVector(1, Interval(0.9, 1.1)), 0.); // initial condition

  vibes::beginDrawing();
  VIBesFigTube fig("picard");
  fig.set_properties(100, 100, 800, 400);

  std::vector<Trajectory> vt(11);

  for (int i = 0; i < 11; ++i) {
    double x0 = 0.9 + 0.2 * (i / 10.);
    vt.emplace_back(tdomain, TFunction((std::to_string(x0) + "*2.*atan(exp(-t)*tan(0.5))").c_str()));
    fig.add_trajectory(&vt.back(), "truth" + std::to_string(i), "#1f77b4[#1f77b4]");
  }

  CtcPicard ctc_picard;
  ctc_picard.contract(TFunction("x", "-sin(x)"), x); // xdot = -sin(x)
  fig.add_tube(&x[0], "x", "#d62728[#2ca02c]");
  fig.show(true);

  // Lohner contractor
  TubeVector y(tdomain, dt, 1); // 1d tube vector
  y.set(IntervalVector(1, Interval(0.9, 1.1)), 0.); // initial condition

  VIBesFigTube fig2("lohner");
  fig2.set_properties(100, 100, 800, 400);
  std::vector<Trajectory> vt2(11);

  for (int i = 0; i < 11; ++i) {
    double x0 = 0.9 + 0.2 * (i / 10.);
    vt2.emplace_back(tdomain, TFunction((std::to_string(x0) + "*2.*atan(exp(-t)*tan(0.5))").c_str()));
    fig2.add_trajectory(&vt2.back(), "truth" + std::to_string(i), "#1f77b4[#1f77b4]");
  }

  CtcLohner ctc_lohner(Function("x", "-sin(x)")); // xdot = -sin(x)
  ctc_lohner.contract(y);
  fig2.add_tube(&y[0], "y", "#d62728[#2ca02c]");
  fig2.show(true);

  vibes::endDrawing();

  return EXIT_SUCCESS;
}