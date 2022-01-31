#include <cstdio>
#include "catch_interval.hpp"
#include "codac_VIBesFigTubeVector.h"
#include "codac_CtcStatic.h"
#include "codac_CtcFunction.h"
#include "codac_ContractorNetwork.h"
#include "vibes.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;

#define VIBES_DRAWING 0

TEST_CASE("CtcStatic")
{
  SECTION("Test CtcStatic")
  {
    double dt = 0.1;
    Interval tdomain(0., 10.);

    CtcFunction ctc_f(Function("t", "x[2]", "(x[0]-t;x[1]-t)"));
    CtcStatic ctc_static(ctc_f, true);

    TubeVector x(tdomain, dt, 2);

    ContractorNetwork cn;
    cn.add(ctc_static, {x});
    cn.contract();

    #if VIBES_DRAWING // drawing results
    vibes::beginDrawing();
    VIBesFigTubeVector fig("x");
    fig.set_properties(100, 100, 600, 300);
    fig.add_tube(&x, "x");
    fig.show();
    vibes::endDrawing();
    #endif

    Tube expected(tdomain, dt, TFunction("t"));
    CHECK(x[0] == expected);
    CHECK(x[1] == expected);
  }
}

TEST_CASE("CtcFunction")
{
  SECTION("Test max tubes")
  {
    CtcFunction ctc_max(Function("x1", "x2","x3", "max(x1,x2)-x3"));
    TubeVector tube(Interval(0,10), 1., 3);
    tube[0].set(Interval(2,3));
    tube[1].set(Interval(4,5));
    tube[2].set(Interval::ALL_REALS);
    ctc_max.contract(tube);
    CHECK(tube[2].codomain() == Interval(4,5));
  }
}