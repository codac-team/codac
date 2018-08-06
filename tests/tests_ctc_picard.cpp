#include "tests.h"
#include <cstdio>
#include "tubex_VibesFigure_Tube.h"
#include "vibes.h"

#define protected public     // Using #define so that we can access protected
#include "tubex_CtcPicard.h" // methods of CtcPicard for tests purposes

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

#define VIBES_DRAWING 0

TEST_CASE("CtcPicard")
{
  SECTION("Test CtcPicard, eval base")
  {
    CtcPicard ctc_picard(1.1);

    Interval t(0.5,1.0);
    Tube tube(t, 1);
    tube.set(Interval(1.5), t.lb());
    Tube tube_raw = tube;
    TubeSlice *slice;

    tube = tube_raw;
    slice = tube.get_first_slice();
    tubex::Function f2("x", "3.");
    ctc_picard.guess_slice_envelope(f2, tube, *slice, FORWARD);
    CHECK(slice->codomain()[0].is_superset(1.5 + Interval(0.,0.5) * 3.));

    tube = tube_raw;
    slice = tube.get_first_slice();
    tubex::Function f3("x", "t");
    ctc_picard.guess_slice_envelope(f3, tube, *slice, FORWARD);
    CHECK(slice->codomain()[0].is_superset(1.5 + Interval(0.,0.5) * t));

    tube = tube_raw;
    slice = tube.get_first_slice();
    f3 = tubex::Function("x", "t^2"); // with operator= of class Function
    ctc_picard.guess_slice_envelope(f3, tube, *slice, FORWARD);
    CHECK(slice->codomain()[0].is_superset(1.5 + Interval(0.,0.5) * t * t));

    tube = tube_raw;
    slice = tube.get_first_slice();
    tubex::Function f5("x", "x");
    ctc_picard.guess_slice_envelope(f5, tube, *slice, FORWARD);
    CHECK(slice->codomain()[0].is_superset(1.5 + Interval(0.,0.5) * Interval(1.,2.)));

    tube = tube_raw;
    slice = tube.get_first_slice();
    tubex::Function f6("x", "x*t");
    ctc_picard.guess_slice_envelope(f6, tube, *slice, FORWARD);
    CHECK(slice->codomain()[0].is_superset(1.5 + Interval(0.,0.5) * Interval(1.,2.) * t));

    tube = tube_raw;
    tube.set(2.*atan(exp(-t.lb())*tan(0.5)), t.lb());
    slice = tube.get_first_slice();
    tubex::Function f7("x", "-sin(x)");
    ctc_picard.guess_slice_envelope(f7, tube, *slice, FORWARD);
    CHECK(slice->codomain()[0].is_superset(2.*atan(exp(-t)*tan(0.5))));

    tube = tube_raw;
    tube.set(exp(-t.lb()), t.lb());
    slice = tube.get_first_slice();
    tubex::Function f8("x", "-x");
    ctc_picard.guess_slice_envelope(f8, tube, *slice, FORWARD);
    CHECK(slice->codomain()[0].is_superset(exp(-t)));
  }

  SECTION("Test CtcPicard / TubeSlice - dim 1")
  {
    Interval domain(0.,0.1);
    Tube tube(domain, 1);
    tube.set(exp(domain.lb()), domain.lb());
    TubeSlice *x = tube.get_first_slice();

    tubex::Function f("x", "x");
    CtcPicard ctc_picard(1.1);

    CHECK(x->codomain()[0] == Interval::ALL_REALS);
    CHECK(x->input_gate()[0] == exp(0.));
    CHECK(x->output_gate()[0] == Interval::ALL_REALS);
    ctc_picard.contract(f, tube, *x, FORWARD);
    CHECK(x->codomain()[0].is_superset(Interval(exp(domain))));
    CHECK(x->output_gate()[0].is_superset(Interval(exp(domain.ub()))));
    CHECK(ctc_picard.picard_iterations() < 4);
  }

  SECTION("Test CtcPicard / TubeVector - dim 1")
  {
    Interval domain(0.,1.);

    TubeVector x_preserve_sampling(domain, 1);
    IntervalVector condition(1, Interval(1.));
    x_preserve_sampling.set(condition, 0.);

    TubeVector x_auto_sampling(x_preserve_sampling);

    tubex::Function f("x", "-x");
    CtcPicard ctc_picard_preserve(true, 1.1);
    CtcPicard ctc_picard_auto(false, 1.1);

    ctc_picard_preserve.contract(f, x_preserve_sampling);
    ctc_picard_auto.contract(f, x_auto_sampling);
    
    CHECK_FALSE(x_preserve_sampling.codomain().is_unbounded());
    CHECK(x_preserve_sampling.codomain()[0].is_superset(exp(-domain)));
    CHECK(x_preserve_sampling(0.)[0].is_superset(Interval(exp(-0.))));
    CHECK(x_preserve_sampling(1.)[0].is_superset(Interval(exp(-1.))));
    CHECK(x_preserve_sampling.nb_slices() == 1);
    
    CHECK(x_auto_sampling.codomain()[0] == x_preserve_sampling.codomain()[0]);
    CHECK(x_auto_sampling(0.)[0] == x_preserve_sampling(0.)[0]);
    CHECK(x_auto_sampling(1.)[0] == x_preserve_sampling(1.)[0]);
    CHECK(x_auto_sampling.nb_slices() != 1);

    if(VIBES_DRAWING) // drawing results
    {
      vibes::beginDrawing();
      VibesFigure_Tube fig_tube("picard", &x_auto_sampling);
      fig_tube.set_properties(100, 100, 500, 500);
      fig_tube.show(true);
      vibes::endDrawing();
    }
  }

  SECTION("Test CtcPicard / TubeVector - dim 2")
  {
    Interval domain(0.,1.);
    
    TubeVector x(domain, 2);
    IntervalVector condition(2, Interval(1.));
    x.set(condition, 0.);
    
    tubex::Function f("x", "y", "(-x ; y)");
    CtcPicard ctc_picard(1.1/*, false*/);
    ctc_picard.contract(f, x);
    
    CHECK_FALSE(x.codomain().is_unbounded());
    CHECK(x.codomain()[0].is_superset(exp(-domain)));
    CHECK(x(0.)[0].is_superset(Interval(exp(-0.))));
    CHECK(x(1.)[0].is_superset(Interval(exp(-1.))));
    CHECK(x.codomain()[1].is_superset(exp(domain)));
    CHECK(x(0.)[1].is_superset(Interval(exp(0.))));
    CHECK(x(1.)[1].is_superset(Interval(exp(1.))));
    
    if(false & VIBES_DRAWING) // drawing results
    {
      vibes::beginDrawing();
      VibesFigure_Tube fig_tube("picard", &x);
      fig_tube.set_properties(100, 100, 500, 500);
      fig_tube.show(true);
      vibes::endDrawing();
    }
  }

  SECTION("Test CtcPicard / TubeVector - dim 1 - forward")
  {
    Interval domain(0.,1.);
    Tube x(domain);
    x.set(1., 0.);

    tubex::Function f("x", "-x");
    CtcPicard ctc_picard(1.1/*, false*/);

    ctc_picard.contract(f, x, FORWARD);
    
    CHECK_FALSE(x.codomain().is_unbounded());
    CHECK(x.codomain().is_superset(exp(-domain)));
    CHECK(x(0.).is_superset(Interval(exp(-0.))));
    CHECK(x(1.).is_superset(Interval(exp(-1))));

    if(false & VIBES_DRAWING) // drawing results
    {
      //vibes::beginDrawing();
      //VibesFigure_Tube fig_tube("picard", &x);
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

    tubex::Function f("x", "-x");
    CtcPicard ctc_picard(1.1/*, false*/);

    ctc_picard.contract(f, x, BACKWARD);
    
    CHECK_FALSE(x.codomain().is_unbounded());
    CHECK(x.codomain().is_superset(exp(-domain)));
    CHECK(x(0.).is_superset(Interval(exp(-0.))));
    CHECK(x(1.).is_superset(Interval(exp(-1))));

    if(false & VIBES_DRAWING) // drawing results
    {
      //vibes::beginDrawing();
      //VibesFigure_Tube fig_tube("picard", &x);
      //fig_tube.set_properties(100, 100, 500, 500);
      //fig_tube.show(true);
      //vibes::endDrawing();
    }
  }
}