#include <codac>

using namespace std;
using namespace codac2;

int main()
{
  ScalarVar u, v;
  VectorVar a(4), a0(3), a1(3), a2(3), b0(3), b1(3), b2(3);

  AnalyticFunction b({u,a0,a1,a2}, sqr(1-u)*a0+2*u*(1-u)*a1+sqr(u)*a2);
  AnalyticFunction f({u,v,a0,a1,a2,b0,b1,b2}, (1-v)*b(u,a0,a1,a2)+v*b(u,b0,b1,b2));
  AnalyticFunction fa({u,v}, 
     f(u,v,
       Vector({0,0,0}),Vector({1,0,1}),Vector({2,0,0}),
       Vector({0,2,0}),Vector({1,2,1}),Vector({2,2,0}))
    );
  AnalyticFunction fb({u,v}, 
     f(u,v,
      Vector({0,0,0.55}),Vector({0,1,-0.45}),Vector({0,2,0.55}),
      Vector({2,0,0.55}),Vector({2,1,-0.45}),Vector({2,2,0.55}))
    );

  AnalyticFunction h({a}, fa(a[0],a[1])-fb(a[2],a[3]));
  CtcInverse ctc(h, IntervalVector::zero(3));

  IntervalVector x0({{0,1},{0,1},{0,1},{0,1}});
  DefaultFigure::pave(x0, ctc, 1e-3);

  /*
  // to be restored...
  Figure2D g("(u1,v1)", GraphicOutput::VIBES);
  g.set_axes(axis(0,x0[0]), axis(1,x0[1]));
  g.set_window_properties({100,100},{600,600});
  p.set_figure(&g);

  list<IntervalVector> l = p.pave(ctc, 0.01), l_12, l_34;

  for(const auto& li : l)
  {
    assert(li.size() == 4);
    l_12.push_back(fa.eval_centered(li[0],li[1]));
    l_34.push_back(fb.eval_centered(li[2],li[3]));
  }
  export_to_ObjectFileFormat(l_12, "output_u1v1.off");
  export_to_ObjectFileFormat(l_34, "output_u2v2.off");
  */
}