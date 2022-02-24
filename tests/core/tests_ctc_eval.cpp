#include <cstdio>
#include "catch_interval.hpp"
#include "codac_TFunction.h"
#include "codac_CtcEval.h"
#include "codac_CtcDeriv.h"
#include "codac_VIBesFigTube.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;

#define VIBES_DRAWING 0

TEST_CASE("CtcEval")
{
  SECTION("Test CtcEval, 1")
  {
    Interval t(0.,6.), z(-1.,1.);
    Tube x(Interval(-1.,7.), 2.);
    Tube v(x);
    v.set(Interval(-1.), 0);
    v.set(Interval(-1.,1.), 1);
    v.set(Interval(-1.), 2);
    v.set(Interval(1.), 3);

    CHECK(x.nb_slices() == 4);
    CtcEval ctc_eval;
    ctc_eval.preserve_slicing(false);
    ctc_eval.enable_time_propag(false);
    ctc_eval.contract(t, z, x, v);
    CHECK(x.nb_slices() == 6);

    CHECK(x.codomain() == Interval::ALL_REALS); // only gates should be affected
    CHECK(x(-1.) == Interval::ALL_REALS);
    CHECK(x(0.) == Interval(-2.,6.));
    CHECK(x(1.) == Interval(-3.,5.));
    CHECK(x(3.) == Interval(-4.,3.));
    CHECK(x(5.) == Interval(-6.,1.));
    CHECK(x(6.) == Interval(-5.,2.));
    CHECK(x(7.) == Interval::ALL_REALS);

    if(false & VIBES_DRAWING) // drawing results
    {
      vibes::beginDrawing();
      x &= Interval(-10.,10.);
      VIBesFigTube fig_tube("ctceval", &x);
      fig_tube.set_properties(100, 100, 500, 500);
      fig_tube.set_tube_derivative(&x, &v);
      fig_tube.show(true);
      vibes::endDrawing();
    }
  }

  SECTION("Test CtcEval, 2")
  {
    Interval t(0.,6.), z(-1.,1.);
    Tube x(Interval(-1.,7.), 2.);
    Tube v(x);
    v.set(Interval(-1.), 0);
    v.set(Interval(-1.,1.), 1);
    v.set(Interval(-1.), 2);
    v.set(Interval(1.), 3);

    x.set(Interval(0.,1.), 2.);
    v.sample(2.);

    CHECK(x.nb_slices() == 5);

    CtcEval ctc_eval;
    ctc_eval.preserve_slicing(false);
    ctc_eval.enable_time_propag(false);
    ctc_eval.contract(t, z, x, v);

    CHECK(x.nb_slices() == 7);

    CHECK(x.codomain() == Interval::ALL_REALS); // only gates should be affected
    CHECK(x(-1.) == Interval::ALL_REALS);
    CHECK(x(0.) == Interval(0.,3.));
    CHECK(x(1.) == Interval(-1.,2.));
    CHECK(x(2.) == Interval(0.,1.));
    CHECK(x(3.) == Interval(-1.,2.));
    CHECK(x(5.) == Interval(-3.,1.));
    CHECK(x(6.) == Interval(-2.,2.));
    CHECK(x(7.) == Interval::ALL_REALS);

    t = Interval(1.75,5.5);
    z = Interval(1.6);

    ctc_eval.enable_time_propag(true);
    ctc_eval.contract(t, z, x, v);

    VIBesFigTube *fig_tube;
    if(VIBES_DRAWING) // drawing results
    {
      vibes::beginDrawing();
      x &= Interval(-10.,10.);
      fig_tube = new VIBesFigTube("ctceval", &x);
      fig_tube->set_properties(1500, 100, 500, 500);
      fig_tube->set_tube_derivative(&x, &v);
      fig_tube->show(true);
    }

    IntervalVector box(2);
    box[0] = Interval(1.75,5.5);
    box[1] = Interval(1.6);

    if(VIBES_DRAWING) fig_tube->draw_box(box, vibesParams("figure", "ctceval", "red"));
    ctc_eval.contract(box[0], box[1], x, v);
    if(VIBES_DRAWING) fig_tube->draw_box(box, vibesParams("figure", "ctceval", "blue"));

    CHECK(ApproxIntv(x.interpol(3.4,v)) == Interval(0.8,1.6));
    CHECK(box[0] == Interval(2.6,3.4));
    CHECK(box[1] == Interval(1.6));

    if(VIBES_DRAWING)
    {
      delete fig_tube;
      vibes::endDrawing();
    }
  }
}

#define macro_init_special_cases() \
\
Tube xdot(Interval(0., 10.), 1.0); \
xdot.set(Interval(-0.5,1.)); \
\
Tube x(xdot); \
x.set(Interval::ALL_REALS); \
x.set(Interval(-1.5,1.), 4); \
x.set(Interval(-1.,1.5), 5); \
CHECK(x.nb_slices() == 10); \
\
CtcDeriv ctc_deriv; \
ctc_deriv.contract(x, xdot); \
\
Tube x_raw(x), xdot_raw(xdot); \
\
Interval intv_t, intv_y; \
CtcEval ctc_eval_propa, ctc_eval_nopropa; \
ctc_eval_propa.preserve_slicing(false); \
ctc_eval_nopropa.preserve_slicing(false); \
ctc_eval_propa.enable_time_propag(true); \
ctc_eval_nopropa.enable_time_propag(false); \

TEST_CASE("CtcEval (special cases, A)")
{
  SECTION("Test CtcEval, special cases (0, test init)")
  {
    macro_init_special_cases();

    // Checking the tube...
    CHECK(x_raw(0) == Interval(-5.5,3.));
    CHECK(x_raw(1) == Interval(-4.5,2.5));
    CHECK(x_raw(2) == Interval(-3.5,2.));
    CHECK(x_raw(3) == Interval(-2.5,1.5));
    CHECK(x_raw(4) == Interval(-1.5,1.));
    CHECK(x_raw(5) == Interval(-1.,1.5));
    CHECK(x_raw(6) == Interval(-1.5,2.5));
    CHECK(x_raw(7) == Interval(-2.,3.5));
    CHECK(x_raw(8) == Interval(-2.5,4.5));
    CHECK(x_raw(9) == Interval(-3.,5.5));
  }

  SECTION("Test CtcEval, special cases (case A no propa)")
  {
    macro_init_special_cases();

    // Test A (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = 1.;
    intv_y = Interval(-0.5,1.);
    CHECK(x(1.) == Interval(-4.5,2.5));
    ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(x.nb_slices() == 10);
    CHECK(intv_t == 1.);
    CHECK(intv_y == Interval(-0.5,1.));
    CHECK(x(0) == Interval(-5.5,3.));
    CHECK(x(1.) == intv_y);
    CHECK(x(1) == Interval(-4.5,2.5));
    CHECK(x(2) == Interval(-3.5,2.));
    CHECK(x(3) == Interval(-2.5,1.5));
    CHECK(x(4) == Interval(-1.5,1.));
    CHECK(x(5) == Interval(-1.,1.5));
    CHECK(x(6) == Interval(-1.5,2.5));
    CHECK(x(7) == Interval(-2.,3.5));
    CHECK(x(8) == Interval(-2.5,4.5));
    CHECK(x(9) == Interval(-3.,5.5));
  }

  SECTION("Test CtcEval, special cases (case A propa)")
  {
    macro_init_special_cases();
    
    // Test A (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = 1.;
    intv_y = Interval(-0.5,1.);
    CHECK(x(1.) == Interval(-4.5,2.5));
    ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(x.nb_slices() == 10);
    CHECK(intv_t == 1.);
    CHECK(intv_y == Interval(-0.5,1.));
    CHECK(x(0) == Interval(-1.5,1.5));
    CHECK(x(1.) == intv_y);
    CHECK(x(1) == Interval(-1.,2.));
    CHECK(x(2) == Interval(-1.5,2.));
    CHECK(ApproxIntv(x(3)) == Interval(-1.5-1./3.,1.5));
    CHECK(x(4) == Interval(-1.5,1.));
    CHECK(x(5) == Interval(-1.,1.5));
    CHECK(x(6) == Interval(-1.5,2.5));
    CHECK(x(7) == Interval(-2.,3.5));
    CHECK(x(8) == Interval(-2.5,4.5));
    CHECK(x(9) == Interval(-3.,5.5));
  }

  SECTION("Test CtcEval, special cases (case A, deriv)")
  {
    macro_init_special_cases();
    
    // Test A: ctc_deriv should not be effective after ctc_eval(true)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = 1.;
    intv_y = Interval(-0.5,1.);
    ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
  }
}

TEST_CASE("CtcEval (special cases, B)")
{
  SECTION("Test CtcEval, special cases (case B no propa)")
  {
    macro_init_special_cases();
    
    // Test B (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(0.5,2.5);
    intv_y = -2.;
    ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(x.nb_slices() == 12);
    CHECK(intv_t == Interval(0.5,2.5));
    CHECK(intv_y == -2.);
    CHECK(x(0) == Interval(-5.5,3.));
    CHECK(x(0.5) == Interval(-4.,-1.));
    CHECK(x(1) == Interval(-5.5,3.));
    CHECK(x(1.) == Interval(-3.5,-1.25));
    CHECK(x(2) == Interval(-4.5,2.5));
    CHECK(x(2.) == Interval(-2.75,-0.5));
    CHECK(x(3) == Interval(-3.5,2.));
    CHECK(x(2.5) == Interval(-3.,-0.));
    CHECK(x(4) == Interval(-3.5,2.));
    CHECK(x(5) == Interval(-2.5,1.5));
    CHECK(x(6) == Interval(-1.5,1.));
    CHECK(x(7) == Interval(-1.,1.5));
    CHECK(x(8) == Interval(-1.5,2.5));
    CHECK(x(9) == Interval(-2.,3.5));
    CHECK(x(10) == Interval(-2.5,4.5));
    CHECK(x(11) == Interval(-3.,5.5));
  }

  SECTION("Test CtcEval, special cases (case B propa)")
  {
    macro_init_special_cases();
    
    // Test B (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(0.5,2.5);
    intv_y = -2.;
    ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(x.nb_slices() == 12);
    CHECK(intv_t == Interval(0.5,2.5));
    CHECK(intv_y == -2.);
    CHECK(x(0) == Interval(-4.5,-0.75));
    CHECK(x(0.5) == Interval(-4.,-1.));
    CHECK(x(1) == Interval(-4.,-1.));
    CHECK(x(1.) == Interval(-3.5,-1.25));
    CHECK(x.slice(0.6)->tdomain() == Interval(0.5,1.));
    CHECK(x.slice(0.6)->input_gate() == Interval(-4.,-1.));
    CHECK(x.slice(0.6)->output_gate() == Interval(-3.5,-1.25));
    CHECK(xdot(2) == Interval(-0.5,1.));
    //CHECK(x(2) == Interval(-3.5,-0.5)); // todo: optimal implementation?
    CHECK(x(2.) == Interval(-2.75,-0.5));
    CHECK(x(3) == Interval(-3.,0.));
    CHECK(x(2.5) == Interval(-3.,-0.));
    CHECK(x(4) == Interval(-3.,0.5));
    CHECK(ApproxIntv(x(5)) == Interval(-2.5,1.+1./6.));
    CHECK(x(6) == Interval(-1.5,1.));
    CHECK(x(7) == Interval(-1.,1.5));
    CHECK(x(8) == Interval(-1.5,2.5));
    CHECK(x(9) == Interval(-2.,3.5));
    CHECK(x(10) == Interval(-2.5,4.5));
    CHECK(x(11) == Interval(-3.,5.5));
  }
}

TEST_CASE("CtcEval (special cases, C)")
{
  SECTION("Test CtcEval, special cases (case C no propa)")
  {
    macro_init_special_cases();
    
    // Test C (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(0.5,3.5);
    intv_y = -4.;
    ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(x.nb_slices() == 12);
    CHECK(intv_t == Interval(0.5,1.5));
    CHECK(intv_y == -4.);
    CHECK(x(0) == Interval(-5.5,3.));
    CHECK(x(0.5) == Interval(-5.,-3.5));
    CHECK(x(1) == Interval(-5.5,3.));
    CHECK(x(1.) == Interval(-4.5,-3.5));
    CHECK(x(2) == Interval(-4.5,2.5));
    CHECK(x(1.5) == Interval(-4.,-3.));
    CHECK(x(3) == Interval(-4.5,2.5));
    CHECK(x(4) == Interval(-3.5,2.));
    CHECK(x(5) == Interval(-2.5,1.5));
    CHECK(x(6) == Interval(-1.5,1.));
    CHECK(x(7) == Interval(-1.,1.5));
    CHECK(x(8) == Interval(-1.5,2.5));
    CHECK(x(9) == Interval(-2.,3.5));
    CHECK(x(10) == Interval(-2.5,4.5));
    CHECK(x(11) == Interval(-3.,5.5));
  }

  SECTION("Test CtcEval, special cases (case C propa)")
  {
    macro_init_special_cases();
    
    // Test C (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(0.5,3.5);
    intv_y = -4.;
    ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(x.nb_slices() == 12);
    CHECK(intv_t == Interval(0.5,1.5));
    CHECK(intv_y == -4.);
    CHECK(x(0) == Interval(-5.5,-3.25));
    CHECK(x(0.5) == Interval(-5.,-3.5));
    //CHECK(x(1) == Interval(-5.,-3.5)); // optimality
    CHECK(x(1.) == Interval(-4.5,-3.5));
    CHECK(x(2) == Interval(-4.5,-3.));
    CHECK(x(1.5) == Interval(-4.,-3.));
    CHECK(x(3) == Interval(-4.,-2.5));
    CHECK(x(4) == Interval(-3.5,-1.5));
    CHECK(x(5) == Interval(-2.5,-0.5));
    CHECK(x(6) == Interval(-1.5,0.5));
    CHECK(x(7) == Interval(-1.,1.5));
    CHECK(x(8) == Interval(-1.5,2.5));
    CHECK(x(9) == Interval(-2.,3.5));
    CHECK(x(10) == Interval(-2.5,4.5));
    CHECK(x(11) == Interval(-3.,5.5));

    if(false & VIBES_DRAWING) // drawing results
    {
      //x = x_raw;
      //xdot = xdot_raw;
      //IntervalVector box(2);
      //box[0] = Interval(0.5,3.5);
      //box[1] = Interval(-4.);
      //vibes::beginDrawing();
      //VIBesFigTube fig_tube("ctceval", &x);
      //fig_tube.set_properties(100, 100, 500, 500);
      //fig_tube.set_tube_derivative(&x, &xdot);
      //fig_tube.show(true);
      //contraction = ctc_eval_propa.contract(box[0], box[1], x, xdot);
      //fig_tube.show(true);
      //vibes::drawBox(box, vibesParams("figure", "ctceval", "blue"));
      //vibes::endDrawing();
    }
  }
}

TEST_CASE("CtcEval (special cases, D)")
{
  SECTION("Test CtcEval, special cases (case D no propa)")
  {
    macro_init_special_cases();
    
    // Test D (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = 3.5;
    intv_y = Interval(-3.5,-0.5);
    CHECK(x.nb_slices() == 10);
    ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(x.nb_slices() == 11);
    CHECK(intv_t == 3.5);
    CHECK(intv_y == Interval(-2.,-0.5));
    CHECK(x(0) == Interval(-5.5,3.));
    CHECK(x(1) == Interval(-4.5,2.5));
    CHECK(x(2) == Interval(-3.5,2.));
    CHECK(x(3) == Interval(-2.5,1.5));
    CHECK(x(3.5) == Interval(-2.,-0.5));
    CHECK(x(4) == Interval(-2.5,1.5));
    CHECK(x(5) == Interval(-1.5,1.));
    CHECK(x(6) == Interval(-1.,1.5));
    CHECK(x(7) == Interval(-1.5,2.5));
    CHECK(x(8) == Interval(-2.,3.5));
    CHECK(x(9) == Interval(-2.5,4.5));
    CHECK(x(10) == Interval(-3.,5.5));
  }

  SECTION("Test CtcEval, special cases (case D, propa)")
  {
    macro_init_special_cases();
    
    // Test D (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = 3.5;
    intv_y = Interval(-3.5,-0.5);
    CHECK(x.nb_slices() == 10);
    ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(x.nb_slices() == 11);
    CHECK(intv_t == 3.5);
    CHECK(intv_y == Interval(-2.,-0.5));
    CHECK(x(0) == Interval(-5.5,1.25));
    CHECK(x(1) == Interval(-4.5,0.75));
    CHECK(x(2) == Interval(-3.5,0.25));
    CHECK(x(3) == Interval(-2.5,-0.25));
    CHECK(x(3.5) == Interval(-2.,-0.5));
    CHECK(x(4) == Interval(-2.,0.));
    CHECK(x(5) == Interval(-1.5,1.));
    CHECK(x(6) == Interval(-1.,1.5));
    CHECK(x(7) == Interval(-1.5,2.5));
    CHECK(x(8) == Interval(-2.,3.5));
    CHECK(x(9) == Interval(-2.5,4.5));
    CHECK(x(10) == Interval(-3.,5.5));
  }
}

TEST_CASE("CtcEval (special cases, E)")
{
  SECTION("Test CtcEval, special cases (case E, no propa)")
  {
    macro_init_special_cases();
    
    // Test E (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(6.5, 8.5);
    intv_y = Interval(-4.5, -1.75);
     ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(x.nb_slices() == 12);
    CHECK(intv_t == Interval(7.5, 8.5));
    CHECK(intv_y == Interval(-2.25, -1.75));
    CHECK(x(0) == Interval(-5.5,3.));
    CHECK(x(1) == Interval(-4.5,2.5));
    CHECK(x(2) == Interval(-3.5,2.));
    CHECK(x(3) == Interval(-2.5,1.5));
    CHECK(x(4) == Interval(-1.5,1.));
    CHECK(x(5) == Interval(-1.,1.5));
    CHECK(x(6) == Interval(-1.5,2.5));
    CHECK(x(7) == Interval(-2.,3.5));
    CHECK(x(7.5) == Interval(-1.75,-1.25));
    CHECK(x(8) == Interval(-2.,3.5));
    CHECK(x(8.) == Interval(-2.,-1.25));
    CHECK(x(9) == Interval(-2.5,4.5));
    CHECK(x(10) == Interval(-2.5,4.5));
    CHECK(x(11) == Interval(-3.,5.5));
  }

  SECTION("Test CtcEval, special cases (case E, propa)")
  {
    macro_init_special_cases();
    
    // Test E (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(6.5, 8.5);
    intv_y = Interval(-4.5, -1.75);
    ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(x.nb_slices() == 12);
    CHECK(intv_t == Interval(7.5, 8.5));
    CHECK(intv_y == Interval(-2.25, -1.75));
    CHECK(x(0) == Interval(-5.5,2.5));
    CHECK(x(1) == Interval(-4.5,2.));
    CHECK(x(2) == Interval(-3.5,1.5));
    CHECK(x(3) == Interval(-2.5,1.));
    CHECK(x(4) == Interval(-1.5,0.5));
    CHECK(x(5) == Interval(-1.,0.));
    CHECK(x(6) == Interval(-1.5,-0.5));
    CHECK(x(7) == Interval(-1.75,-1.));
    CHECK(x(7.5) == Interval(-1.75,-1.25));
    //CHECK(x(8) == Interval(-2.,-1.25)); // optimality
    CHECK(x(8.) == Interval(-2.,-1.25));
    CHECK(x(9) == Interval(-2.25,-0.75));
    CHECK(x(10) == Interval(-2.5,-0.25));
    CHECK(x(11) == Interval(-3.,0.75));
  }
}

TEST_CASE("CtcEval (special cases, F)")
{
  SECTION("Test CtcEval, special cases (case F, no propa)")
  {
    macro_init_special_cases();
    
    // Test F (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(8.5,9.5);
    intv_y = Interval(-0.5,2.5);
    ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(x.nb_slices() == 12);
    CHECK(intv_t == Interval(8.5,9.5));
    CHECK(intv_y == Interval(-0.5,2.5));
    CHECK(x(0) == Interval(-5.5,3.));
    CHECK(x(1) == Interval(-4.5,2.5));
    CHECK(x(2) == Interval(-3.5,2.));
    CHECK(x(3) == Interval(-2.5,1.5));
    CHECK(x(4) == Interval(-1.5,1.));
    CHECK(x(5) == Interval(-1.,1.5));
    CHECK(x(6) == Interval(-1.5,2.5));
    CHECK(x(7) == Interval(-2.,3.5));
    CHECK(x(8) == Interval(-2.5,4.5));
    CHECK(x(8.5) == Interval(-1.5,3.));
    CHECK(x(9) == Interval(-2.5,4.5));
    CHECK(x(10) == Interval(-3.,5.5));
    CHECK(x(9.5) == Interval(-1.,3.5));
    CHECK(x(11) == Interval(-3,5.5));
  }

  SECTION("Test CtcEval, special cases (case F, propa)")
  {
    macro_init_special_cases();
    
    // Test F (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(8.5,9.5);
    intv_y = Interval(-0.5,2.5);
    ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(x.nb_slices() == 12);
    CHECK(intv_t == Interval(8.5,9.5));
    CHECK(intv_y == Interval(-0.5,2.5));
    CHECK(x(0) == Interval(-5.5,3.));
    CHECK(x(1) == Interval(-4.5,2.5));
    CHECK(x(2) == Interval(-3.5,2.));
    CHECK(x(3) == Interval(-2.5,1.5));
    CHECK(x(4) == Interval(-1.5,1.));
    CHECK(x(5) == Interval(-1.,1.5));
    CHECK(x(6) == Interval(-1.5,2.5));
    CHECK(ApproxIntv(x(7)) == Interval(-2.,3.+1./3.));
    CHECK(x(8) == Interval(-2.,3.25));
    CHECK(x(8.5) == Interval(-1.5,3.));
    //CHECK(x(9) == Interval(-1.5,3.)); // optimality
    //CHECK(x(10) == Interval(-1.,3.5)); // optimality
    CHECK(x(9.5) == Interval(-1.,3.5));
    CHECK(x(11) == Interval(-1.25,4.));
  }
}

TEST_CASE("CtcEval (special cases, G)")
{
  SECTION("Test CtcEval, special cases (case G, no propa)")
  {
    macro_init_special_cases();
    
    // Test G (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(6.5,7.5);
    intv_y = Interval(3.,4.);
    ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(intv_t == Interval(7.5));
    CHECK(intv_y == Interval(3.));
    CHECK(x(0) == Interval(-5.5,3.));
    CHECK(x(1) == Interval(-4.5,2.5));
    CHECK(x(2) == Interval(-3.5,2.));
    CHECK(x(3) == Interval(-2.5,1.5));
    CHECK(x(4) == Interval(-1.5,1.));
    CHECK(x(5) == Interval(-1.,1.5));
    CHECK(x(6) == Interval(-1.5,2.5));
    CHECK(x(7) == Interval(-2.,3.5));
    CHECK(x(7.5) == Interval(3.));
    CHECK(x(8) == Interval(-2.,3.5));
    CHECK(x(9) == Interval(-2.5,4.5));
    CHECK(x(10) == Interval(-3.,5.5));
  }

  SECTION("Test CtcEval, special cases (case G, propa)")
  {
    macro_init_special_cases();
    
    // Test G (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(6.5,7.5);
    intv_y = Interval(3.,4.);
    ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(intv_t == Interval(7.5));
    CHECK(intv_y == Interval(3.));
    CHECK(x(0) == Interval(-4.5,3.));
    CHECK(x(1) == Interval(-3.5,2.5));
    CHECK(x(2) == Interval(-2.5,2.));
    CHECK(x(3) == Interval(-1.5,1.5));
    CHECK(x(4) == Interval(-0.5,1.));
    CHECK(x(5) == Interval(0.5,1.5));
    CHECK(x(6) == Interval(1.5,2.5));
    CHECK(x(7) == Interval(2.5,3.));
    CHECK(x(7.5) == Interval(3.));
    CHECK(x(8) == Interval(2.75,3.5));
    CHECK(x(9) == Interval(2.25,4.5));
    CHECK(x(10) == Interval(1.75,5.5));
  }
}

TEST_CASE("CtcEval (special cases, H)")
{
  SECTION("Test CtcEval, special cases (case H, no propa)")
  {
    macro_init_special_cases();
    
    // Test H (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(7.,8.25);
    intv_y = Interval(4.25,5.);
    ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(intv_t == Interval::EMPTY_SET);
    CHECK(intv_y == Interval::EMPTY_SET);
    CHECK(x.nb_slices() == 10);
    CHECK(x(0) == Interval::EMPTY_SET);
    CHECK(x(1) == Interval::EMPTY_SET);
    CHECK(x(2) == Interval::EMPTY_SET);
    CHECK(x(3) == Interval::EMPTY_SET);
    CHECK(x(4) == Interval::EMPTY_SET);
    CHECK(x(5) == Interval::EMPTY_SET);
    CHECK(x(6) == Interval::EMPTY_SET);
    CHECK(x(7) == Interval::EMPTY_SET);
    CHECK(x(8) == Interval::EMPTY_SET);
    CHECK(x(9) == Interval::EMPTY_SET);
    CHECK(x.is_empty());
  }

  SECTION("Test CtcEval, special cases (case H, propa)")
  {
    macro_init_special_cases();
    
    // Test H (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(7.,8.25);
    intv_y = Interval(4.25,5.);
    ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(intv_t == Interval::EMPTY_SET);
    CHECK(intv_y == Interval::EMPTY_SET);
    CHECK(x.nb_slices() == 10);
    CHECK(x(0) == Interval::EMPTY_SET);
    CHECK(x(1) == Interval::EMPTY_SET);
    CHECK(x(2) == Interval::EMPTY_SET);
    CHECK(x(3) == Interval::EMPTY_SET);
    CHECK(x(4) == Interval::EMPTY_SET);
    CHECK(x(5) == Interval::EMPTY_SET);
    CHECK(x(6) == Interval::EMPTY_SET);
    CHECK(x(7) == Interval::EMPTY_SET);
    CHECK(x(8) == Interval::EMPTY_SET);
    CHECK(x(9) == Interval::EMPTY_SET);
    CHECK(x.is_empty());
  }
}

TEST_CASE("CtcEval (special cases, I)")
{
  SECTION("Test CtcEval, special cases (case I, no propa)")
  {
    macro_init_special_cases();
    
    // Test I (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = 2.5;
    intv_y = 0.5;
    ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(intv_t == Interval(2.5));
    CHECK(intv_y == Interval(0.5));
    CHECK(x.nb_slices() == 11);
    CHECK(x(0) == Interval(-5.5,3.));
    CHECK(x(1) == Interval(-4.5,2.5));
    CHECK(x(2) == Interval(-3.5,2.));
    CHECK(x(2.5) == Interval(0.5));
    CHECK(x(3) == Interval(-3.5,2.));
    CHECK(x(4) == Interval(-2.5,1.5));
    CHECK(x(5) == Interval(-1.5,1.));
    CHECK(x(6) == Interval(-1.,1.5));
    CHECK(x(7) == Interval(-1.5,2.5));
    CHECK(x(8) == Interval(-2.,3.5));
    CHECK(x(9) == Interval(-2.5,4.5));
    CHECK(x(10) == Interval(-3.,5.5));
  }

  SECTION("Test CtcEval, special cases (case I, propa)")
  {
    macro_init_special_cases();
    
    // Test I (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = 2.5;
    intv_y = 0.5;
    ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(intv_t == Interval(2.5));
    CHECK(intv_y == Interval(0.5));
    CHECK(x.nb_slices() == 11);
    CHECK(x(0) == Interval(-2.,1.75));
    CHECK(x(1) == Interval(-1.,1.25));
    CHECK(x(2) == Interval(-0.,0.75));
    CHECK(x(2.5) == Interval(0.5));
    CHECK(x(3) == Interval(0.25,1.));
    CHECK(ApproxIntv(x(4)) == Interval(-0.25,1.+1./3.));
    CHECK(x(5) == Interval(-0.75,1.));
    CHECK(x(6) == Interval(-1.,1.5));
    CHECK(x(7) == Interval(-1.5,2.5));
    CHECK(x(8) == Interval(-2.,3.5));
    CHECK(x(9) == Interval(-2.5,4.5));
    CHECK(x(10) == Interval(-3.,5.5));
  }
}

TEST_CASE("CtcEval (special cases, J)")
{
  SECTION("Test CtcEval, special cases (case J, no propa)")
  {
    macro_init_special_cases();
    
    // Test J (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = 6.25;
    intv_y = Interval(0.5,1.);
    ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(intv_t == 6.25);
    CHECK(intv_y == Interval(0.5,1.));
    CHECK(x.nb_slices() == 11);
    CHECK(x(0) == Interval(-5.5,3.));
    CHECK(x(1) == Interval(-4.5,2.5));
    CHECK(x(2) == Interval(-3.5,2.));
    CHECK(x(3) == Interval(-2.5,1.5));
    CHECK(x(4) == Interval(-1.5,1.));
    CHECK(x(5) == Interval(-1.,1.5));
    CHECK(x(6) == Interval(-1.5,2.5));
    CHECK(x(6.25) == Interval(0.5,1.));
    CHECK(x(7) == Interval(-1.5,2.5));
    CHECK(x(8) == Interval(-2.,3.5));
    CHECK(x(9) == Interval(-2.5,4.5));
    CHECK(x(10) == Interval(-3.,5.5));
  }

  SECTION("Test CtcEval, special cases (case J, propa)")
  {
    macro_init_special_cases();
    
    // Test J (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = 6.25;
    intv_y = Interval(0.5,1.);
    ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(intv_t == 6.25);
    CHECK(intv_y == Interval(0.5,1.));
    CHECK(x.nb_slices() == 11);
    CHECK(x(0) == Interval(-5.5,3.));
    CHECK(x(1) == Interval(-4.5,2.5));
    CHECK(x(2) == Interval(-3.5,2.));
    CHECK(x(3) == Interval(-2.5,1.5));
    CHECK(x(4) == Interval(-1.5,1.));
    CHECK(x(5.) == Interval(-0.75,1.));
    CHECK(ApproxIntv(x(5)) == Interval(-0.75,1.4+1./60.));
    CHECK(x(6) == Interval(0.25,1.125));
    CHECK(x(7) == Interval(0.125,1.75));
    CHECK(x(8) == Interval(0.125-0.5,2.75));
    CHECK(x(9) == Interval(0.125-1.,3.75));
    CHECK(x(10) == Interval(0.125-1.5,4.75));
  }
}

TEST_CASE("CtcEval (special cases, K)")
{
  SECTION("Test CtcEval, special cases (case K, no propa)")
  {
    macro_init_special_cases();
    
    // Test K (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(5.25, 8.25);
    intv_y = Interval(-0.5, 0.);
    ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(intv_t == Interval(5.25, 8.25));
    CHECK(intv_y == Interval(-0.5, 0.));
    CHECK(x.nb_slices() == 12);
    CHECK(x(0) == Interval(-5.5,3.));
    CHECK(x(1) == Interval(-4.5,2.5));
    CHECK(x(2) == Interval(-3.5,2.));
    CHECK(x(3) == Interval(-2.5,1.5));
    CHECK(x(4) == Interval(-1.5,1.));
    CHECK(x(5) == Interval(-1.,1.5));
    CHECK(x(5.25) == Interval(-1.,1.25));
    CHECK(x(6) == Interval(-1.,1.5));
    CHECK(x(7) == Interval(-1.5,2.5));
    CHECK(x(8) == Interval(-2.,3.5));
    CHECK(x(9) == Interval(-2.5,4.5));
    CHECK(x(8.25) == Interval(-2.,3.));
    CHECK(x(10) == Interval(-2.5,4.5));
    CHECK(x(11) == Interval(-3.,5.5));
  }

  SECTION("Test CtcEval, special cases (case K, propa)")
  {
    macro_init_special_cases();
    
    // Test K (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(5.25, 8.25);
    intv_y = Interval(-0.5, 0.);
    ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(intv_t == Interval(5.25, 8.25));
    CHECK(intv_y == Interval(-0.5, 0.));
    CHECK(x.nb_slices() == 12);
    CHECK(x(0) == Interval(-5.5,3.));
    CHECK(x(1) == Interval(-4.5,2.5));
    CHECK(x(2) == Interval(-3.5,2.));
    CHECK(x(3) == Interval(-2.5,1.5));
    CHECK(x(4) == Interval(-1.5,1.));
    CHECK(x(5) == Interval(-1.,1.25));
    CHECK(x(5.25) == Interval(-1.,1.25));
    CHECK(ApproxIntv(x(6)) == Interval(-1.,1.4+1./60.));
    CHECK(x(7) == Interval(-1.5,1.875));
    CHECK(ApproxIntv(x(8)) == Interval(-1.625-1./3.,2.75));
    CHECK(x(9) == Interval(-2.,3.));
    CHECK(x(8.25) == Interval(-2.,3.));
    CHECK(x(10) == Interval(-2.375,3.75));
    CHECK(x(11) == Interval(-2.875,4.75));
  }
}

TEST_CASE("CtcEval (special cases, L)")
{
  SECTION("Test CtcEval, special cases (case L, no propa)")
  {
    macro_init_special_cases();
    
    // Test L (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(1.5,4.5);
    intv_y = Interval(1.75,2.75);
    ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(intv_t == Interval(1.5,2.5));
    CHECK(intv_y == Interval(1.75,2.25));
    CHECK(x.nb_slices() == 12);
    CHECK(x(0) == Interval(-5.5,3.));
    CHECK(x(1) == Interval(-4.5,2.5));
    CHECK(x(1.5) == Interval(0.75,2.25));
    CHECK(x(2) == Interval(-4.5,2.5));
    CHECK(x(3) == Interval(-3.5,2.));
    CHECK(x(2.5) == Interval(1.25,1.75));
    CHECK(x(4) == Interval(-3.5,2.));
    CHECK(x(5) == Interval(-2.5,1.5));
    CHECK(x(6) == Interval(-1.5,1.));
    CHECK(x(7) == Interval(-1.,1.5));
    CHECK(x(8) == Interval(-1.5,2.5));
    CHECK(x(9) == Interval(-2.,3.5));
    CHECK(x(10) == Interval(-2.5,4.5));
    CHECK(x(11) == Interval(-3.,5.5));
  }

  SECTION("Test CtcEval, special cases (case L, propa)")
  {
    macro_init_special_cases();
    
    // Test L (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(1.5,2.5);
    intv_y = Interval(1.75,2.75);
    ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(intv_t == Interval(1.5,2.5));
    CHECK(intv_y == Interval(1.75,2.25));
    CHECK(x.nb_slices() == 12);
    CHECK(x(0) == Interval(-0.75,3.));
    CHECK(x(1) == Interval(0.25,2.5));
    CHECK(x(1.5) == Interval(0.75,2.25));
    CHECK(x(2) == Interval(0.75,2.25));
    //CHECK(x(3) == Interval(1.25,2.)); // optimality
    CHECK(x(2.5) == Interval(1.25,1.75));
    CHECK(x(4) == Interval(1.,1.75));
    CHECK(x(5) == Interval(0.5,1.5));
    CHECK(x(6) == Interval(0.,1.));
    CHECK(x(7) == Interval(-0.5,1.5));
    CHECK(x(8) == Interval(-1.,2.5));
    CHECK(x(9) == Interval(-1.5,3.5));
    CHECK(x(10) == Interval(-2.,4.5));
    CHECK(x(11) == Interval(-2.5,5.5));
  }
}

TEST_CASE("CtcEval (special cases, M)")
{
  SECTION("Test CtcEval, special cases (case M, no propa)")
  {
    macro_init_special_cases();
    
    // Test M (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(0.75,1.25);
    intv_y = Interval(-5.75,-5.25);
    ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(intv_t == Interval::EMPTY_SET);
    CHECK(intv_y == Interval::EMPTY_SET);
    CHECK(x.nb_slices() == 10);
    CHECK(x(0) == Interval::EMPTY_SET);
    CHECK(x(1) == Interval::EMPTY_SET);
    CHECK(x(2) == Interval::EMPTY_SET);
    CHECK(x(3) == Interval::EMPTY_SET);
    CHECK(x(4) == Interval::EMPTY_SET);
    CHECK(x(5) == Interval::EMPTY_SET);
    CHECK(x(6) == Interval::EMPTY_SET);
    CHECK(x(7) == Interval::EMPTY_SET);
    CHECK(x(8) == Interval::EMPTY_SET);
    CHECK(x(9) == Interval::EMPTY_SET);
    CHECK(x.is_empty());
  }

  SECTION("Test CtcEval, special cases (case M, propa)")
  {
    macro_init_special_cases();
    
    // Test M (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(0.75,1.25);
    intv_y = Interval(-5.75,-5.25);
    ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(intv_t == Interval::EMPTY_SET);
    CHECK(intv_y == Interval::EMPTY_SET);
    CHECK(x.nb_slices() == 10);
    CHECK(x(0) == Interval::EMPTY_SET);
    CHECK(x(1) == Interval::EMPTY_SET);
    CHECK(x(2) == Interval::EMPTY_SET);
    CHECK(x(3) == Interval::EMPTY_SET);
    CHECK(x(4) == Interval::EMPTY_SET);
    CHECK(x(5) == Interval::EMPTY_SET);
    CHECK(x(6) == Interval::EMPTY_SET);
    CHECK(x(7) == Interval::EMPTY_SET);
    CHECK(x(8) == Interval::EMPTY_SET);
    CHECK(x(9) == Interval::EMPTY_SET);
    CHECK(x.is_empty());
  }
}

TEST_CASE("CtcEval (special cases, N)")
{
  SECTION("Test CtcEval, special cases (case N, no propa)")
  {
    macro_init_special_cases();
    
    // Test N (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(5.5,8.5);
    intv_y = Interval(2.,5.5);
    ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(intv_t == Interval(6.5,8.5));
    CHECK(intv_y == Interval(2.,4.));
    CHECK(x.nb_slices() == 12);
    CHECK(x(0) == Interval(-5.5,3.));
    CHECK(x(1) == Interval(-4.5,2.5));
    CHECK(x(2) == Interval(-3.5,2.));
    CHECK(x(3) == Interval(-2.5,1.5));
    CHECK(x(4) == Interval(-1.5,1.));
    CHECK(x(5) == Interval(-1.,1.5));
    CHECK(x(6) == Interval(-1.5,2.5));
    CHECK(x(6.5) == Interval(0.,2.));
    CHECK(x(7) == Interval(-1.5,2.5));
    CHECK(x(8) == Interval(-2.,3.5));
    CHECK(x(9) == Interval(-2.5,4.5));
    CHECK(x(8.5) == Interval(1.,4.));
    CHECK(x(10) == Interval(-2.5,4.5));
    CHECK(x(11) == Interval(-3.,5.5));
  }

  SECTION("Test CtcEval, special cases (case N, propa)")
  {
    macro_init_special_cases();
    
    // Test N (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(5.5,8.5);
    intv_y = Interval(2.,5.5);
    ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(intv_t == Interval(6.5,8.5));
    CHECK(intv_y == Interval(2.,4.));
    CHECK(x.nb_slices() == 12);
    CHECK(x(0) == Interval(-5.5,3.));
    CHECK(x(1) == Interval(-4.5,2.5));
    CHECK(x(2) == Interval(-3.5,2.));
    CHECK(x(3) == Interval(-2.5,1.5));
    CHECK(x(4) == Interval(-1.5,1.));
    CHECK(x(5) == Interval(-1.,1.5));
    CHECK(x(6) == Interval(-0.5,2.));
    CHECK(x(6.5) == Interval(0.,2.));
    CHECK(x(7) == Interval(0.,2.5));
    //CHECK(x(8) == Interval(xxx)); // optimality
    CHECK(x(9) == Interval(1.,4.));
    CHECK(x(8.5) == Interval(1.,4.));
    CHECK(x(10) == Interval(0.75,4.5));
    CHECK(x(11) == Interval(0.25,5.5));
  }
}

TEST_CASE("CtcEval (special cases, O)")
{
  SECTION("Test CtcEval, special cases (case O, no propa)")
  {
    macro_init_special_cases();
    
    // Test O (no propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(2.5,9.5);
    intv_y = Interval(-5.,-2.5);
    ctc_eval_nopropa.contract(intv_t, intv_y, x, xdot);
    CHECK(intv_t == Interval(2.5,9.5));
    CHECK(intv_y == Interval(-3.,-2.5));
    CHECK(x.nb_slices() == 12);
    CHECK(x(0) == Interval(-5.5,3.));
    CHECK(x(1) == Interval(-4.5,2.5));
    CHECK(x(2) == Interval(-3.5,2.));
    CHECK(x(2.5) == Interval(-3.,1.));
    CHECK(x(3) == Interval(-3.5,2.));
    CHECK(x(4) == Interval(-2.5,1.5));
    CHECK(x(5) == Interval(-1.5,1.));
    CHECK(x(6) == Interval(-1.,1.5));
    CHECK(x(7) == Interval(-1.5,2.5));
    CHECK(x(8) == Interval(-2.,3.5));
    CHECK(x(9) == Interval(-2.5,4.5));
    CHECK(x(10) == Interval(-3.,5.5));
    CHECK(x(9.5) == Interval(-2.75,4.5));
    CHECK(x(11) == Interval(-3.,5.5));
  }

  SECTION("Test CtcEval, special cases (case O, propa)")
  {
    macro_init_special_cases();
    
    // Test 0 (propa)
    x = x_raw;
    xdot = xdot_raw;
    intv_t = Interval(2.5,9.5);
    intv_y = Interval(-5.,-2.5);
    ctc_eval_propa.contract(intv_t, intv_y, x, xdot);
    CHECK(intv_t == Interval(2.5,9.5));
    CHECK(intv_y == Interval(-3.,-2.5));
    CHECK(x.nb_slices() == 12);
    CHECK(x(0) == Interval(-5.5,2.25));
    CHECK(x(1) == Interval(-4.5,1.75));
    CHECK(x(2) == Interval(-3.5,1.25));
    CHECK(x(2.5) == Interval(-3.,1.));
    CHECK(x(3) == Interval(-3.,1.));
    CHECK(x(4) == Interval(-2.5,0.75));
    //CHECK(x(5) == Interval(xxx)); // optimality
    CHECK(x(6) == Interval(-1.,1.));
    CHECK(x(7) == Interval(-1.5,2.));
    CHECK(x(8) == Interval(-2.,3.));
    CHECK(x(9) == Interval(-2.5,4.));
    CHECK(x(10) == Interval(-2.75,4.5));
    CHECK(x(9.5) == Interval(-2.75,4.5));
    CHECK(x(11) == Interval(-3.,5.));
  }
}

TEST_CASE("CtcEval (other tests)")
{
  SECTION("Test CtcEval, multi eval")
  {
    Tube x(Interval(0.,20.), 0.1, TFunction("cos(t)+t*[-0.1,0.2]"));
    Tube v(Interval(0.,20.), 0.1, TFunction("-sin(t)+[-0.1,0.2]"));
    
    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, v);

    IntervalVector box(2);
    CtcEval ctc_eval;
    ctc_eval.enable_time_propag(true);
    Tube x_c(x); Tube v_c(v);

    box[0] = Interval(11.98);
    box[1] = Interval(1.);
    ctc_eval.contract(box[0], box[1], x_c, v_c);

    box[0] = Interval(6.5);
    box[1] = Interval(1.);
    ctc_eval.contract(box[0], box[1], x_c, v_c);

    if(VIBES_DRAWING) // drawing results
    {
      //vibes::beginDrawing();
      //VIBesFigTube fig_tube("ctceval", &x);
      //fig_tube.set_properties(100, 100, 800, 400);
      //fig_tube.set_tube_derivative(&x, &v);
      //fig_tube.show(true);
      //x = x_c;
      //v = v_c;
      //fig_tube.show(true);
      //vibes::drawBox(box, vibesParams("figure", "ctceval", "red"));
    }
  }

  SECTION("Test CtcEval, non-zero derivative (negative case)")
  {
    Tube x(Interval(0.,11.), 1.);
    Tube v(x);
    v.set(Interval(-1.5,-1.));
    x.set(Interval(6.,8.), 0);

    CtcEval ctc_eval;
    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, v);

    CHECK(x(0) == Interval(6.,8.)); 
    CHECK(x(1) == Interval(4.5,7.));
    CHECK(x(2) == Interval(3.,6.));
    CHECK(x(3) == Interval(1.5,5.));
    CHECK(x(4) == Interval(-0.,4.));
    CHECK(x(5) == Interval(-1.5,3.));
    CHECK(x(6) == Interval(-3.,2.));
    CHECK(x(7) == Interval(-4.5,1.));
    CHECK(x(8) == Interval(-6.,0.));
    CHECK(x(9) == Interval(-7.5,-1.));
    CHECK(x(10) == Interval(-9.,-2.));

    Interval t, y;
    Tube x_raw(x);
    Tube v_raw(v);

    x = x_raw;
    v = v_raw;
    t = Interval(3.5,6.);
    y = Interval(3.5,6.);
    ctc_eval.contract(t, y, x, v);
    /*CHECK(t == Interval(3.5,5.));
    CHECK(y == Interval(3.5,4.5));
    CHECK(x(0) == Interval(6.,8.));
    CHECK(x(1) == Interval(4.5,7.));
    CHECK(x(2) == Interval(4.,6.));
    CHECK(x(3) == Interval(2.75,5.));
    CHECK(x(4) == Interval(1.25,4.));
    CHECK(x(5) == Interval(-0.25,3.));
    CHECK(x(6) == Interval(-3.,2.));
    CHECK(x(7) == Interval(-4.5,1.));
    CHECK(x(8) == Interval(-6.,0.));
    CHECK(x(9) == Interval(-7.5,-1.));
    CHECK(x(10) == Interval(-9.,-2.));

    x = x_raw;
    v = v_raw;
    t = Interval(3.,8.5);
    y = Interval(-7.,-2.5);
    ctc_eval.contract(t, y, x, v);
    CHECK(t == Interval(6.,8.5));
    CHECK(y == Interval(-5.25,-2.5));
    CHECK(x(0) == Interval(6.,8.)); 
    CHECK(x(1) == Interval(4.5,7.));
    CHECK(x(2) == Interval(3.,6.));
    CHECK(x(3) == Interval(1.5,5.));
    CHECK(x(4) == Interval(-0.,4.));
    CHECK(x(5) == Interval(-1.5,2.75));
    CHECK(x(6) == Interval(-3.,1.25));
    CHECK(x(7) == Interval(-4.5,-0.25));
    CHECK(x(8) == Interval(-6.,-1.75));
    CHECK(x(9) == Interval(-7.5,-3.));
    CHECK(x(10) == Interval(-9.,-2.));

    x = x_raw;
    v = v_raw;
    t = Interval(7.,8.);
    y = Interval(-2.,-1.);
    ctc_eval.contract(t, y, x, v);
    CHECK(t == Interval(7.,8.));
    CHECK(y == Interval(-2.,-1.));
    CHECK(x(0) == Interval(6.,8.)); 
    CHECK(x(1) == Interval(4.5,7.));
    CHECK(x(2) == Interval(3.,6.));
    CHECK(x(3) == Interval(1.5,5.));
    CHECK(x(4) == Interval(-0.,4.));
    CHECK(x(5) == Interval(-1.,3.));
    CHECK(x(6) == Interval(-2.,2.));
    CHECK(x(7) == Interval(-3.5,0.5));
    CHECK(x(8) == Interval(-5.,-1.));
    CHECK(x(9) == Interval(-6.5,-2.));
    CHECK(x(10) == Interval(-9.,-2.));

    x = x_raw;
    v = v_raw;
    t = Interval(0.);
    y = Interval(6.,8.);
    ctc_eval.contract(t, y, x, v);
    CHECK(t == Interval(0.));
    CHECK(y == Interval(7.,8.));
    CHECK(x(0) == Interval(6.,8.)); 
    CHECK(x(1) == Interval(4.5,7.));
    CHECK(x(2) == Interval(3.,6.));
    CHECK(x(3) == Interval(1.5,5.));
    CHECK(x(4) == Interval(-0.,4.));
    CHECK(x(5) == Interval(-1.5,3.));
    CHECK(x(6) == Interval(-3.,2.));
    CHECK(x(7) == Interval(-4.5,1.));
    CHECK(x(8) == Interval(-6.,0.));
    CHECK(x(9) == Interval(-7.5,-1.));
    CHECK(x(10) == Interval(-9.,-2.));
  }

  SECTION("Test CtcEval, non-zero derivative (positive case)")
  {
    Tube tube(Interval(0.,20.), 1.);
    Tube derivative(tube);
    derivative.set(Interval(1.,1.5));
    tube.set(Interval(-8.,-6.), 0);
    tube.ctc_deriv(derivative);

    CHECK(tube(0) == Interval(-8.,-6.)); 
    CHECK(tube(1) == Interval(-7.,-4.5)); 
    CHECK(tube(2) == Interval(-6.,-3.)); 
    CHECK(tube(3) == Interval(-5.,-1.5)); 
    CHECK(tube(4) == Interval(-4.,0.)); 
    CHECK(tube(5) == Interval(-3.,1.5)); 
    CHECK(tube(6) == Interval(-2.,3.)); 
    CHECK(tube(7) == Interval(-1.,4.5)); 
    CHECK(tube(8) == Interval(-0.,6.)); 
    CHECK(tube(9) == Interval(1.,7.5)); 
    CHECK(tube(10) == Interval(2.,9.)); 

    Interval intv_t, intv_y;
    Tube tube_raw(tube);

    intv_t = Interval(5.,10.);
    intv_y = Interval(-3.,-1.);
    tube.ctc_eval(intv_t, intv_y, derivative, false);
    CHECK(intv_t == Interval(5.,8.));
    CHECK(intv_y == Interval(-3.,-1.));
    CHECK(tube(0) == Interval(-8.,-6.)); 
    CHECK(tube(1) == Interval(-7.,-4.5)); 
    CHECK(tube(2) == Interval(-6.,-3.)); 
    CHECK(tube(3) == Interval(-5.,-2.)); 
    CHECK(tube(4) == Interval(-4.,-1.)); 
    CHECK(tube(5) == Interval(-3.,0.5)); 
    CHECK(tube(6) == Interval(-2.,2.)); 
    CHECK(tube(7) == Interval(-1.,3.5)); 
    CHECK(tube(8) == Interval(0.,5.)); 
    CHECK(tube(9) == Interval(1.,7.5)); 
    CHECK(tube(10) == Interval(2.,9.)); 

    tube = tube_raw;
    intv_t = Interval(2.,5.5);
    intv_y = Interval(-0.5,3.);
    tube.ctc_eval(intv_t, intv_y, derivative, false);
    CHECK(intv_t == Interval(4.,5.5));
    CHECK(intv_y == Interval(-0.5,0.75));
    CHECK(tube(0) == Interval(-8.,-6.)); 
    CHECK(tube(1) == Interval(-7.,-4.5)); 
    CHECK(tube(2) == Interval(-6.,-3.)); 
    CHECK(tube(3) == Interval(-4.25,-1.5)); 
    CHECK(tube(4) == Interval(-2.75,0.)); 
    CHECK(tube(5) == Interval(-1.25,1.5)); 
    CHECK(tube(6) == Interval(0.,3.)); 
    CHECK(tube(7) == Interval(-1.,4.5)); 
    CHECK(tube(8) == Interval(0.,6.));
    CHECK(tube(9) == Interval(1.,7.5)); 
    CHECK(tube(10) == Interval(2.,9.)); 

    tube = tube_raw;
    intv_t = Interval(3.5,7.);
    intv_y = Interval(-5.2,-4.8);
    tube.ctc_eval(intv_t, intv_y, derivative, false);
    CHECK(intv_t == Interval::EMPTY_SET);
    CHECK(intv_y == Interval::EMPTY_SET);
    CHECK(tube(0) == Interval(-8.,-6.)); 
    CHECK(tube(1) == Interval(-7.,-4.5)); 
    CHECK(tube(2) == Interval::EMPTY_SET); 
    CHECK(tube(3) == Interval::EMPTY_SET); 
    CHECK(tube(4) == Interval::EMPTY_SET); 
    CHECK(tube(5) == Interval(-3.,1.5)); 
    CHECK(tube(6) == Interval(-2.,3.)); 
    CHECK(tube(7) == Interval(-1.,4.5)); 
    CHECK(tube(8) == Interval(-0.,6.)); 
    CHECK(tube(9) == Interval(1.,7.5)); 
    CHECK(tube(10 == Interval(2.,9.));*/
  }

  SECTION("Test CtcEval, outside tdomain")
  {
    Tube y(Interval(2,3), 0.1, Interval(4,5));
    CtcEval ctc_eval;

    {
      Interval t(-3,5), z(6.);
      ctc_eval.contract(t, z, y);
      CHECK(t == Interval(-3,5));
      CHECK(z == Interval(6));
      CHECK(y.codomain() == Interval(4,5));
    }

    {
      Interval t(2.5,7.), z;
      ctc_eval.contract(t, z, y);
      CHECK(t == Interval(2.5,7.));
      CHECK(z == Interval::all_reals());
      CHECK(y.codomain() == Interval(4,5));
    }
  }

  SECTION("Test CtcEval, dependency on t")
  {
    double dt = 0.1;
    Interval tdomain(0,10);
    Interval t(0,10);
    IntervalVector b({{0,2},{0,2}});
    TubeVector x(tdomain, dt, TFunction("(sin(t) ; -sin(t))"));
    CtcEval ctc_eval;
    ctc_eval.contract(t, b, x);
    CHECK(b.contains(Vector(2,0.)));
    CHECK(b.max_diam() < 0.02);
    CHECK(t == Interval(x[0].slice(0.)->tdomain().lb(),x[0].slice(3.*M_PI)->tdomain().ub()));
  }
}