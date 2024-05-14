#include <codac-core.h>

using namespace std;
using namespace codac2;

int main()
{
  VectorVar x(4);
  AnalyticFunction f({x}, vec(
    sqr(x[0])-sqr(x[1])+2*exp(-x[0]*x[2])*(x[1]*sin(x[1]*x[2])+x[0]*cos(x[1]*x[2]))+exp(-x[0]*x[3])*cos(x[1]*x[3]),
    2*x[0]*x[1]+2*exp(-x[0]*x[2])*(x[1]*cos(x[1]*x[2])-x[0]*sin(x[1]*x[2]))-exp(-x[0]*x[3])*sin(x[1]*x[3])
  ));

  CtcInverse_<IntervalVector> ctc(f, IntervalVector({{0.,0.}}));
  IntervalVector x0({{-10,10},{0,20},{1,1},{2,2}});

  Figure g("Evans", GraphicOutputMode::VIBES);
  g.set_axes(axis(0,x0[0]), axis(1,x0[1]));
  g.set_window_properties({50,50}, {800,800});

  Paver p(x0);
  p.set_figure(&g);
  p.pave(CtcLazy(ctc), 0.001);
}