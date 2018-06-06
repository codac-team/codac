#include "tests.h"
#include <cstdio>

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

TEST_CASE("CtcEval")
{
  SECTION("Test CtcEval, special cases")
  {
    Tube xdot(Interval(0., 10.), 1.0);
    xdot.set(Interval(-0.5,1.));

    Tube x(xdot);
    x.set(Interval::ALL_REALS);
    x.set(Interval(-1.5,1.), 4);
    x.set(Interval(-1.,1.5), 5);
    x.ctcFwdBwd(xdot);
    Tube x_raw(x);

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

    // Test A
    x = x_raw;
    intv_t = 1.;
    intv_y = Interval(-0.5,1.);
    CHECK(x[1.] == Interval(-4.5,2.5));
    contraction = x.ctcEval(intv_t, intv_y, xdot, false);
    CHECK(contraction);
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

    // Test A propag
    x = x_raw;
    intv_t = 1.;
    intv_y = Interval(-0.5,1.);
    CHECK(x[1.] == Interval(-4.5,2.5));
    contraction = x.ctcEval(intv_t, intv_y, xdot, true);
    CHECK(contraction);
    CHECK(intv_t == 1.);
    CHECK(intv_y == Interval(-0.5,1.));
    CHECK(x[0.] == Interval(-1.5,1.5));
    CHECK(x[0] == Interval(-1.5,1.5));
    CHECK(x[1.] == intv_y);
    CHECK(x[1] == Interval(-1.,2.));
    CHECK(x[2.] == Interval(-1.,2.));
    CHECK(x[2] == Interval(-1.5,2.));
    CHECK(x[3.] == Interval(-1.5,1.5));
    CHECK(ApproxIntv(x[3]) == Interval(-1.83333333,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(-1.5,2.5));
    CHECK(x[7] == Interval(-2.,3.5));
    CHECK(x[8] == Interval(-2.5,4.5));
    CHECK(x[9] == Interval(-3.,5.5));

    // Test A: ctcFwdBwd should not be effective after ctcEval(true)
    x = x_raw;
    intv_t = 1.;
    intv_y = Interval(-0.5,1.);
    contraction = x.ctcEval(intv_t, intv_y, xdot, true);
    CHECK(contraction);
    CHECK(intv_t == 1.);
    CHECK(intv_y == Interval(-0.5,1.));
    CHECK(x[0] == Interval(-1.5,1.5)); // optim: Interval(-1.5,1.5)
    CHECK(x[1] == Interval(-1.,2.)); // optim: Interval(-1.,2.)
    CHECK(x[2] == Interval(-1.5,2.)); // optim: Interval(-1.5,2.)
    CHECK(ApproxIntv(x[3]) == Interval(-1.83333333,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(-1.5,2.5));
    CHECK(x[7] == Interval(-2.,3.5));
    CHECK(x[8] == Interval(-2.5,4.5));
    CHECK(x[9] == Interval(-3.,5.5));
    CHECK_FALSE(x.ctcFwdBwd(xdot)); // no contraction expected
    CHECK(x[0] == Interval(-1.5,1.5));
    CHECK(x[1] == Interval(-1.,2.));
    CHECK(x[2] == Interval(-1.5,2.));
    CHECK(ApproxIntv(x[3]) == Interval(-1.83333333,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(-1.5,2.5));
    CHECK(x[7] == Interval(-2.,3.5));
    CHECK(x[8] == Interval(-2.5,4.5));
    CHECK(x[9] == Interval(-3.,5.5));

    /*// Test B
    x = x_raw;
    intv_t = Interval(0.5,2.5);
    intv_y = -2.;
    contraction = x.ctcEval(intv_t, intv_y, xdot, false);
    CHECK(contraction);
    CHECK(intv_t == Interval(0.5,2.5));
    CHECK(intv_y == -2.);
    CHECK(x[0] == Interval(-4.5,-0.75)); // optim: Interval(-4.5,-0.75)
    CHECK(x[1] == Interval(-3.5,-0.5)); // optim: Interval(-3.5,-0.5)
    CHECK(x[2] == Interval(-3.25,0.5)); // optim: Interval(-3.,0.5)
    CHECK(x[3] == Interval(-2.5,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(-1.5,2.5));
    CHECK(x[7] == Interval(-2.,3.5));
    CHECK(x[8] == Interval(-2.5,4.5));
    CHECK(x[9] == Interval(-3.,5.5));

    // Test C
    x = x_raw;
    intv_t = Interval(0.5,3.5);
    intv_y = -4.;
    contraction = x.ctcEval(intv_t, intv_y, xdot, false);
    CHECK(contraction);
    CHECK(intv_t == Interval(0.5,2.));
    CHECK(intv_y == -4.);
    CHECK(x[0] == Interval(-5.5,-3.)); // optim: Interval(-5.5,-3.25)
    CHECK(x[1] == Interval(-4.5,-2.5)); // optim: Interval(-4.5,-2.5)
    CHECK(x[2] == Interval(-3.5,-1.5)); // optim: Interval(-3.5,-1.5)
    CHECK(x[3] == Interval(-2.5,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(-1.5,2.5));
    CHECK(x[7] == Interval(-2.,3.5));
    CHECK(x[8] == Interval(-2.5,4.5));
    CHECK(x[9] == Interval(-3.,5.5));

    // Test D
    x = x_raw;
    intv_t = 3.5;
    intv_y = Interval(-3.5,-0.5);
    contraction = x.ctcEval(intv_t, intv_y, xdot, false);
    CHECK(contraction);
    CHECK(intv_t == 3.5);
    CHECK(intv_y == Interval(-2.,-0.5)); // optim: Interval(-2.,-0.5)
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1] == Interval(-4.5,2.5));
    CHECK(x[2] == Interval(-3.5,0.25)); // optim: Interval(-3.5,0.25)
    CHECK(x[3] == Interval(-2.5,0.)); // optim: Interval(-2.5,0.)
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(-1.5,2.5));
    CHECK(x[7] == Interval(-2.,3.5));
    CHECK(x[8] == Interval(-2.5,4.5));
    CHECK(x[9] == Interval(-3.,5.5));

    // Test E
    x = x_raw;
    intv_t = Interval(6.5, 8.5);
    intv_y = Interval(-4.5, -1.75);
    contraction = x.ctcEval(intv_t, intv_y, xdot, false);
    CHECK(contraction);
    CHECK(intv_t == Interval(7., 8.5)); // optim: Interval(7.5, 8.5)
    CHECK(intv_y == Interval(-2.25, -1.75)); // optim: Interval(-2.25, -1.75)
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1] == Interval(-4.5,2.5));
    CHECK(x[2] == Interval(-3.5,2.));
    CHECK(x[3] == Interval(-2.5,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(-1.5,-0.5)); // optim: Interval(-1.5,-0.5)
    CHECK(x[7] == Interval(-2.,-0.75)); // optim: Interval(-2.,-1.)
    CHECK(x[8] == Interval(-2.5,0.25)); // optim: Interval(-2.5,-0.25)
    CHECK(x[9] == Interval(-3.,1.25)); // optim: Interval(-3.,0.25)

    // Test E: ctcFwdBwd should not be effective after ctcEval(true)
    x = x_raw;
    intv_t = Interval(6.5, 8.5);
    intv_y = Interval(-4.5, -1.75);
    contraction = x.ctcEval(intv_t, intv_y, xdot, true);
    CHECK(contraction);
    CHECK(intv_t == Interval(7., 8.5)); // optim: Interval(7.5, 8.5)
    CHECK(intv_y == Interval(-2.25, -1.75)); // optim: Interval(-2.25, -1.75)
    CHECK(x[0] == Interval(-5.5,2.5));
    CHECK(x[1] == Interval(-4.5,2.));
    CHECK(x[2] == Interval(-3.5,1.5));
    CHECK(x[3] == Interval(-2.5,1.));
    CHECK(x[4] == Interval(-1.5,0.5));
    CHECK(x[5] == Interval(-1.,0.));
    CHECK(x[6] == Interval(-1.5,-0.5)); // optim: Interval(-1.5,-0.5)
    CHECK(x[7] == Interval(-2.,-0.75)); // optim: Interval(-2.,-1.)
    CHECK(x[8] == Interval(-2.5,0.25)); // optim: Interval(-2.5,-0.25)
    CHECK(x[9] == Interval(-3.,1.25)); // optim: Interval(-3.,0.25)
    CHECK(!x.ctcFwdBwd(xdot)); // no contraction expected
    CHECK(x[0] == Interval(-5.5,2.5));
    CHECK(x[1] == Interval(-4.5,2.));
    CHECK(x[2] == Interval(-3.5,1.5));
    CHECK(x[3] == Interval(-2.5,1.));
    CHECK(x[4] == Interval(-1.5,0.5));
    CHECK(x[5] == Interval(-1.,0.));
    CHECK(x[6] == Interval(-1.5,-0.5));
    CHECK(x[7] == Interval(-2.,-0.75));
    CHECK(x[8] == Interval(-2.5,0.25));
    CHECK(x[9] == Interval(-3.,1.25));

    // Test F
    x = x_raw;
    intv_t = Interval(8.5,9.5);
    intv_y = Interval(-0.5,2.5);
    contraction = x.ctcEval(intv_t, intv_y, xdot, false);
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
    CHECK(x[7] == Interval(-2.,3.5)); // optim: Interval(-2.,3.3125)
    CHECK(x[8] == Interval(-2.,3.25)); // optim: Interval(-2.,3.25)
    CHECK(x[9] == Interval(-1.25,4.)); // optim: Interval(-1.25,4.)

    // Test F: ctcFwdBwd should not be effective after ctcEval(true)
    x = x_raw;
    intv_t = Interval(8.5,9.5);
    intv_y = Interval(-0.5,2.5);
    contraction = x.ctcEval(intv_t, intv_y, xdot, true);
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
    CHECK(x[7] == Interval(-2.,3.5)); // optim: Interval(-2.,3.3125)
    CHECK(x[8] == Interval(-2.,3.25)); // optim: Interval(-2.,3.25)
    CHECK(x[9] == Interval(-1.25,4.)); // optim: Interval(-1.25,4.)
    CHECK(!x.ctcFwdBwd(xdot)); // no contraction expected
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1] == Interval(-4.5,2.5));
    CHECK(x[2] == Interval(-3.5,2.));
    CHECK(x[3] == Interval(-2.5,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(-1.5,2.5));
    CHECK(x[7] == Interval(-2.,3.5));
    CHECK(x[8] == Interval(-2.,3.25));
    CHECK(x[9] == Interval(-1.25,4.));

    // Test G
    x = x_raw;
    intv_t = Interval(6.5,7.5);
    intv_y = Interval(3.,4.);
    contraction = x.ctcEval(intv_t, intv_y, xdot, false);
    CHECK(contraction);
    CHECK(intv_t == Interval(7.,7.5)); // optim: Interval(7.5)
    CHECK(intv_y == Interval(3.)); // optim: Interval(3.)
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1] == Interval(-4.5,2.5));
    CHECK(x[2] == Interval(-3.5,2.));
    CHECK(x[3] == Interval(-2.5,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(1.5,2.5)); // optim: Interval(1.5,2.5)
    CHECK(x[7] == Interval(2.5,3.5)); // optim: Interval(2.5,2.5)
    CHECK(x[8] == Interval(2.,4.5)); // optim: Interval(2.25,4.5)
    CHECK(x[9] == Interval(-3.,5.5));

    // Test H
    x = x_raw;
    intv_t = Interval(7.,8.25);
    intv_y = Interval(4.25,5.);
    contraction = x.ctcEval(intv_t, intv_y, xdot, false);
    CHECK(contraction);
    CHECK(intv_t == Interval::EMPTY_SET); // optim: Interval::EMPTY_SET
    CHECK(intv_y == Interval::EMPTY_SET); // optim: Interval::EMPTY_SET
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1] == Interval(-4.5,2.5));
    CHECK(x[2] == Interval(-3.5,2.));
    CHECK(x[3] == Interval(-2.5,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(-1.5,2.5));
    CHECK(x[7] == Interval::EMPTY_SET); // optim: Interval::EMPTY_SET
    CHECK(x[8] == Interval::EMPTY_SET); // optim: Interval::EMPTY_SET
    CHECK(x[9] == Interval(-3.,5.5));

    // Test I
    x = x_raw;
    intv_t = 2.5;
    intv_y = 0.5;
    contraction = x.ctcEval(intv_t, intv_y, xdot, false);
    CHECK(contraction);
    CHECK(intv_t == 2.5); // optim: Interval(2.5)
    CHECK(intv_y == 0.5); // optim: Interval(0.5)
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1] == Interval(-1.,1.25)); // optim: Interval(-1.,1.25)
    CHECK(x[2] == Interval(0.,1.)); // optim: Interval(0.,1.)
    CHECK(x[3] == Interval(-0.25,1.5)); // optim: Interval(-0.25,1.5)
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(-1.5,2.5));
    CHECK(x[7] == Interval(-2.,3.5));
    CHECK(x[8] == Interval(-2.5,4.5));
    CHECK(x[9] == Interval(-3.,5.5));

    // Test J
    x = x_raw;
    intv_t = 6.25;
    intv_y = Interval(0.5,1.);
    contraction = x.ctcEval(intv_t, intv_y, xdot, false);
    CHECK(contraction);
    CHECK(intv_t == 6.25); // optim: Interval(6.25)
    CHECK(intv_y == Interval(0.5,1.)); // optim: Interval(0.5,1.)
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1] == Interval(-4.5,2.5));
    CHECK(x[2] == Interval(-3.5,2.));
    CHECK(x[3] == Interval(-2.5,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-0.75,1.5)); // optim: Interval(-0.75,1.5)
    CHECK(x[6] == Interval(0.125,1.75)); // optim: Interval(0.125,1.75)
    CHECK(x[7] == Interval(-0.375,2.75)); // optim: Interval(-0.375,2.75)
    CHECK(x[8] == Interval(-2.5,4.5));
    CHECK(x[9] == Interval(-3.,5.5));

    // Test K
    x = x_raw;
    intv_t = Interval(5.25, 8.25);
    intv_y = Interval(-0.5, 0.);
    contraction = x.ctcEval(intv_t, intv_y, xdot, false);
    CHECK(contraction);
    CHECK(intv_t == Interval(5.25, 8.25));
    CHECK(intv_y == Interval(-0.5, 0.));
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1] == Interval(-4.5,2.5));
    CHECK(x[2] == Interval(-3.5,2.));
    CHECK(x[3] == Interval(-2.5,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(-1.5,1.75)); // optim: Interval(-1.5,1.75)
    CHECK(x[7] == Interval(-1.875,2.75)); // optim: Interval(-1.875,2.75)
    CHECK(x[8] == Interval(-2.375,3.75)); // optim: Interval(-2.375,3.75)
    CHECK(x[9] == Interval(-2.875,4.75)); // optim: Interval(-2.875,4.75)

    // Test L
    x = x_raw;
    intv_t = Interval(1.5,4.5);
    intv_y = Interval(1.75,2.75);
    contraction = x.ctcEval(intv_t, intv_y, xdot, false);
    CHECK(contraction);
    CHECK(intv_t == Interval(1.5,3.)); // optim: Interval(1.5,2.5)
    CHECK(intv_y == Interval(1.75,2.25)); // optim: Interval(1.75,2.25)
    CHECK(x[0] == Interval(-1.25,3.)); // optim: Interval(-0.75,3.)
    CHECK(x[1] == Interval(-0.25,2.5)); // optim: Interval(0.25,2.5)
    CHECK(x[2] == Interval(0.75,2.)); // optim: Interval(1.,2.)
    CHECK(x[3] == Interval(0.5,1.5)); // optim: Interval(0.5,1.5)
    CHECK(x[4] == Interval(-1.5,1.)); // optim: Interval(0.,1.)
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(-1.5,2.5));
    CHECK(x[7] == Interval(-2.,3.5));
    CHECK(x[8] == Interval(-2.5,4.5));
    CHECK(x[9] == Interval(-3.,5.5));

    // Test M
    x = x_raw;
    intv_t = Interval(0.75,1.25);
    intv_y = Interval(-5.75,-5.25);
    contraction = x.ctcEval(intv_t, intv_y, xdot, false);
    CHECK(contraction);
    CHECK(intv_t == Interval::EMPTY_SET); // optim: Interval::EMPTY_SET
    CHECK(intv_y == Interval::EMPTY_SET); // optim: Interval::EMPTY_SET
    CHECK(x[0] == Interval::EMPTY_SET); // optim: Interval::EMPTY_SET
    CHECK(x[1] == Interval::EMPTY_SET); // optim: Interval::EMPTY_SET
    CHECK(x[2] == Interval(-3.5,2.));
    CHECK(x[3] == Interval(-2.5,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(-1.5,2.5));
    CHECK(x[7] == Interval(-2.,3.5));
    CHECK(x[8] == Interval(-2.5,4.5));
    CHECK(x[9] == Interval(-3.,5.5));

    // Test N
    x = x_raw;
    intv_t = Interval(5.5,8.5);
    intv_y = Interval(2.,5.5);
    contraction = x.ctcEval(intv_t, intv_y, xdot, false);
    CHECK(contraction);
    CHECK(intv_t == Interval(6.,8.5)); // optim: Interval(6.5,8.5)
    CHECK(intv_y == Interval(2.,4.)); // optim: Interval(2.,4.)
    CHECK(x[0] == Interval(-5.5,3.));
    CHECK(x[1] == Interval(-4.5,2.5));
    CHECK(x[2] == Interval(-3.5,2.));
    CHECK(x[3] == Interval(-2.5,1.5));
    CHECK(x[4] == Interval(-1.5,1.));
    CHECK(x[5] == Interval(-1.,1.5));
    CHECK(x[6] == Interval(-0.5, 2.5)); // optim: Interval(-0.5, 2.5)
    CHECK(x[7] == Interval(0.5,3.5)); // optim: Interval(0.5, 3.5)
    CHECK(x[8] == Interval(0.5,4.5)); // optim: Interval(1.5, 4.5)
    CHECK(x[9] == Interval(0.,5.5)); // optim: Interval(0.25, 5.5)
  }

  SECTION("Test CtcEval, non-zero derivative (negative case)")
  {
    Tube tube(Interval(0.,20.), 1.);
    Tube derivative(tube);
    derivative.set(Interval(-1.5,-1.));
    tube.set(Interval(6.,8.), 0);
    tube.ctcFwdBwd(derivative);

    CHECK(tube[0] == Interval(6.,8.)); 
    CHECK(tube[1] == Interval(4.5,7.));
    CHECK(tube[2] == Interval(3.,6.));
    CHECK(tube[3] == Interval(1.5,5.));
    CHECK(tube[4] == Interval(-0.,4.));
    CHECK(tube[5] == Interval(-1.5,3.));
    CHECK(tube[6] == Interval(-3.,2.));
    CHECK(tube[7] == Interval(-4.5,1.));
    CHECK(tube[8] == Interval(-6.,0.));
    CHECK(tube[9] == Interval(-7.5,-1.));
    CHECK(tube[10] == Interval(-9.,-2.));

    bool contraction;
    Interval intv_t, intv_y;
    Tube tube_raw(tube);

    intv_t = Interval(3.5,6.);
    intv_y = Interval(3.5,6.);
    contraction = tube.ctcEval(intv_t, intv_y, derivative, false);
    CHECK(contraction);
    CHECK(intv_t == Interval(3.5,5.)); // optim: Interval(3.5,4.5)
    CHECK(intv_y == Interval(3.5,4.5)); // optim: Interval(3.5,4.5)
    CHECK(tube[0] == Interval(6.,8.));
    CHECK(tube[1] == Interval(4.5,7.));
    CHECK(tube[2] == Interval(4.,6.));
    CHECK(tube[3] == Interval(2.75,5.));
    CHECK(tube[4] == Interval(1.25,4.));
    CHECK(tube[5] == Interval(-0.25,3.));
    CHECK(tube[6] == Interval(-3.,2.));
    CHECK(tube[7] == Interval(-4.5,1.));
    CHECK(tube[8] == Interval(-6.,0.));
    CHECK(tube[9] == Interval(-7.5,-1.));
    CHECK(tube[10] == Interval(-9.,-2.));

    tube = tube_raw;
    intv_t = Interval(3.,8.5);
    intv_y = Interval(-7.,-2.5);
    contraction = tube.ctcEval(intv_t, intv_y, derivative, false);
    CHECK(contraction);
    CHECK(intv_t == Interval(6.,8.5)); // optim: Interval(6.5+?,8.5)
    CHECK(intv_y == Interval(-5.25,-2.5)); // optim: Interval(-5.25,-2.5)
    CHECK(tube[0] == Interval(6.,8.)); 
    CHECK(tube[1] == Interval(4.5,7.));
    CHECK(tube[2] == Interval(3.,6.));
    CHECK(tube[3] == Interval(1.5,5.));
    CHECK(tube[4] == Interval(-0.,4.));
    CHECK(tube[5] == Interval(-1.5,2.75));
    CHECK(tube[6] == Interval(-3.,1.25));
    CHECK(tube[7] == Interval(-4.5,-0.25));
    CHECK(tube[8] == Interval(-6.,-1.75));
    CHECK(tube[9] == Interval(-7.5,-3.));
    CHECK(tube[10] == Interval(-9.,-2.));

    tube = tube_raw;
    intv_t = Interval(7.,8.);
    intv_y = Interval(-2.,-1.);
    contraction = tube.ctcEval(intv_t, intv_y, derivative, false);
    CHECK(contraction);
    CHECK(intv_t == Interval(7.,8.));
    CHECK(intv_y == Interval(-2.,-1.));
    CHECK(tube[0] == Interval(6.,8.)); 
    CHECK(tube[1] == Interval(4.5,7.));
    CHECK(tube[2] == Interval(3.,6.));
    CHECK(tube[3] == Interval(1.5,5.));
    CHECK(tube[4] == Interval(-0.,4.));
    CHECK(tube[5] == Interval(-1.,3.));
    CHECK(tube[6] == Interval(-2.,2.));
    CHECK(tube[7] == Interval(-3.5,0.5));
    CHECK(tube[8] == Interval(-5.,-1.));
    CHECK(tube[9] == Interval(-6.5,-2.));
    CHECK(tube[10] == Interval(-9.,-2.));

    tube = tube_raw;
    intv_t = Interval(0.);
    intv_y = Interval(6.,8.);
    contraction = tube.ctcEval(intv_t, intv_y, derivative, false);
    CHECK(contraction);
    CHECK(intv_t == Interval(0.));
    CHECK(intv_y == Interval(7.,8.));
    CHECK(tube[0] == Interval(6.,8.)); 
    CHECK(tube[1] == Interval(4.5,7.));
    CHECK(tube[2] == Interval(3.,6.));
    CHECK(tube[3] == Interval(1.5,5.));
    CHECK(tube[4] == Interval(-0.,4.));
    CHECK(tube[5] == Interval(-1.5,3.));
    CHECK(tube[6] == Interval(-3.,2.));
    CHECK(tube[7] == Interval(-4.5,1.));
    CHECK(tube[8] == Interval(-6.,0.));
    CHECK(tube[9] == Interval(-7.5,-1.));
    CHECK(tube[10] == Interval(-9.,-2.));
  }

  SECTION("Test CtcEval, non-zero derivative (positive case)")
  {
    Tube tube(Interval(0.,20.), 1.);
    Tube derivative(tube);
    derivative.set(Interval(1.,1.5));
    tube.set(Interval(-8.,-6.), 0);
    tube.ctcFwdBwd(derivative);

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