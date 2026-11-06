// Example from the publication:
// https://www.ensta-bretagne.fr/jaulin/paper_centeredActa.pdf

#include <codac>

using namespace std;
using namespace codac2;

int main()
{
  VectorVar x(3);
#if 1
  AnalyticFunction f { {x},
    {
      -sqr(x[2])+2*x[2]*sin(x[2]*x[0])+cos(x[2]*x[1]),
      2*x[2]*cos(x[2]*x[0])-sin(x[2]*x[1])
    }
  };
#else
  AnalyticFunction f { {x},
    {
      -sqr(x[1])+2*x[1]*sin(x[1]*x[2])+cos(x[1]*x[0]),
      2*x[1]*cos(x[1]*x[2])-sin(x[1]*x[0])
    }
  };
#endif


  CtcInverse ctc(f, {0,0},true,true,false);
  DefaultFigure::pave({{0,2},{2,4},{0,10}}, ctc, 0.00390625);
}
