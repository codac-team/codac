#include "tests.h"
#include <cstdio>

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

TEST_CASE("CtcPicard")
{
  SECTION("Test CtcPicard")
  {
    Interval domain(0.,0.1);
    TubeSlice x(domain);
    x.setInputGate(Interval(1.));

    Function f("x", "x");
    CtcPicard tube_picard;

    CHECK(x.codomain() == Interval::ALL_REALS);
    tube_picard.contract(f, x);
    CHECK(x.codomain().is_superset(Interval(exp(domain))));
    CHECK(x.outputGate().is_superset(Interval(exp(domain.ub()))));
  }
}