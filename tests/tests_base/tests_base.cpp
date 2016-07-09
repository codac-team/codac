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

/*namespace ibex
{
  using namespace std;*/

  TEST_CASE("Reading tube", "[core]")
  {
    SECTION("Test tube1")
    {
      Tube tube = tubeTest1();
      tube.setY(ibex::Interval(-4,2), 14); // to test primitives pre-computation

      REQUIRE(tube.getY() == ibex::Interval(-11,13));
      REQUIRE(tube[7] == ibex::Interval(-11,-7));
      REQUIRE(tube[3.5] == ibex::Interval(-4,4));
      REQUIRE(tube[17.5] == ibex::Interval(2,7));
      REQUIRE(tube[32.5] == ibex::Interval(2,5));
      REQUIRE(tube[17.0] == ibex::Interval(2,6)); // t belongs to two slices
      REQUIRE(tube[16.9999999] == ibex::Interval(0,6));
      REQUIRE(tube[17.0000001] == ibex::Interval(2,7));
      REQUIRE(tube[ibex::Interval(9.5,17.5)] == ibex::Interval(-9,7));
      REQUIRE(tube[ibex::Interval(16.1,37.9)] == ibex::Interval(0,13));
      REQUIRE(tube[ibex::Interval(37.4,45.6)] == ibex::Interval(-1,6));
      REQUIRE(tube[ibex::Interval(14.0,27.0)] == ibex::Interval(-4,13));
      REQUIRE(tube[ibex::Interval(19.2,35.7)] == ibex::Interval(1,13));
      REQUIRE(tube[ibex::Interval(15.0,16.0)] == ibex::Interval(-2,4));
      REQUIRE(tube[ibex::Interval(0.)] == ibex::Interval(4,8));
      REQUIRE(tube[ibex::Interval(5.)] == ibex::Interval(-7,-5));
      REQUIRE(tube[ibex::Interval(24.)] == ibex::Interval(8,12));
      REQUIRE(tube[ibex::Interval(46.)] == ibex::Interval(-1,3));
    }

    SECTION("Test tube4")
    {
      Tube tube = tubeTest4();

      REQUIRE(tube.getY() == ibex::Interval(-1.5,2));
      REQUIRE(tube[0] == ibex::Interval(1,2));
      REQUIRE(tube[11] == ibex::Interval(-1.5,-0.5));
      REQUIRE(tube[11.0] == ibex::Interval(-1,-0.5));
      REQUIRE(tube[1.5] == ibex::Interval(1,2));
    }
  }

  TEST_CASE("Computing tube's volume", "[core]")
  {
    SECTION("Test tube1")
    {
      Tube tube = tubeTest1();
      tube.setY(ibex::Interval(-4,2), 14); // to test primitives pre-computation
      REQUIRE(tube.volume() == 197.);
    }

    SECTION("Test tube4")
    {
      Tube tube = tubeTest4();
      REQUIRE(tube.volume() == 23.);
    }
  }

  TEST_CASE("Converting inputs to indexes", "[core]")
  {
    SECTION("Test tube1")
    {
      Tube tube = tubeTest1();

      REQUIRE(tube.input2index(0.0) == 0);
      REQUIRE(tube.input2index(0.1) == 0);
      REQUIRE(tube.input2index(0.5) == 0);
      REQUIRE(tube.input2index(0.6) == 0);
      REQUIRE(tube.input2index(0.9) == 0);
      REQUIRE(tube.input2index(ibex::previous_float(1.0)) == 0);
      REQUIRE(tube.input2index(1.0) == 1);
      REQUIRE(tube.input2index(46.0) == 45);
    }
  }

  TEST_CASE("Converting indexes to inputs", "[core]")
  {
    SECTION("Test tube1")
    {
      Tube tube = tubeTest1();

      REQUIRE(tube.index2input(0) == 0.0);
      REQUIRE(tube.input2index(1) == 1.0);
      REQUIRE(tube.input2index(2) == 2.0);
      for(int i = 0 ; i < 46 ; i++)
        REQUIRE(i == tube.input2index(tube.index2input(i)));
    }
  }

  TEST_CASE("Testing union operation", "[core]")
  {
    SECTION("Test tube1|tube2")
    {
      Tube tube1 = tubeTest1();
      tube1.setY(ibex::Interval(-4,2), 14); // to test primitives pre-computation
      Tube tube2 = tubeTest2();
      tube1 |= tube2;

      REQUIRE(tube1.getY() == ibex::Interval(-11,13));
      REQUIRE(tube1[0] == ibex::Interval(-2,8));
      REQUIRE(tube1[5.5] == ibex::Interval(-9,6));
      REQUIRE(tube1[11] == ibex::Interval(-7,3));
      REQUIRE(tube1[21.3] == ibex::Interval(2,11));
      REQUIRE(tube1[45] == ibex::Interval(-1,3));
      REQUIRE(tube1[ibex::Interval(5,10)] == ibex::Interval(-11,6));
      REQUIRE(tube1[ibex::Interval(15,20)] == ibex::Interval(-3,9));
      REQUIRE(tube1[ibex::Interval(8.2,39.9)] == ibex::Interval(-10,13));
    }
  }

  TEST_CASE("Testing intersection operation", "[core]")
  {
    SECTION("Test tube1&tube2")
    {
      Tube tube1 = tubeTest1();
      tube1.setY(ibex::Interval(-4,2), 14); // to test primitives pre-computation
      Tube tube2 = tubeTest2();
      tube1 &= tube2;
      REQUIRE(tube1.getY() == ibex::Interval(-2,4));
      REQUIRE(tube1[0] == ibex::Interval::EMPTY_SET);
      REQUIRE(tube1[2.5] == ibex::Interval(1,3));
      REQUIRE(tube1[15.0] == ibex::Interval(-2,0));
      REQUIRE(tube1[44.5] == ibex::Interval(0,2));
      REQUIRE(tube1[39] == ibex::Interval(-1));
      REQUIRE(tube1[ibex::Interval(0,6)] == ibex::Interval(1,4));
      REQUIRE(tube1[ibex::Interval(11,19)] == ibex::Interval(-2,1));
      REQUIRE(tube1[ibex::Interval(39.5,40.3)] == ibex::Interval(-1));
    }
  }

  TEST_CASE("Testing enclosed bounds", "[core]")
  {
    SECTION("Test tube1")
    {
      Tube tube1 = tubeTest1();
      tube1.setY(ibex::Interval(-4,2), 14); // to test primitives pre-computation

      REQUIRE(tube1.getEnclosedBounds() == std::make_pair(ibex::Interval(-11,9), ibex::Interval(-7,13)));
      REQUIRE(tube1.getEnclosedBounds(ibex::Interval(0.5,25.5)) == std::make_pair(ibex::Interval(-11,9), ibex::Interval(-7,13)));
      REQUIRE(tube1.getEnclosedBounds(ibex::Interval(7.1,19.8)) == std::make_pair(ibex::Interval(-11,6), ibex::Interval(-7,9)));
      REQUIRE(tube1.getEnclosedBounds(ibex::Interval(6.0,9.0)) == std::make_pair(ibex::Interval(-11,-10), ibex::Interval(-7,-6)));
      REQUIRE(tube1.getEnclosedBounds(ibex::Interval(0.)) == std::make_pair(ibex::Interval(4), ibex::Interval(8)));
      REQUIRE(tube1.getEnclosedBounds(ibex::Interval(5.)) == std::make_pair(ibex::Interval(-7), ibex::Interval(-5)));
      REQUIRE(tube1.getEnclosedBounds(ibex::Interval(5.2)) == std::make_pair(ibex::Interval(-9), ibex::Interval(-5)));
      REQUIRE(tube1.getEnclosedBounds(ibex::Interval(23.,24.)) == std::make_pair(ibex::Interval(8), ibex::Interval(13)));
      REQUIRE(tube1.getEnclosedBounds(ibex::Interval(22.,25.)) == std::make_pair(ibex::Interval(7,9), ibex::Interval(12,13)));
      REQUIRE(tube1.getEnclosedBounds(ibex::Interval(21.,23.)) == std::make_pair(ibex::Interval(8,9), ibex::Interval(11,12)));
    }

    SECTION("Test tube4")
    {
      Tube tube4 = tubeTest4();
      Tube tube4_primitive = tube4.primitive();
      REQUIRE(tube4_primitive.getEnclosedBounds(ibex::Interval(12.5,14.5)) == std::make_pair(ibex::Interval(6,6.5), ibex::Interval(21,24.5)));
    }
  }

  TEST_CASE("Testing set inversion", "[core]")
  {
    SECTION("Scalar set inversion")
    {
      Tube tube = tubeTest1();
      tube.setY(ibex::Interval(-4,2), 14); // to test primitives pre-computation
      REQUIRE(tube.setInversion(ibex::Interval(0.)) == ibex::Interval(3.0,46.0));
      REQUIRE(tube.setInversion(ibex::Interval::ALL_REALS) == ibex::Interval(0.0,46.0));
      REQUIRE(tube.setInversion(ibex::Interval(-12.0,14.0)) == ibex::Interval(0.0,46.0));
      REQUIRE(tube.setInversion(ibex::Interval(-20,-18)) == ibex::Interval::EMPTY_SET);
      REQUIRE(tube.setInversion(ibex::Interval(-1.0,1.0)) == ibex::Interval(2.0,46.0));
      REQUIRE(tube.setInversion(ibex::Interval(-10.5)) == ibex::Interval(7.0,8.0));
      REQUIRE(tube.setInversion(ibex::Interval(-12.0,-7.0)) == ibex::Interval(4.0,12.0));
      REQUIRE(tube.setInversion(ibex::Interval(10.0,11.0)) == ibex::Interval(20.0,27.0));
      REQUIRE(tube.setInversion(ibex::Interval(6.01,7.0)) == ibex::Interval(0.0,30.0));
      REQUIRE(tube.setInversion(ibex::Interval(6.0,7.0)) == ibex::Interval(0.0,43.0));
      REQUIRE(tube.setInversion(ibex::Interval(5.9,7.0)) == ibex::Interval(0.0,43.0));
    }
    
    SECTION("std::vector set inversion")
    {
      Tube tube = tubeTest1();
      tube.setY(ibex::Interval(-4,2), 14); // to test primitives pre-computation

      std::vector<ibex::Interval> v;

      tube.setInversion(ibex::Interval(0.), v);
      REQUIRE(v.size() == 4);

      if(v.size() == 4)
      {
        REQUIRE(v[0] == ibex::Interval(3.0,4.0));
        REQUIRE(v[1] == ibex::Interval(14.0,17.0));
        REQUIRE(v[2] == ibex::Interval(37.0,42.0));
        REQUIRE(v[3] == ibex::Interval(43.0,46.0));
      }

      tube.setInversion(ibex::Interval(-1.0,1.0), v);
      REQUIRE(v.size() == 4);

      if(v.size() == 4)
      {
        REQUIRE(v[0] == ibex::Interval(2.0,5.0));
        REQUIRE(v[1] == ibex::Interval(13.0,17.0));
        REQUIRE(v[2] == ibex::Interval(34.0,35.0));
        REQUIRE(v[3] == ibex::Interval(36.0,46.0));
      }

      tube.setInversion(ibex::Interval::ALL_REALS, v);
      REQUIRE(v.size() == 1);

      if(v.size() == 1)
        REQUIRE(v[0] == ibex::Interval(0.0,46.0));

      tube.setInversion(ibex::Interval(-30.0,-29.0), v);
      REQUIRE(v.size() == 0);

      tube.setInversion(ibex::Interval(3.5), v);
      REQUIRE(v.size() == 5);

      if(v.size() == 5)
      {
        REQUIRE(v[0] == ibex::Interval(1.0,4.0));
        REQUIRE(v[1] == ibex::Interval(15.0,18.0));
        REQUIRE(v[2] == ibex::Interval(26.0,27.0));
        REQUIRE(v[3] == ibex::Interval(30.0,38.0));
        REQUIRE(v[4] == ibex::Interval(40.0,45.0));
      }

      tube.setInversion(ibex::Interval(9.5,30.0), v);
      REQUIRE(v.size() == 1);

      if(v.size() == 1)
        REQUIRE(v[0] == ibex::Interval(20.0,27.0));

      tube.setInversion(ibex::Interval(12.0,13.0), v);
      REQUIRE(v.size() == 1);

      if(v.size() == 1)
        REQUIRE(v[0] == ibex::Interval(22.0,25.0));

      tube.setInversion(ibex::Interval(-4.0,-3.0), v);
      REQUIRE(v.size() == 3);

      if(v.size() == 3)
      {
        REQUIRE(v[0] == ibex::Interval(3.0,5.0));
        REQUIRE(v[1] == ibex::Interval(9.0,10.0));
        REQUIRE(v[2] == ibex::Interval(11.0,15.0));
      }
    }
  }
//}