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
    // Scalar functions

      Interval domain(0.,0.1);
      TubeSlice x(domain);
      x.setInputGate(Interval(1.));

      Function f("x", "x");
      CtcPicard tube_picard;

      CHECK(x.codomain() == Interval::ALL_REALS);
      tube_picard.contract(f, x);
      CHECK(x.codomain().is_superset(Interval(exp(domain))));
      CHECK(x.outputGate().is_superset(Interval(exp(domain.ub()))));

    // Vector functions

      TubeSlice x2(domain);
      x2.setInputGate(Interval(1.));

      vector<TubeSlice*> v_x_ptr;
      Variable var_x;
      v_x_ptr.push_back(&x2);
      Function f_vec(var_x, var_x);

      CHECK(x2.codomain() == Interval::ALL_REALS);
      tube_picard.contract(f_vec, v_x_ptr);
      CHECK(x2.codomain().is_superset(Interval(exp(domain))));
      CHECK(x2.outputGate().is_superset(Interval(exp(domain.ub()))));
  }
}