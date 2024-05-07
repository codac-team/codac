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

  vibes::beginDrawing();

  CtcInverse ctc_hc4revise(f, IntervalVector({0.,0.}), false);
  SIVIA(ctc_hc4revise, IntervalVector({{0,2},{2,4},{0,10}}), 0.004, "[Codac] HC4Revise");

  CtcInverse ctc_inverse(f, IntervalVector({{0.},{0.}}));
  SIVIA(ctc_inverse, IntervalVector({{0,2},{2,4},{0,10}}), 0.01, "[Codac] CtcInverse");

  vibes::endDrawing();
  return EXIT_SUCCESS;
}



  //graphics::Figure g("Sep paver", VIBES | IPE);
  //g.axis_limits({{-10,10},{-10,10}});
  //g.window_properties({50,50},{600,600});
  //Paver p(c, {{-10,10},{-10,10}});
  //p.set_figure(g);