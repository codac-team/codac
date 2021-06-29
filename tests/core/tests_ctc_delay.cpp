#include <cstdio>
#include "catch_interval.hpp"
#include "codac_VIBesFigTube.h"
#include "codac_CtcDelay.h"
#include "vibes.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;

#define VIBES_DRAWING 0

TEST_CASE("CtcDelay")
{
  SECTION("Test CtcDelay, tube contraction")
  {
    Interval tdomain(0.,10.);
    Tube x(tdomain, 0.01, TFunction("cos(t)"));
    Tube y(tdomain, 0.01);

    CtcDelay ctc_delay;
    Interval pi = Interval::pi();
    ctc_delay.contract(pi, x, y);

    if(false && VIBES_DRAWING)
    {
      vibes::beginDrawing();
      VIBesFigTube fig_x("delay x", &x);
      fig_x.show();
      VIBesFigTube fig_y("delay y", &y);
      fig_y.show();
      vibes::endDrawing();
    }
    
    CHECK(y(Interval(0.,M_PI)) == Interval::ALL_REALS);
    CHECK(ApproxIntv(y(Interval(M_PI+0.01,10.))) == Interval(-1.,1.));
    CHECK(y(5.).is_superset(x(5. - M_PI)));
  }

  SECTION("Test CtcDelay, delay contraction")
  {
    double dt = 0.01;
    Interval tdomain(0.,10.);
    Tube x(tdomain, dt, TFunction("cos(t)"));
    Tube y(tdomain, dt, TFunction("sin(t)"));

    CtcDelay ctc_delay;
    Interval delay(0., 2.*M_PI);
    ctc_delay.contract(delay, x, y);
    ctc_delay.contract(delay, x, y);

    if(true && VIBES_DRAWING)
    {
      vibes::beginDrawing();
      VIBesFigTube fig_x("delay x", &x);
      fig_x.show();
      VIBesFigTube fig_y("delay y", &y);
      fig_y.show();
      vibes::endDrawing();
    }
    
    CHECK(delay.contains(M_PI/2.));
    CHECK(delay.diam() < 3.*dt);
  }
}