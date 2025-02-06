#include <codac>

using namespace std;
using namespace codac2;

int main()
{
  VectorVar x(4);
  AnalyticFunction f { {x},
    {
      sqr(x[0])-sqr(x[1])+2*exp(-x[0]*x[2])*(x[1]*sin(x[1]*x[2])
        +x[0]*cos(x[1]*x[2]))+exp(-x[0]*x[3])*cos(x[1]*x[3]),
      2*x[0]*x[1]+2*exp(-x[0]*x[2])*(x[1]*cos(x[1]*x[2])
        -x[0]*sin(x[1]*x[2]))-exp(-x[0]*x[3])*sin(x[1]*x[3])
    }
  };

  CtcInverse_ ctc(f, IntervalVector::zero(2));
  IntervalVector x0({{-10,10},{0,20},{1,1},{2,2}});

  shared_ptr<Figure2D> g = make_shared<Figure2D>("Evans", GraphicOutput::VIBES);
  g->set_axes(axis(0,x0[0]), axis(1,x0[1]));
  g->set_window_properties({50,50}, {800,800});

  draw_while_paving(x0, CtcLazy(ctc), 0.001, g);

  // Revealing the tiny solutions:
  for(const auto& ci : vector<Vector>({{-2.653,13.95},{-2.062,7.589},{-0.318,1.337}}))
    g->draw_circle({ci},0.2);
}