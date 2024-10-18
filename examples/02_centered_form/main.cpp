// Example from the publication:
// https://www.ensta-bretagne.fr/jaulin/paper_centeredActa.pdf

#include <codac>

using namespace std;
using namespace codac2;

int main()
{
  VectorVar x(3);
  AnalyticFunction f({x}, vec(
    -sqr(x[2])+2*x[2]*sin(x[2]*x[0])+cos(x[2]*x[1]),
    2*x[2]*cos(x[2]*x[0])-sin(x[2]*x[1])
  ));

  CtcInverse_<IntervalVector> ctc(f, {0.,0.});
  draw_while_paving({{0,2},{2,4},{0,10}}, ctc, 0.004);
}