#include "tests.h"
#include <cstdio>
#include "tubex_CtcDeriv.h"
#include "tubex_VibesFigure_Tube.h"
#include "vibes.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

#define VIBES_DRAWING 0

// todo: test cases of infinite upper/lower bound only on derivative or tube itself

TEST_CASE("CtcDeriv")
{
  SECTION("Test slice, envelope contraction")
  {
    Slice x(Interval(-1.,3.), Interval(-10.,20.));
    x.set_input_gate(Interval(-1.,2.));
    x.set_output_gate(Interval(-2.,0.));

    Slice v(x.domain(), Interval(-1.,1.));

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached

    CHECK(contraction);
    CHECK(x.input_gate() == Interval(-1.,2.));
    CHECK(x.output_gate() == Interval(-2.,0.));
    CHECK(x.codomain() == Interval(-3.5,3.));
  }

  SECTION("Test slice, output gate contraction")
  {
    Slice x(Interval(-1.,3.), Interval(-5.,3.));
    x.set_input_gate(Interval(-1.,3.));
    x.set_output_gate(Interval(-5.,0.5));

    Slice v(x.domain(), Interval(-1.));

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached

    CHECK(contraction);
    CHECK(x.input_gate() == Interval(-1.,3.));
    CHECK(x.output_gate() == Interval(-5.,-1.));
    CHECK(x.codomain() == Interval(-5.,3.));
  }

  SECTION("Test slice, complete contraction (degenerate tube)")
  {
    Slice x(Interval(-1.,3.), Interval(-5.,3.));
    x.set_input_gate(Interval(1.,3.));
    x.set_output_gate(Interval(-4.,-3.));

    Slice v(x.domain(), Interval(-1.,1.));

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached

    CHECK(contraction);
    CHECK(x.input_gate() == Interval(1.));
    CHECK(x.output_gate() == Interval(-3.));
    CHECK(x.codomain() == Interval(-3.,1.));
    CHECK(x.interpol(-1., v) == Interval(1.));
    CHECK(x.interpol(1., v) == Interval(-1.));
    CHECK(x.interpol(3., v) == Interval(-3.));
    CHECK(x.interpol(0., v) == Interval(0.));
  }

  SECTION("Test slice, empty case")
  {
    Slice x(Interval(-1.,3.), Interval(-10.,20.));
    x.set_input_gate(Interval(-1.,2.));
    x.set_output_gate(Interval(-2.,0.));

    Slice v(x.domain(), Interval::EMPTY_SET);

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached

    CHECK(contraction);
    CHECK(x.input_gate().is_empty());
    CHECK(x.output_gate().is_empty());
    CHECK(x.codomain().is_empty());
  }

  SECTION("Test slice, empty case (bis)")
  {
    Slice x(Interval(-1.,3.), Interval(-10.,20.));
    x.set_input_gate(Interval(-1.,2.));
    x.set_output_gate(Interval::EMPTY_SET);

    Slice v(x.domain(), Interval(-1.,1.));

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);
    CHECK(contraction);
    CHECK(x.input_gate().is_empty());
    CHECK(x.output_gate().is_empty());
    CHECK(x.codomain().is_empty());
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached
  }/*
}

  SECTION("Test slice, unbounded slice")
  {
    Slice x(Interval(-1.,3.));
    Slice v(x.domain(), IntervalVector(1, Interval(0.,1.)));

    CHECK(x.input_gate() == IntervalVector(1, Interval::ALL_REALS));
    CHECK(x.output_gate() == IntervalVector(1, Interval::ALL_REALS));
    CHECK(x.codomain() == IntervalVector(1, Interval::ALL_REALS));

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached

    CHECK(!contraction);
    CHECK(x.input_gate()[0] == Interval::ALL_REALS);
    CHECK(x.output_gate()[0] == Interval::ALL_REALS);
    CHECK(x.codomain()[0] == Interval::ALL_REALS);
  }

  SECTION("Test slice, unbounded derivative (1)")
  {
    Slice x(Interval(-1.,3.));
    Slice v(x.domain());

    CHECK(x.input_gate()[0] == Interval::ALL_REALS);
    CHECK(x.output_gate()[0] == Interval::ALL_REALS);
    CHECK(x.codomain()[0] == Interval::ALL_REALS);

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached

    CHECK(!contraction);
    CHECK(x.input_gate()[0] == Interval::ALL_REALS);
    CHECK(x.output_gate()[0] == Interval::ALL_REALS);
    CHECK(x.codomain()[0] == Interval::ALL_REALS);
  }

  SECTION("Test slice, unbounded derivative ([-oo,1])")
  {
    Slice x(Interval(-1.,3.));
    x.set_input_gate(IntervalVector(1, Interval(-1.,2.)));
    x.set_output_gate(IntervalVector(1, Interval(-2.,0.)));
    Slice v(x.domain(), IntervalVector(1, Interval(NEG_INFINITY,1.)));

    CHECK(x.input_gate()[0] == Interval(-1.,2.));
    CHECK(x.output_gate()[0] == Interval(-2.,0.));
    CHECK(x.codomain()[0] == Interval::ALL_REALS);
    CHECK(v.codomain()[0] == Interval(NEG_INFINITY,1.));

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached

    CHECK(contraction);
    CHECK(x.input_gate()[0] == Interval(-1.,2.));
    CHECK(x.output_gate()[0] == Interval(-2.,0.));
    CHECK(x.codomain()[0] == Interval(-6.,6.));
    CHECK(v.codomain()[0] == Interval(NEG_INFINITY,1.));
  }

  SECTION("Test slice, unbounded derivative ([1,oo])")
  {
    Slice x(Interval(-1.,3.));
    x.set_input_gate(IntervalVector(1, Interval(-1.,2.)));
    x.set_output_gate(IntervalVector(1, Interval(-2.,0.)));
    Slice v(x.domain(), IntervalVector(1, Interval(-1.,POS_INFINITY)));

    CHECK(x.input_gate()[0] == Interval(-1.,2.));
    CHECK(x.output_gate()[0] == Interval(-2.,0.));
    CHECK(x.codomain()[0] == Interval::ALL_REALS);
    CHECK(v.codomain()[0] == Interval(-1.,POS_INFINITY));

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached

    CHECK(contraction);
    CHECK(x.input_gate()[0] == Interval(-1.,2.));
    CHECK(x.output_gate()[0] == Interval(-2.,0.));
    CHECK(x.codomain()[0] == Interval(-5.,4.));
    CHECK(v.codomain()[0] == Interval(-1.,POS_INFINITY));
  }*/

  SECTION("Test fwd")
  {
    Tube tube(Interval(0., 6.), 1.0);
    Tube tubedot(tube);
    tubedot.set(Interval(-0.5, 1.));
    tube.set(Interval(-1.,1.), 0);
    CtcDeriv ctc;

    Tube tube_test_fwd(tube);
    ctc.contract(tube_test_fwd, tubedot, FORWARD);
    CHECK_FALSE(ctc.contract(tube_test_fwd, tubedot, FORWARD)); // fixed point already reached

    Tube tube_test_fwdbwd(tube);
    ctc.contract(tube_test_fwdbwd, tubedot);
    CHECK_FALSE(ctc.contract(tube_test_fwdbwd, tubedot)); // fixed point already reached

    CHECK(tube_test_fwd == tube_test_fwdbwd);
    CHECK(tube_test_fwd(0) == Interval(-1.,1.));
    CHECK(tube_test_fwd(1) == Interval(-1.5,2.));
    CHECK(tube_test_fwd(2) == Interval(-2.,3.));
    CHECK(tube_test_fwd(3) == Interval(-2.5,4.));
    CHECK(tube_test_fwd(4) == Interval(-3.,5.));
    CHECK(tube_test_fwd(5) == Interval(-3.5,6.));
    CHECK(tube_test_fwd.codomain() == Interval(-3.5,6.));
  }

  SECTION("Test bwd")
  {
    Tube tube(Interval(0., 6.), 1.0);
    Tube tubedot(tube);
    tubedot.set(Interval(-1., 0.5));
    tube.set(Interval(-1.,1.), 5);

    CtcDeriv ctc;

    Tube tube_test_bwd(tube);
    ctc.contract(tube_test_bwd, tubedot, BACKWARD);
    CHECK_FALSE(ctc.contract(tube_test_bwd, tubedot, BACKWARD)); // fixed point already reached

    Tube tube_test_fwdbwd(tube);
    ctc.contract(tube_test_fwdbwd, tubedot);
    CHECK_FALSE(ctc.contract(tube_test_fwdbwd, tubedot)); // fixed point already reached

    CHECK(tube_test_bwd == tube_test_fwdbwd);
    CHECK(tube_test_bwd(0) == Interval(-3.5,6.));
    CHECK(tube_test_bwd(1) == Interval(-3.,5.));
    CHECK(tube_test_bwd(2) == Interval(-2.5,4.));
    CHECK(tube_test_bwd(3) == Interval(-2.,3.));
    CHECK(tube_test_bwd(4) == Interval(-1.5,2.));
    CHECK(tube_test_bwd(5) == Interval(-1.,1.));
    CHECK(tube_test_fwdbwd.codomain() == Interval(-3.5,6.));
    CHECK(tube_test_bwd.codomain() == Interval(-3.5,6.));
  }

  SECTION("Test fwd/bwd")
  {
    Tube tube(Interval(0., 6.), 1.0);
    Tube tubedot(tube);
    tubedot.set(Interval(-1.,0.5));
    tube.set(Interval(-1.,1.), 5);
    tube.set(Interval(-1.,1.), 0);

    CtcDeriv ctc;
    ctc.contract(tube, tubedot);
    CHECK_FALSE(ctc.contract(tube, tubedot)); // fixed point already reached

    CHECK(tube(0) == Interval(-1.,1.));
    CHECK(tube(1) == Interval(-2,1.5));
    CHECK(ApproxIntv(tube(2)) == Interval(-7./3.,2.));
    CHECK(ApproxIntv(tube(3)) == Interval(-2.,7./3.));
    CHECK(tube(4) == Interval(-1.5,2.));
    CHECK(tube(5) == Interval(-1.,1.));
    CHECK(ApproxIntv(tube.codomain()) == Interval(-7./3.,7./3.));
  }

  SECTION("Test fwd/bwd (example from tubint paper)")
  {
    Tube tube(Interval(0., 5.), 1.0);
    Tube tubedot(tube);

    tube.set(Interval(0.), 0.);
    tube.set(Interval(4.), 5.);

    tubedot.set(Interval(1.,2.), 0);
    tubedot.set(Interval(0.5,1.5), 1);
    tubedot.set(Interval(0.,0.5), 2);
    tubedot.set(Interval(0.), 3);
    tubedot.set(Interval(-0.5,0.5), 4);

    CtcDeriv ctc;
    ctc.contract(tube, tubedot);
    CHECK_FALSE(ctc.contract(tube, tubedot)); // fixed point already reached

    CHECK(tube(0) == Interval(0.,2.));
    CHECK(tube(1) == Interval(1.5,3.5));
    CHECK(tube(2) == Interval(3.,4.));
    CHECK(tube(3) == Interval(3.5,4.));
    CHECK(tube(4) == Interval(3.5,4.25));
    CHECK(tube.codomain() == Interval(0.,4.25));

    if(VIBES_DRAWING) // drawing results
    {
      //vibes::beginDrawing();
      //VibesFigure_Tube fig_tube("tubint", &tube);
      //fig_tube.set_properties(100, 100, 500, 500);
      //fig_tube.set_tube_derivative(&tube, &tubedot);
      //fig_tube.show(true);
      //vibes::endDrawing();
    }
  }

  SECTION("Test fwd/bwd (other example)")
  {
    Tube tube(Interval(0., 26.), Interval(-1.,7.));

    tube.set(Interval(2.,3.), 0.);
    tube.set(Interval(3.,4.), 4.);
    tube.set(Interval(1.), 8.);
    tube.set(Interval(1.), 12.);
    tube.set(Interval(5.5), 14.);
    tube.set(Interval(5.5), 16.);
    tube.set(Interval(-1.), 18.);
    tube.set(Interval(5.5), 20.);
    tube.set(Interval(5.5), 23.);
    tube.set(Interval::ALL_REALS, 8);
    tube.set(Interval(5.5), 23.); // setting gate again

    Tube tubedot(tube, Interval(-1.,1.));

    CHECK(tube.nb_slices() == 9);
    CHECK(tubedot.nb_slices() == 9);

    tubedot.set(Interval(-1.5,4.), 0);
    tubedot.set(Interval(-0.75,-0.5), 1);
    tubedot.set(Interval(-1./3.,1.), 2);
    tubedot.set(Interval(4.5/2.), 3);
    tubedot.set(Interval(0.), 4);
    tubedot.set(Interval(NEG_INFINITY,-6.5/2.), 5);
    tubedot.set(Interval(6.5/2.,POS_INFINITY), 6);
    tubedot.set(Interval::ALL_REALS, 7);
    tubedot.set(Interval::ALL_REALS, 8);

    #if VIBES_DRAWING // drawing results
      vibes::beginDrawing();
      VibesFigure_Tube fig_tube("test", &tube);
      fig_tube.set_properties(100, 100, 500, 500);
      fig_tube.set_tube_derivative(&tube, &tubedot);
      fig_tube.show(true);
      fig_tube.show(true);
    #endif

    CtcDeriv ctc;
    ctc.contract(*(tube.get_slice(0)), *(tubedot.get_slice(0)));
    ctc.contract(*(tube.get_slice(1)), *(tubedot.get_slice(1)));
    ctc.contract(*(tube.get_slice(2)), *(tubedot.get_slice(2)));
    ctc.contract(*(tube.get_slice(3)), *(tubedot.get_slice(3)));
    ctc.contract(*(tube.get_slice(4)), *(tubedot.get_slice(4)));
    ctc.contract(*(tube.get_slice(5)), *(tubedot.get_slice(5)));
    ctc.contract(*(tube.get_slice(6)), *(tubedot.get_slice(6)));
    ctc.contract(*(tube.get_slice(7)), *(tubedot.get_slice(7)));
    ctc.contract(*(tube.get_slice(8)), *(tubedot.get_slice(8)));
    CHECK_FALSE(ctc.contract(tube, tubedot)); // fixed point already reached

    CHECK(tube(0.) == Interval(2.,3.));
    CHECK(tube(0) == Interval(-1.,7.));
    CHECK(tube(4.) == Interval(3.,4.));
    CHECK(tube(1) == Interval(1.,4.));
    CHECK(tube(8.) == Interval(1.));
    CHECK(ApproxIntv(tube(2)) == Interval(0.,2.));
    CHECK(tube(12.) == Interval(1.));
    CHECK(tube(3) == Interval(1.,5.5));
    CHECK(tube(14.) == Interval(5.5));
    CHECK(tube(4) == Interval(5.5));
    CHECK(tube(16.) == Interval(5.5));
    CHECK(tube(5) == Interval(-1.,5.5));
    CHECK(tube(18.) == Interval(-1.));
    CHECK(tube(6) == Interval(-1.,5.5));
    CHECK(tube(20.) == Interval(5.5));
    CHECK(tube(7) == Interval(-1.,7.));
    CHECK(tube(23.) == Interval(5.5));
    // todo: CHECK(tube(8) == Interval::ALL_REALS);
    // todo: CHECK(tube(26.) == Interval::ALL_REALS);
    // todo: CHECK(tube.codomain() == Interval::ALL_REALS);

    #if VIBES_DRAWING // drawing results
      fig_tube.show(true);
      vibes::endDrawing();
    #endif
  }

  SECTION("From: Test slice, output gate contraction")
  {
    Slice x(Interval(-1.,3.), Interval(-5.,3.));
    x.set_input_gate(Interval(-1.,3.));
    x.set_output_gate(Interval(-5.,0.5));
    Slice v(x.domain(), Interval(-1.));

    bool contraction;
    CtcDeriv ctc;

    contraction = ctc.contract(x, v);
    CHECK_FALSE(ctc.contract(x, v)); // fixed point already reached
    CHECK(contraction);
    CHECK(x.input_gate() == Interval(-1.,3.));
    CHECK(x.output_gate() == Interval(-5.,-1.));
    CHECK(x.interpol(Interval(2.), v) == Interval(-4.,0.));
    CHECK(x.interpol(Interval(-1.,3.), v) == Interval(-5.,3.));
  }

  SECTION("From: Test slice, complete contraction (degenerate tube)")
  {
    Slice x(Interval(-1.,3.), Interval(-5.,3.));
    x.set_input_gate(Interval(1.,3.));
    x.set_output_gate(Interval(-4.,-3.));
    Slice v(x.domain(), Interval(-1.,1.));

    bool contraction;
    CtcDeriv ctc;

    contraction = ctc.contract(x, v);
    CHECK_FALSE(ctc.contract(x, v)); // fixed point already reached
    CHECK(contraction);

    CHECK(x.interpol(Interval(0.5,2.), v) == Interval(-2.,-0.5));
    CHECK(x.interpol(Interval(1.), v) == Interval(-1.));
    CHECK(x.interpol(Interval(-1.,3.), v) == Interval(-3.,1.));
  }
}