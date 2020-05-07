#include <cstdio>
#include "catch_interval.hpp"
#include "tubex_VIBesFigTube.h"
#include "tubex_CtcDelay.h"
#include "vibes.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

#define VIBES_DRAWING 0

TEST_CASE("CtcDelay")
{
  SECTION("Test CtcDelay")
  {
    Interval domain(0.,10.);
    Tube x(domain, 0.01, tubex::Function("cos(t)"));
    Tube y(domain, 0.01);

    CtcDelay ctc_delay;
    Interval pi = Interval::pi();
    ctc_delay.contract(pi, x, y);

    if(true)
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
}