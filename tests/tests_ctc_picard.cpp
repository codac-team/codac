#include "tests.h"
#include <cstdio>
#include "tubex_CtcPicard.h"
#include "tubex_VibesFigure_Tube.h"
#include "vibes.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

#define VIBES_DRAWING 0

TEST_CASE("CtcPicard")
{
  SECTION("Test CtcPicard / TubeSlice - dim 1")
  {
    Interval domain(0.,0.1);
    TubeSlice x(domain, 1);
    x.setInputGate(IntervalVector(1, Interval(1.)));

    ibex::Function f("x", "x");
    CtcPicard ctc_picard(1.1);

    CHECK(x.codomain()[0] == Interval::ALL_REALS);
    ctc_picard.contract_fwd(f, x);
    CHECK(x.codomain()[0].is_superset(Interval(exp(domain))));
    CHECK(x.outputGate()[0].is_superset(Interval(exp(domain.ub()))));
    CHECK(ctc_picard.picardIterations() < 4);
  }

  SECTION("Test CtcPicard / TubeSlice - dim 2")
  {
    Interval domain(0.,0.1);
    TubeSlice x(domain, 2);

    IntervalVector condition(2, 1.);
    x.setInputGate(condition);

    Variable var_x, var_y;
    ibex::Function f(var_x, var_y, Return(var_x,var_y));
    CtcPicard ctc_picard;

    CHECK(x.codomain() == IntervalVector(2, Interval::ALL_REALS));
    ctc_picard.contract_fwd(f, x);
    CHECK(x.codomain()[0].is_superset(Interval(exp(domain))));
    CHECK(x.outputGate()[0].is_superset(Interval(exp(domain.ub()))));
  }

  SECTION("Test CtcPicard / TubeVector - dim 1")
  {
    Interval domain(0.,1.);

    TubeVector x_preserve_sampling(domain, 1);
    IntervalVector condition(1, Interval(1.));
    x_preserve_sampling.set(condition, 0.);

    TubeVector x_auto_sampling(x_preserve_sampling);

    ibex::Function f("x", "-x");
    CtcPicard ctc_picard_preserve(1.1, true);
    CtcPicard ctc_picard_auto(1.1, false);

    ctc_picard_preserve.contract(f, x_preserve_sampling);
    ctc_picard_auto.contract(f, x_auto_sampling);
    
    CHECK_FALSE(x_preserve_sampling.codomain().is_unbounded());
    CHECK(x_preserve_sampling.codomain()[0].is_superset(exp(-domain)));
    CHECK(x_preserve_sampling[0.][0].is_superset(Interval(exp(-0.))));
    CHECK(x_preserve_sampling[1.][0].is_superset(Interval(exp(-1.))));
    CHECK(x_preserve_sampling.nbSlices() == 1);
    
    CHECK(x_auto_sampling.codomain()[0] == x_preserve_sampling.codomain()[0]);
    CHECK(x_auto_sampling[0.][0] == x_preserve_sampling[0.][0]);
    CHECK(x_auto_sampling[1.][0] == x_preserve_sampling[1.][0]);
    CHECK(x_auto_sampling.nbSlices() != 1);

    if(VIBES_DRAWING) // drawing results
    {
      vibes::beginDrawing();
      VibesFigure_Tube fig_tube("picard", &x_auto_sampling);
      fig_tube.setProperties(100, 100, 500, 500);
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

    Variable var_x, var_y;
    ibex::Function f(var_x, var_y, Return(-var_x, -var_y));
    CtcPicard ctc_picard(1.1, false);
    ctc_picard.contract(f, x);
    
    CHECK_FALSE(x.codomain().is_unbounded());
    CHECK(x.codomain()[0].is_superset(exp(-domain)));
    CHECK(x[0.][0].is_superset(Interval(exp(-0.))));
    CHECK(x[1.][0].is_superset(Interval(exp(-1.))));

    if(VIBES_DRAWING) // drawing results
    {
      vibes::beginDrawing();
      VibesFigure_Tube fig_tube("picard", &x);
      fig_tube.setProperties(100, 100, 500, 500);
      fig_tube.show(true);
      vibes::endDrawing();
    }
  }

  SECTION("Test CtcPicard / TubeVector - dim 1 - backward")
  {
    Interval domain(0.,1.);

    TubeVector x(domain, 1);
    IntervalVector condition(1, Interval(1.));
    x.set(condition, 1.);

    ibex::Function f("x", "-x");
    CtcPicard ctc_picard(1.1, false);

    ctc_picard.contract_bwd(f, x);
    
    CHECK_FALSE(x.codomain().is_unbounded());
    CHECK(x.codomain()[0].is_superset(exp(-(domain - 1.))));
    CHECK(x[0.][0].is_superset(Interval(exp(1.))));
    CHECK(x[1.][0].is_superset(Interval(exp(0.))));

    if(VIBES_DRAWING) // drawing results
    {
      vibes::beginDrawing();
      VibesFigure_Tube fig_tube("picard", &x);
      fig_tube.setProperties(100, 100, 500, 500);
      fig_tube.show(true);
      vibes::endDrawing();
    }
  }
}