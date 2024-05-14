#include <codac-core.h>

using namespace std;
using namespace codac2;

int main()
{
  ScalarVar u, v;
  VectorVar a(4), a0(3), a1(3), a2(3), b0(3), b1(3), b2(3);

  AnalyticFunction b({u,a0,a1,a2}, sqr(1-u)*a0+2*u*(1-u)*a1+sqr(u)*a2);
  AnalyticFunction f({u,v,a0,a1,a2,b0,b1,b2}, (1-v)*b(u,a0,a1,a2)+v*b(u,b0,b1,b2));

  AnalyticFunction h({a},
     f(a[0],a[1],
      Vector({0,0,0}),Vector({1,0,1}),Vector({2,0,0}),
      Vector({0,2,0}),Vector({1,2,1}),Vector({2,2,0}))
    -f(a[2],a[3],
      Vector({0,0,0.55}),Vector({0,1,-0.45}),Vector({0,2,0.55}),
      Vector({2,0,0.55}),Vector({2,1,-0.45}),Vector({2,2,0.55}))
  );

  CtcInverse_<IntervalVector> ctc(h, IntervalVector({{0.},{0.},{0.}}));
  IntervalVector x0({{0,1},{0,1},{0,1},{0,1}});

  Figure g("Lofted parabolas", GraphicOutputMode::VIBES | GraphicOutputMode::IPE);
  g.set_axes(axis(0,x0[0],"u"), axis(1,x0[1],"v"));
  g.set_window_properties({50,50}, {800,800});

  Paver p(x0);
  p.set_figure(&g);
  p.pave(CtcLazy(ctc), 0.001);
}