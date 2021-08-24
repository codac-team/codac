#include <cstdio>
#include "catch_interval.hpp"
#include "codac_VIBesFigTube.h"
#include "codac_CtcDeriv.h"
#include "codac_CtcFunction.h"
#include "vibes.h"

// Using #define so that we can access protected methods
// of the class for tests purposes
#define protected public
#include "codac_CtcChain.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;

//#define VIBES_DRAWING 0

TEST_CASE("CtcChain")
{
  SECTION("Test CtcDeriv (graphical example)")
  {
    double dt = 0.01;
    Interval tdomain(0., M_PI);

    Tube a(tdomain, dt, TFunction("[-0.25,0.25]+cos(t)"));
    Tube b(tdomain, dt, TFunction("t+abs(1.5-t)^2*[-0.51,0.51]"));

    CtcFunction ctc_cos(Function("b", "a", "a-cos(b)"));

    #ifdef VIBES_DRAWING // drawing results
    vibes::beginDrawing();

    VIBesFigTube fig_b("b");
    fig_b.set_properties(100, 100, 600, 300);
    fig_b.add_tube(&b, "b");
    fig_b.set_tube_color(&b, "#004B62[#006A82]", "#868686[#D2D2D2]");
    fig_b.show();

    VIBesFigTube fig_a("a");
    fig_a.set_properties(100, 500, 600, 300);
    fig_a.add_tube(&a, "a");
    fig_a.set_tube_color(&a, "#004B62[#006A82]", "#868686[#D2D2D2]");
    fig_a.show();
    #endif

    ctc_cos.contract(b, a);
    ctc_cos.contract(b, a);

    #ifdef VIBES_DRAWING // drawing results
    fig_a.show();
    fig_b.show();
    vibes::endDrawing();
    #endif
  }
  
  /*SECTION("Test CtcDeriv (graphical example)")
  {
    double dt = 0.01;
    Interval tdomain(0., M_PI);

    Tube v(tdomain, dt, TFunction("sin(t+3.14+(3.14/2))/5+[-0.05,0.05]+(3.14-t)*[-0.01,0.01]"));
    Tube x(tdomain, dt, TFunction("[-0.05,0.05]+2+t*t*[-0.01,0.01]"));

    #ifdef VIBES_DRAWING // drawing results
    vibes::beginDrawing();

    VIBesFigTube fig_v("v C_deriv");
    fig_v.set_properties(100, 100, 600, 300);
    fig_v.add_tube(&v, "v");
    fig_v.set_tube_color(&v, "#004B62[#006A82]", "#868686[#D2D2D2]");
    fig_v.show();

    VIBesFigTube fig_x("x C_deriv");
    fig_x.set_properties(100, 500, 600, 300);
    fig_x.add_tube(&x, "x");
    fig_x.set_tube_color(&x, "#004B62[#006A82]", "#868686[#D2D2D2]");
    fig_x.show();
    #endif

    CtcDeriv ctc_deriv;
    //ctc_deriv.contract(x, v);

    #ifdef VIBES_DRAWING // drawing results
    fig_x.show();
    vibes::endDrawing();
    #endif
  }*/

  SECTION("Test CtcChain")
  {
    double dt = 0.003;
    Interval tdomain(0.,10.);

    Tube x(
      Trajectory(tdomain, TFunction("-(((t-5)^2)/10)-(t/10)-2")),
      Trajectory(tdomain, TFunction("cos(t)-(t/4)")),
      dt);
    Tube v(tdomain, dt), u(tdomain, dt, Interval(0.));

    // TRUTH (FOR CHECK ASSERT)

      Trajectory traj1, traj2, traj3;

      traj1.set(-0.9, tdomain.lb());
      traj1.set(-3.9, tdomain.ub());
      
      traj2.set(-1.3, tdomain.lb());
      traj2.set(-3.5, tdomain.ub());

      Tube enclosed_tube(tdomain, dt, Interval::empty_set());
      enclosed_tube |= traj1;
      enclosed_tube |= traj2;

    //

    #ifdef VIBES_DRAWING // drawing results
    vibes::beginDrawing();
    VIBesFigTube fig_x("x");
    fig_x.set_properties(100, 100, 600, 300);
    fig_x.add_tube(&x, "x");
    fig_x.set_tube_color(&x, "#004B62[#006A82]", "#868686[#D2D2D2]");
    //fig_x.add_tube(&enclosed_tube, "y");
    //fig_x.set_tube_color(&enclosed_tube, "red[red]");
    fig_x.show();
    #endif

    CtcChain ctc_chain;
    ctc_chain.contract(x, v, u);

    #ifdef VIBES_DRAWING // drawing results
    fig_x.show();
    vibes::endDrawing();
    #endif

    CHECK(enclosed_tube.is_subset(x));
  }
}