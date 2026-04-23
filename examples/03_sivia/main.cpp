#include <codac>

using namespace codac2;

int main()
{
  set_nb_threads(max_threads());

  VectorVar x(2);
  AnalyticFunction f { {x}, sqr(x[0])*sin(sqr(x[0])+sqr(x[1]))-sqr(x[1]) };
  auto p = pave_multithread({{-5,5},{-4,4}}, f, {0,oo}, 1e-1, true);
  DefaultFigure::draw_paving(p);
}