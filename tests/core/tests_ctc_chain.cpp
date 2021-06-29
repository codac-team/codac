#include <cstdio>
#include "catch_interval.hpp"
#include "codac_VIBesFigTube.h"
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

#define VIBES_DRAWING 0

TEST_CASE("CtcChain")
{
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