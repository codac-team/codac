// Example from 
// https://www.tuhh.de/ti3/rump/intlab/demos/html/dglobal.html#1

#include <codac-core.h>
#include <codac-graphics.h>

using namespace std;
using namespace codac2;

#include "../sivia.cpp"

int main()
{
  VectorVar y(2);
  AnalyticFunction g({y}, 
    -pow(5*y[1]-20*pow(y[1],3.)+16*pow(y[1],5.),6.)
    +pow(-pow(5*y[0]-20*pow(y[0],3.)+16*pow(y[0],5.),3.)+5*y[1]-20*pow(y[1],3.)+16*pow(y[1],5.),2.)
  );

  vibes::beginDrawing();
  
  SIVIA(
    CtcInverse(g, Interval(0.)),
    IntervalVector({{-1,1},{-1,1}}),
    0.001, "Rump example Ctc");
  
  SIVIA_sep(
    SepInverse(g, Interval(-0.2,0.2)),
    IntervalVector({{-1,1},{-1,1}}),
    0.001, "Rump example Sep");

  vibes::endDrawing();
}