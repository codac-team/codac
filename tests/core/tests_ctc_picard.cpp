#include <cstdio>
#include "catch_interval.hpp"
#include "codac_VIBesFigTube.h"
#include "vibes.h"

// Using #define so that we can access protected methods
// of the class for tests purposes
#define protected public
#include "codac_CtcPicard.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;

#define VIBES_DRAWING 0

TEST_CASE("CtcPicard")
{
  SECTION("Test CtcPicard, eval base")
  {
    float delta = 1.1;

    Interval t(0.5,1.0);
    TubeVector tube(t, 1);
    tube.set(IntervalVector(1, 1.5), t.lb());
    TubeVector tube_raw = tube;
    Slice *slice;

    tube = tube_raw;
    TFunction f2("x", "3.");
    CtcPicard ctc_picard_f2(f2, delta);
    Interval dtest = tube[0].slice_tdomain(0);
    IntervalVector test = f2.eval_vector((const Interval)dtest, (const TubeVector)tube);
    ctc_picard_f2.guess_kth_slices_envelope(tube, 0, TimePropag::FORWARD);
    CHECK(tube(0).is_superset(1.5 + Interval(0.,0.5) * 3.));

    tube = tube_raw;
    TFunction f3("x", "t");
    CtcPicard ctc_picard_f3(f3, delta);
    ctc_picard_f3.guess_kth_slices_envelope(tube, 0, TimePropag::FORWARD);
    CHECK(tube(0).is_superset(1.5 + Interval(0.,0.5) * t));

    tube = tube_raw;
    TFunction f4("x", "t^2");
    CtcPicard ctc_picard_f4(f4, delta); // with operator= of class TFunction
    ctc_picard_f4.guess_kth_slices_envelope(tube, 0, TimePropag::FORWARD);
    CHECK(tube(0).is_superset(1.5 + Interval(0.,0.5) * t * t));

    tube = tube_raw;
    TFunction f5("x", "x");
    CtcPicard ctc_picard_f5(f5, delta); 
    ctc_picard_f5.guess_kth_slices_envelope(tube, 0, TimePropag::FORWARD);
    CHECK(tube(0).is_superset(1.5 + Interval(0.,0.5) * Interval(1.,2.)));

    tube = tube_raw;
    TFunction f6("x", "x*t");
    CtcPicard ctc_picard_f6(f6, delta); 
    ctc_picard_f6.guess_kth_slices_envelope(tube, 0, TimePropag::FORWARD);
    CHECK(tube(0).is_superset(1.5 + Interval(0.,0.5) * Interval(1.,2.) * t));

    tube = tube_raw;
    tube.set(IntervalVector(1, 2.*atan(exp(-t.lb())*tan(0.5))), t.lb());
    TFunction f7("x", "-sin(x)");
    CtcPicard ctc_picard_f7(f7, delta);
    ctc_picard_f7.guess_kth_slices_envelope(tube, 0, TimePropag::FORWARD);
    CHECK(tube(0).is_superset(Interval(2.*atan(exp(-t.lb())*tan(0.5))) | 2.*atan(exp(-t.ub())*tan(0.5))));
    
    tube = tube_raw;
    tube.set(IntervalVector(1, exp(-t.lb())), t.lb());
    TFunction f8("x", "-x");
    CtcPicard ctc_picard_f8(f8, delta);
    ctc_picard_f8.guess_kth_slices_envelope(tube, 0, TimePropag::FORWARD);
    CHECK(tube(0).is_superset(IntervalVector(Interval(exp(-t.lb()))) | IntervalVector(Interval(exp(-t.ub())))));
  }

  SECTION("Test CtcPicard / Slice - dim 1")
  {
    Interval domain(0.,0.1);
    TubeVector tube(domain, 1);
    tube.set(IntervalVector(1, exp(domain.lb())), domain.lb());
    Slice *x = tube[0].first_slice();

    TFunction f("x", "x");
    CtcPicard ctc_picard(f, 1.1);

    CHECK(x->codomain() == Interval::ALL_REALS);
    CHECK(x->input_gate() == exp(0.));
    CHECK(x->output_gate() == Interval::ALL_REALS);
    ctc_picard.contract(tube, TimePropag::FORWARD);
    x = tube[0].first_slice();
    // TODO : CHECK(x->codomain()==(Interval(exp(domain))));
    CHECK(x->output_gate().is_superset(Interval(exp(domain.ub()))));
    CHECK(ctc_picard.picard_iterations() != 1);
    CHECK(ctc_picard.picard_iterations() < 4);
  }

  SECTION("Test CtcPicard / TubeVector - dim 1")
  {
    Interval domain(0.,1.);

    TubeVector x_preserve_sampling(domain, 1);
    IntervalVector condition(1, Interval(1.));
    x_preserve_sampling.set(condition, 0.);

    TubeVector x_auto_sampling(x_preserve_sampling);

    TFunction f("x", "-x");
    CtcPicard ctc_picard_preserve(f, 1.1);
    ctc_picard_preserve.preserve_slicing(true);
    CtcPicard ctc_picard_auto(f, 1.1);
    ctc_picard_auto.preserve_slicing(false);

    ctc_picard_preserve.contract(x_preserve_sampling, TimePropag::FORWARD); // todo: remove FORWARD
    ctc_picard_auto.contract(x_auto_sampling, TimePropag::FORWARD); // todo: remove FORWARD
    
    CHECK_FALSE(x_preserve_sampling.codomain().is_unbounded());
    // TODO : CHECK(x_preserve_sampling.codomain().is_superset(exp(-domain)));
    CHECK(x_preserve_sampling(0.).is_superset(Interval(exp(-0.))));
    CHECK(x_preserve_sampling(1.).is_superset(Interval(exp(-1.))));
    CHECK(x_preserve_sampling.nb_slices() == 1);
    
    CHECK(x_auto_sampling.codomain() == x_preserve_sampling.codomain());
    CHECK(x_auto_sampling(0.) == x_preserve_sampling(0.));
    CHECK(x_auto_sampling(1.) == x_preserve_sampling(1.));
    CHECK(x_auto_sampling.nb_slices() != 1);

    //if(VIBES_DRAWING) // drawing results
    //{
    //  vibes::beginDrawing();
    //  VIBesFigTube fig_tube("picard", &x_auto_sampling);
    //  fig_tube.set_properties(100, 100, 500, 500);
    //  fig_tube.show(true);
    //  vibes::endDrawing();
    //}
  }

  SECTION("Test CtcPicard / TubeVector - dim 2")
  {
    Interval domain(0.,1.);
    
    TubeVector x(domain, 2);
    IntervalVector condition(2, Interval(1.));
    x.set(condition, 0.);
    
    TFunction f("x", "y", "(-x ; y)");
    CtcPicard ctc_picard(f, 1.1);
    ctc_picard.contract(x, TimePropag::FORWARD);

    CHECK_FALSE(x.codomain().is_unbounded());
    // TODO : CHECK(x.codomain()[0].is_superset(exp(-domain)));
    CHECK(x(0.)[0].is_superset(Interval(exp(-0.))));
    CHECK(x(1.)[0].is_superset(Interval(exp(-1.))));
    // TODO : CHECK(x.codomain()[1].is_superset(exp(domain)));
    CHECK(x(0.)[1].is_superset(Interval(exp(0.))));
    CHECK(x(1.)[1].is_superset(Interval(exp(1.))));
    
    //if(false & VIBES_DRAWING) // drawing results
    //{
    //  vibes::beginDrawing();
    //  VIBesFigTube fig_tube("picard", &x);
    //  fig_tube.set_properties(100, 100, 500, 500);
    //  fig_tube.show(true);
    //  vibes::endDrawing();
    //}
  }

  SECTION("Test CtcPicard / TubeVector - dim 1 - forward")
  {
    Interval domain(0.,1.);
    Tube x(domain);
    x.set(1., 0.);

    TFunction f("x", "-x");
    CtcPicard ctc_picard(f, 1.1);

    ctc_picard.contract(x, TimePropag::FORWARD);
    
    CHECK_FALSE(x.codomain().is_unbounded());
    // TODO : CHECK(x.codomain().is_superset(exp(-domain)));
    CHECK(x(0.).is_superset(Interval(exp(-0.))));
    CHECK(x(1.).is_superset(Interval(exp(-1))));

    if(false & VIBES_DRAWING) // drawing results
    {
      //vibes::beginDrawing();
      //VIBesFigTube fig_tube("picard", &x);
      //fig_tube.set_properties(100, 100, 500, 500);
      //fig_tube.show(true);
      //vibes::endDrawing();
    }
  }

  SECTION("Test CtcPicard / TubeVector - dim 1 - backward")
  {
    Interval domain(0.,1.);
    Tube x(domain);
    x.set(exp(-1), 1.);

    TFunction f("x", "-x");
    CtcPicard ctc_picard(f, 1.1);

    ctc_picard.contract(x, TimePropag::BACKWARD);
    CHECK_FALSE(x.codomain().is_unbounded());
    CHECK(x.codomain().is_superset(Interval(exp(-domain.lb())) | exp(-domain.ub())));
    CHECK(x(0.).is_superset(Interval(exp(-0.))));
    CHECK(x(1.).is_superset(Interval(exp(-1))));

    if(false & VIBES_DRAWING) // drawing results
    {
      //vibes::beginDrawing();
      //VIBesFigTube fig_tube("picard", &x);
      //fig_tube.set_properties(100, 100, 500, 500);
      //fig_tube.show(true);
      //vibes::endDrawing();
    }
  }
}