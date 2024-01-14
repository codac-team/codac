#include <codac.h>

using namespace std;
using namespace codac;

int main()
{
  {
    auto tdomain = codac2::create_tdomain(Interval(0,10), 1., false);
    codac2::Tube<IntervalVector> x(tdomain, IntervalVector(2));

    for(auto& sx : x)
    {
      if(sx.t0_tf().contains(5.2))
      {
        cout << "sample" << endl;
        tdomain->sample(5.2);
      }
      cout << sx << endl;
    }
  }

  {
    TFunction f("x1", "x2", "u1", "u2", "(t+u1+x1;t+u2+x2)");
    IntervalVector x({{2,3},{5,6}});
    IntervalVector u({{0,0.1},{0,0.1}});
    Interval t(5.);
    cout << f.eval_vector(t,x,u) << endl;
  }

  
  return EXIT_SUCCESS;
}