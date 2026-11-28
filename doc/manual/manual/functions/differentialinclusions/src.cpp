#include <codac2_DiffInclusion.h>
#include <codac2_RS_box.h>
#include <codac2_DI_Integration.h>
#include <codac2_Figure2D.h>
#include <codac2_RS_zonotope.h>

using namespace std;
using namespace codac2;

int main()
{
  {
    // [1-beg]
    VectorVar x(2);
    ScalarVar u;
    AnalyticFunction f({x,u}, {-x[1]+u, x[0]-0.1*x[1]});
    DiffInclusion<VectorVar,ScalarVar> di(f);

    int q=0;
    DI_Integration<DiffInclusion<VectorVar,ScalarVar>,
        RS_box> integration0(di);
    IntervalVector state {{1.0,1.01},{0.0,0.01}};
    for (double t=0;t<2.0;t+=0.01) {
      IntervalVector s1 = IntervalVector::constant(2,Interval());
      IntervalVector s2 = IntervalVector::constant(2,Interval());
      integration0.contract_one_step(Interval(t,t+0.01),
                state,
                s1,s2, Interval(-0.03,0.03),true);
      state=s2;
      if ((++q)%10==0) 
        DefaultFigure::draw_box(state,Color::purple());
      else
        DefaultFigure::draw_box(state,Color::purple(0.1));
    }
    // [1-end]

    // [2-beg]
    state = {{1.0,1.01},{0.0,0.01}};
    Zonotope zstate(state);
    DI_Integration<DiffInclusion<VectorVar,ScalarVar>,
        RS_zonotope> integration1(di);
    q=0;
      /* high limit the number of zonotope generators */
    integration1.get_reachabilitystep()->set_max_zonotope_order(200.0);
    for (double t=0;t<10.0;t+=0.02) {
      IntervalVector s1 = IntervalVector::constant(2,Interval());
      Zonotope z2;
      integration1.contract_one_step(Interval(t,t+0.02),
                zstate,
                s1,z2, Interval(-0.03,0.03),true);
      zstate=z2;
      if ((++q)%10==0) {
        DefaultFigure::draw_zonotope(zstate,Color::red());
      } else 
        DefaultFigure::draw_zonotope(zstate,Color::red(0.1));
    }
    // [2-end]

    state = {{1.0,1.01},{0.0,0.01}};
    zstate = Zonotope(state);
    for (double t=0;t<10.0;t+=0.02) {
      IntervalVector s1 = IntervalVector::constant(2,Interval());
      Zonotope z2;
      integration1.contract_one_step(Interval(t,t+0.02),
                zstate,
                s1,z2, Interval(-0.00,0.00),true);
      zstate=z2;
      if ((++q)%10==0) {
        DefaultFigure::draw_zonotope(zstate,Color::black());
      } else 
        DefaultFigure::draw_zonotope(zstate,Color::black(0.1));
    }

    // [3-beg]
      /* low limit the number of zonotope generators */
    integration1.get_reachabilitystep()->set_max_zonotope_order(2.0);
    state = {{-2.0,2.0},{-2.0,2.0}};
    zstate = Zonotope(state);
      DefaultFigure::draw_zonotope(zstate,Color::blue());
    q=0;
    for (double t=0;t<11.0;t+=0.01) {
      IntervalVector s1 = IntervalVector::constant(2,Interval());
      Zonotope z2;
      integration1.contract_one_step(Interval(t,t+0.01),
                zstate,
                s1,z2, Interval(-0.03,0.03),true);
      zstate=z2;
      if ((++q)%30==0) DefaultFigure::draw_zonotope(zstate,Color::red(0.2));
    }
      DefaultFigure::draw_zonotope(zstate,Color::black());
    // [3-end]
   }

  {
    // [4-beg]
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

    tube.set(IntervalVector({{0.85,0.9},{0.15,0.2}}),Interval(5.0));
    integration1.contract_sliced_tube
              (tube,Interval(-0.03,0.03),TimePropag::FWD_BWD);

    DefaultFigure::plot_tube(tube[0],Color::red(1.0));
    DefaultFigure::plot_tube(tube[1],Color::blue(1.0));
    // [4-end]
  }
}
