#include <cstdio>
#include "catch_interval.hpp"
#include "codac_VIBesFigTube.h"
#include "codac_VIBesFigTubeVector.h"
#include "codac_ContractorNetwork.h"
#include "vibes.h"

// Using #define so that we can access protected methods
// of the class for tests purposes
#define protected public
#include "codac_CtcLohner.h"


using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;

#define VIBES_DRAWING 0

TEST_CASE("CtcLohner")
{
  SECTION("Test CtcLohner, eval base")
  {
    float delta = 1.1;

    Interval t(0.5, 1.0);
    TubeVector tube(t, 1);
    tube.set(IntervalVector(1, 1.5), t.lb());
    TubeVector tube_raw = tube;

    tube = tube_raw;
    Function f5("x", "x");
    CtcLohner ctc_lohner_f5(f5);
    ctc_lohner_f5.contract(tube, TimePropag::FORWARD);
    CHECK(tube(0).is_superset(1.5 + Interval(0., 0.5) * Interval(1., 2.)));
    // todo: these tests are not useful (supersets)

    tube = tube_raw;
    tube.set(IntervalVector(1, 2. * atan(exp(-t.lb()) * tan(0.5))), t.lb());
    Function f7("x", "-sin(x)");
    CtcLohner ctc_lohner_f7(f7);
    ctc_lohner_f7.contract(tube, TimePropag::FORWARD);
    CHECK(tube(0).is_superset(Interval(2. * atan(exp(-t.lb()) * tan(0.5))) | 2. * atan(exp(-t.ub()) * tan(0.5))));
    // todo: these tests are not useful (supersets)

    tube = tube_raw;
    tube.set(IntervalVector(1, exp(-t.lb())), t.lb());
    Function f8("x", "-x");
    CtcLohner ctc_lohner_f8(f8);
    ctc_lohner_f8.contract(tube, TimePropag::FORWARD);
    CHECK(tube(0).is_superset(IntervalVector(Interval(exp(-t.lb()))) | IntervalVector(Interval(exp(-t.ub())))));
    // todo: these tests are not useful (supersets)
  }

  SECTION("Test CtcLohner / Slice - dim 1")
  {
    Interval domain(0., 0.1);
    TubeVector tube(domain, 1);
    tube.set(IntervalVector(1, exp(domain.lb())), domain.lb());
    Slice *x = tube[0].first_slice();

    Function f("x", "x");
    CtcLohner ctc_lohner(f);

    CHECK(x->codomain() == Interval::ALL_REALS);
    CHECK(x->input_gate() == exp(0.));
    CHECK(x->output_gate() == Interval::ALL_REALS);
    ctc_lohner.contract(tube, TimePropag::FORWARD);
    x = tube[0].first_slice();
    CHECK(x->output_gate().is_superset(Interval(exp(domain.ub()))));
    // todo: these tests are not useful (supersets)
  }

  SECTION("Test CtcLohner / TubeVector - dim 2")
  {
    Interval domain(0., 1.);
    double dt = 0.1;
    TubeVector x(domain, dt, 2);
    IntervalVector condition(2, Interval(1.));
    x.set(condition, 0.);

    Function f("x", "y", "(-x ; y)");
    CtcLohner ctc_lohner(f);
    ctc_lohner.contract(x, TimePropag::FORWARD);

    CHECK_FALSE(x.codomain().is_unbounded());
    // TODO : CHECK(x.codomain()[0].is_superset(exp(-domain)));
    CHECK(x(0.)[0].is_superset(Interval(exp(-0.))));
    CHECK(x(1.)[0].is_superset(Interval(exp(-1.))));
    // TODO : CHECK(x.codomain()[1].is_superset(exp(domain)));
    CHECK(x(0.)[1].is_superset(Interval(exp(0.))));
    CHECK(x(1.)[1].is_superset(Interval(exp(1.))));
    // todo: these tests are not useful (supersets)

    //if(false & VIBES_DRAWING) // drawing results
    //{
    //  vibes::beginDrawing();
    //  VIBesFigTube fig_tube("picard", &x);
    //  fig_tube.set_properties(100, 100, 500, 500);
    //  fig_tube.show(true);
    //  vibes::endDrawing();
    //}
  }

  SECTION("Test CtcLohner / TubeVector - dim 1 - forward")
  {
    Interval domain(0., 1.);
    double dt = 0.1;
    Tube x(domain, dt);
    x.set(1., 0.);

    Function f("x", "-x");
    CtcLohner ctc_lohner(f);

    ctc_lohner.contract(x, TimePropag::FORWARD);

    CHECK_FALSE(x.codomain().is_unbounded());
    CHECK(x.codomain().is_superset(Interval(exp(-domain.lb())) | exp(-domain.ub())));
    CHECK(x(0.).is_superset(Interval(exp(-0.))));
    CHECK(x(1.).is_superset(Interval(exp(-1))));
    // todo: these tests are not useful (supersets)

    if (false & VIBES_DRAWING) // drawing results
    {
      //vibes::beginDrawing();
      //VIBesFigTube fig_tube("picard", &x);
      //fig_tube.set_properties(100, 100, 500, 500);
      //fig_tube.show(true);
      //vibes::endDrawing();
    }
  }

  SECTION("Test CtcLohner / TubeVector - dim 1 - backward")
  {
    double dt = 0.1;
    Interval domain(0., 1.);
    Tube x(domain, dt);
    x.set(exp(-1), 1.);

    Function f("x", "-x");
    CtcLohner ctc_lohner(f);

    ctc_lohner.contract(x, TimePropag::BACKWARD);
    CHECK_FALSE(x.codomain().is_unbounded());
    CHECK(x.codomain().is_superset(Interval(exp(-domain.lb())) | exp(-domain.ub())));
    CHECK(x(0.).is_superset(Interval(exp(-0.))));
    CHECK(x(1.).is_superset(Interval(exp(-1))));
    // todo: these tests are not useful (supersets)

    if (false & VIBES_DRAWING) // drawing results
    {
      //vibes::beginDrawing();
      //VIBesFigTube fig_tube("picard", &x);
      //fig_tube.set_properties(100, 100, 500, 500);
      //fig_tube.show(true);
      //vibes::endDrawing();
    }
  }

  SECTION("Test CtcLohner in CN")
  {
    Interval domain(0., 1.);
    double dt = 0.1;
    TubeVector x(domain, dt, 2);
    IntervalVector condition(2, Interval(1.));
    x.set(condition, 0.);

    Function f("x", "y", "(-x ; y)");
    CtcLohner ctc_lohner(f);

    //ctc_lohner.contract(x);

    ContractorNetwork cn;
    cn.add(ctc_lohner, {x});
    cn.contract();

    CHECK_FALSE(x.codomain().is_unbounded());
    // TODO : CHECK(x.codomain()[0].is_superset(exp(-domain)));
    CHECK(x(0.)[0].is_superset(Interval(exp(-0.))));
    CHECK(x(1.)[0].is_superset(Interval(exp(-1.))));
    // TODO : CHECK(x.codomain()[1].is_superset(exp(domain)));
    CHECK(x(0.)[1].is_superset(Interval(exp(0.))));
    CHECK(x(1.)[1].is_superset(Interval(exp(1.))));

    if (false & VIBES_DRAWING) // drawing results
    {
      vibes::beginDrawing();
      VIBesFigTubeVector fig_tube("fig", &x);
      fig_tube.set_properties(100, 100, 500, 500);
      fig_tube.show(true);
      vibes::endDrawing();
    }
  }
}