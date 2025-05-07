#include <codac>

using namespace codac2;

// Download and run https://github.com/ENSTABretagneRobotics/VIBES/releases 
// before to be able to see the tube in VIBes window...

int main()
{
  VectorVar x(3);
  AnalyticFunction f({x}, vec(
    -sqr(x[2])+2*x[2]*sin(x[2]*x[0])+cos(x[2]*x[1]),
    2*x[2]*cos(x[2]*x[0])-sin(x[2]*x[1])
  ));

  CtcInverse ctc(f, {0.,0.});
  draw_while_paving(IntervalVector({{0,2},{2,4},{0,10}}), ctc, 0.01);
}