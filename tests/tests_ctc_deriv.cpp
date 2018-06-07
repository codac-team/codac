#include "tests.h"
#include <cstdio>

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

// todo: test cases of infinite upper/lower bound only on derivative or tube itself

TEST_CASE("CtcDeriv")
{
  SECTION("Test slice, envelope contraction")
  {
    TubeSlice x(Interval(-1.,3.), Interval(-10.,20.));
    x.setInputGate(Interval(-1.,2.));
    x.setOutputGate(Interval(-2.,0.));

    TubeSlice v(x.domain(), Interval(-1.,1.));

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached

    CHECK(contraction);
    CHECK(x.inputGate() == Interval(-1.,2.));
    CHECK(x.outputGate() == Interval(-2.,0.));
    CHECK(x.codomain() == Interval(-3.5,3.));
  }

  SECTION("Test slice, output gate contraction")
  {
    TubeSlice x(Interval(-1.,3.), Interval(-5.,3.));
    x.setInputGate(Interval(-1.,3.));
    x.setOutputGate(Interval(-5.,0.5));

    TubeSlice v(x.domain(), Interval(-1.));

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached

    CHECK(contraction);
    CHECK(x.inputGate() == Interval(-1.,3.));
    CHECK(x.outputGate() == Interval(-5.,-1.));
    CHECK(x.codomain() == Interval(-5.,3.));
  }

  SECTION("Test slice, complete contraction (degenerate tube)")
  {
    TubeSlice x(Interval(-1.,3.), Interval(-5.,3.));
    x.setInputGate(Interval(1.,3.));
    x.setOutputGate(Interval(-4.,-3.));

    TubeSlice v(x.domain(), Interval(-1.,1.));

    /*CtcDeriv ctc;
    cout << "AAAA1" << endl;
    bool contraction = ctc.contract(x,v);
    cout << "AAAA2" << endl;
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached

    CHECK(contraction);
    CHECK(x.inputGate() == Interval(1.));
    CHECK(x.outputGate() == Interval(-3.));
    CHECK(x.codomain() == Interval(-3.,1.));
    CHECK(x.interpol(-1., v) == Interval(1.));
    CHECK(x.interpol(1., v) == Interval(-1.));
    CHECK(x.interpol(3., v) == Interval(-3.));
    CHECK(x.interpol(0., v) == Interval(0.));*/
  }
/*
  SECTION("Test slice, empty case")
  {
    TubeSlice x(Interval(-1.,3.), Interval(-10.,20.));
    x.setInputGate(Interval(-1.,2.));
    x.setOutputGate(Interval(-2.,0.));

    TubeSlice v(x.domain(), Interval::EMPTY_SET);

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached

    CHECK(contraction);
    CHECK(x.inputGate().is_empty());
    CHECK(x.outputGate().is_empty());
    CHECK(x.codomain().is_empty());
  }

  SECTION("Test slice, all reals case")
  {
    TubeSlice x(Interval(-1.,3.));
    TubeSlice v(x.domain());

    CHECK(x.inputGate() == Interval::ALL_REALS);
    CHECK(x.outputGate() == Interval::ALL_REALS);
    CHECK(x.codomain() == Interval::ALL_REALS);

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached

    CHECK(!contraction);
    CHECK(x.inputGate() == Interval::ALL_REALS);
    CHECK(x.outputGate() == Interval::ALL_REALS);
    CHECK(x.codomain() == Interval::ALL_REALS);
  }

  SECTION("Test slice, empty case (bis)")
  {
    TubeSlice x(Interval(-1.,3.), Interval(-10.,20.));
    x.setInputGate(Interval(-1.,2.));
    x.setOutputGate(Interval::EMPTY_SET);

    TubeSlice v(x.domain(), Interval(-1.,1.));

    CtcDeriv ctc;
    bool contraction = ctc.contract(x,v);
    CHECK_FALSE(ctc.contract(x,v)); // fixed point already reached

    CHECK(contraction);
    CHECK(x.inputGate().is_empty());
    CHECK(x.outputGate().is_empty());
    CHECK(x.codomain().is_empty());
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

    if(true) // drawing results
    {
      vibes::beginDrawing();
      VibesFigure_Tube fig_tube("tubint", &tube);
      fig_tube.setProperties(100, 100, 500, 500);
      fig_tube.setTubeDerivative(&tube, &tubedot);
      fig_tube.show(true);
      vibes::endDrawing();
    }
  }
}

TEST_CASE("CtcDeriv (interpol)")
{
  SECTION("From: Test slice, envelope contraction")
  {
    TubeSlice x(Interval(-1.,3.), Interval(-10.,20.));
    x.setInputGate(Interval(-1.,2.));
    x.setOutputGate(Interval(-2.,0.));
    TubeSlice v(x.domain(), Interval(-1.,1.));
    
    bool contraction;
    Interval y;
    CtcDeriv ctc;

    y = Interval::ALL_REALS;
    contraction = ctc.contract(x, v, Interval(0.5,2.), y);
    CHECK_FALSE(ctc.contract(x, v, Interval(0.5,2.), y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y == Interval(-3.5,2.5));

    y = Interval::ALL_REALS;
    contraction = ctc.contract(x, v, Interval(-1.), y);
    CHECK_FALSE(ctc.contract(x, v, Interval(-1.), y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y == Interval(-1.,2.));

    y = Interval::ALL_REALS;
    contraction = ctc.contract(x, v, Interval(-0.5,0.), y);
    CHECK_FALSE(ctc.contract(x, v, Interval(-0.5,0.), y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y == Interval(-2.,3.));

    y = Interval::ALL_REALS;
    contraction = ctc.contract(x, v, Interval(1.5), y);
    CHECK_FALSE(ctc.contract(x, v, Interval(1.5), y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y == Interval(-3.5,1.5));

    y = Interval::ALL_REALS;
    contraction = ctc.contract(x, v, Interval(2.5), y);
    CHECK_FALSE(ctc.contract(x, v, Interval(2.5), y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y == Interval(-2.5,0.5));

    y = Interval::ALL_REALS;
    contraction = ctc.contract(x, v, Interval(3.), y);
    CHECK_FALSE(ctc.contract(x, v, Interval(3.), y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y == Interval(-2.,0.));

    y = Interval::ALL_REALS;
    contraction = ctc.contract(x, v, Interval(-1.,3.), y);
    CHECK_FALSE(ctc.contract(x, v, Interval(-1.,3.), y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y == Interval(-3.5,3.));

    y = Interval::ALL_REALS;
    CHECK_THROWS(contraction = ctc.contract(x, v, Interval(-10.,3.), y));
  }

  SECTION("From: Test slice, output gate contraction")
  {
    TubeSlice x(Interval(-1.,3.), Interval(-5.,3.));
    x.setInputGate(Interval(-1.,3.));
    x.setOutputGate(Interval(-5.,0.5));
    TubeSlice v(x.domain(), Interval(-1.));

    bool contraction;
    Interval y;
    CtcDeriv ctc;

    y = Interval::ALL_REALS;
    contraction = ctc.contract(x, v, Interval(2.), y);
    CHECK_FALSE(ctc.contract(x, v, Interval(2.), y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y == Interval(-4.,0.));

    y = Interval::ALL_REALS;
    contraction = ctc.contract(x, v, Interval(-1.,3.), y);
    CHECK_FALSE(ctc.contract(x, v, Interval(-1.,3.), y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y == Interval(-5.,3.));
  }

  SECTION("From: Test slice, complete contraction (degenerate tube)")
  {
    TubeSlice x(Interval(-1.,3.), Interval(-5.,3.));
    x.setInputGate(Interval(1.,3.));
    x.setOutputGate(Interval(-4.,-3.));
    TubeSlice v(x.domain(), Interval(-1.,1.));

    bool contraction;
    Interval y;
    CtcDeriv ctc;

    y = Interval::ALL_REALS;
    contraction = ctc.contract(x, v, Interval(0.5,2.), y);
    CHECK_FALSE(ctc.contract(x, v, Interval(0.5,2.), y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y == Interval(-2.,-0.5));

    y = Interval(-1.,300.);
    contraction = ctc.contract(x, v, Interval(1.), y);
    CHECK_FALSE(ctc.contract(x, v, Interval(1.), y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y == Interval(-1.));

    y = Interval(1.,300.);
    contraction = ctc.contract(x, v, Interval(1.), y);
    CHECK_FALSE(ctc.contract(x, v, Interval(1.), y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y == Interval::EMPTY_SET);

    y = Interval::ALL_REALS;
    contraction = ctc.contract(x, v, 1., y);
    CHECK_FALSE(ctc.contract(x, v, 1., y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y == Interval(-1.));

    y = Interval(-20.,1.);
    contraction = ctc.contract(x, v, Interval(-1.,3.), y);
    CHECK_FALSE(ctc.contract(x, v, Interval(-1.,3.), y)); // fixed point already reached
    CHECK(contraction);
    CHECK(y == Interval(-3.,1.));
  }

  SECTION("Interpolation over a tube (example from tubint paper)")
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

    Interval y;
    bool contraction;
    CtcDeriv ctc;

    // Tube evaluation before tube contraction
    y = Interval::ALL_REALS;
    contraction = ctc.contract(tube, tubedot, Interval(0.,5.), y);
    CHECK(!contraction);
    CHECK(tube.codomain() == Interval::ALL_REALS);
    CHECK(y == Interval::ALL_REALS);

    ctc.contract(tube, tubedot); // contraction of the tube
    CHECK_FALSE(ctc.contract(tube, tubedot)); // fixed point already reached
    CHECK(tube.codomain() == Interval(0.,4.25));
    CHECK(tube[0] == Interval(0.,2.));
    CHECK(tube[1] == Interval(1.5,3.5));
    CHECK(tube[2] == Interval(3.,4.));
    CHECK(tube[3] == Interval(3.5,4.));
    CHECK(tube[4] == Interval(3.5,4.25));

    // Tube evaluation after tube contraction
    y = Interval::ALL_REALS;
    contraction = ctc.contract(tube, tubedot, Interval(0.,5.), y);
    CHECK(contraction);
    CHECK(y == Interval(0.,4.25));
    contraction = ctc.contract(tube, tubedot, Interval(0.,5.), y);
    CHECK_FALSE(contraction);
    CHECK(y == Interval(0.,4.25));

    y = Interval(3.,10.);
    contraction = ctc.contract(tube, tubedot, Interval(1.,3.), y);
    CHECK(contraction);
    CHECK(y == Interval(3.,4.));

    y = Interval::EMPTY_SET;
    contraction = ctc.contract(tube, tubedot, Interval(1.,3.), y);
    CHECK_FALSE(contraction);
    CHECK(y == Interval::EMPTY_SET);

    y = Interval::ALL_REALS;
    contraction = ctc.contract(tube, tubedot, Interval(2.2,2.7), y);
    CHECK(contraction);
    CHECK(y == Interval(3.1,3.85));

    y = Interval::ALL_REALS;
    contraction = ctc.contract(tube, tubedot, Interval(4.5), y);
    CHECK(contraction);
    CHECK(y == Interval(3.75,4.25));

    y = Interval::ALL_REALS;
    contraction = ctc.contract(tube, tubedot, Interval(4.1,4.9), y);
    CHECK(contraction);
    CHECK(y == Interval(3.55,4.25));

    y = Interval(0.5,1.5);
    contraction = ctc.contract(tube, tubedot, Interval(0.25,0.75), y);
    CHECK_FALSE(contraction);
    CHECK(y == Interval(0.5,1.5));

    y = Interval::ALL_REALS;
    CHECK_THROWS(ctc.contract(tube, tubedot, Interval(-10.,10.), y));
    contraction = ctc.contract(tube, tubedot, tube.domain(), y);
    CHECK(contraction);
    CHECK(y == tube.codomain());
  }*/
}