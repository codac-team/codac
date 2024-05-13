#include <codac-core.h>
#include <codac-graphics.h>

using namespace std;
using namespace codac2;

#include "../sivia.cpp"

int main()
{
  VectorVar x(4);
  AnalyticFunction f({x}, vec(
    sqr(x[0])-sqr(x[1])+2*exp(-x[0]*x[2])*(x[1]*sin(x[1]*x[2])+x[0]*cos(x[1]*x[2]))+exp(-x[0]*x[3])*cos(x[1]*x[3]),
    2*x[0]*x[1]+2*exp(-x[0]*x[2])*(x[1]*cos(x[1]*x[2])-x[0]*sin(x[1]*x[2]))-exp(-x[0]*x[3])*sin(x[1]*x[3])
  ));

  vibes::beginDrawing();
  CtcInverse ctc_inverse(f, IntervalVector({0.,0.}), false);
  CtcInverse ctc_inverse_centered(f, IntervalVector({0.,0.}));
  SIVIA(ctc_inverse, IntervalVector({{-10,10},{0,20},{1,1},{2,2}}), 0.001, "Evans", "yellow");
  SIVIA(ctc_inverse_centered, IntervalVector({{-10,10},{0,20},{1,1},{2,2}}), 0.001, "Evans", "purple", false);
  vibes::endDrawing();  
}