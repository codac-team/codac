#include "tests.h"
#include <cstdio>

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

TEST_CASE("CtcPicard")
{
  SECTION("Test CtcPicard - dim 1")
  {
    // TubeSlice functions

      Interval domain(0.,0.1);
      TubeSlice x(domain, 1);
      x.setInputGate(IntervalVector(1, Interval(1.)));

      Function f("x", "x");
      CtcPicard ctc_picard;

      CHECK(x.codomain()[0] == Interval::ALL_REALS);
      ctc_picard.contract_fwd(f, x);
      CHECK(x.codomain()[0].is_superset(Interval(exp(domain))));
      CHECK(x.outputGate()[0].is_superset(Interval(exp(domain.ub()))));
  }
}