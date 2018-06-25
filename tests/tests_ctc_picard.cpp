#include "tests.h"
#include <cstdio>

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

#define VIBES_DRAWING 1

TEST_CASE("CtcPicard")
{
  SECTION("Test CtcPicard / TubeSlice - dim 1")
  {
    Interval domain(0.,0.1);
    TubeSlice x(domain, 1);
    x.setInputGate(IntervalVector(1, Interval(1.)));

    Function f("x", "x");
    CtcPicard ctc_picard(1.1);

    CHECK(x.codomain()[0] == Interval::ALL_REALS);
    ctc_picard.contract_fwd(f, x);
    CHECK(x.codomain()[0].is_superset(Interval(exp(domain))));
    CHECK(x.outputGate()[0].is_superset(Interval(exp(domain.ub()))));
    //CHECK(ctc_picard.picardIterations() < 10);
  }

  SECTION("Test CtcPicard / TubeSlice - dim 2")
  {
    Interval domain(0.,0.1);
    TubeSlice x(domain, 2);

    IntervalVector condition(2, 1.);
    x.setInputGate(condition);

    Variable var_x, var_y;
    Function f(var_x, var_y, Return(var_x,var_y));
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

    Function f("x", "-x");
    CtcPicard ctc_picard;

    ctc_picard.contract(f, x_preserve_sampling, true);
    ctc_picard.contract(f, x_auto_sampling, false);
    
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
      cout << x_auto_sampling << endl;
      cout << x_preserve_sampling << endl;
      vibes::beginDrawing();
      VibesFigure_Tube fig_tube("picard", &x_auto_sampling);
      fig_tube.setProperties(100, 100, 500, 500);
      fig_tube.show(true);
      vibes::endDrawing();
    }
  }
}