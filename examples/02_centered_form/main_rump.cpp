// Example from 
// https://www.tuhh.de/ti3/rump/intlab/demos/html/dglobal.html#1

#include <codac>

using namespace std;
using namespace codac2;

int main()
{
  VectorVar y(2);
  AnalyticFunction f({y}, 
    - pow(5*y[1]-20*pow(y[1],3.)+16*pow(y[1],5.),6.)
    + pow(-pow(5*y[0]-20*pow(y[0],3.)+16*pow(y[0],5.),3.)+5*y[1]-20*pow(y[1],3.)+16*pow(y[1],5.),2.)
  );

  CtcInverse ctc(f, 0);
  DefaultFigure::pave({{-1,1},{-1,1}}, ctc, 1e-2);
}