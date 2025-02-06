#include <codac>

using namespace codac2;

int main()
{
  VectorVar x(2);
  AnalyticFunction f { {x}, sqr(x[0])*sin(sqr(x[0])+sqr(x[1]))-sqr(x[1]) };
  auto p = sivia({{-5,5},{-4,4}}, f, {0,oo}, 1e-2, true);
  DefaultView::draw_paving(p);
}