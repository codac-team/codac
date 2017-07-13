/* ============================================================================
 *  tubex-lib - Tube tests
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#define CATCH_CONFIG_MAIN
#include "../catch/catch.hpp"
#include "../catch/catch_interval.hpp"
#include "../tests_cases.hpp"

using namespace Catch;
using namespace Detail;
using namespace tubex;

TEST_CASE("Equality, consistency", "[core]")
{
  SECTION("Test tube1")
  {
    Tube tube_a = tubeTest1();
    Tube tube_b = tubeTest1();
    REQUIRE(tube_a == tube_a);
    REQUIRE(tube_a == tube_b);
  }

  SECTION("Test tube4")
  {
    Tube tube_a = tubeTest4();
    Tube tube_b = tubeTest4();
    REQUIRE(tube_a == tube_a);
    REQUIRE(tube_a == tube_b);
  }
}

TEST_CASE("Reading tube", "[core]")
{
  SECTION("Test tube1")
  {
    Tube tube = tubeTest1();
    tube.set(Interval(-4,2), 14); // to test primitives pre-computation

    REQUIRE(tube.image() == Interval(-11,13));
    REQUIRE(tube[7] == Interval(-11,-7));
    REQUIRE(tube[3.5] == Interval(-4,4));
    REQUIRE(tube[17.5] == Interval(2,7));
    REQUIRE(tube[32.5] == Interval(2,5));
    REQUIRE(tube[17.0] == Interval(2,6)); // t belongs to two slices
    REQUIRE(tube[16.9999999] == Interval(0,6));
    REQUIRE(tube[17.0000001] == Interval(2,7));
    REQUIRE(tube[Interval(9.5,17.5)] == Interval(-9,7));
    REQUIRE(tube[Interval(16.1,37.9)] == Interval(0,13));
    REQUIRE(tube[Interval(37.4,45.6)] == Interval(-1,6));
    REQUIRE(tube[Interval(14.0,27.0)] == Interval(-4,13));
    REQUIRE(tube[Interval(19.2,35.7)] == Interval(1,13));
    REQUIRE(tube[Interval(15.0,16.0)] == Interval(-2,4));
    REQUIRE(tube[Interval(0.)] == Interval(4,8));
    REQUIRE(tube[Interval(5.)] == Interval(-7,-5));
    REQUIRE(tube[Interval(24.)] == Interval(8,12));
    REQUIRE(tube[Interval(46.)] == Interval(-1,3));
  }

  SECTION("Test tube4")
  {
    Tube tube = tubeTest4();

    REQUIRE(tube.image() == Interval(-1.5,2));
    REQUIRE(tube[0] == Interval(1,2));
    REQUIRE(tube[11] == Interval(-1.5,-0.5));
    REQUIRE(tube[11.0] == Interval(-1,-0.5));
    REQUIRE(tube[1.5] == Interval(1,2));
  }
}

TEST_CASE("Computing tube's volume", "[core]")
{
  SECTION("Test tube1")
  {
    Tube tube = tubeTest1();
    tube.set(Interval(-4,2), 14); // to test primitives pre-computation
    REQUIRE(tube.volume() == 197.);
  }

  SECTION("Test tube4")
  {
    Tube tube = tubeTest4();
    REQUIRE(tube.volume() == 23.);
  }
}

TEST_CASE("Computing empty test", "[core]")
{
  SECTION("Test tube1")
  {
    Tube tube = tubeTest1();
    tube.set(Interval::EMPTY_SET);
    REQUIRE(tube.isEmpty());
  }

  SECTION("Test tube1 bis")
  {
    Tube tube = tubeTest1();
    tube.set(Interval::EMPTY_SET, 10);
    REQUIRE(!tube.isEmpty());
  }
}

TEST_CASE("Computing discontinuity", "[core]")
{
  SECTION("Test tube1")
  {
    Tube tube = tubeTest1();
    tube.set(Interval(4,7), 20);
    REQUIRE(tube.isDiscontinuous());
  }

  SECTION("Test tube1 bis")
  {
    Tube tube = tubeTest1();
    tube.set(Interval::EMPTY_SET, 10);
    REQUIRE(tube.isDiscontinuous());
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
    REQUIRE(tube.input2index(previous_float(1.0)) == 0);
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
    tube1.set(Interval(-4,2), 14); // to test primitives pre-computation
    Tube tube2 = tubeTest2();
    tube1 |= tube2;

    REQUIRE(tube1.image() == Interval(-11,13));
    REQUIRE(tube1[0] == Interval(-2,8));
    REQUIRE(tube1[5.5] == Interval(-9,6));
    REQUIRE(tube1[11] == Interval(-7,3));
    REQUIRE(tube1[21.3] == Interval(2,11));
    REQUIRE(tube1[45] == Interval(-1,3));
    REQUIRE(tube1[Interval(5,10)] == Interval(-11,6));
    REQUIRE(tube1[Interval(15,20)] == Interval(-3,9));
    REQUIRE(tube1[Interval(8.2,39.9)] == Interval(-10,13));
  }
}

TEST_CASE("Testing intersection operation", "[core]")
{
  SECTION("Test tube1&tube2")
  {
    Tube tube1 = tubeTest1();
    tube1.set(Interval(-4,2), 14); // to test primitives pre-computation
    Tube tube2 = tubeTest2();
    tube1 &= tube2;
    REQUIRE(tube1.image() == Interval(-2,4));
    REQUIRE(tube1[0] == Interval::EMPTY_SET);
    REQUIRE(tube1[2.5] == Interval(1,3));
    REQUIRE(tube1[15.0] == Interval(-2,0));
    REQUIRE(tube1[44.5] == Interval(0,2));
    REQUIRE(tube1[39] == Interval(-1));
    REQUIRE(tube1[Interval(0,6)] == Interval(1,4));
    REQUIRE(tube1[Interval(11,19)] == Interval(-2,1));
    REQUIRE(tube1[Interval(39.5,40.3)] == Interval(-1));
  }
}

TEST_CASE("Testing enclosed bounds", "[core]")
{
  SECTION("Test tube1")
  {
    Tube tube1 = tubeTest1();
    tube1.set(Interval(-4,2), 14); // to test primitives pre-computation

    REQUIRE(tube1.eval() == make_pair(Interval(-11,9), Interval(-7,13)));
    REQUIRE(tube1.eval(Interval(0.5,25.5)) == make_pair(Interval(-11,9), Interval(-7,13)));
    REQUIRE(tube1.eval(Interval(7.1,19.8)) == make_pair(Interval(-11,6), Interval(-7,9)));
    REQUIRE(tube1.eval(Interval(6.0,9.0)) == make_pair(Interval(-11,-10), Interval(-7,-6)));
    REQUIRE(tube1.eval(Interval(0.)) == make_pair(Interval(4), Interval(8)));
    REQUIRE(tube1.eval(Interval(5.)) == make_pair(Interval(-7), Interval(-5)));
    REQUIRE(tube1.eval(Interval(5.2)) == make_pair(Interval(-9), Interval(-5)));
    REQUIRE(tube1.eval(Interval(23.,24.)) == make_pair(Interval(8), Interval(13)));
    REQUIRE(tube1.eval(Interval(22.,25.)) == make_pair(Interval(7,9), Interval(12,13)));
    REQUIRE(tube1.eval(Interval(21.,23.)) == make_pair(Interval(8,9), Interval(11,12)));
  }

  SECTION("Test tube4")
  {
    Tube tube4 = tubeTest4();
    Tube tube4_primitive = tube4.primitive();
    REQUIRE(tube4_primitive.eval(Interval(12.5,14.5)) == make_pair(Interval(6,6.5), Interval(21,24.5)));
  }
}

TEST_CASE("Testing set inversion", "[core]")
{
  SECTION("Scalar set inversion")
  {
    Tube tube = tubeTest1();
    tube.set(Interval(-4,2), 14); // to test primitives pre-computation
    REQUIRE(tube.invert(Interval(0.)) == Interval(3.0,46.0));
    REQUIRE(tube.invert(Interval(-7.)) == Interval(4.0,12.0));
    REQUIRE(tube.invert(Interval::ALL_REALS) == Interval(0.0,46.0));
    REQUIRE(tube.invert(Interval(-12.0,14.0)) == Interval(0.0,46.0));
    REQUIRE(tube.invert(Interval(-20,-18)) == Interval::EMPTY_SET);
    REQUIRE(tube.invert(Interval(-1.0,1.0)) == Interval(2.0,46.0));
    REQUIRE(tube.invert(Interval(-10.5)) == Interval(7.0,8.0));
    REQUIRE(tube.invert(Interval(-12.0,-7.0)) == Interval(4.0,12.0));
    REQUIRE(tube.invert(Interval(10.0,11.0)) == Interval(20.0,27.0));
    REQUIRE(tube.invert(Interval(6.01,7.0)) == Interval(0.0,30.0));
    REQUIRE(tube.invert(Interval(6.0,7.0)) == Interval(0.0,43.0));
    REQUIRE(tube.invert(Interval(5.9,7.0)) == Interval(0.0,43.0));
  }
  
  SECTION("Vector set inversion")
  {
    Tube tube = tubeTest1();
    tube.set(Interval(-4,2), 14); // to test primitives pre-computation

    vector<Interval> v;

    tube.invert(Interval(0.), v);
    REQUIRE(v.size() == 4);

    if(v.size() == 4)
    {
      REQUIRE(v[0] == Interval(3.0,4.0));
      REQUIRE(v[1] == Interval(14.0,17.0));
      REQUIRE(v[2] == Interval(37.0,42.0));
      REQUIRE(v[3] == Interval(43.0,46.0));
    }

    // The same, with a custom domain:
    tube.invert(Interval(0.), v, Interval(3.8,42.5));
    REQUIRE(v.size() == 3);

    if(v.size() == 3)
    {
      REQUIRE(v[0] == Interval(3.8,4.0));
      REQUIRE(v[1] == Interval(14.0,17.0));
      REQUIRE(v[2] == Interval(37.0,42.0));
    }

    tube.invert(Interval(-1.0,1.0), v, Interval::ALL_REALS);
    REQUIRE(v.size() == 4);

    if(v.size() == 4)
    {
      REQUIRE(v[0] == Interval(2.0,5.0));
      REQUIRE(v[1] == Interval(13.0,17.0));
      REQUIRE(v[2] == Interval(34.0,35.0));
      REQUIRE(v[3] == Interval(36.0,46.0));
    }

    // The same, with a custom domain (empty):
    tube.invert(Interval(-1.0,1.0), v, Interval::EMPTY_SET);
    REQUIRE(v.size() == 0);

    tube.invert(Interval(-6.9999), v);
    REQUIRE(v.size() == 2);

    if(v.size() == 2)
    {
      REQUIRE(v[0] == Interval(4.,7.));
      REQUIRE(v[1] == Interval(8.,12.));
    }

    tube.invert(Interval::ALL_REALS, v);
    REQUIRE(v.size() == 1);

    if(v.size() == 1)
      REQUIRE(v[0] == Interval(0.0,46.0));

    tube.invert(Interval(-30.0,-29.0), v);
    REQUIRE(v.size() == 0);

    tube.invert(Interval(3.5), v);
    REQUIRE(v.size() == 5);

    if(v.size() == 5)
    {
      REQUIRE(v[0] == Interval(1.0,4.0));
      REQUIRE(v[1] == Interval(15.0,18.0));
      REQUIRE(v[2] == Interval(26.0,27.0));
      REQUIRE(v[3] == Interval(30.0,38.0));
      REQUIRE(v[4] == Interval(40.0,45.0));
    }

    tube.invert(Interval(9.5,30.0), v);
    REQUIRE(v.size() == 1);

    if(v.size() == 1)
      REQUIRE(v[0] == Interval(20.0,27.0));

    tube.invert(Interval(12.0,13.0), v);
    REQUIRE(v.size() == 1);

    if(v.size() == 1)
      REQUIRE(v[0] == Interval(22.0,25.0));

    tube.invert(Interval(-4.0,-3.0), v);
    REQUIRE(v.size() == 3);

    if(v.size() == 3)
    {
      REQUIRE(v[0] == Interval(3.0,5.0));
      REQUIRE(v[1] == Interval(9.0,10.0));
      REQUIRE(v[2] == Interval(11.0,15.0));
    }
  }
}

TEST_CASE("Testing subtube", "[core]")
{
  SECTION("Test tube1")
  {
    Tube tube = tubeTest1();
    Tube subtube = tube.subtube(Interval(3.5,13.5));
    REQUIRE(subtube.size() == 11);
    REQUIRE(subtube.dt() == tube.dt());
    REQUIRE(subtube[0] == Interval(-4.,4.));
    REQUIRE(subtube[1] == Interval(-7.,-1.));
    REQUIRE(subtube[2] == Interval(-9.,-5.));
    REQUIRE(subtube[3] == Interval(-10.,-6.));
    REQUIRE(subtube[4] == Interval(-11.,-7.));
    REQUIRE(subtube[5] == Interval(-10.,-6.));
    REQUIRE(subtube[6] == Interval(-9.,-4.));
    REQUIRE(subtube[7] == Interval(-8.,-5.));
    REQUIRE(subtube[8] == Interval(-7.,-4.));
    REQUIRE(subtube[9] == Interval(-6.,-2.));
    REQUIRE(subtube[10] == Interval(-5.,-1.));
  }

  SECTION("Test tube1 bis")
  {
    Tube tube = tubeTest1();
    Tube subtube = tube.subtube(Interval(10.,12.));
    REQUIRE(subtube.size() == 2);
    REQUIRE(subtube.dt() == tube.dt());
    REQUIRE(subtube[0] == Interval(-8.,-5.));
    REQUIRE(subtube[1] == Interval(-7.,-4.));
  }

  SECTION("Test tube1: consistency")
  {
    Tube tube = tubeTest1();
    Tube subtube = tube.subtube(tube.domain());
    REQUIRE(tube == subtube);
  }
}

TEST_CASE("Testing interpol (double)", "[core]")
{
  SECTION("Test tube1")
  {
    Tube tube_derivative = tubeTest4();
    Tube tube = tube_derivative.primitive();
    REQUIRE(ApproxIntv(tube.interpol(0., tube_derivative)) == Interval(0.,1.));
    REQUIRE(ApproxIntv(tube.interpol(0.1, tube_derivative)) == Interval(0.1,1.1));
    REQUIRE(ApproxIntv(tube.interpol(0.5, tube_derivative)) == Interval(0.5,1.5));
    REQUIRE(ApproxIntv(tube.interpol(1.0, tube_derivative)) == Interval(1.,2.));
    REQUIRE(ApproxIntv(tube.interpol(1.1, tube_derivative)) == Interval(1.1,2.2));
    REQUIRE(ApproxIntv(tube.interpol(1.95, tube_derivative)) == Interval(1.95,3.9));
    REQUIRE(ApproxIntv(tube.interpol(2.0, tube_derivative)) == Interval(2.0,4.0));
    REQUIRE(ApproxIntv(tube.interpol(2.2, tube_derivative)) == Interval(2.2,4.4));
    REQUIRE(ApproxIntv(tube.interpol(21., tube_derivative)) == Interval(13.5,36.5));
  }
}

TEST_CASE("Testing interpol (Interval)", "[core]")
{
  SECTION("Test tube1")
  {
    Tube tube_derivative = tubeTest4();
    Tube tube = tube_derivative.primitive();
    REQUIRE(ApproxIntv(tube.interpol(Interval(0.), tube_derivative)) == Interval(0.,1.));
    REQUIRE(ApproxIntv(tube.interpol(Interval(0.1), tube_derivative)) == Interval(0.1,1.1));
    REQUIRE(ApproxIntv(tube.interpol(Interval(0.5), tube_derivative)) == Interval(0.5,1.5));
    REQUIRE(ApproxIntv(tube.interpol(Interval(1.0), tube_derivative)) == Interval(1.,2.));
    REQUIRE(ApproxIntv(tube.interpol(Interval(1.1), tube_derivative)) == Interval(1.1,2.2));
    REQUIRE(ApproxIntv(tube.interpol(Interval(1.95), tube_derivative)) == Interval(1.95,3.9));
    REQUIRE(ApproxIntv(tube.interpol(Interval(2.0), tube_derivative)) == Interval(2.0,4.0));
    REQUIRE(ApproxIntv(tube.interpol(Interval(2.2), tube_derivative)) == Interval(2.2,4.4));
    REQUIRE(ApproxIntv(tube.interpol(Interval(21.), tube_derivative)) == Interval(13.5,36.5));

    REQUIRE(ApproxIntv(tube.interpol(Interval(0.,5.), tube_derivative)) == Interval(0.,10.));
    REQUIRE(ApproxIntv(tube.interpol(Interval(0.1,4.9), tube_derivative)) == Interval(0.1,9.8));
    REQUIRE(ApproxIntv(tube.interpol(Interval(0.,21.), tube_derivative)) == Interval(0.,36.5));
    REQUIRE(ApproxIntv(tube.interpol(Interval(7.5,11.5), tube_derivative)) == Interval(7.5,20.25));
  }
}

TEST_CASE("Testing interpol (pair<Interval,Interval>)", "[core]")
{
  SECTION("Test tube1")
  {
    Tube tube_derivative = tubeTest4();
    Tube tube = tube_derivative.primitive();
    REQUIRE(ApproxIntvPair(tube.partialInterpol(Interval(0.), tube_derivative)) == make_pair(Interval(0.),Interval(1.)));
    REQUIRE(ApproxIntvPair(tube.partialInterpol(Interval(0.1), tube_derivative)) == make_pair(Interval(0.1),Interval(1.1)));
    REQUIRE(ApproxIntvPair(tube.partialInterpol(Interval(0.5), tube_derivative)) == make_pair(Interval(0.5),Interval(1.5)));
    REQUIRE(ApproxIntvPair(tube.partialInterpol(Interval(1.0), tube_derivative)) == make_pair(Interval(1.),Interval(2.)));
    REQUIRE(ApproxIntvPair(tube.partialInterpol(Interval(1.1), tube_derivative)) == make_pair(Interval(1.1),Interval(2.2)));
    REQUIRE(ApproxIntvPair(tube.partialInterpol(Interval(1.95), tube_derivative)) == make_pair(Interval(1.95),Interval(3.9)));
    REQUIRE(ApproxIntvPair(tube.partialInterpol(Interval(2.0), tube_derivative)) == make_pair(Interval(2.0),Interval(4.0)));
    REQUIRE(ApproxIntvPair(tube.partialInterpol(Interval(2.2), tube_derivative)) == make_pair(Interval(2.2),Interval(4.4)));
    REQUIRE(ApproxIntvPair(tube.partialInterpol(Interval(21.), tube_derivative)) == make_pair(Interval(13.5),Interval(36.5)));

    REQUIRE(ApproxIntvPair(tube.partialInterpol(Interval(0.,5.), tube_derivative)) == make_pair(Interval(0.,5.),Interval(1.,10.)));
    REQUIRE(ApproxIntvPair(tube.partialInterpol(Interval(0.1,4.9), tube_derivative)) == make_pair(Interval(0.1,4.9),Interval(1.1,9.8)));
    REQUIRE(ApproxIntvPair(tube.partialInterpol(Interval(0.,21.), tube_derivative)) == make_pair(Interval(0.,13.5),Interval(1.,36.5)));
    REQUIRE(ApproxIntvPair(tube.partialInterpol(Interval(7.5,9.5), tube_derivative)) == make_pair(Interval(7.5,9.25),Interval(15.,18.75)));
  }
}

TEST_CASE("Testing intersect", "[core]")
{
  SECTION("Test tube1")
  {
    Tube tube = tubeTest1();
    tube.intersect(Interval(1.9,7.1), 1);
    REQUIRE(tube[1] == Interval(2,7));
    tube.intersect(Interval(0.9), 2);
    REQUIRE(tube[2] == Interval::EMPTY_SET);
    tube.intersect(Interval(-1.,1.), 3);
    REQUIRE(tube[3] == Interval(-1.,1.));
    tube.intersect(Interval(-2.,10.), 4);
    REQUIRE(tube[4] == Interval(-2.,-1.));
    tube.intersect(Interval::ALL_REALS, 5);
    REQUIRE(tube[5] == Interval(-9,-5));
    tube.intersect(Interval::EMPTY_SET, 6);
    REQUIRE(tube[6] == Interval::EMPTY_SET);
  }
}