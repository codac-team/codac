/** 
 *  Codac - Examples
 *  Contractors on tubes: simple example
 * ----------------------------------------------------------------------------
 *
 *  \brief      Example from the paper "Reliable non-linear state estimation involving time uncertainties"
 *              Simon Rohou, Luc Jaulin, Lyudmila Mihaylova, Fabrice Le Bars, Sandor M. Veres
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

void contract(TubeVector& x)
{
  // Creating contractors
  CtcDeriv ctc_deriv;
  CtcFunction ctc_f(Function("x", "y", "a", "p", "q",
                            "(x + y - a ; \
                              atan(y) - p ; \
                              2*sin(0.5*a) + sqrt(exp(p^2)) - q)"),
                    Vector(3,0.));

  // Contractions up to the fixed point
  TubeVector old_x(x);
  do
  {
    old_x = x;
    ctc_deriv.contract(x[1], Tube(x[1], TFunction("4*sin(t-5) + (t-3.3)*[-0.1,0.1]")));
    ctc_f.contract(x);
  } while(old_x != x);
}

int main()
{
  /* =========== PARAMETERS =========== */

    Interval tdomain(0,10);
    double timestep = 0.01;
    TubeVector x(tdomain, timestep, 5);

    // An evaluation will be made at t=7., so we sample the vector beforehand,
    x.sample(7.); // so that all components share the same slicing

    x[0] = Tube(x[0], TFunction("(t-5)^2 + [-0.5,0.5]"));
    x[1] = Tube(x[1], TFunction("[-0.5,0.5] - 4*cos(t-5) + [-0.2,0.2]*(t-3.3)^2"));
    contract(x); // applying constraints with contractors on tubes, before the evaluation

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    VIBesFigTubeVector fig_x("x", &x);
    fig_x.set_properties(100, 100, 600, 250);
    fig_x.show();

  /* =========== PROPAGATION (CONTRACTORS) =========== */

    x[1].set(Interval(1.,1.2), 7.); // local evaluation on x[1] to be propagated
    contract(x); // applying constraints with contractors on tubes
    fig_x.show();
    vibes::endDrawing();


  // Checking if this example still works:
  return (fabs(x.volume() - 64.4152) < 1e-2) ? EXIT_SUCCESS : EXIT_FAILURE;
}