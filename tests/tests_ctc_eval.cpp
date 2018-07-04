#include "tests.h"
#include <cstdio>

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

#define VIBES_DRAWING 0

TEST_CASE("CtcEval")
{
  /*SECTION("Test CtcEval, 1")
  {
    Interval t(0.,6.), z(-1.,1.);
    Tube x(Interval(-1.,7.), 2.);
      x &= Interval(-10.,10.);
    Tube v(x);
    v.set(Interval(-1.), 0);
    v.set(Interval(-1.,1.), 1);
    v.set(Interval(-1.), 2);
    v.set(Interval(1.), 3);

    CHECK(x.nbSlices() == 4);
    CtcEval ctc_eval(false);
    ctc_eval.contract(t, z, x, v);
    CHECK(x.nbSlices() == 6);

    CHECK(x.codomain() == Interval::ALL_REALS); // only gates should be affected
    CHECK(x[-1.] == Interval::ALL_REALS);
    CHECK(x[0.] == Interval(-2.,6.));
    CHECK(x[1.] == Interval(-3.,5.));
    CHECK(x[3.] == Interval(-4.,3.));
    CHECK(x[5.] == Interval(-6.,1.));
    CHECK(x[6.] == Interval(-5.,2.));
    CHECK(x[7.] == Interval::ALL_REALS);

    if(VIBES_DRAWING) // drawing results
    {
      vibes::beginDrawing();
      x &= Interval(-10.,10.);
      VibesFigure_Tube fig_tube("ctceval", &x);
      fig_tube.setProperties(100, 100, 500, 500);
      fig_tube.setTubeDerivative(&x, &v);
      fig_tube.show(true);
      vibes::endDrawing();
    }
  }
/*
  SECTION("Test CtcEval, 2")
  {
    Interval t(0.,6.), z(-1.,1.);
    Tube x(Interval(-1.,7.), 2.);
      x &= Interval(-10.,10.);
    Tube v(x);
    v.set(Interval(-1.), 0);
    v.set(Interval(-1.,1.), 1);
    v.set(Interval(-1.), 2);
    v.set(Interval(1.), 3);

    x.set(Interval(0.,1.), 2.);
    v.sample(2.);

    CHECK(x.nbSlices() == 5);

    CtcEval ctc_eval(false);
    ctc_eval.contract(t, z, x, v);

    CHECK(x.nbSlices() == 7);

    CHECK(x.codomain() == Interval::ALL_REALS); // only gates should be affected
    CHECK(x[-1.] == Interval::ALL_REALS);
    CHECK(x[0.] == Interval(0.,3.));
    CHECK(x[1.] == Interval(-1.,2.));
    CHECK(x[2.] == Interval(0.,1.));
    CHECK(x[3.] == Interval(-1.,2.));
    CHECK(x[5.] == Interval(-3.,1.));
    CHECK(x[6.] == Interval(-2.,2.));
    CHECK(x[7.] == Interval::ALL_REALS);

    t = Interval(1.75,5.5);
    z = Interval(1.6);

    ctc_eval = CtcEval(true);
    ctc_eval.contract(t, z, x, v);

    if(VIBES_DRAWING) // drawing results
    {
      vibes::beginDrawing();
      x &= Interval(-10.,10.);
      VibesFigure_Tube fig_tube("ctceval", &x);
      fig_tube.setProperties(100, 100, 500, 500);
      fig_tube.setTubeDerivative(&x, &v);
      fig_tube.show(true);
    }

    IntervalVector box(2);
    box[0] = Interval(1.75,5.5);
    box[1] = Interval(1.6);

    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "ctceval", "red"));
    ctc_eval.contract(box[0], box[1], x, v);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "ctceval", "blue"));

    CHECK(x.interpol(3.8,v) == Interval(-1.8,1.6));
    CHECK(box[0] == Interval(2.6,3.8));
    CHECK(box[1] == Interval(1.6));

    if(VIBES_DRAWING) vibes::endDrawing();
  }*/

  SECTION("Test CtcEval, special cases")
  {
    Tube xdot(Interval(0., 10.), 1.0);
    xdot.set(Interval(-0.5,1.));

    Tube x(xdot);
    x.set(Interval(-10.,10.));
    // todo: apply this: x.set(Interval::ALL_REALS);
    x.set(Interval(-1.5,1.), 4);
    x.set(Interval(-1.,1.5), 5);
    CHECK(x.nbSlices() == 10);
    
    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, xdot);

    Tube x_raw(x), xdot_raw(xdot);

    // Checking the tube...
    CHECK(x_raw[0] == Interval(-5.5,3.));
    CHECK(x_raw[1] == Interval(-4.5,2.5));
    CHECK(x_raw[2] == Interval(-3.5,2.));
    CHECK(x_raw[3] == Interval(-2.5,1.5));
    CHECK(x_raw[4] == Interval(-1.5,1.));
    CHECK(x_raw[5] == Interval(-1.,1.5));
    CHECK(x_raw[6] == Interval(-1.5,2.5));
    CHECK(x_raw[7] == Interval(-2.,3.5));
    CHECK(x_raw[8] == Interval(-2.5,4.5));
    CHECK(x_raw[9] == Interval(-3.,5.5));

    bool contraction;
    Interval intv_t, intv_y;
    CtcEval ctc_eval_propa(true), ctc_eval_nopropa(false);

    // Test A (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = 1.;
    intv_y = Interval(-0.5,1.);
    CHECK(x[1.] == Interval(-4.5,2.5));
    contraction = ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK_FALSE(ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot)); // fixed point already reached
    CHECK(contraction);
    CHECK(x.nbSlices() == 10);
    CHECK(intv_t == 1.);
    CHECK(intv_y == Interval(-0.5,1.));
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1.] == intv_y);
    CHECK(x[1] == Interval(-4.5,2.5));
    CHECK(x[2] == Interval(-3.5,2.));
    CHECK(x[3] == Interval(-2.5,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(-1.5,2.5));
    CHECK(x[7] == Interval(-2.,3.5));
    CHECK(x[8] == Interval(-2.5,4.5));
    CHECK(x[9] == Interval(-3.,5.5));

    // Test A (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = 1.;
    intv_y = Interval(-0.5,1.);
    CHECK(x[1.] == Interval(-4.5,2.5));
    contraction = ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK_FALSE(ctc_eval_propa.contract(intv_t, intv_y, x, xdot)); // fixed point already reached
    CHECK(contraction);
    CHECK(x.nbSlices() == 10);
    CHECK(intv_t == 1.);
    CHECK(intv_y == Interval(-0.5,1.));
    CHECK(x[0] == Interval(-1.5,1.5));
    CHECK(x[1.] == intv_y);
    CHECK(x[1] == Interval(-1.,2.));
    CHECK(x[2] == Interval(-1.5,2.));
    CHECK(ApproxIntv(x[3]) == Interval(-1.5-1./3.,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(-1.5,2.5));
    CHECK(x[7] == Interval(-2.,3.5));
    CHECK(x[8] == Interval(-2.5,4.5));
    CHECK(x[9] == Interval(-3.,5.5));

    // Test A: ctcDeriv should not be effective after ctcEval(true)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = 1.;
    intv_y = Interval(-0.5,1.);
    contraction = ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK_FALSE(ctc_eval_propa.contract(intv_t, intv_y, x, xdot)); // fixed point already reached

    // Test B (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(0.5,2.5);
    intv_y = -2.;
    contraction = ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK_FALSE(ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot)); // fixed point already reached
    CHECK(contraction);
    CHECK(x.nbSlices() == 12);
    CHECK(intv_t == Interval(0.5,2.5));
    CHECK(intv_y == -2.);
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[0.5] == Interval(-4.,-1.));
    CHECK(x[1] == Interval(-5.5,3.));
    CHECK(x[1.] == Interval(-3.5,-1.25));
    CHECK(x[2] == Interval(-4.5,2.5));
    CHECK(x[2.] == Interval(-2.75,-0.5));
    CHECK(x[3] == Interval(-3.5,2.));
    CHECK(x[2.5] == Interval(-3.,-0.));
    CHECK(x[4] == Interval(-3.5,2.));
    CHECK(x[5] == Interval(-2.5,1.5));
    CHECK(x[6] == Interval(-1.5,1.));
    CHECK(x[7] == Interval(-1.,1.5));
    CHECK(x[8] == Interval(-1.5,2.5));
    CHECK(x[9] == Interval(-2.,3.5));
    CHECK(x[10] == Interval(-2.5,4.5));
    CHECK(x[11] == Interval(-3.,5.5));

    // Test B (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(0.5,2.5);
    intv_y = -2.;
    contraction = ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK_FALSE(ctc_eval_propa.contract(intv_t, intv_y, x, xdot)); // fixed point already reached
    CHECK(contraction);
    CHECK(x.nbSlices() == 12);
    CHECK(intv_t == Interval(0.5,2.5));
    CHECK(intv_y == -2.);
    CHECK(x[0] == Interval(-4.5,-0.75));
    CHECK(x[0.5] == Interval(-4.,-1.));
    CHECK(x[1] == Interval(-4.,-1.));
    CHECK(x[1.] == Interval(-3.5,-1.25));
    CHECK(x.getSlice(0.6)->domain() == Interval(0.5,1.));
    CHECK(x.getSlice(0.6)->inputGate() == Interval(-4.,-1.));
    CHECK(x.getSlice(0.6)->outputGate() == Interval(-3.5,-1.25));
    CHECK(xdot[2] == Interval(-0.5,1.));
    //CHECK(x[2] == Interval(-3.5,-0.5)); // todo: optimal implementation?
    CHECK(x[2.] == Interval(-2.75,-0.5));
    CHECK(x[3] == Interval(-3.,0.));
    CHECK(x[2.5] == Interval(-3.,-0.));
    CHECK(x[4] == Interval(-3.,0.5));
    CHECK(ApproxIntv(x[5]) == Interval(-2.5,1.+1./6.));
    CHECK(x[6] == Interval(-1.5,1.));
    CHECK(x[7] == Interval(-1.,1.5));
    CHECK(x[8] == Interval(-1.5,2.5));
    CHECK(x[9] == Interval(-2.,3.5));
    CHECK(x[10] == Interval(-2.5,4.5));
    CHECK(x[11] == Interval(-3.,5.5));

    // Test C (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(0.5,3.5);
    intv_y = -4.;
    contraction = ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK_FALSE(ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot)); // fixed point already reached
    CHECK(contraction);
    CHECK(x.nbSlices() == 12);
    CHECK(intv_t == Interval(0.5,1.5));
    CHECK(intv_y == -4.);
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[0.5] == Interval(-5.,-3.5));
    CHECK(x[1] == Interval(-5.5,3.));
    CHECK(x[1.] == Interval(-4.5,-3.5));
    CHECK(x[2] == Interval(-4.5,2.5));
    CHECK(x[1.5] == Interval(-4.,-3.));
    CHECK(x[3] == Interval(-4.5,2.5));
    CHECK(x[4] == Interval(-3.5,2.));
    CHECK(x[5] == Interval(-2.5,1.5));
    CHECK(x[6] == Interval(-1.5,1.));
    CHECK(x[7] == Interval(-1.,1.5));
    CHECK(x[8] == Interval(-1.5,2.5));
    CHECK(x[9] == Interval(-2.,3.5));
    CHECK(x[10] == Interval(-2.5,4.5));
    CHECK(x[11] == Interval(-3.,5.5));

    // Test C (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(0.5,3.5);
    intv_y = -4.;
    contraction = ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK_FALSE(ctc_eval_propa.contract(intv_t, intv_y, x, xdot)); // fixed point already reached
    CHECK(contraction);
    CHECK(x.nbSlices() == 12);
    CHECK(intv_t == Interval(0.5,1.5));
    CHECK(intv_y == -4.);
    CHECK(x[0] == Interval(-5.5,-3.25));
    CHECK(x[0.5] == Interval(-5.,-3.5));
    //CHECK(x[1] == Interval(-5.,-3.5)); // optimality
    CHECK(x[1.] == Interval(-4.5,-3.5));
    CHECK(x[2] == Interval(-4.5,-3.));
    CHECK(x[1.5] == Interval(-4.,-3.));
    CHECK(x[3] == Interval(-4.,-2.5));
    CHECK(x[4] == Interval(-3.5,-1.5));
    CHECK(x[5] == Interval(-2.5,-0.5));
    CHECK(x[6] == Interval(-1.5,0.5));
    CHECK(x[7] == Interval(-1.,1.5));
    CHECK(x[8] == Interval(-1.5,2.5));
    CHECK(x[9] == Interval(-2.,3.5));
    CHECK(x[10] == Interval(-2.5,4.5));
    CHECK(x[11] == Interval(-3.,5.5));

    if(false & VIBES_DRAWING) // drawing results
    {
      x = x_raw;
      xdot = xdot_raw;
      IntervalVector box(2);
      box[0] = Interval(0.5,3.5);
      box[1] = Interval(-4.);
      vibes::beginDrawing();
      VibesFigure_Tube fig_tube("ctceval", &x);
      fig_tube.setProperties(100, 100, 500, 500);
      fig_tube.setTubeDerivative(&x, &xdot);
      fig_tube.show(true);
      contraction = ctc_eval_propa.contract(box[0], box[1], x, xdot);
      fig_tube.show(true);
      vibes::drawBox(box, vibesParams("figure", "ctceval", "blue"));
      vibes::endDrawing();
    }

    // Test D (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = 3.5;
    intv_y = Interval(-3.5,-0.5);
    CHECK(x.nbSlices() == 10);
    contraction = ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(x.nbSlices() == 11);
    CHECK(contraction);
    CHECK(intv_t == 3.5);
    CHECK(intv_y == Interval(-2.,-0.5));
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1] == Interval(-4.5,2.5));
    CHECK(x[2] == Interval(-3.5,2.));
    CHECK(x[3] == Interval(-2.5,1.5));
    CHECK(x[3.5] == Interval(-2.,-0.5));
    CHECK(x[4] == Interval(-2.5,1.5));
    CHECK(x[5] == Interval(-1.5,1.));
    CHECK(x[6] == Interval(-1.,1.5));
    CHECK(x[7] == Interval(-1.5,2.5));
    CHECK(x[8] == Interval(-2.,3.5));
    CHECK(x[9] == Interval(-2.5,4.5));
    CHECK(x[10] == Interval(-3.,5.5));

    // Test D (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = 3.5;
    intv_y = Interval(-3.5,-0.5);
    CHECK(x.nbSlices() == 10);
    contraction = ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(x.nbSlices() == 11);
    CHECK(contraction);
    CHECK(intv_t == 3.5);
    CHECK(intv_y == Interval(-2.,-0.5));
    CHECK(x[0] == Interval(-5.5,1.25));
    CHECK(x[1] == Interval(-4.5,0.75));
    CHECK(x[2] == Interval(-3.5,0.25));
    CHECK(x[3] == Interval(-2.5,-0.25));
    CHECK(x[3.5] == Interval(-2.,-0.5));
    CHECK(x[4] == Interval(-2.,0.));
    CHECK(x[5] == Interval(-1.5,1.));
    CHECK(x[6] == Interval(-1.,1.5));
    CHECK(x[7] == Interval(-1.5,2.5));
    CHECK(x[8] == Interval(-2.,3.5));
    CHECK(x[9] == Interval(-2.5,4.5));
    CHECK(x[10] == Interval(-3.,5.5));

    // Test E (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(6.5, 8.5);
    intv_y = Interval(-4.5, -1.75);
    contraction = ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(x.nbSlices() == 12);
    CHECK(contraction);
    CHECK(intv_t == Interval(7.5, 8.5));
    CHECK(intv_y == Interval(-2.25, -1.75));
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1] == Interval(-4.5,2.5));
    CHECK(x[2] == Interval(-3.5,2.));
    CHECK(x[3] == Interval(-2.5,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(-1.5,2.5));
    CHECK(x[7] == Interval(-2.,3.5));
    CHECK(x[7.5] == Interval(-1.75,-1.25));
    CHECK(x[8] == Interval(-2.,3.5));
    CHECK(x[8.] == Interval(-2.,-1.25));
    CHECK(x[9] == Interval(-2.5,4.5));
    CHECK(x[10] == Interval(-2.5,4.5));
    CHECK(x[11] == Interval(-3.,5.5));

    // Test E (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(6.5, 8.5);
    intv_y = Interval(-4.5, -1.75);
    contraction = ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(x.nbSlices() == 12);
    CHECK(contraction);
    CHECK(intv_t == Interval(7.5, 8.5));
    CHECK(intv_y == Interval(-2.25, -1.75));
    CHECK(x[0] == Interval(-5.5,2.5));
    CHECK(x[1] == Interval(-4.5,2.));
    CHECK(x[2] == Interval(-3.5,1.5));
    CHECK(x[3] == Interval(-2.5,1.));
    CHECK(x[4] == Interval(-1.5,0.5));
    CHECK(x[5] == Interval(-1.,0.));
    CHECK(x[6] == Interval(-1.5,-0.5));
    CHECK(x[7] == Interval(-1.75,-1.));
    CHECK(x[7.5] == Interval(-1.75,-1.25));
    //CHECK(x[8] == Interval(-2.,-1.25)); // optimality
    CHECK(x[8.] == Interval(-2.,-1.25));
    CHECK(x[9] == Interval(-2.25,-0.75));
    CHECK(x[10] == Interval(-2.5,-0.25));
    CHECK(x[11] == Interval(-3.,0.75));

    // Test F (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(8.5,9.5);
    intv_y = Interval(-0.5,2.5);
    contraction = ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(x.nbSlices() == 12);
    CHECK(contraction);
    CHECK(intv_t == Interval(8.5,9.5));
    CHECK(intv_y == Interval(-0.5,2.5));
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1] == Interval(-4.5,2.5));
    CHECK(x[2] == Interval(-3.5,2.));
    CHECK(x[3] == Interval(-2.5,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(-1.5,2.5));
    CHECK(x[7] == Interval(-2.,3.5));
    CHECK(x[8] == Interval(-2.5,4.5));
    CHECK(x[8.5] == Interval(-1.5,3.));
    CHECK(x[9] == Interval(-2.5,4.5));
    CHECK(x[10] == Interval(-3.,5.5));
    CHECK(x[9.5] == Interval(-1.,3.5));
    CHECK(x[11] == Interval(-3,5.5));

    // Test F (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(8.5,9.5);
    intv_y = Interval(-0.5,2.5);
    contraction = ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(x.nbSlices() == 12);
    CHECK(contraction);
    CHECK(intv_t == Interval(8.5,9.5));
    CHECK(intv_y == Interval(-0.5,2.5));
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1] == Interval(-4.5,2.5));
    CHECK(x[2] == Interval(-3.5,2.));
    CHECK(x[3] == Interval(-2.5,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(-1.5,2.5));
    CHECK(ApproxIntv(x[7]) == Interval(-2.,3.+1./3.));
    CHECK(x[8] == Interval(-2.,3.25));
    CHECK(x[8.5] == Interval(-1.5,3.));
    //CHECK(x[9] == Interval(-1.5,3.)); // optimality
    //CHECK(x[10] == Interval(-1.,3.5)); // optimality
    CHECK(x[9.5] == Interval(-1.,3.5));
    CHECK(x[11] == Interval(-1.25,4.));

    // Test G (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(6.5,7.5);
    intv_y = Interval(3.,4.);
    contraction = ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(contraction);
    CHECK(intv_t == Interval(7.5));
    CHECK(intv_y == Interval(3.));
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1] == Interval(-4.5,2.5));
    CHECK(x[2] == Interval(-3.5,2.));
    CHECK(x[3] == Interval(-2.5,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(-1.5,2.5));
    CHECK(x[7] == Interval(-2.,3.5));
    CHECK(x[7.5] == Interval(3.));
    CHECK(x[8] == Interval(-2.,3.5));
    CHECK(x[9] == Interval(-2.5,4.5));
    CHECK(x[10] == Interval(-3.,5.5));

    // Test G (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(6.5,7.5);
    intv_y = Interval(3.,4.);
    contraction = ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(contraction);
    CHECK(intv_t == Interval(7.5));
    CHECK(intv_y == Interval(3.));
    CHECK(x[0] == Interval(-4.5,3.));
    CHECK(x[1] == Interval(-3.5,2.5));
    CHECK(x[2] == Interval(-2.5,2.));
    CHECK(x[3] == Interval(-1.5,1.5));
    CHECK(x[4] == Interval(-0.5,1.));
    CHECK(x[5] == Interval(0.5,1.5));
    CHECK(x[6] == Interval(1.5,2.5));
    CHECK(x[7] == Interval(2.5,3.));
    CHECK(x[7.5] == Interval(3.));
    CHECK(x[8] == Interval(2.75,3.5));
    CHECK(x[9] == Interval(2.25,4.5));
    CHECK(x[10] == Interval(1.75,5.5));

    // Test H (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(7.,8.25);
    intv_y = Interval(4.25,5.);
    contraction = ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(contraction);
    CHECK(intv_t == Interval::EMPTY_SET);
    CHECK(intv_y == Interval::EMPTY_SET);
    CHECK(x.nbSlices() == 10);
    CHECK(x[0] == Interval::EMPTY_SET);
    CHECK(x[1] == Interval::EMPTY_SET);
    CHECK(x[2] == Interval::EMPTY_SET);
    CHECK(x[3] == Interval::EMPTY_SET);
    CHECK(x[4] == Interval::EMPTY_SET);
    CHECK(x[5] == Interval::EMPTY_SET);
    CHECK(x[6] == Interval::EMPTY_SET);
    CHECK(x[7] == Interval::EMPTY_SET);
    CHECK(x[8] == Interval::EMPTY_SET);
    CHECK(x[9] == Interval::EMPTY_SET);
    CHECK(x.isEmpty());

    // Test H (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(7.,8.25);
    intv_y = Interval(4.25,5.);
    contraction = ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(contraction);
    CHECK(intv_t == Interval::EMPTY_SET);
    CHECK(intv_y == Interval::EMPTY_SET);
    CHECK(x.nbSlices() == 10);
    CHECK(x[0] == Interval::EMPTY_SET);
    CHECK(x[1] == Interval::EMPTY_SET);
    CHECK(x[2] == Interval::EMPTY_SET);
    CHECK(x[3] == Interval::EMPTY_SET);
    CHECK(x[4] == Interval::EMPTY_SET);
    CHECK(x[5] == Interval::EMPTY_SET);
    CHECK(x[6] == Interval::EMPTY_SET);
    CHECK(x[7] == Interval::EMPTY_SET);
    CHECK(x[8] == Interval::EMPTY_SET);
    CHECK(x[9] == Interval::EMPTY_SET);
    CHECK(x.isEmpty());

    // Test I (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = 2.5;
    intv_y = 0.5;
    contraction = ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(contraction);
    CHECK(intv_t == Interval(2.5));
    CHECK(intv_y == Interval(0.5));
    CHECK(x.nbSlices() == 11);
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1] == Interval(-4.5,2.5));
    CHECK(x[2] == Interval(-3.5,2.));
    CHECK(x[2.5] == Interval(0.5));
    CHECK(x[3] == Interval(-3.5,2.));
    CHECK(x[4] == Interval(-2.5,1.5));
    CHECK(x[5] == Interval(-1.5,1.));
    CHECK(x[6] == Interval(-1.,1.5));
    CHECK(x[7] == Interval(-1.5,2.5));
    CHECK(x[8] == Interval(-2.,3.5));
    CHECK(x[9] == Interval(-2.5,4.5));
    CHECK(x[10] == Interval(-3.,5.5));

    // Test I (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = 2.5;
    intv_y = 0.5;
    contraction = ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(contraction);
    CHECK(intv_t == Interval(2.5));
    CHECK(intv_y == Interval(0.5));
    CHECK(x.nbSlices() == 11);
    CHECK(x[0] == Interval(-2.,1.75));
    CHECK(x[1] == Interval(-1.,1.25));
    CHECK(x[2] == Interval(-0.,0.75));
    CHECK(x[2.5] == Interval(0.5));
    CHECK(x[3] == Interval(0.25,1.));
    CHECK(ApproxIntv(x[4]) == Interval(-0.25,1.+1./3.));
    CHECK(x[5] == Interval(-0.75,1.));
    CHECK(x[6] == Interval(-1.,1.5));
    CHECK(x[7] == Interval(-1.5,2.5));
    CHECK(x[8] == Interval(-2.,3.5));
    CHECK(x[9] == Interval(-2.5,4.5));
    CHECK(x[10] == Interval(-3.,5.5));

    // Test J (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = 6.25;
    intv_y = Interval(0.5,1.);
    contraction = ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(contraction);
    CHECK(intv_t == 6.25);
    CHECK(intv_y == Interval(0.5,1.));
    CHECK(x.nbSlices() == 11);
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1] == Interval(-4.5,2.5));
    CHECK(x[2] == Interval(-3.5,2.));
    CHECK(x[3] == Interval(-2.5,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(-1.5,2.5));
    CHECK(x[6.25] == Interval(0.5,1.));
    CHECK(x[7] == Interval(-1.5,2.5));
    CHECK(x[8] == Interval(-2.,3.5));
    CHECK(x[9] == Interval(-2.5,4.5));
    CHECK(x[10] == Interval(-3.,5.5));

    // Test J (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = 6.25;
    intv_y = Interval(0.5,1.);
    contraction = ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(contraction);
    CHECK(intv_t == 6.25);
    CHECK(intv_y == Interval(0.5,1.));
    CHECK(x.nbSlices() == 11);
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1] == Interval(-4.5,2.5));
    CHECK(x[2] == Interval(-3.5,2.));
    CHECK(x[3] == Interval(-2.5,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5.] == Interval(-0.75,1.));
    CHECK(ApproxIntv(x[5]) == Interval(-0.75,1.4+1./60.));
    CHECK(x[6] == Interval(0.25,1.125));
    CHECK(x[7] == Interval(0.125,1.75));
    CHECK(x[8] == Interval(0.125-0.5,2.75));
    CHECK(x[9] == Interval(0.125-1.,3.75));
    CHECK(x[10] == Interval(0.125-1.5,4.75));

    // Test K (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(5.25, 8.25);
    intv_y = Interval(-0.5, 0.);
    contraction = ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(contraction);
    CHECK(intv_t == Interval(5.25, 8.25));
    CHECK(intv_y == Interval(-0.5, 0.));
    CHECK(x.nbSlices() == 12);
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1] == Interval(-4.5,2.5));
    CHECK(x[2] == Interval(-3.5,2.));
    CHECK(x[3] == Interval(-2.5,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[5.25] == Interval(-1.,1.25));
    CHECK(x[6] == Interval(-1.,1.5));
    CHECK(x[7] == Interval(-1.5,2.5));
    CHECK(x[8] == Interval(-2.,3.5));
    CHECK(x[9] == Interval(-2.5,4.5));
    CHECK(x[8.25] == Interval(-2.,3.));
    CHECK(x[10] == Interval(-2.5,4.5));
    CHECK(x[11] == Interval(-3.,5.5));

    // Test K (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(5.25, 8.25);
    intv_y = Interval(-0.5, 0.);
    contraction = ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(contraction);
    CHECK(intv_t == Interval(5.25, 8.25));
    CHECK(intv_y == Interval(-0.5, 0.));
    CHECK(x.nbSlices() == 12);
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1] == Interval(-4.5,2.5));
    CHECK(x[2] == Interval(-3.5,2.));
    CHECK(x[3] == Interval(-2.5,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.25));
    CHECK(x[5.25] == Interval(-1.,1.25));
    CHECK(ApproxIntv(x[6]) == Interval(-1.,1.4+1./60.));
    CHECK(x[7] == Interval(-1.5,1.875));
    CHECK(ApproxIntv(x[8]) == Interval(-1.625-1./3.,2.75));
    CHECK(x[9] == Interval(-2.,3.));
    CHECK(x[8.25] == Interval(-2.,3.));
    CHECK(x[10] == Interval(-2.375,3.75));
    CHECK(x[11] == Interval(-2.875,4.75));

    // Test L (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(1.5,4.5);
    intv_y = Interval(1.75,2.75);
    contraction = ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(contraction);
    CHECK(intv_t == Interval(1.5,2.5));
    CHECK(intv_y == Interval(1.75,2.25));
    CHECK(x.nbSlices() == 12);
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1] == Interval(-4.5,2.5));
    CHECK(x[1.5] == Interval(0.75,2.25));
    CHECK(x[2] == Interval(-4.5,2.5));
    CHECK(x[3] == Interval(-3.5,2.));
    CHECK(x[2.5] == Interval(1.25,1.75));
    CHECK(x[4] == Interval(-3.5,2.));
    CHECK(x[5] == Interval(-2.5,1.5));
    CHECK(x[6] == Interval(-1.5,1.));
    CHECK(x[7] == Interval(-1.,1.5));
    CHECK(x[8] == Interval(-1.5,2.5));
    CHECK(x[9] == Interval(-2.,3.5));
    CHECK(x[10] == Interval(-2.5,4.5));
    CHECK(x[11] == Interval(-3.,5.5));

    // Test L (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(1.5,2.5);
    intv_y = Interval(1.75,2.75);
    contraction = ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(contraction);
    CHECK(intv_t == Interval(1.5,2.5));
    CHECK(intv_y == Interval(1.75,2.25));
    CHECK(x.nbSlices() == 12);
    CHECK(x[0] == Interval(-0.75,3.));
    CHECK(x[1] == Interval(0.25,2.5));
    CHECK(x[1.5] == Interval(0.75,2.25));
    CHECK(x[2] == Interval(0.75,2.25));
    //CHECK(x[3] == Interval(1.25,2.)); // optimality
    CHECK(x[2.5] == Interval(1.25,1.75));
    CHECK(x[4] == Interval(1.,1.75));
    CHECK(x[5] == Interval(0.5,1.5));
    CHECK(x[6] == Interval(0.,1.));
    CHECK(x[7] == Interval(-0.5,1.5));
    CHECK(x[8] == Interval(-1.,2.5));
    CHECK(x[9] == Interval(-1.5,3.5));
    CHECK(x[10] == Interval(-2.,4.5));
    CHECK(x[11] == Interval(-2.5,5.5));

    // Test M (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(0.75,1.25);
    intv_y = Interval(-5.75,-5.25);
    contraction = ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(contraction);
    CHECK(intv_t == Interval::EMPTY_SET);
    CHECK(intv_y == Interval::EMPTY_SET);
    CHECK(x.nbSlices() == 10);
    CHECK(x[0] == Interval::EMPTY_SET);
    CHECK(x[1] == Interval::EMPTY_SET);
    CHECK(x[2] == Interval::EMPTY_SET);
    CHECK(x[3] == Interval::EMPTY_SET);
    CHECK(x[4] == Interval::EMPTY_SET);
    CHECK(x[5] == Interval::EMPTY_SET);
    CHECK(x[6] == Interval::EMPTY_SET);
    CHECK(x[7] == Interval::EMPTY_SET);
    CHECK(x[8] == Interval::EMPTY_SET);
    CHECK(x[9] == Interval::EMPTY_SET);
    CHECK(x.isEmpty());

    // Test M (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(0.75,1.25);
    intv_y = Interval(-5.75,-5.25);
    contraction = ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(contraction);
    CHECK(intv_t == Interval::EMPTY_SET);
    CHECK(intv_y == Interval::EMPTY_SET);
    CHECK(x.nbSlices() == 10);
    CHECK(x[0] == Interval::EMPTY_SET);
    CHECK(x[1] == Interval::EMPTY_SET);
    CHECK(x[2] == Interval::EMPTY_SET);
    CHECK(x[3] == Interval::EMPTY_SET);
    CHECK(x[4] == Interval::EMPTY_SET);
    CHECK(x[5] == Interval::EMPTY_SET);
    CHECK(x[6] == Interval::EMPTY_SET);
    CHECK(x[7] == Interval::EMPTY_SET);
    CHECK(x[8] == Interval::EMPTY_SET);
    CHECK(x[9] == Interval::EMPTY_SET);
    CHECK(x.isEmpty());

    // Test N (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(5.5,8.5);
    intv_y = Interval(2.,5.5);
    contraction = ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(contraction);
    CHECK(intv_t == Interval(6.5,8.5));
    CHECK(intv_y == Interval(2.,4.));
    CHECK(x.nbSlices() == 12);
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1] == Interval(-4.5,2.5));
    CHECK(x[2] == Interval(-3.5,2.));
    CHECK(x[3] == Interval(-2.5,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(-1.5,2.5));
    CHECK(x[6.5] == Interval(0.,2.));
    CHECK(x[7] == Interval(-1.5,2.5));
    CHECK(x[8] == Interval(-2.,3.5));
    CHECK(x[9] == Interval(-2.5,4.5));
    CHECK(x[8.5] == Interval(1.,4.));
    CHECK(x[10] == Interval(-2.5,4.5));
    CHECK(x[11] == Interval(-3.,5.5));

    // Test N (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(5.5,8.5);
    intv_y = Interval(2.,5.5);
    contraction = ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(contraction);
    CHECK(intv_t == Interval(6.5,8.5));
    CHECK(intv_y == Interval(2.,4.));
    CHECK(x.nbSlices() == 12);
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1] == Interval(-4.5,2.5));
    CHECK(x[2] == Interval(-3.5,2.));
    CHECK(x[3] == Interval(-2.5,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(-0.5,2.));
    CHECK(x[6.5] == Interval(0.,2.));
    CHECK(x[7] == Interval(0.,2.5));
    //CHECK(x[8] == Interval(xxx)); // optimality
    CHECK(x[9] == Interval(1.,4.));
    CHECK(x[8.5] == Interval(1.,4.));
    CHECK(x[10] == Interval(0.75,4.5));
    CHECK(x[11] == Interval(0.25,5.5));

    // Test 0 (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(2.5,9.5);
    intv_y = Interval(-5.,-2.5);
    contraction = ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(contraction);
    CHECK(intv_t == Interval(2.5,9.5));
    CHECK(intv_y == Interval(-3.,-2.5));
    CHECK(x.nbSlices() == 12);
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1] == Interval(-4.5,2.5));
    CHECK(x[2] == Interval(-3.5,2.));
    CHECK(x[2.5] == Interval(-3.,1.));
    CHECK(x[3] == Interval(-3.5,2.));
    CHECK(x[4] == Interval(-2.5,1.5));
    CHECK(x[5] == Interval(-1.5,1.));
    CHECK(x[6] == Interval(-1.,1.5));
    CHECK(x[7] == Interval(-1.5,2.5));
    CHECK(x[8] == Interval(-2.,3.5));
    CHECK(x[9] == Interval(-2.5,4.5));
    CHECK(x[10] == Interval(-3.,5.5));
    CHECK(x[9.5] == Interval(-2.75,4.5));
    CHECK(x[11] == Interval(-3.,5.5));

    // Test 0 (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(2.5,9.5);
    intv_y = Interval(-5.,-2.5);
    contraction = ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(contraction);
    CHECK(intv_t == Interval(2.5,9.5));
    CHECK(intv_y == Interval(-3.,-2.5));
    CHECK(x.nbSlices() == 12);
    CHECK(x[0] == Interval(-5.5,2.25));
    CHECK(x[1] == Interval(-4.5,1.75));
    CHECK(x[2] == Interval(-3.5,1.25));
    CHECK(x[2.5] == Interval(-3.,1.));
    CHECK(x[3] == Interval(-3.,1.));
    CHECK(x[4] == Interval(-2.5,0.75));
    //CHECK(x[5] == Interval(xxx)); // optimality
    CHECK(x[6] == Interval(-1.,1.));
    CHECK(x[7] == Interval(-1.5,2.));
    CHECK(x[8] == Interval(-2.,3.));
    CHECK(x[9] == Interval(-2.5,4.));
    CHECK(x[10] == Interval(-2.75,4.5));
    CHECK(x[9.5] == Interval(-2.75,4.5));
    CHECK(x[11] == Interval(-3.,5.));
  }

  SECTION("Test CtcEval, multi eval")
  {
    Function f_x("t", "cos(t)+t*[-0.1,0.2]");
    Function f_v("t", "-sin(t)+[-0.1,0.2]");

    Tube x(Interval(0.,20.), 0.1, f_x);
    Tube v(Interval(0.,20.), 0.1, f_v);
    x.ctcDeriv(v);

    IntervalVector box(2);
    CtcEval ctc_eval(true);
    Tube x_c(x); Tube v_c(v);

    box[0] = Interval(11.98);
    box[1] = Interval(1.);
    ctc_eval.contract(box[0], box[1], x_c, v_c);

    box[0] = Interval(6.5);
    box[1] = Interval(1.);
    ctc_eval.contract(box[0], box[1], x_c, v_c);

    if(VIBES_DRAWING) // drawing results
    {
      vibes::beginDrawing();
      VibesFigure_Tube fig_tube("ctceval", &x);
      fig_tube.setProperties(100, 100, 800, 400);
      fig_tube.setTubeDerivative(&x, &v);
      fig_tube.show(true);
      x = x_c;
      v = v_c;
      fig_tube.show(true);
      vibes::drawBox(box, vibesParams("figure", "ctceval", "red"));
    }
  }

  SECTION("Test CtcEval, non-zero derivative (negative case)")
  {
    Tube x(Interval(0.,11.), 1.);
    Tube v(x);
    v.set(Interval(-1.5,-1.));
    x.set(Interval(6.,8.), 0);
    x.ctcDeriv(v);

    CtcEval ctc_eval;

    CHECK(x[0] == Interval(6.,8.)); 
    CHECK(x[1] == Interval(4.5,7.));
    CHECK(x[2] == Interval(3.,6.));
    CHECK(x[3] == Interval(1.5,5.));
    CHECK(x[4] == Interval(-0.,4.));
    CHECK(x[5] == Interval(-1.5,3.));
    CHECK(x[6] == Interval(-3.,2.));
    CHECK(x[7] == Interval(-4.5,1.));
    CHECK(x[8] == Interval(-6.,0.));
    CHECK(x[9] == Interval(-7.5,-1.));
    CHECK(x[10] == Interval(-9.,-2.));

    bool contraction;
    Interval t, y;
    Tube x_raw(x);
    Tube v_raw(v);

    x = x_raw;
    v = v_raw;
    t = Interval(3.5,6.);
    y = Interval(3.5,6.);
    contraction = ctc_eval.contract(t, y, x, v);
    //CHECK(contraction);
    /*CHECK(t == Interval(3.5,5.));
    CHECK(y == Interval(3.5,4.5));
    CHECK(x[0] == Interval(6.,8.));
    CHECK(x[1] == Interval(4.5,7.));
    CHECK(x[2] == Interval(4.,6.));
    CHECK(x[3] == Interval(2.75,5.));
    CHECK(x[4] == Interval(1.25,4.));
    CHECK(x[5] == Interval(-0.25,3.));
    CHECK(x[6] == Interval(-3.,2.));
    CHECK(x[7] == Interval(-4.5,1.));
    CHECK(x[8] == Interval(-6.,0.));
    CHECK(x[9] == Interval(-7.5,-1.));
    CHECK(x[10] == Interval(-9.,-2.));

    x = x_raw;
    v = v_raw;
    t = Interval(3.,8.5);
    y = Interval(-7.,-2.5);
    contraction = ctc_eval.contract(t, y, x, v);
    //CHECK(contraction);
    CHECK(t == Interval(6.,8.5));
    CHECK(y == Interval(-5.25,-2.5));
    CHECK(x[0] == Interval(6.,8.)); 
    CHECK(x[1] == Interval(4.5,7.));
    CHECK(x[2] == Interval(3.,6.));
    CHECK(x[3] == Interval(1.5,5.));
    CHECK(x[4] == Interval(-0.,4.));
    CHECK(x[5] == Interval(-1.5,2.75));
    CHECK(x[6] == Interval(-3.,1.25));
    CHECK(x[7] == Interval(-4.5,-0.25));
    CHECK(x[8] == Interval(-6.,-1.75));
    CHECK(x[9] == Interval(-7.5,-3.));
    CHECK(x[10] == Interval(-9.,-2.));

    x = x_raw;
    v = v_raw;
    t = Interval(7.,8.);
    y = Interval(-2.,-1.);
    contraction = ctc_eval.contract(t, y, x, v);
    //CHECK(contraction);
    CHECK(t == Interval(7.,8.));
    CHECK(y == Interval(-2.,-1.));
    CHECK(x[0] == Interval(6.,8.)); 
    CHECK(x[1] == Interval(4.5,7.));
    CHECK(x[2] == Interval(3.,6.));
    CHECK(x[3] == Interval(1.5,5.));
    CHECK(x[4] == Interval(-0.,4.));
    CHECK(x[5] == Interval(-1.,3.));
    CHECK(x[6] == Interval(-2.,2.));
    CHECK(x[7] == Interval(-3.5,0.5));
    CHECK(x[8] == Interval(-5.,-1.));
    CHECK(x[9] == Interval(-6.5,-2.));
    CHECK(x[10] == Interval(-9.,-2.));

    x = x_raw;
    v = v_raw;
    t = Interval(0.);
    y = Interval(6.,8.);
    contraction = ctc_eval.contract(t, y, x, v);
    //CHECK(contraction);
    CHECK(t == Interval(0.));
    CHECK(y == Interval(7.,8.));
    CHECK(x[0] == Interval(6.,8.)); 
    CHECK(x[1] == Interval(4.5,7.));
    CHECK(x[2] == Interval(3.,6.));
    CHECK(x[3] == Interval(1.5,5.));
    CHECK(x[4] == Interval(-0.,4.));
    CHECK(x[5] == Interval(-1.5,3.));
    CHECK(x[6] == Interval(-3.,2.));
    CHECK(x[7] == Interval(-4.5,1.));
    CHECK(x[8] == Interval(-6.,0.));
    CHECK(x[9] == Interval(-7.5,-1.));
    CHECK(x[10] == Interval(-9.,-2.));/*
  }

  SECTION("Test CtcEval, non-zero derivative (positive case)")
  {
    Tube tube(Interval(0.,20.), 1.);
    Tube derivative(tube);
    derivative.set(Interval(1.,1.5));
    tube.set(Interval(-8.,-6.), 0);
    tube.ctcDeriv(derivative);

    CHECK(tube[0] == Interval(-8.,-6.)); 
    CHECK(tube[1] == Interval(-7.,-4.5)); 
    CHECK(tube[2] == Interval(-6.,-3.)); 
    CHECK(tube[3] == Interval(-5.,-1.5)); 
    CHECK(tube[4] == Interval(-4.,0.)); 
    CHECK(tube[5] == Interval(-3.,1.5)); 
    CHECK(tube[6] == Interval(-2.,3.)); 
    CHECK(tube[7] == Interval(-1.,4.5)); 
    CHECK(tube[8] == Interval(-0.,6.)); 
    CHECK(tube[9] == Interval(1.,7.5)); 
    CHECK(tube[10] == Interval(2.,9.)); 

    bool contraction;
    Interval intv_t, intv_y;
    Tube tube_raw(tube);

    intv_t = Interval(5.,10.);
    intv_y = Interval(-3.,-1.);
    contraction = tube.ctcEval(intv_t, intv_y, derivative, false);
    CHECK(contraction);
    CHECK(intv_t == Interval(5.,8.));
    CHECK(intv_y == Interval(-3.,-1.));
    CHECK(tube[0] == Interval(-8.,-6.)); 
    CHECK(tube[1] == Interval(-7.,-4.5)); 
    CHECK(tube[2] == Interval(-6.,-3.)); 
    CHECK(tube[3] == Interval(-5.,-2.)); 
    CHECK(tube[4] == Interval(-4.,-1.)); 
    CHECK(tube[5] == Interval(-3.,0.5)); 
    CHECK(tube[6] == Interval(-2.,2.)); 
    CHECK(tube[7] == Interval(-1.,3.5)); 
    CHECK(tube[8] == Interval(0.,5.)); 
    CHECK(tube[9] == Interval(1.,7.5)); 
    CHECK(tube[10] == Interval(2.,9.)); 

    tube = tube_raw;
    intv_t = Interval(2.,5.5);
    intv_y = Interval(-0.5,3.);
    contraction = tube.ctcEval(intv_t, intv_y, derivative, false);
    CHECK(contraction);
    CHECK(intv_t == Interval(4.,5.5));
    CHECK(intv_y == Interval(-0.5,0.75));
    CHECK(tube[0] == Interval(-8.,-6.)); 
    CHECK(tube[1] == Interval(-7.,-4.5)); 
    CHECK(tube[2] == Interval(-6.,-3.)); 
    CHECK(tube[3] == Interval(-4.25,-1.5)); 
    CHECK(tube[4] == Interval(-2.75,0.)); 
    CHECK(tube[5] == Interval(-1.25,1.5)); 
    CHECK(tube[6] == Interval(0.,3.)); 
    CHECK(tube[7] == Interval(-1.,4.5)); 
    CHECK(tube[8] == Interval(0.,6.));
    CHECK(tube[9] == Interval(1.,7.5)); 
    CHECK(tube[10] == Interval(2.,9.)); 

    tube = tube_raw;
    intv_t = Interval(3.5,7.);
    intv_y = Interval(-5.2,-4.8);
    contraction = tube.ctcEval(intv_t, intv_y, derivative, false);
    CHECK(contraction);
    CHECK(intv_t == Interval::EMPTY_SET);
    CHECK(intv_y == Interval::EMPTY_SET);
    CHECK(tube[0] == Interval(-8.,-6.)); 
    CHECK(tube[1] == Interval(-7.,-4.5)); 
    CHECK(tube[2] == Interval::EMPTY_SET); 
    CHECK(tube[3] == Interval::EMPTY_SET); 
    CHECK(tube[4] == Interval::EMPTY_SET); 
    CHECK(tube[5] == Interval(-3.,1.5)); 
    CHECK(tube[6] == Interval(-2.,3.)); 
    CHECK(tube[7] == Interval(-1.,4.5)); 
    CHECK(tube[8] == Interval(-0.,6.)); 
    CHECK(tube[9] == Interval(1.,7.5)); 
    CHECK(tube[10] == Interval(2.,9.));*/
  }
}