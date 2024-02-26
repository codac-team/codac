#include "catch_interval.hpp"
#include "codac_CtcDelay.h"
#include "codac-capd.h"
#include "codac_VIBesFigTube.h"

using namespace std;
using namespace Catch;
using namespace Detail;
using namespace ibex;
using namespace codac;

#define VIBES_DRAWING 0

TEST_CASE("CAPD_integrateODE")
{
  SECTION("Integration 1")
  {
    double dt = 0.0001;
    Interval tdomain(0,5);
    TFunction f("x","y", "(x^3+x*y^2-x+y; y^3+x^2*y-x-y)");
    IntervalVector x0({{0.5,0.5},{0.,0.}});

    TubeVector output = CAPD_integrateODE(tdomain, f, x0, dt, 20);

    IntervalVector expected({
      {0.1121125007098844, 0.1123948125529081},
      {-0.1748521323811479, -0.1747971075720925}
    });

    CHECK(expected.is_superset(output(1.0)));
  }

  SECTION("Integration 2")
  {
    double dt = 0.1;
    Interval tdomain(0.,10.);
    TFunction f("x", "y", "(-sin(x);-y)");
    IntervalVector x0({{1.,1.},{1.,1.}});
    TrajectoryVector solution(tdomain, TFunction("(2.*atan(exp(-t)*tan(0.5));exp(-t))"));

    TubeVector output = CAPD_integrateODE(tdomain, f, x0, dt, 20);

    #if VIBES_DRAWING // drawing results
      vibes::beginDrawing();
      VIBesFigTube fig_x("Integration 2");
      fig_x.add_tube(&output[0], "x0");
      fig_x.add_trajectory(&solution[0], "x0*");
      fig_x.add_tube(&output[1], "x1");
      fig_x.add_trajectory(&solution[1], "x1*");
      fig_x.set_properties(100, 100, 1000, 500);
      fig_x.show(true);
      vibes::endDrawing();
    #endif

    CHECK(output.tdomain() == solution.tdomain());
    CHECK(output.contains(solution) != NO);
    CHECK(output.nb_slices() == ceil(tdomain.diam()/dt));
  }

  SECTION("Integration 3: 2d problem")
  {
    double dt = 0.01;
    Interval tdomain(0.,10.);
    TFunction f("x", "y", "(x+2*y;3*x+2*y)");
    IntervalVector x0({{6.,6.},{4.,4.}});
    TrajectoryVector solution(tdomain, TFunction("(4*exp(4*t)+2*exp(-t);6*exp(4*t)-2*exp(-t))"));

    TubeVector output = CAPD_integrateODE(tdomain, f, x0, dt, 20);

    #if VIBES_DRAWING // drawing results
      vibes::beginDrawing();
      VIBesFigTube fig_x("Integration 3");
      fig_x.add_tube(&output[0], "x0");
      fig_x.add_trajectory(&solution[0], "x0*");
      fig_x.add_tube(&output[1], "x1");
      fig_x.add_trajectory(&solution[1], "x1*");
      fig_x.set_properties(100, 100, 1000, 500);
      fig_x.show(true);
      vibes::endDrawing();
    #endif

    CHECK(output.tdomain() == solution.tdomain());
    CHECK(output.contains(solution) != NO);
    CHECK(output.nb_slices() == ceil(tdomain.diam()/dt));
  }
}