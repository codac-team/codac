// Example from 
// https://www.tuhh.de/ti3/rump/intlab/demos/html/dglobal.html#1

#include <codac-core.h>

using namespace std;
using namespace codac2;

int main()
{
  VectorVar y(2);
  AnalyticFunction f({y}, 
    -pow(5*y[1]-20*pow(y[1],3.)+16*pow(y[1],5.),6.)
    +pow(-pow(5*y[0]-20*pow(y[0],3.)+16*pow(y[0],5.),3.)+5*y[1]-20*pow(y[1],3.)+16*pow(y[1],5.),2.)
  );

  CtcInverse_<Interval> ctc(f, Interval(0.));
  IntervalVector x0({{-1,1},{-1,1}});

  Figure g("Rump example", GraphicOutputMode::VIBES);
  g.set_axes(axis(0,x0[0]), axis(1,x0[1]));
  g.set_window_properties({50,50}, {800,800});

  Paver p(x0);
  p.set_figure(&g);
  p.pave(CtcLazy(ctc), 0.001);
}