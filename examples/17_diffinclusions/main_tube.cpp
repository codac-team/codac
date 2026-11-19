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

      IntervalVector init = {{-0.3,0.3},{0.8,1.0}};
      DI_Integration<DiffInclusion<VectorVar,ScalarVar>,
		     RS_zonotope> integration1(di);
      auto tube = integration1.create_sliced_tube
		(create_tdomain(Interval(0,10),0.03),
			init, IntervalVector({{-10,10},{-10,10}}),
			Interval(-0.03,0.03),true);

      DefaultFigure::plot_tube(tube[0],Color::red(0.1));
      DefaultFigure::plot_tube(tube[1],Color::blue(0.1));

      tube.set(IntervalVector({{0.1,0.2},{-0.3,-0.2}}),Interval(10.0));
      integration1.contract_sliced_tube
		(tube,Interval(-0.03,0.03),TimePropag::FWD_BWD);

      DefaultFigure::plot_tube(tube[0],Color::red(1.0));
      DefaultFigure::plot_tube(tube[1],Color::blue(1.0));
    }
}
