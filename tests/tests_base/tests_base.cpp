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




/*TEST_CASE("Testing subtube", "[core]")
{
  SECTION("Test tube1")
  {
    Tube tube = tubeTest1();
    Tube subtube = tube.subtube(Interval(3.5,13.5));
    REQUIRE(subtube.size() == 11);
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
    REQUIRE(subtube[0] == Interval(-8.,-5.));
    REQUIRE(subtube[1] == Interval(-7.,-4.));
  }

  SECTION("Test tube1: consistency")
  {
    Tube tube = tubeTest1();
    Tube subtube = tube.subtube(tube.domain());
    REQUIRE(tube == subtube);
  }
}*/

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

TEST_CASE("Testing sampling", "[core]")
{
  SECTION("Test tube1")
  {
    Tube tube = tubeTest1();
    REQUIRE(ApproxIntv(tube.domain(0)) == Interval(0.,1.));
    REQUIRE(ApproxIntv(tube.domain(1)) == Interval(1.,2.));
    REQUIRE(tube.size() == 46);
    tube.sample(0);
    REQUIRE(ApproxIntv(tube.domain(0)) == Interval(0.,0.5));
    REQUIRE(ApproxIntv(tube.domain(1)) == Interval(0.5,1.));
    REQUIRE(ApproxIntv(tube.domain(2)) == Interval(1.,2.));
    REQUIRE(tube.size() == 47);
    tube.sample(0);
    REQUIRE(ApproxIntv(tube.domain(0)) == Interval(0.,0.25));
    REQUIRE(ApproxIntv(tube.domain(1)) == Interval(0.25,0.5));
    REQUIRE(ApproxIntv(tube.domain(2)) == Interval(0.5,1.));
    REQUIRE(ApproxIntv(tube.domain(3)) == Interval(1.,2.));
    REQUIRE(tube.size() == 48);
  }
}