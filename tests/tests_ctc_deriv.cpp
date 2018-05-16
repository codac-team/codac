#include "tests.h"
#include <cstdio>

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

TEST_CASE("CtcDeriv")
{
  /*SECTION("Test slice, envelope contraction")
  {
    TubeSlice x(Interval(-1.,3.), Interval(-10.,20.));
    x.setInputGate(Interval(-1.,2.));
    x.setOutputGate(Interval(-2.,0.));

    TubeSlice v(x.domain(), Interval(-1.,1.));

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);

    CHECK(contraction);
    CHECK(x.inputGate() == Interval(-1.,2.));
    CHECK(x.outputGate() == Interval(-2.,0.));
    CHECK(x.codomain() == Interval(-3.5,3.));
  }*/

  SECTION("Test slice, output gate contraction")
  {
    TubeSlice x(Interval(-1.,3.), Interval(-5.,3.));
    x.setInputGate(Interval(-1.,3.));
    x.setOutputGate(Interval(-5.,0.5));

    TubeSlice v(x.domain(), Interval(-1.));

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);

    CHECK(contraction);
    CHECK(x.inputGate() == Interval(-1.,3.));
    CHECK(x.outputGate() == Interval(-5.,-1.));
    CHECK(x.codomain() == Interval(-5.,3.));
  }

  /*SECTION("Test slice, complete contraction (degenerate tube)")
  {
    TubeSlice x(Interval(-1.,3.), Interval(-5.,3.));
    x.setInputGate(Interval(1.,3.));
    x.setOutputGate(Interval(-4.,-3.));

    TubeSlice v(x.domain(), Interval(-1.,1.));

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);

    CHECK(contraction);
    CHECK(x.inputGate() == Interval(1.));
    CHECK(x.outputGate() == Interval(-3.));
    CHECK(x.codomain() == Interval(-3.,1.));
    CHECK(x.interpol(-1., v) == Interval(1.));
    CHECK(x.interpol(1., v) == Interval(-1.));
    CHECK(x.interpol(3., v) == Interval(-3.));
    CHECK(x.interpol(0., v) == Interval(0.));
  }*/
}