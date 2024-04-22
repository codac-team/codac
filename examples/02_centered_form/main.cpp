// Example from the publication:
// https://www.ensta-bretagne.fr/jaulin/paper_centeredActa.pdf

#include <codac-core.h>
#include <codac-graphics.h>

using namespace std;
using namespace codac2;

#include "../sivia.cpp"

int main()
{
  VectorVar x(3);
  AnalyticFunction f({x}, vec(
    -sqr(x[2])+2*x[2]*sin(x[2]*x[0])+cos(x[2]*x[1]),
    2*x[2]*cos(x[2]*x[0])-sin(x[2]*x[1])
  ));

  AnalyticFunction J({x}, mat(
    vec(
      2*sqr(x[2])*cos(x[2]*x[0]), // A11
      -2*sqr(x[2])*sin(x[2]*x[0])), // A21
    vec(
      -x[2]*sin(x[2]*x[1]), // A12
      -x[2]*cos(x[2]*x[1])), // A22
    vec(
      x[2]*(-2+2*x[0]*cos(x[2]*x[0]))+2*sin(x[2]*x[0])-x[1]*sin(x[2]*x[1]), // A13
      (-2*x[0]*x[2]*sin(x[2]*x[0])+2*cos(x[2]*x[0])-x[1]*cos(x[2]*x[1]))) // A23
  ));

  cout << f.eval(IntervalVector({1,2,3})) << endl;
  cout << J.eval(IntervalVector({1,2,3})) << endl;

  vibes::beginDrawing();

  CtcInverse ctc_inverse(f, IntervalVector({0.,0.}));
  SIVIA(ctc_inverse, IntervalVector({{0,2},{2,4},{0,10}}), 0.01, "Without centered form");

  CtcCenteredForm ctc_centered(f, J, IntervalVector({0.,0.}));
  SIVIA(ctc_centered, IntervalVector({{0,2},{2,4},{0,10}}), 0.01, "With centered form");

  vibes::endDrawing();

  /*graphics::Figure g("Sep paver", VIBES | IPE);
  g.axis_limits({{-10,10},{-10,10}});
  g.window_properties({50,50},{600,600});*/
  /*Paver p(c, {{-10,10},{-10,10}});
  p.set_figure(g);*/
}