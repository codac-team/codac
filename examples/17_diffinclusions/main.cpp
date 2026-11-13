#include <codac>

using namespace std;
using namespace codac2;

int main()
{
    {
      VectorVar x(2);
      ScalarVar u;
      AnalyticFunction f({x,u}, {-x[1]+u, x[0]});
      DiffInclusion<IntervalVector,Interval> di(f);

      IntervalVector state {{1.0,1.0},{0.0,0.0}};
      for (double t=0;t<2.0;t+=0.01) {
         DefaultFigure::draw_box(state);
         IntervalVector s1 = IntervalVector::constant(2,Interval());
         IntervalVector s2 = IntervalVector::constant(2,Interval());
         di.basic_contract<true>(Interval(t,t+0.01),
                        state,
                        s1,s2, Interval(-0.01,0.01));
         state=s2;
      }
    }
}
