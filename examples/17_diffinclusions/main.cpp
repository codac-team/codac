#include <codac>

using namespace std;
using namespace codac2;

int main()
{
    {
      VectorVar x(2);
      ScalarVar u;
      AnalyticFunction f({x,u}, {-x[1]+u, x[0]-0.1*x[1]});
      DiffInclusion<VectorVar,ScalarVar> di(f);

      DI_Integration<DiffInclusion<VectorVar,ScalarVar>,
		     ReachabilityStep0> integration0(di);
      IntervalVector state {{1.0,1.0001},{0.0,0.0001}};
      for (double t=0;t<2.0;t+=0.01) {
         DefaultFigure::draw_box(state);
         IntervalVector s1 = IntervalVector::constant(2,Interval());
         IntervalVector s2 = IntervalVector::constant(2,Interval());
         integration0.contract_one_step(Interval(t,t+0.01),
                        state,
                        s1,s2, Interval(-0.01,0.01),true);
         state=s2;
      }
      state = {{1.0,1.0001},{0.0,0.0001}};
      Zonotope zstate(state);
      DI_Integration<DiffInclusion<VectorVar,ScalarVar>,
		     ReachabilityStep1> integration1(di);
      for (double t=0;t<2.0;t+=0.01) {
         IntervalVector s1 = IntervalVector::constant(2,Interval());
         Zonotope z2;
         integration1.contract_one_step(Interval(t,t+0.01),
                        zstate,
                        s1,z2, Interval(-0.01,0.01),true);
         zstate=z2;
         DefaultFigure::draw_zonotope(zstate,Color::red());
      }
      state = {{-1.0,1.0},{-1.0,1.0}};
      zstate = Zonotope(state);
         DefaultFigure::draw_zonotope(zstate,Color::green());
      int q=0;
      for (double t=0;t<10.0;t+=0.03125) {
         IntervalVector s1 = IntervalVector::constant(2,Interval());
         Zonotope z2;
         integration1.contract_one_step(Interval(t,t+0.01),
                        zstate,
                        s1,z2, Interval(-0.01,0.01),true);
         zstate=z2;
         if ((++q)%10==0) DefaultFigure::draw_zonotope(zstate,Color::red());
      }
         DefaultFigure::draw_zonotope(zstate,Color::green());

    }
}
