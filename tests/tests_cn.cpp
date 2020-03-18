#include "tests.h"
#include "tubex_ContractorNetwork.h"
#include "ibex_CtcFwdBwd.h"
#include "vibes.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

#define VIBES_DRAWING 0

TEST_CASE("CN simple")
{
  SECTION("Simple static case")
  {
    ibex::CtcFwdBwd ctc_plus(*new ibex::Function("a", "b", "c", "a+b-c")); // algebraic constraint a+b=c

    IntervalVector a(2, Interval(0,1)), b(2, Interval(-1,1)), c(2, Interval(1.5,2));

    ContractorNetwork cn;
    cn.add(ctc_plus, a[0], b[0], c[0]);
    cn.contract();

    CHECK(a[0] == Interval(0.5,1));
    CHECK(b[0] == Interval(0.5,1));
    CHECK(c[0] == Interval(1.5,2));
  }

  SECTION("Dependencies on vector components")
  {
    ibex::CtcFwdBwd ctc_plus(*new ibex::Function("a", "b", "c", "a+b-c")); // algebraic constraint a+b=c

    IntervalVector a(2, Interval(0,1)), b(2, Interval(-1,1)), c(2, Interval(1.5,2)), d(2, Interval(0.)), e(2);

    ContractorNetwork cn;
    // Contractors are added to the graph, 
    // the first one contracts the vector sets, the second one then contracts the components intervals,
    // and so it should trigger again the first one since components have changed.
    cn.add(ctc_plus, b, d, e);
    cn.add(ctc_plus, a[0], b[0], c[0]);
    cn.contract();

    CHECK(a[0] == Interval(0.5,1));
    CHECK(b[0] == Interval(0.5,1));
    CHECK(c[0] == Interval(1.5,2));
    CHECK(d[0] == Interval(0));
    CHECK(e[0] == Interval(0.5,1));
  }
}