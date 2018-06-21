#include "tests.h"
#include <cstdio>

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

#define VIBES_DRAWING 1

// todo: test cases of infinite upper/lower bound only on derivative or tube itself

TEST_CASE("CtcDeriv")
{
  SECTION("Test slice, envelope contraction")
  {
    TubeSlice x(Interval(-1.,3.), IntervalVector(1, Interval(-10.,20.)));
    x.setInputGate(IntervalVector(1, Interval(-1.,2.)));
    x.setOutputGate(IntervalVector(1, Interval(-2.,0.)));

    TubeSlice v(x.domain(), IntervalVector(1, Interval(-1.,1.)));

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached

    CHECK(contraction);
    CHECK(x.inputGate()[0] == Interval(-1.,2.));
    CHECK(x.outputGate()[0] == Interval(-2.,0.));
    CHECK(x.codomain()[0] == Interval(-3.5,3.));
  }

  SECTION("Test slice, output gate contraction")
  {
    TubeSlice x(Interval(-1.,3.), IntervalVector(1, Interval(-5.,3.)));
    x.setInputGate(IntervalVector(1, Interval(-1.,3.)));
    x.setOutputGate(IntervalVector(1, Interval(-5.,0.5)));

    TubeSlice v(x.domain(), IntervalVector(1, Interval(-1.)));

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached

    CHECK(contraction);
    CHECK(x.inputGate()[0] == Interval(-1.,3.));
    CHECK(x.outputGate()[0] == Interval(-5.,-1.));
    CHECK(x.codomain()[0] == Interval(-5.,3.));
  }

  SECTION("Test slice, complete contraction (degenerate tube)")
  {
    TubeSlice x(Interval(-1.,3.), IntervalVector(1, Interval(-5.,3.)));
    x.setInputGate(IntervalVector(1, Interval(1.,3.)));
    x.setOutputGate(IntervalVector(1, Interval(-4.,-3.)));

    TubeSlice v(x.domain(), IntervalVector(1, Interval(-1.,1.)));

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached

    CHECK(contraction);
    CHECK(x.inputGate()[0] == Interval(1.));
    CHECK(x.outputGate()[0] == Interval(-3.));
    CHECK(x.codomain()[0] == Interval(-3.,1.));
    //CHECK(x.interpol(-1., v) == Interval(1.));
    //CHECK(x.interpol(1., v) == Interval(-1.));
    //CHECK(x.interpol(3., v) == Interval(-3.));
    //CHECK(x.interpol(0., v) == Interval(0.));
  }

  SECTION("Test slice, empty case")
  {
    TubeSlice x(Interval(-1.,3.), IntervalVector(1, Interval(-10.,20.)));
    x.setInputGate(IntervalVector(1, Interval(-1.,2.)));
    x.setOutputGate(IntervalVector(1, Interval(-2.,0.)));

    TubeSlice v(x.domain(), IntervalVector(1, Interval::EMPTY_SET));

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached

    CHECK(contraction);
    CHECK(x.inputGate()[0].is_empty());
    CHECK(x.outputGate()[0].is_empty());
    CHECK(x.codomain()[0].is_empty());
  }

  SECTION("Test slice, empty case (bis)")
  {
    TubeSlice x(Interval(-1.,3.), IntervalVector(1, Interval(-10.,20.)));
    x.setInputGate(IntervalVector(1, Interval(-1.,2.)));
    x.setOutputGate(IntervalVector(1, Interval::EMPTY_SET));

    TubeSlice v(x.domain(), IntervalVector(1, Interval(-1.,1.)));

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);
    CHECK(contraction);
    CHECK(x.inputGate()[0].is_empty());
    CHECK(x.outputGate()[0].is_empty());
    CHECK(x.codomain()[0].is_empty());
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached
  }

  SECTION("Test slice, unbounded slice")
  {
    TubeSlice x(Interval(-1.,3.));
    TubeSlice v(x.domain(), IntervalVector(1, Interval(0.,1.)));

    CHECK(x.inputGate() == IntervalVector(1, Interval::ALL_REALS));
    CHECK(x.outputGate() == IntervalVector(1, Interval::ALL_REALS));
    CHECK(x.codomain() == IntervalVector(1, Interval::ALL_REALS));

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached

    CHECK(!contraction);
    CHECK(x.inputGate()[0] == Interval::ALL_REALS);
    CHECK(x.outputGate()[0] == Interval::ALL_REALS);
    CHECK(x.codomain()[0] == Interval::ALL_REALS);
  }

  SECTION("Test slice, unbounded derivative (1)")
  {
    TubeSlice x(Interval(-1.,3.));
    TubeSlice v(x.domain());

    CHECK(x.inputGate()[0] == Interval::ALL_REALS);
    CHECK(x.outputGate()[0] == Interval::ALL_REALS);
    CHECK(x.codomain()[0] == Interval::ALL_REALS);

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached

    CHECK(!contraction);
    CHECK(x.inputGate()[0] == Interval::ALL_REALS);
    CHECK(x.outputGate()[0] == Interval::ALL_REALS);
    CHECK(x.codomain()[0] == Interval::ALL_REALS);
  }

  SECTION("Test slice, unbounded derivative ([-oo,1])")
  {
    TubeSlice x(Interval(-1.,3.));
    x.setInputGate(IntervalVector(1, Interval(-1.,2.)));
    x.setOutputGate(IntervalVector(1, Interval(-2.,0.)));
    TubeSlice v(x.domain(), IntervalVector(1, Interval(NEG_INFINITY,1.)));

    CHECK(x.inputGate()[0] == Interval(-1.,2.));
    CHECK(x.outputGate()[0] == Interval(-2.,0.));
    CHECK(x.codomain()[0] == Interval::ALL_REALS);
    CHECK(v.codomain()[0] == Interval(NEG_INFINITY,1.));

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached

    CHECK(contraction);
    CHECK(x.inputGate()[0] == Interval(-1.,2.));
    CHECK(x.outputGate()[0] == Interval(-2.,0.));
    CHECK(x.codomain()[0] == Interval(-6.,6.));
    CHECK(v.codomain()[0] == Interval(NEG_INFINITY,1.));
  }

  SECTION("Test slice, unbounded derivative ([1,oo])")
  {
    TubeSlice x(Interval(-1.,3.));
    x.setInputGate(IntervalVector(1, Interval(-1.,2.)));
    x.setOutputGate(IntervalVector(1, Interval(-2.,0.)));
    TubeSlice v(x.domain(), IntervalVector(1, Interval(-1.,POS_INFINITY)));

    CHECK(x.inputGate()[0] == Interval(-1.,2.));
    CHECK(x.outputGate()[0] == Interval(-2.,0.));
    CHECK(x.codomain()[0] == Interval::ALL_REALS);
    CHECK(v.codomain()[0] == Interval(-1.,POS_INFINITY));

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached

    CHECK(contraction);
    CHECK(x.inputGate()[0] == Interval(-1.,2.));
    CHECK(x.outputGate()[0] == Interval(-2.,0.));
    CHECK(x.codomain()[0] == Interval(-5.,4.));
    CHECK(v.codomain()[0] == Interval(-1.,POS_INFINITY));
  }

  SECTION("Test fwd")
  {
    Tube tube(Interval(0., 6.), 1.0);
    Tube tubedot(tube);
    tubedot.set(Interval(-0.5, 1.));
    tube.set(Interval(-1.,1.), 0);
    CtcDeriv ctc;

    Tube tube_test_fwd(tube);
    ctc.contractFwd(tube_test_fwd, tubedot);
    CHECK_FALSE(ctc.contractFwd(tube_test_fwd, tubedot)); // fixed point already reached

    Tube tube_test_fwdbwd(tube);
    ctc.contract(tube_test_fwdbwd, tubedot);
    CHECK_FALSE(ctc.contract(tube_test_fwdbwd, tubedot)); // fixed point already reached

    CHECK(tube_test_fwd == tube_test_fwdbwd);
    CHECK(tube_test_fwd[0] == Interval(-1.,1.));
    CHECK(tube_test_fwd[1] == Interval(-1.5,2.));
    CHECK(tube_test_fwd[2] == Interval(-2.,3.));
    CHECK(tube_test_fwd[3] == Interval(-2.5,4.));
    CHECK(tube_test_fwd[4] == Interval(-3.,5.));
    CHECK(tube_test_fwd[5] == Interval(-3.5,6.));
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
    ctc.contractBwd(tube_test_bwd, tubedot);
    CHECK_FALSE(ctc.contractBwd(tube_test_bwd, tubedot)); // fixed point already reached

    Tube tube_test_fwdbwd(tube);
    ctc.contract(tube_test_fwdbwd, tubedot);
    CHECK_FALSE(ctc.contract(tube_test_fwdbwd, tubedot)); // fixed point already reached

    CHECK(tube_test_bwd == tube_test_fwdbwd);
    CHECK(tube_test_bwd[0] == Interval(-3.5,6.));
    CHECK(tube_test_bwd[1] == Interval(-3.,5.));
    CHECK(tube_test_bwd[2] == Interval(-2.5,4.));
    CHECK(tube_test_bwd[3] == Interval(-2.,3.));
    CHECK(tube_test_bwd[4] == Interval(-1.5,2.));
    CHECK(tube_test_bwd[5] == Interval(-1.,1.));
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

    CHECK(tube[0] == Interval(-1.,1.));
    CHECK(tube[1] == Interval(-2,1.5));
    CHECK(ApproxIntv(tube[2]) == Interval(-2.333333,2.));
    CHECK(ApproxIntv(tube[3]) == Interval(-2.,2.333333));
    CHECK(tube[4] == Interval(-1.5,2.));
    CHECK(tube[5] == Interval(-1.,1.));
    CHECK(ApproxIntv(tube.codomain()) == Interval(-2.333333,2.333333));
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

    CHECK(tube[0] == Interval(0.,2.));
    CHECK(tube[1] == Interval(1.5,3.5));
    CHECK(tube[2] == Interval(3.,4.));
    CHECK(tube[3] == Interval(3.5,4.));
    CHECK(tube[4] == Interval(3.5,4.25));
    CHECK(tube.codomain() == Interval(0.,4.25));

    if(VIBES_DRAWING) // drawing results
    {
      vibes::beginDrawing();
      VibesFigure_Tube fig_tube("tubint", &tube);
      fig_tube.setProperties(100, 100, 500, 500);
      fig_tube.setTubeDerivative(&tube, &tubedot);
      fig_tube.show(true);
      vibes::endDrawing();
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

    CHECK(tube.nbSlices() == 9);
    CHECK(tubedot.nbSlices() == 9);

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
      //fig_tube.setProperties(600, 100, 500, 500);
      fig_tube.setProperties(100, 100, 500, 500);
      fig_tube.setTubeDerivative(&tube, &tubedot);
      fig_tube.show(true);
      fig_tube.show(true);
    #endif

    CtcDeriv ctc;
    ctc.contract(*(tube.getSlice(0)), *(tubedot.getSlice(0)));
    ctc.contract(*(tube.getSlice(1)), *(tubedot.getSlice(1)));
    ctc.contract(*(tube.getSlice(2)), *(tubedot.getSlice(2)));
    ctc.contract(*(tube.getSlice(3)), *(tubedot.getSlice(3)));
    ctc.contract(*(tube.getSlice(4)), *(tubedot.getSlice(4)));
    ctc.contract(*(tube.getSlice(5)), *(tubedot.getSlice(5)));
    ctc.contract(*(tube.getSlice(6)), *(tubedot.getSlice(6)));
    ctc.contract(*(tube.getSlice(7)), *(tubedot.getSlice(7)));
    ctc.contract(*(tube.getSlice(8)), *(tubedot.getSlice(8)));
    CHECK_FALSE(ctc.contract(tube, tubedot)); // fixed point already reached

    //cout << "#0" << ctc.getPolygon(0, *tube.getSlice(8), *tubedot.getSlice(8)) << endl;

    CHECK(tube[0.] == Interval(2.,3.));
    CHECK(tube[0] == Interval(-1.,7.));
    CHECK(tube[4.] == Interval(3.,4.));
    CHECK(tube[1] == Interval(1.,4.));
    CHECK(tube[8.] == Interval(1.));
    CHECK(ApproxIntv(tube[2]) == Interval(0.,2.));
    CHECK(tube[12.] == Interval(1.));
    CHECK(tube[3] == Interval(1.,5.5));
    CHECK(tube[14.] == Interval(5.5));
    CHECK(tube[4] == Interval(5.5));
    CHECK(tube[16.] == Interval(5.5));
    CHECK(tube[5] == Interval(-1.,5.5));
    CHECK(tube[18.] == Interval(-1.));
    CHECK(tube[6] == Interval(-1.,5.5));
    CHECK(tube[20.] == Interval(5.5));
    CHECK(tube[7] == Interval(-1.,7.));
    CHECK(tube[23.] == Interval(5.5));
    CHECK(tube[8] == Interval::ALL_REALS);
    CHECK(tube[26.] == Interval::ALL_REALS);
    CHECK(tube.codomain() == Interval::ALL_REALS);

    #if VIBES_DRAWING // drawing results
      fig_tube.show(true);
      vibes::endDrawing();
    #endif
  }
}

TEST_CASE("CtcDeriv (interpol)")
{
  SECTION("From: Test slice, envelope contraction")
  {
    TubeSlice x(Interval(-1.,3.), IntervalVector(1, Interval(-10.,20.)));
    x.setInputGate(IntervalVector(1, Interval(-1.,2.)));
    x.setOutputGate(IntervalVector(1, Interval(-2.,0.)));
    TubeSlice v(x.domain(), IntervalVector(1, Interval(-1.,1.)));
    
    bool contraction;
    Interval t;
    IntervalVector y(1);
    CtcDeriv ctc;

    y[0] = Interval::ALL_REALS;
    t = Interval(0.5,2.);
    contraction = ctc.contract(x, v, t, y);
    CHECK_FALSE(ctc.contract(x, v, t, y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y[0] == Interval(-3.5,2.5));

    y[0] = Interval::ALL_REALS;
    t = Interval(-1.);
    contraction = ctc.contract(x, v, t, y);
    CHECK_FALSE(ctc.contract(x, v, t, y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y[0] == Interval(-1.,2.));

    y[0] = Interval::ALL_REALS;
    t = Interval(-0.5,0.);
    contraction = ctc.contract(x, v, t, y);
    CHECK_FALSE(ctc.contract(x, v, t, y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y[0] == Interval(-2.,3.));

    y[0] = Interval::ALL_REALS;
    t = Interval(1.5);
    contraction = ctc.contract(x, v, t, y);
    CHECK_FALSE(ctc.contract(x, v, t, y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y[0] == Interval(-3.5,1.5));

    y[0] = Interval::ALL_REALS;
    t = Interval(2.5);
    contraction = ctc.contract(x, v, t, y);
    CHECK_FALSE(ctc.contract(x, v, t, y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y[0] == Interval(-2.5,0.5));

    y[0] = Interval::ALL_REALS;
    t = Interval(3.);
    contraction = ctc.contract(x, v, t, y);
    CHECK_FALSE(ctc.contract(x, v, t, y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y[0] == Interval(-2.,0.));

    y[0] = Interval::ALL_REALS;
    t = Interval(-1.,3.);
    contraction = ctc.contract(x, v, t, y);
    CHECK_FALSE(ctc.contract(x, v, t, y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y[0] == Interval(-3.5,3.));

    y[0] = Interval::ALL_REALS;
    t = Interval(-10.,3.);
    CHECK(ctc.contract(x, v, t, y)); // fixed point already reached
    CHECK_FALSE(ctc.contract(x, v, t, y)); // fixed point already reached
    CHECK(t == Interval(-1.,3.));
    CHECK(y[0] == Interval(-3.5,3.));
  }

  SECTION("From: Test slice, output gate contraction")
  {
    TubeSlice x(Interval(-1.,3.), IntervalVector(1, Interval(-5.,3.)));
    x.setInputGate(IntervalVector(1, Interval(-1.,3.)));
    x.setOutputGate(IntervalVector(1, Interval(-5.,0.5)));
    TubeSlice v(x.domain(), IntervalVector(1, Interval(-1.)));

    bool contraction;
    Interval t;
    IntervalVector y(1);
    CtcDeriv ctc;

    y[0] = Interval::ALL_REALS;
    t = Interval(2.);
    contraction = ctc.contract(x, v, t, y);
    CHECK_FALSE(ctc.contract(x, v, t, y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y[0] == Interval(-4.,0.));

    y[0] = Interval::ALL_REALS;
    t = Interval(-1.,3.);
    contraction = ctc.contract(x, v, t, y);
    CHECK_FALSE(ctc.contract(x, v, t, y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y[0] == Interval(-5.,3.));
  }

  SECTION("From: Test slice, complete contraction (degenerate tube)")
  {
    TubeSlice x(Interval(-1.,3.), IntervalVector(1, Interval(-5.,3.)));
    x.setInputGate(IntervalVector(1, Interval(1.,3.)));
    x.setOutputGate(IntervalVector(1, Interval(-4.,-3.)));
    TubeSlice v(x.domain(), IntervalVector(1, Interval(-1.,1.)));

    bool contraction;
    Interval t;
    IntervalVector y(1);
    CtcDeriv ctc;

    y[0] = Interval::ALL_REALS;
    t = Interval(0.5,2.);
    contraction = ctc.contract(x, v, t, y);
    CHECK_FALSE(ctc.contract(x, v, t, y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y[0] == Interval(-2.,-0.5));

    y[0] = Interval(-1.,300.);
    t = Interval(1.);
    contraction = ctc.contract(x, v, t, y);
    CHECK_FALSE(ctc.contract(x, v, t, y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y[0] == Interval(-1.));

    y[0] = Interval(1.,300.);
    t = Interval(1.);
    contraction = ctc.contract(x, v, t, y);
    CHECK(contraction);
    CHECK(y[0] == Interval::EMPTY_SET);
    CHECK(t == Interval::EMPTY_SET);
    CHECK_FALSE(ctc.contract(x, v, t, y)); // fixed point already reached
    CHECK_FALSE(ctc.contract(x, v, t, y)); // fixed point already reached
    CHECK_FALSE(ctc.contract(x, v, t, y)); // fixed point already reached

    y[0] = Interval::ALL_REALS;
    t = 1.;
    contraction = ctc.contract(x, v, t, y);
    CHECK_FALSE(ctc.contract(x, v, t, y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y[0] == Interval(-1.));

    y[0] = Interval(-20.,1.);
    t = Interval(-1.,3.);
    contraction = ctc.contract(x, v, t, y);
    CHECK_FALSE(ctc.contract(x, v, t, y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y[0] == Interval(-3.,1.));
  }

  SECTION("Interpolation over a tube (example from tubint paper)")
  {
    TubeVector tube(Interval(0., 5.), 1.0, 1);
    TubeVector tubedot(tube);

    tube.set(IntervalVector(1, Interval(0.)), 0.);
    tube.set(IntervalVector(1, Interval(4.)), 5.);

    tubedot.set(IntervalVector(1, Interval(1.,2.)), 0);
    tubedot.set(IntervalVector(1, Interval(0.5,1.5)), 1);
    tubedot.set(IntervalVector(1, Interval(0.,0.5)), 2);
    tubedot.set(IntervalVector(1, Interval(0.)), 3);
    tubedot.set(IntervalVector(1, Interval(-0.5,0.5)), 4);

    Interval t;
    IntervalVector y(1);
    bool contraction;
    CtcDeriv ctc;

    // Tube evaluation before tube contraction
    y[0] = Interval::ALL_REALS;
    t = Interval::ALL_REALS;
    contraction = ctc.contract(tube, tubedot, t, y);
    CHECK(contraction);
    CHECK(tube.codomain()[0] == Interval::ALL_REALS);
    CHECK(y == tube.codomain());
    CHECK(t == Interval(0.,5.));

    ctc.contract(tube, tubedot); // contraction of the tube
    CHECK_FALSE(ctc.contract(tube, tubedot)); // fixed point already reached
    CHECK(tube.codomain()[0] == Interval(0.,4.25));
    CHECK(tube[0][0] == Interval(0.,2.));
    CHECK(tube[1][0] == Interval(1.5,3.5));
    CHECK(tube[2][0] == Interval(3.,4.));
    CHECK(tube[3][0] == Interval(3.5,4.));
    CHECK(tube[4][0] == Interval(3.5,4.25));

    if(VIBES_DRAWING) // drawing results
    {
      vibes::beginDrawing();
      VibesFigure_Tube fig_tube("tubint", &tube);
      fig_tube.setProperties(100, 100, 500, 500);
      fig_tube.setTubeDerivative(&tube, &tubedot);
      fig_tube.show(true);
      vibes::endDrawing();
    }

    // Tube evaluation after tube contraction
    y[0] = Interval::ALL_REALS;
    t = Interval(0.,5.);
    contraction = ctc.contract(tube, tubedot, t, y);
    CHECK(contraction);
    CHECK(y[0] == Interval(0.,4.25));
    contraction = ctc.contract(tube, tubedot, t, y);
    CHECK_FALSE(contraction);
    CHECK(y[0] == Interval(0.,4.25));

    y[0] = Interval(3.,10.);
    t = Interval(1.,3.);
    contraction = ctc.contract(tube, tubedot, t, y);
    CHECK(contraction);
    CHECK(y[0] == Interval(3.,4.));

    y[0] = Interval::EMPTY_SET;
    t = Interval(1.,3.);
    contraction = ctc.contract(tube, tubedot, t, y);
    CHECK(contraction);
    CHECK(t == Interval::EMPTY_SET);
    CHECK(y[0] == Interval::EMPTY_SET);

    y[0] = Interval::ALL_REALS;
    t = Interval(2.2,2.7);
    contraction = ctc.contract(tube, tubedot, t, y);
    CHECK(contraction);
    CHECK(y[0] == Interval(3.1,3.85));

    y[0] = Interval::ALL_REALS;
    t = Interval(4.5);
    contraction = ctc.contract(tube, tubedot, t, y);
    CHECK(contraction);
    CHECK(y[0] == Interval(3.75,4.25));

    y[0] = Interval::ALL_REALS;
    t = Interval(4.1,4.9);
    contraction = ctc.contract(tube, tubedot, t, y);
    CHECK(contraction);
    CHECK(y[0] == Interval(3.55,4.25));

    y[0] = Interval::ALL_REALS;
    t = Interval(-10.,10.);
    CHECK(ctc.contract(tube, tubedot, t, y));
    CHECK(t == tube.domain());
    CHECK(y == tube.codomain());

    y[0] = Interval::ALL_REALS;
    t = Interval(-10.,10.);
    CHECK(ctc.contract(tube, tubedot, t, y));
    CHECK(t == tube.domain());
    CHECK(y == tube.codomain());

    //if(VIBES_DRAWING) vibes::beginDrawing();
    
    /*IntervalVector box(2);

    box[0] = Interval(0.25,0.75);
    box[1] = Interval(0.5,1.5);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "blue"));
    CHECK_FALSE(ctc.contract(tube, tubedot, box[0], box[1]));
    CHECK(box[0] == Interval(0.25,0.75));
    CHECK(box[1] == Interval(0.5,1.5));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "red"));

    box[0] = Interval(-1.,6.);
    box[1] = Interval(-0.5,4.5);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "blue"));
    CHECK(ctc.contract(tube, tubedot, box[0], box[1]));
    CHECK(box[0] == Interval(0.,5.));
    CHECK(box[1] == Interval(0.,4.25));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "red"));

    box[0] = Interval(0.4,0.8);
    box[1] = Interval(0.3,0.6);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "blue"));
    CHECK(ctc.contract(tube, tubedot, box[0], box[1]));
    CHECK(box[0] == Interval(0.4,0.55));
    CHECK(box[1] == Interval(0.4,0.6));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "red"));

    box[0] = Interval(1.2,1.5);
    box[1] = Interval(1.,4.);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "blue"));
    CHECK(ctc.contract(tube, tubedot, box[0], box[1]));
    CHECK(box[0] == Interval(1.2,1.5));
    CHECK(ApproxIntv(box[1]) == Interval(1.8,2.75));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "red"));

    box[0] = Interval(1.8,3.1);
    box[1] = Interval(2.5,4.2);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "blue"));
    CHECK(ctc.contract(tube, tubedot, box[0], box[1]));
    CHECK(box[0] == Interval(1.8,3.1));
    CHECK(ApproxIntv(box[1]) == Interval(2.7,4.));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "red"));

    box[0] = Interval(0.2);
    box[1] = Interval(0.1,0.8);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "blue"));
    CHECK(ctc.contract(tube, tubedot, box[0], box[1]));
    CHECK(box[0] == Interval(0.2));
    CHECK(box[1] == Interval(0.2,0.4));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "red"));

    box[0] = Interval(0.8);
    box[1] = Interval(0.8,1.8);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "blue"));
    CHECK(ctc.contract(tube, tubedot, box[0], box[1]));
    CHECK(box[0] == Interval(0.8));
    CHECK(box[1] == Interval(1.1,1.6));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "red"));

    box[0] = Interval(0.5,2.5);
    box[1] = Interval(2.2);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "blue"));
    CHECK(ctc.contract(tube, tubedot, box[0], box[1]));
    CHECK(ApproxIntv(box[0]) == Interval(1.1333333,1.4666666));
    CHECK(box[1] == Interval(2.2));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "red"));

    box[0] = Interval(4.1,4.9);
    box[1] = Interval(4.1,4.5);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "blue"));
    CHECK(ctc.contract(tube, tubedot, box[0], box[1]));
    CHECK(ApproxIntv(box[0]) == Interval(4.2,4.8));
    CHECK(box[1] == Interval(4.1,4.25));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "red"));

    box[0] = Interval(4.2,4.7);
    box[1] = Interval(3.55,3.8);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "blue"));
    CHECK(ctc.contract(tube, tubedot, box[0], box[1]));
    CHECK(ApproxIntv(box[0]) == Interval(4.2,4.6));
    CHECK(box[1] == Interval(4.1,4.25));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "red"));

    if(VIBES_DRAWING) vibes::endDrawing();
  }

  SECTION("Interpolation over a tube (other example)")
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
    tubedot.set(Interval(-1.5,4.), 0);
    tubedot.set(Interval(-0.75,-0.5), 1);
    tubedot.set(Interval(-1./3.,1.), 2);
    tubedot.set(Interval(4.5/2.), 3);
    tubedot.set(Interval(0.), 4);
    tubedot.set(Interval(NEG_INFINITY,-6.5/2.), 5);
    tubedot.set(Interval(6.5/2.,POS_INFINITY), 6);
    tubedot.set(Interval::ALL_REALS, 7);
    tubedot.set(Interval::ALL_REALS, 8);

    CtcDeriv ctc;
    ctc.contract(tube, tubedot);

    #if VIBES_DRAWING // drawing results
      vibes::beginDrawing();
      VibesFigure_Tube fig_tube("test", &tube);
      //fig_tube.setProperties(600, 100, 500, 500);
      fig_tube.setProperties(100, 100, 1000, 500);
      fig_tube.setTubeDerivative(&tube, &tubedot);
      fig_tube.show(true);
    #endif

    IntervalVector box(2);

    box[0] = Interval(8.1,11.8);
    box[1] = Interval(1.9,3.);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "blue"));
    CHECK(ctc.contract(tube, tubedot, box[0], box[1]));
    CHECK(box[0] == Interval(6.5,9.3));
    CHECK(box[1] == Interval(1.9,2.125));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "red"));

    box[0] = Interval(9.,11.8);
    box[1] = Interval(0.2,0.5);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "blue"));
    CHECK(ctc.contract(tube, tubedot, box[0], box[1]));
    CHECK(box[0] == Interval(6.5,9.3));
    CHECK(box[1] == Interval(1.9,2.125));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "red"));

    #if VIBES_DRAWING // drawing results
      vibes::endDrawing();
    #endif*/
  }
}