#include <cstdio>
#include "catch_interval.hpp"
#include "codac_VIBesFigTube.h"
#include "codac_CtcEval.h"
#include "codac_CtcFunction.h"
#include "codac_CtcCN.h"
#include "vibes.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;

#define VIBES_DRAWING 0

TEST_CASE("CtcCN")
{
  SECTION("Test CtcCN") // x = a(t) + 2
  {
    Interval tdomain(0.,10.);
    Tube a(tdomain, 0.01, TFunction("t"));
    double t = 1.0;
    IntervalVector x({{t,t},Interval::ALL_REALS});

    CtcFunction ctc_f(Function("x[2]","a","(x[1]-2-a)"));
    CtcEval ctc_eval;

    ContractorNetwork cn;
    IntervalVectorVar box(2);
    Interval& at = cn.create_interm_var(Interval());
    cn.add(ctc_eval,{box[0],at,a});
    cn.add(ctc_f,{box,at});
    CtcCN ctc_cn(cn,box);

    ctc_cn.contract(x);
    CHECK(ApproxIntv(x[1]) == a(t)+ 2);
  }
}