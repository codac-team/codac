/* ============================================================================
 *  tubex-lib - Tube tests
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2016
 * ---------------------------------------------------------------------------- */

#define CATCH_CONFIG_MAIN
#include "../catch/catch.hpp"
#include "../tests_cases.hpp"

using namespace tubex;

TEST_CASE("Contracting tube: ctcFwd(xdot)", "[ctc]")
{
  SECTION("Test ctcFwd")
  {
    Tube tube(Interval(0., 6.), 1.0);
    Tube tubedot(tube);
    tubedot.set(Interval(-0.5,1.));
    tube.set(Interval(-1.,1.), 0);

    Tube tube_test_fwd(tube);
    tube_test_fwd.ctcFwd(tubedot);

    Tube tube_test_fwdbwd(tube);
    tube_test_fwdbwd.ctcFwdBwd(tubedot);

    REQUIRE(tube_test_fwd == tube_test_fwdbwd);
    REQUIRE(tube_test_fwd[0] == Interval(-1.,1.));
    REQUIRE(tube_test_fwd[1] == Interval(-1.5,2.));
    REQUIRE(tube_test_fwd[2] == Interval(-2.,3.));
    REQUIRE(tube_test_fwd[3] == Interval(-2.5,4.));
    REQUIRE(tube_test_fwd[4] == Interval(-3.,5.));
    REQUIRE(tube_test_fwd[5] == Interval(-3.5,6.));
  }
}

TEST_CASE("Contracting tube: ctcBwd(xdot)", "[ctc]")
{
  SECTION("Test ctcBwd")
  {
    Tube tube(Interval(0., 6.), 1.0);
    Tube tubedot(tube);
    tubedot.set(Interval(-1.,0.5));
    tube.set(Interval(-1.,1.), 5);

    Tube tube_test_bwd(tube);
    tube_test_bwd.ctcBwd(tubedot);

    Tube tube_test_fwdbwd(tube);
    tube_test_fwdbwd.ctcFwdBwd(tubedot);

    REQUIRE(tube_test_bwd == tube_test_fwdbwd);
    REQUIRE(tube_test_bwd[0] == Interval(-3.5,6.));
    REQUIRE(tube_test_bwd[1] == Interval(-3.,5.));
    REQUIRE(tube_test_bwd[2] == Interval(-2.5,4.));
    REQUIRE(tube_test_bwd[3] == Interval(-2.,3.));
    REQUIRE(tube_test_bwd[4] == Interval(-1.5,2.));
    REQUIRE(tube_test_bwd[5] == Interval(-1.,1.));
  }
}

TEST_CASE("Contracting tube: ctcFwdBwd(xdot)", "[ctc]")
{
  SECTION("Test ctcFwdBwd")
  {
    Tube tube(Interval(0., 6.), 1.0);
    Tube tubedot(tube);
    tubedot.set(Interval(-1.,0.5));
    tube.set(Interval(-1.,1.), 5);
    tube.set(Interval(-1.,1.), 0);

    tube.ctcFwdBwd(tubedot);

    REQUIRE(tube[0] == Interval(-1.,1.));
    REQUIRE(tube[1] == Interval(-2,1.5));
    REQUIRE(tube[2] == Interval(-2.5,2.));
    REQUIRE(tube[3] == Interval(-2.,2.5));
    REQUIRE(tube[4] == Interval(-1.5,2.));
    REQUIRE(tube[5] == Interval(-1.,1.));
  }
}

TEST_CASE("Contracting tube: ctcIntertemporal(t1, t2)", "[ctc]")
{
  SECTION("Test tube1, case1")
  {
    Tube tube = tubeTest1();
    Interval t1(0., 8.);
    Interval t2(29.5);
    tube.ctcIntertemporal(t1, t2);
    REQUIRE(t1 == Interval(0., 4.));
    REQUIRE(t2 == Interval(29.5));
  }

  SECTION("Test tube1, case2")
  {
    Tube tube = tubeTest1();
    Interval t1(4.2, 23.5);
    Interval t2(32., 36.);
    tube.ctcIntertemporal(t1, t2);
    REQUIRE(t1 == Interval(14., 19.));
    REQUIRE(t2 == Interval(32., 36.));
  }

  SECTION("Test tube1, case3")
  {
    Tube tube = tubeTest1();
    Interval t1(21., 26.);
    Interval t2(5., 9.);
    tube.ctcIntertemporal(t1, t2);
    REQUIRE(t1 == Interval::EMPTY_SET);
    REQUIRE(t2 == Interval::EMPTY_SET);
  }
}

TEST_CASE("Contracting tube: ctcIntertemporal(y, t1, t2)", "[ctc]")
{
  SECTION("Test tube1, case1")
  {
    Tube tube = tubeTest1();
    Interval y(-2.);
    Interval t1(0., 8.);
    Interval t2(9., 22.);
    tube.ctcIntertemporal(y, t1, t2);
    REQUIRE( y == -2);
    REQUIRE(t1 == Interval(3., 5.));
    REQUIRE(t2 == Interval(12., 16.));
  }

  SECTION("Test tube1, case2")
  {
    Tube tube = tubeTest1();
    Interval y(5.5, 20.);
    Interval t1(9., 37.);
    Interval t2(40., 45.);
    tube.ctcIntertemporal(y, t1, t2);
    REQUIRE( y == Interval(5.5, 13.));
    REQUIRE(t1 == Interval(16., 31.));
    REQUIRE(t2 == Interval(42., 43.));
  }

  SECTION("Test tube1, case3")
  {
    Tube tube = tubeTest1();
    Interval y(12., 20.);
    Interval t1(9., 37.);
    Interval t2(40., 45.);
    tube.ctcIntertemporal(y, t1, t2);
    REQUIRE( y == Interval(12., 13.));
    REQUIRE(t1 == Interval(22., 25.));
    REQUIRE(t2 == Interval::EMPTY_SET);
  }

  SECTION("Test tube1, case4")
  {
    Tube tube = tubeTest1();
    Interval y(16., 20.);
    Interval t1(9., 37.);
    Interval t2(40., 45.);
    tube.ctcIntertemporal(y, t1, t2);
    REQUIRE( y == Interval::EMPTY_SET);
    REQUIRE(t1 == Interval::EMPTY_SET);
    REQUIRE(t2 == Interval::EMPTY_SET);
  }
}

TEST_CASE("Contracting tube: ctcObs(xdot, y, t)", "[ctc]")
{
  SECTION("Test ctcIn, special cases")
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
    REQUIRE(x_raw[0] == Interval(-5.5,3.));
    REQUIRE(x_raw[1] == Interval(-4.5,2.5));
    REQUIRE(x_raw[2] == Interval(-3.5,2.));
    REQUIRE(x_raw[3] == Interval(-2.5,1.5));
    REQUIRE(x_raw[4] == Interval(-1.5,1.));
    REQUIRE(x_raw[5] == Interval(-1.,1.5));
    REQUIRE(x_raw[6] == Interval(-1.5,2.5));
    REQUIRE(x_raw[7] == Interval(-2.,3.5));
    REQUIRE(x_raw[8] == Interval(-2.5,4.5));
    REQUIRE(x_raw[9] == Interval(-3.,5.5));

    bool contraction;
    Interval intv_t, intv_y;

    // Test A
    x = x_raw;
    intv_t = 1.;
    intv_y = Interval(-0.5,1.);
    contraction = x.ctcObs(xdot, intv_t, intv_y, false);
    REQUIRE(contraction);
    REQUIRE(intv_t == 1.);
    REQUIRE(intv_y == Interval(-0.5,1.));
    REQUIRE(x[0] == Interval(-1.5,1.5)); // optim: Interval(-1.5,1.5)
    REQUIRE(x[1] == Interval(-1.,2.)); // optim: Interval(-1.,2.)
    REQUIRE(x[2] == Interval(-1.5,2.)); // optim: Interval(-1.5,2.)
    REQUIRE(x[3] == Interval(-2.5,1.5));
    REQUIRE(x[4] == Interval(-1.5,1.));
    REQUIRE(x[5] == Interval(-1.,1.5));
    REQUIRE(x[6] == Interval(-1.5,2.5));
    REQUIRE(x[7] == Interval(-2.,3.5));
    REQUIRE(x[8] == Interval(-2.5,4.5));
    REQUIRE(x[9] == Interval(-3.,5.5));

    // Test A: ctcFwdBwd should not be effective after ctcObs(true)
    x = x_raw;
    intv_t = 1.;
    intv_y = Interval(-0.5,1.);
    contraction = x.ctcObs(xdot, intv_t, intv_y, true);
    REQUIRE(contraction);
    REQUIRE(intv_t == 1.);
    REQUIRE(intv_y == Interval(-0.5,1.));
    REQUIRE(x[0] == Interval(-1.5,1.5)); // optim: Interval(-1.5,1.5)
    REQUIRE(x[1] == Interval(-1.,2.)); // optim: Interval(-1.,2.)
    REQUIRE(x[2] == Interval(-1.5,2.)); // optim: Interval(-1.5,2.)
    REQUIRE(x[3] == Interval(-2.,1.5));
    REQUIRE(x[4] == Interval(-1.5,1.));
    REQUIRE(x[5] == Interval(-1.,1.5));
    REQUIRE(x[6] == Interval(-1.5,2.5));
    REQUIRE(x[7] == Interval(-2.,3.5));
    REQUIRE(x[8] == Interval(-2.5,4.5));
    REQUIRE(x[9] == Interval(-3.,5.5));
    REQUIRE(!x.ctcFwdBwd(xdot)); // no contraction expected
    REQUIRE(x[0] == Interval(-1.5,1.5));
    REQUIRE(x[1] == Interval(-1.,2.));
    REQUIRE(x[2] == Interval(-1.5,2.));
    REQUIRE(x[3] == Interval(-2.,1.5));
    REQUIRE(x[4] == Interval(-1.5,1.));
    REQUIRE(x[5] == Interval(-1.,1.5));
    REQUIRE(x[6] == Interval(-1.5,2.5));
    REQUIRE(x[7] == Interval(-2.,3.5));
    REQUIRE(x[8] == Interval(-2.5,4.5));
    REQUIRE(x[9] == Interval(-3.,5.5));

    // Test B
    x = x_raw;
    intv_t = Interval(0.5,2.5);
    intv_y = -2.;
    contraction = x.ctcObs(xdot, intv_t, intv_y, false);
    REQUIRE(contraction);
    REQUIRE(intv_t == Interval(0.5,2.5));
    REQUIRE(intv_y == -2.);
    REQUIRE(x[0] == Interval(-4.5,-0.75)); // optim: Interval(-4.5,-0.75)
    REQUIRE(x[1] == Interval(-3.5,-0.5)); // optim: Interval(-3.5,-0.5)
    REQUIRE(x[2] == Interval(-3.25,0.5)); // optim: Interval(-3.,0.5)
    REQUIRE(x[3] == Interval(-2.5,1.5));
    REQUIRE(x[4] == Interval(-1.5,1.));
    REQUIRE(x[5] == Interval(-1.,1.5));
    REQUIRE(x[6] == Interval(-1.5,2.5));
    REQUIRE(x[7] == Interval(-2.,3.5));
    REQUIRE(x[8] == Interval(-2.5,4.5));
    REQUIRE(x[9] == Interval(-3.,5.5));

    // Test C
    x = x_raw;
    intv_t = Interval(0.5,3.5);
    intv_y = -4.;
    contraction = x.ctcObs(xdot, intv_t, intv_y, false);
    REQUIRE(contraction);
    REQUIRE(intv_t == Interval(0.5,2.));
    REQUIRE(intv_y == -4.);
    REQUIRE(x[0] == Interval(-5.5,-3.)); // optim: Interval(-5.5,-3.25)
    REQUIRE(x[1] == Interval(-4.5,-2.5)); // optim: Interval(-4.5,-2.5)
    REQUIRE(x[2] == Interval(-3.5,-1.5)); // optim: Interval(-3.5,-1.5)
    REQUIRE(x[3] == Interval(-2.5,1.5));
    REQUIRE(x[4] == Interval(-1.5,1.));
    REQUIRE(x[5] == Interval(-1.,1.5));
    REQUIRE(x[6] == Interval(-1.5,2.5));
    REQUIRE(x[7] == Interval(-2.,3.5));
    REQUIRE(x[8] == Interval(-2.5,4.5));
    REQUIRE(x[9] == Interval(-3.,5.5));

    // Test D
    x = x_raw;
    intv_t = 3.5;
    intv_y = Interval(-3.5,-0.5);
    contraction = x.ctcObs(xdot, intv_t, intv_y, false);
    REQUIRE(contraction);
    REQUIRE(intv_t == 3.5);
    REQUIRE(intv_y == Interval(-2.,-0.5)); // optim: Interval(-2.,-0.5)
    REQUIRE(x[0] == Interval(-5.5,3.));
    REQUIRE(x[1] == Interval(-4.5,2.5));
    REQUIRE(x[2] == Interval(-3.5,0.25)); // optim: Interval(-3.5,0.25)
    REQUIRE(x[3] == Interval(-2.5,0.)); // optim: Interval(-2.5,0.)
    REQUIRE(x[4] == Interval(-1.5,1.));
    REQUIRE(x[5] == Interval(-1.,1.5));
    REQUIRE(x[6] == Interval(-1.5,2.5));
    REQUIRE(x[7] == Interval(-2.,3.5));
    REQUIRE(x[8] == Interval(-2.5,4.5));
    REQUIRE(x[9] == Interval(-3.,5.5));

    // Test E
    x = x_raw;
    intv_t = Interval(6.5, 8.5);
    intv_y = Interval(-4.5, -1.75);
    contraction = x.ctcObs(xdot, intv_t, intv_y, false);
    REQUIRE(contraction);
    REQUIRE(intv_t == Interval(7., 8.5)); // optim: Interval(7.5, 8.5)
    REQUIRE(intv_y == Interval(-2.25, -1.75)); // optim: Interval(-2.25, -1.75)
    REQUIRE(x[0] == Interval(-5.5,3.));
    REQUIRE(x[1] == Interval(-4.5,2.5));
    REQUIRE(x[2] == Interval(-3.5,2.));
    REQUIRE(x[3] == Interval(-2.5,1.5));
    REQUIRE(x[4] == Interval(-1.5,1.));
    REQUIRE(x[5] == Interval(-1.,1.5));
    REQUIRE(x[6] == Interval(-1.5,-0.5)); // optim: Interval(-1.5,-0.5)
    REQUIRE(x[7] == Interval(-2.,-0.75)); // optim: Interval(-2.,-1.)
    REQUIRE(x[8] == Interval(-2.5,0.25)); // optim: Interval(-2.5,-0.25)
    REQUIRE(x[9] == Interval(-3.,1.25)); // optim: Interval(-3.,0.25)

    // Test E: ctcFwdBwd should not be effective after ctcObs(true)
    x = x_raw;
    intv_t = Interval(6.5, 8.5);
    intv_y = Interval(-4.5, -1.75);
    contraction = x.ctcObs(xdot, intv_t, intv_y, true);
    REQUIRE(contraction);
    REQUIRE(intv_t == Interval(7., 8.5)); // optim: Interval(7.5, 8.5)
    REQUIRE(intv_y == Interval(-2.25, -1.75)); // optim: Interval(-2.25, -1.75)
    REQUIRE(x[0] == Interval(-5.5,2.5));
    REQUIRE(x[1] == Interval(-4.5,2.));
    REQUIRE(x[2] == Interval(-3.5,1.5));
    REQUIRE(x[3] == Interval(-2.5,1.));
    REQUIRE(x[4] == Interval(-1.5,0.5));
    REQUIRE(x[5] == Interval(-1.,0.));
    REQUIRE(x[6] == Interval(-1.5,-0.5)); // optim: Interval(-1.5,-0.5)
    REQUIRE(x[7] == Interval(-2.,-0.75)); // optim: Interval(-2.,-1.)
    REQUIRE(x[8] == Interval(-2.5,0.25)); // optim: Interval(-2.5,-0.25)
    REQUIRE(x[9] == Interval(-3.,1.25)); // optim: Interval(-3.,0.25)
    REQUIRE(!x.ctcFwdBwd(xdot)); // no contraction expected
    REQUIRE(x[0] == Interval(-5.5,2.5));
    REQUIRE(x[1] == Interval(-4.5,2.));
    REQUIRE(x[2] == Interval(-3.5,1.5));
    REQUIRE(x[3] == Interval(-2.5,1.));
    REQUIRE(x[4] == Interval(-1.5,0.5));
    REQUIRE(x[5] == Interval(-1.,0.));
    REQUIRE(x[6] == Interval(-1.5,-0.5));
    REQUIRE(x[7] == Interval(-2.,-0.75));
    REQUIRE(x[8] == Interval(-2.5,0.25));
    REQUIRE(x[9] == Interval(-3.,1.25));

    // Test F
    x = x_raw;
    intv_t = Interval(8.5,9.5);
    intv_y = Interval(-0.5,2.5);
    contraction = x.ctcObs(xdot, intv_t, intv_y, false);
    REQUIRE(contraction);
    REQUIRE(intv_t == Interval(8.5,9.5));
    REQUIRE(intv_y == Interval(-0.5,2.5));
    REQUIRE(x[0] == Interval(-5.5,3.));
    REQUIRE(x[1] == Interval(-4.5,2.5));
    REQUIRE(x[2] == Interval(-3.5,2.));
    REQUIRE(x[3] == Interval(-2.5,1.5));
    REQUIRE(x[4] == Interval(-1.5,1.));
    REQUIRE(x[5] == Interval(-1.,1.5));
    REQUIRE(x[6] == Interval(-1.5,2.5));
    REQUIRE(x[7] == Interval(-2.,3.5)); // optim: Interval(-2.,3.3125)
    REQUIRE(x[8] == Interval(-2.,3.25)); // optim: Interval(-2.,3.25)
    REQUIRE(x[9] == Interval(-1.25,4.)); // optim: Interval(-1.25,4.)

    // Test F: ctcFwdBwd should not be effective after ctcObs(true)
    x = x_raw;
    intv_t = Interval(8.5,9.5);
    intv_y = Interval(-0.5,2.5);
    contraction = x.ctcObs(xdot, intv_t, intv_y, true);
    REQUIRE(contraction);
    REQUIRE(intv_t == Interval(8.5,9.5));
    REQUIRE(intv_y == Interval(-0.5,2.5));
    REQUIRE(x[0] == Interval(-5.5,3.));
    REQUIRE(x[1] == Interval(-4.5,2.5));
    REQUIRE(x[2] == Interval(-3.5,2.));
    REQUIRE(x[3] == Interval(-2.5,1.5));
    REQUIRE(x[4] == Interval(-1.5,1.));
    REQUIRE(x[5] == Interval(-1.,1.5));
    REQUIRE(x[6] == Interval(-1.5,2.5));
    REQUIRE(x[7] == Interval(-2.,3.5)); // optim: Interval(-2.,3.3125)
    REQUIRE(x[8] == Interval(-2.,3.25)); // optim: Interval(-2.,3.25)
    REQUIRE(x[9] == Interval(-1.25,4.)); // optim: Interval(-1.25,4.)
    REQUIRE(!x.ctcFwdBwd(xdot)); // no contraction expected
    REQUIRE(x[0] == Interval(-5.5,3.));
    REQUIRE(x[1] == Interval(-4.5,2.5));
    REQUIRE(x[2] == Interval(-3.5,2.));
    REQUIRE(x[3] == Interval(-2.5,1.5));
    REQUIRE(x[4] == Interval(-1.5,1.));
    REQUIRE(x[5] == Interval(-1.,1.5));
    REQUIRE(x[6] == Interval(-1.5,2.5));
    REQUIRE(x[7] == Interval(-2.,3.5));
    REQUIRE(x[8] == Interval(-2.,3.25));
    REQUIRE(x[9] == Interval(-1.25,4.));

    // Test G
    x = x_raw;
    intv_t = Interval(6.5,7.5);
    intv_y = Interval(3.,4.);
    contraction = x.ctcObs(xdot, intv_t, intv_y, false);
    REQUIRE(contraction);
    REQUIRE(intv_t == Interval(7.,7.5)); // optim: Interval(7.5)
    REQUIRE(intv_y == Interval(3.)); // optim: Interval(3.)
    REQUIRE(x[0] == Interval(-5.5,3.));
    REQUIRE(x[1] == Interval(-4.5,2.5));
    REQUIRE(x[2] == Interval(-3.5,2.));
    REQUIRE(x[3] == Interval(-2.5,1.5));
    REQUIRE(x[4] == Interval(-1.5,1.));
    REQUIRE(x[5] == Interval(-1.,1.5));
    REQUIRE(x[6] == Interval(1.5,2.5)); // optim: Interval(1.5,2.5)
    REQUIRE(x[7] == Interval(2.5,3.5)); // optim: Interval(2.5,2.5)
    REQUIRE(x[8] == Interval(2.,4.5)); // optim: Interval(2.25,4.5)
    REQUIRE(x[9] == Interval(-3.,5.5));

    // Test H
    x = x_raw;
    intv_t = Interval(7.,8.25);
    intv_y = Interval(4.25,5.);
    contraction = x.ctcObs(xdot, intv_t, intv_y, false);
    REQUIRE(contraction);
    REQUIRE(intv_t == Interval::EMPTY_SET); // optim: Interval::EMPTY_SET
    REQUIRE(intv_y == Interval::EMPTY_SET); // optim: Interval::EMPTY_SET
    REQUIRE(x[0] == Interval(-5.5,3.));
    REQUIRE(x[1] == Interval(-4.5,2.5));
    REQUIRE(x[2] == Interval(-3.5,2.));
    REQUIRE(x[3] == Interval(-2.5,1.5));
    REQUIRE(x[4] == Interval(-1.5,1.));
    REQUIRE(x[5] == Interval(-1.,1.5));
    REQUIRE(x[6] == Interval(-1.5,2.5));
    REQUIRE(x[7] == Interval::EMPTY_SET); // optim: Interval::EMPTY_SET
    REQUIRE(x[8] == Interval::EMPTY_SET); // optim: Interval::EMPTY_SET
    REQUIRE(x[9] == Interval(-3.,5.5));

    // Test I
    x = x_raw;
    intv_t = 2.5;
    intv_y = 0.5;
    contraction = x.ctcObs(xdot, intv_t, intv_y, false);
    REQUIRE(contraction);
    REQUIRE(intv_t == 2.5); // optim: Interval(2.5)
    REQUIRE(intv_y == 0.5); // optim: Interval(0.5)
    REQUIRE(x[0] == Interval(-5.5,3.));
    REQUIRE(x[1] == Interval(-1.,1.25)); // optim: Interval(-1.,1.25)
    REQUIRE(x[2] == Interval(0.,1.)); // optim: Interval(0.,1.)
    REQUIRE(x[3] == Interval(-0.25,1.5)); // optim: Interval(-0.25,1.5)
    REQUIRE(x[4] == Interval(-1.5,1.));
    REQUIRE(x[5] == Interval(-1.,1.5));
    REQUIRE(x[6] == Interval(-1.5,2.5));
    REQUIRE(x[7] == Interval(-2.,3.5));
    REQUIRE(x[8] == Interval(-2.5,4.5));
    REQUIRE(x[9] == Interval(-3.,5.5));

    // Test J
    x = x_raw;
    intv_t = 6.25;
    intv_y = Interval(0.5,1.);
    contraction = x.ctcObs(xdot, intv_t, intv_y, false);
    REQUIRE(contraction);
    REQUIRE(intv_t == 6.25); // optim: Interval(6.25)
    REQUIRE(intv_y == Interval(0.5,1.)); // optim: Interval(0.5,1.)
    REQUIRE(x[0] == Interval(-5.5,3.));
    REQUIRE(x[1] == Interval(-4.5,2.5));
    REQUIRE(x[2] == Interval(-3.5,2.));
    REQUIRE(x[3] == Interval(-2.5,1.5));
    REQUIRE(x[4] == Interval(-1.5,1.));
    REQUIRE(x[5] == Interval(-0.75,1.5)); // optim: Interval(-0.75,1.5)
    REQUIRE(x[6] == Interval(0.125,1.75)); // optim: Interval(0.125,1.75)
    REQUIRE(x[7] == Interval(-0.375,2.75)); // optim: Interval(-0.375,2.75)
    REQUIRE(x[8] == Interval(-2.5,4.5));
    REQUIRE(x[9] == Interval(-3.,5.5));

    // Test K
    x = x_raw;
    intv_t = Interval(5.25, 8.25);
    intv_y = Interval(-0.5, 0.);
    contraction = x.ctcObs(xdot, intv_t, intv_y, false);
    REQUIRE(contraction);
    REQUIRE(intv_t == Interval(5.25, 8.25));
    REQUIRE(intv_y == Interval(-0.5, 0.));
    REQUIRE(x[0] == Interval(-5.5,3.));
    REQUIRE(x[1] == Interval(-4.5,2.5));
    REQUIRE(x[2] == Interval(-3.5,2.));
    REQUIRE(x[3] == Interval(-2.5,1.5));
    REQUIRE(x[4] == Interval(-1.5,1.));
    REQUIRE(x[5] == Interval(-1.,1.5));
    REQUIRE(x[6] == Interval(-1.5,1.75)); // optim: Interval(-1.5,1.75)
    REQUIRE(x[7] == Interval(-1.875,2.75)); // optim: Interval(-1.875,2.75)
    REQUIRE(x[8] == Interval(-2.375,3.75)); // optim: Interval(-2.375,3.75)
    REQUIRE(x[9] == Interval(-2.875,4.75)); // optim: Interval(-2.875,4.75)

    // Test L
    x = x_raw;
    intv_t = Interval(1.5,4.5);
    intv_y = Interval(1.75,2.75);
    contraction = x.ctcObs(xdot, intv_t, intv_y, false);
    REQUIRE(contraction);
    REQUIRE(intv_t == Interval(1.5,3.)); // optim: Interval(1.5,2.5)
    REQUIRE(intv_y == Interval(1.75,2.25)); // optim: Interval(1.75,2.25)
    REQUIRE(x[0] == Interval(-1.25,3.)); // optim: Interval(-0.75,3.)
    REQUIRE(x[1] == Interval(-0.25,2.5)); // optim: Interval(0.25,2.5)
    REQUIRE(x[2] == Interval(0.75,2.)); // optim: Interval(1.,2.)
    REQUIRE(x[3] == Interval(0.5,1.5)); // optim: Interval(0.5,1.5)
    REQUIRE(x[4] == Interval(-1.5,1.)); // optim: Interval(0.,1.)
    REQUIRE(x[5] == Interval(-1.,1.5));
    REQUIRE(x[6] == Interval(-1.5,2.5));
    REQUIRE(x[7] == Interval(-2.,3.5));
    REQUIRE(x[8] == Interval(-2.5,4.5));
    REQUIRE(x[9] == Interval(-3.,5.5));

    // Test M
    x = x_raw;
    intv_t = Interval(0.75,1.25);
    intv_y = Interval(-5.75,-5.25);
    contraction = x.ctcObs(xdot, intv_t, intv_y, false);
    REQUIRE(contraction);
    REQUIRE(intv_t == Interval::EMPTY_SET); // optim: Interval::EMPTY_SET
    REQUIRE(intv_y == Interval::EMPTY_SET); // optim: Interval::EMPTY_SET
    REQUIRE(x[0] == Interval::EMPTY_SET); // optim: Interval::EMPTY_SET
    REQUIRE(x[1] == Interval::EMPTY_SET); // optim: Interval::EMPTY_SET
    REQUIRE(x[2] == Interval(-3.5,2.));
    REQUIRE(x[3] == Interval(-2.5,1.5));
    REQUIRE(x[4] == Interval(-1.5,1.));
    REQUIRE(x[5] == Interval(-1.,1.5));
    REQUIRE(x[6] == Interval(-1.5,2.5));
    REQUIRE(x[7] == Interval(-2.,3.5));
    REQUIRE(x[8] == Interval(-2.5,4.5));
    REQUIRE(x[9] == Interval(-3.,5.5));

    // Test N
    x = x_raw;
    intv_t = Interval(5.5,8.5);
    intv_y = Interval(2.,5.5);
    contraction = x.ctcObs(xdot, intv_t, intv_y, false);
    REQUIRE(contraction);
    REQUIRE(intv_t == Interval(6.,8.5)); // optim: Interval(6.5,8.5)
    REQUIRE(intv_y == Interval(2.,4.)); // optim: Interval(2.,4.)
    REQUIRE(x[0] == Interval(-5.5,3.));
    REQUIRE(x[1] == Interval(-4.5,2.5));
    REQUIRE(x[2] == Interval(-3.5,2.));
    REQUIRE(x[3] == Interval(-2.5,1.5));
    REQUIRE(x[4] == Interval(-1.5,1.));
    REQUIRE(x[5] == Interval(-1.,1.5));
    REQUIRE(x[6] == Interval(-0.5, 2.5)); // optim: Interval(-0.5, 2.5)
    REQUIRE(x[7] == Interval(0.5,3.5)); // optim: Interval(0.5, 3.5)
    REQUIRE(x[8] == Interval(0.5,4.5)); // optim: Interval(1.5, 4.5)
    REQUIRE(x[9] == Interval(0.,5.5)); // optim: Interval(0.25, 5.5)
  }

  SECTION("Test ctcIn, non-zero derivative (negative case)")
  {
    Tube tube(Interval(0.,20.), 1.);
    Tube derivative(tube);
    derivative.set(Interval(-1.5,-1.));
    tube.set(Interval(6.,8.), 0);
    tube.ctcFwdBwd(derivative);

    REQUIRE(tube[0] == Interval(6.,8.)); 
    REQUIRE(tube[1] == Interval(4.5,7.));
    REQUIRE(tube[2] == Interval(3.,6.));
    REQUIRE(tube[3] == Interval(1.5,5.));
    REQUIRE(tube[4] == Interval(-0.,4.));
    REQUIRE(tube[5] == Interval(-1.5,3.));
    REQUIRE(tube[6] == Interval(-3.,2.));
    REQUIRE(tube[7] == Interval(-4.5,1.));
    REQUIRE(tube[8] == Interval(-6.,0.));
    REQUIRE(tube[9] == Interval(-7.5,-1.));
    REQUIRE(tube[10] == Interval(-9.,-2.));

    bool contraction;
    Interval intv_t, intv_y;
    Tube tube_raw(tube);

    intv_t = Interval(3.5,6.);
    intv_y = Interval(3.5,6.);
    contraction = tube.ctcObs(derivative, intv_t, intv_y, false);
    REQUIRE(contraction);
    REQUIRE(intv_t == Interval(3.5,5.)); // optim: Interval(3.5,4.5)
    REQUIRE(intv_y == Interval(3.5,4.5)); // optim: Interval(3.5,4.5)
    REQUIRE(tube[0] == Interval(6.,8.));
    REQUIRE(tube[1] == Interval(4.5,7.));
    REQUIRE(tube[2] == Interval(4.,6.));
    REQUIRE(tube[3] == Interval(2.75,5.));
    REQUIRE(tube[4] == Interval(1.25,4.));
    REQUIRE(tube[5] == Interval(-0.25,3.));
    REQUIRE(tube[6] == Interval(-3.,2.));
    REQUIRE(tube[7] == Interval(-4.5,1.));
    REQUIRE(tube[8] == Interval(-6.,0.));
    REQUIRE(tube[9] == Interval(-7.5,-1.));
    REQUIRE(tube[10] == Interval(-9.,-2.));

    tube = tube_raw;
    intv_t = Interval(3.,8.5);
    intv_y = Interval(-7.,-2.5);
    contraction = tube.ctcObs(derivative, intv_t, intv_y, false);
    REQUIRE(contraction);
    REQUIRE(intv_t == Interval(6.,8.5)); // optim: Interval(6.5+?,8.5)
    REQUIRE(intv_y == Interval(-5.25,-2.5)); // optim: Interval(-5.25,-2.5)
    REQUIRE(tube[0] == Interval(6.,8.)); 
    REQUIRE(tube[1] == Interval(4.5,7.));
    REQUIRE(tube[2] == Interval(3.,6.));
    REQUIRE(tube[3] == Interval(1.5,5.));
    REQUIRE(tube[4] == Interval(-0.,4.));
    REQUIRE(tube[5] == Interval(-1.5,2.75));
    REQUIRE(tube[6] == Interval(-3.,1.25));
    REQUIRE(tube[7] == Interval(-4.5,-0.25));
    REQUIRE(tube[8] == Interval(-6.,-1.75));
    REQUIRE(tube[9] == Interval(-7.5,-3.));
    REQUIRE(tube[10] == Interval(-9.,-2.));

    tube = tube_raw;
    intv_t = Interval(7.,8.);
    intv_y = Interval(-2.,-1.);
    contraction = tube.ctcObs(derivative, intv_t, intv_y, false);
    REQUIRE(contraction);
    REQUIRE(intv_t == Interval(7.,8.));
    REQUIRE(intv_y == Interval(-2.,-1.));
    REQUIRE(tube[0] == Interval(6.,8.)); 
    REQUIRE(tube[1] == Interval(4.5,7.));
    REQUIRE(tube[2] == Interval(3.,6.));
    REQUIRE(tube[3] == Interval(1.5,5.));
    REQUIRE(tube[4] == Interval(-0.,4.));
    REQUIRE(tube[5] == Interval(-1.,3.));
    REQUIRE(tube[6] == Interval(-2.,2.));
    REQUIRE(tube[7] == Interval(-3.5,0.5));
    REQUIRE(tube[8] == Interval(-5.,-1.));
    REQUIRE(tube[9] == Interval(-6.5,-2.));
    REQUIRE(tube[10] == Interval(-9.,-2.));

    tube = tube_raw;
    intv_t = Interval(0.);
    intv_y = Interval(6.,8.);
    contraction = tube.ctcObs(derivative, intv_t, intv_y, false);
    REQUIRE(contraction);
    REQUIRE(intv_t == Interval(0.));
    REQUIRE(intv_y == Interval(7.,8.));
    REQUIRE(tube[0] == Interval(6.,8.)); 
    REQUIRE(tube[1] == Interval(4.5,7.));
    REQUIRE(tube[2] == Interval(3.,6.));
    REQUIRE(tube[3] == Interval(1.5,5.));
    REQUIRE(tube[4] == Interval(-0.,4.));
    REQUIRE(tube[5] == Interval(-1.5,3.));
    REQUIRE(tube[6] == Interval(-3.,2.));
    REQUIRE(tube[7] == Interval(-4.5,1.));
    REQUIRE(tube[8] == Interval(-6.,0.));
    REQUIRE(tube[9] == Interval(-7.5,-1.));
    REQUIRE(tube[10] == Interval(-9.,-2.));
  }

  SECTION("Test ctcIn, non-zero derivative (positive case)")
  {
    Tube tube(Interval(0.,20.), 1.);
    Tube derivative(tube);
    derivative.set(Interval(1.,1.5));
    tube.set(Interval(-8.,-6.), 0);
    tube.ctcFwdBwd(derivative);

    REQUIRE(tube[0] == Interval(-8.,-6.)); 
    REQUIRE(tube[1] == Interval(-7.,-4.5)); 
    REQUIRE(tube[2] == Interval(-6.,-3.)); 
    REQUIRE(tube[3] == Interval(-5.,-1.5)); 
    REQUIRE(tube[4] == Interval(-4.,0.)); 
    REQUIRE(tube[5] == Interval(-3.,1.5)); 
    REQUIRE(tube[6] == Interval(-2.,3.)); 
    REQUIRE(tube[7] == Interval(-1.,4.5)); 
    REQUIRE(tube[8] == Interval(-0.,6.)); 
    REQUIRE(tube[9] == Interval(1.,7.5)); 
    REQUIRE(tube[10] == Interval(2.,9.)); 

    bool contraction;
    Interval intv_t, intv_y;
    Tube tube_raw(tube);

    intv_t = Interval(5.,10.);
    intv_y = Interval(-3.,-1.);
    contraction = tube.ctcObs(derivative, intv_t, intv_y, false);
    REQUIRE(contraction);
    REQUIRE(intv_t == Interval(5.,8.));
    REQUIRE(intv_y == Interval(-3.,-1.));
    REQUIRE(tube[0] == Interval(-8.,-6.)); 
    REQUIRE(tube[1] == Interval(-7.,-4.5)); 
    REQUIRE(tube[2] == Interval(-6.,-3.)); 
    REQUIRE(tube[3] == Interval(-5.,-2.)); 
    REQUIRE(tube[4] == Interval(-4.,-1.)); 
    REQUIRE(tube[5] == Interval(-3.,0.5)); 
    REQUIRE(tube[6] == Interval(-2.,2.)); 
    REQUIRE(tube[7] == Interval(-1.,3.5)); 
    REQUIRE(tube[8] == Interval(0.,5.)); 
    REQUIRE(tube[9] == Interval(1.,7.5)); 
    REQUIRE(tube[10] == Interval(2.,9.)); 

    tube = tube_raw;
    intv_t = Interval(2.,5.5);
    intv_y = Interval(-0.5,3.);
    contraction = tube.ctcObs(derivative, intv_t, intv_y, false);
    REQUIRE(contraction);
    REQUIRE(intv_t == Interval(4.,5.5));
    REQUIRE(intv_y == Interval(-0.5,0.75));
    REQUIRE(tube[0] == Interval(-8.,-6.)); 
    REQUIRE(tube[1] == Interval(-7.,-4.5)); 
    REQUIRE(tube[2] == Interval(-6.,-3.)); 
    REQUIRE(tube[3] == Interval(-4.25,-1.5)); 
    REQUIRE(tube[4] == Interval(-2.75,0.)); 
    REQUIRE(tube[5] == Interval(-1.25,1.5)); 
    REQUIRE(tube[6] == Interval(0.,3.)); 
    REQUIRE(tube[7] == Interval(-1.,4.5)); 
    REQUIRE(tube[8] == Interval(0.,6.));
    REQUIRE(tube[9] == Interval(1.,7.5)); 
    REQUIRE(tube[10] == Interval(2.,9.)); 

    tube = tube_raw;
    intv_t = Interval(3.5,7.);
    intv_y = Interval(-5.2,-4.8);
    contraction = tube.ctcObs(derivative, intv_t, intv_y, false);
    REQUIRE(contraction);
    REQUIRE(intv_t == Interval::EMPTY_SET);
    REQUIRE(intv_y == Interval::EMPTY_SET);
    REQUIRE(tube[0] == Interval(-8.,-6.)); 
    REQUIRE(tube[1] == Interval(-7.,-4.5)); 
    REQUIRE(tube[2] == Interval::EMPTY_SET); 
    REQUIRE(tube[3] == Interval::EMPTY_SET); 
    REQUIRE(tube[4] == Interval::EMPTY_SET); 
    REQUIRE(tube[5] == Interval(-3.,1.5)); 
    REQUIRE(tube[6] == Interval(-2.,3.)); 
    REQUIRE(tube[7] == Interval(-1.,4.5)); 
    REQUIRE(tube[8] == Interval(-0.,6.)); 
    REQUIRE(tube[9] == Interval(1.,7.5)); 
    REQUIRE(tube[10] == Interval(2.,9.)); 
  }
}