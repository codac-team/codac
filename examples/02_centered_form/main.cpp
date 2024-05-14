// Example from the publication:
// https://www.ensta-bretagne.fr/jaulin/paper_centeredActa.pdf

#include <codac-core.h>
#include <codac-graphics.h>

using namespace std;
using namespace codac2;

int main()
{
  VectorVar x(3);
  AnalyticFunction f({x}, vec(
    -sqr(x[2])+2*x[2]*sin(x[2]*x[0])+cos(x[2]*x[1]),
    2*x[2]*cos(x[2]*x[0])-sin(x[2]*x[1])
  ));

  IntervalVector x0({{0,2},{2,4},{0,10}});

  {
    CtcInverse_<IntervalVector> ctc(f, IntervalVector({0.,0.}));

    Figure g("[Codac] CtcInverse", GraphicOutputMode::VIBES);
    g.set_axes(axis(0,x0[0],"u"), axis(1,x0[1],"v"));
    g.set_window_properties({50,50}, {800,800});
    //g.center_viewbox({1.6,3.18},{0.24,0.24});

    Paver p(x0);
    p.set_figure(&g);
    p.pave(CtcLazy(ctc), 0.004);
  }

  {
    CtcInverse_<IntervalVector> ctc(f, IntervalVector({0.,0.}), false);

    Figure g("[Codac] HC4Revise", GraphicOutputMode::VIBES);
    g.set_axes(axis(0,x0[0],"u"), axis(1,x0[1],"v"));
    g.set_window_properties({50,850}, {800,800});
    //g.center_viewbox({1.6,3.18},{0.24,0.24});

    Paver p(x0);
    p.set_figure(&g);
    p.pave(CtcLazy(ctc), 0.004);
  }
}