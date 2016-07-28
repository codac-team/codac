/* ============================================================================
 *  tube-lib - Tube class tests
 * ============================================================================
 *  Copyright : Copyright 2016 Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the Apache License, Version 2.0. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#define CATCH_CONFIG_MAIN
#include "../catch/catch.hpp"
#include "../tests_cases.hpp"

TEST_CASE("Contracting tube: ctcIntertemporal(t1, t2)", "[core]")
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

TEST_CASE("Contracting tube: ctcIntertemporal(y, t1, t2)", "[core]")
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