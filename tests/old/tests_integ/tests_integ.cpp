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

TEST_CASE("Computing tube's primitive", "[core]")
{
  SECTION("Test tube3")
  {
    Tube tube = tubeTest3();
    REQUIRE(ApproxIntv(tube[0]) == Interval(1,3));
    REQUIRE(ApproxIntv(tube[1]) == Interval(0,2));
    REQUIRE(ApproxIntv(tube[2]) == Interval(-1,1));
    REQUIRE(ApproxIntv(tube[3]) == Interval(-2,0));
    REQUIRE(ApproxIntv(tube[4]) == Interval(-3,-1));

    Tube tube_primitive = tube.primitive();
    REQUIRE(ApproxIntv(tube_primitive[0]) == Interval(0,3));
    REQUIRE(ApproxIntv(tube_primitive[1]) == Interval(1,5));
    REQUIRE(ApproxIntv(tube_primitive[2]) == Interval(0,6));
    REQUIRE(ApproxIntv(tube_primitive[3]) == Interval(-2,6));
    REQUIRE(ApproxIntv(tube_primitive[4]) == Interval(-5,6));
  }

  SECTION("Test tube4")
  {
    Tube tube = tubeTest4();
    Tube tube_primitive = tube.primitive();
    REQUIRE(ApproxIntv(tube_primitive[0]) == Interval(0,2));
    REQUIRE(ApproxIntv(tube_primitive[1]) == Interval(1,4));
    REQUIRE(ApproxIntv(tube_primitive[2]) == Interval(2,6));
    REQUIRE(ApproxIntv(tube_primitive[3]) == Interval(3,8));
    REQUIRE(ApproxIntv(tube_primitive[4]) == Interval(4,10));
    REQUIRE(ApproxIntv(tube_primitive[5]) == Interval(5,12));
    REQUIRE(ApproxIntv(tube_primitive[6]) == Interval(6,14));
    REQUIRE(ApproxIntv(tube_primitive[7]) == Interval(7,16));
    REQUIRE(ApproxIntv(tube_primitive[8]) == Interval(8,18));
    REQUIRE(ApproxIntv(tube_primitive[9]) == Interval(9,19.5));
    REQUIRE(ApproxIntv(tube_primitive[10]) == Interval(8.5,20.5));
    REQUIRE(ApproxIntv(tube_primitive[11]) == Interval(7,20.5));
    REQUIRE(ApproxIntv(tube_primitive[12]) == Interval(6,21));
    REQUIRE(ApproxIntv(tube_primitive[13]) == Interval(6,22.5));
    REQUIRE(ApproxIntv(tube_primitive[14]) == Interval(6.5,24.5));
    REQUIRE(ApproxIntv(tube_primitive[15]) == Interval(7.5,26.5));
    REQUIRE(ApproxIntv(tube_primitive[16]) == Interval(8.5,28.5));
    REQUIRE(ApproxIntv(tube_primitive[17]) == Interval(9.5,30.5));
    REQUIRE(ApproxIntv(tube_primitive[18]) == Interval(10.5,32.5));
    REQUIRE(ApproxIntv(tube_primitive[19]) == Interval(11.5,34.5));
    REQUIRE(ApproxIntv(tube_primitive[20]) == Interval(12.5,36.5));
  }

  SECTION("Test tube1")
  {
    Tube tube = tubeTest1();
    tube.set(Interval(-4,2), 14); // to test primitives pre-computation
    Tube tube_primitive = tube.primitive();
    REQUIRE(ApproxIntv(tube_primitive[0]) == Interval(0,8));
    REQUIRE(ApproxIntv(tube_primitive[1]) == Interval(4,15));
    REQUIRE(ApproxIntv(tube_primitive[2]) == Interval(6,21));
  }

  SECTION("Test tube4(05)")
  {
    Tube tube = tubeTest4_05();
    Tube tube_primitive = tube.primitive();
    REQUIRE(ApproxIntv(tube_primitive[0]) == Interval(0.0,1));
    REQUIRE(ApproxIntv(tube_primitive[1]) == Interval(0.5,2));
    REQUIRE(ApproxIntv(tube_primitive[2]) == Interval(1.0,3));
    REQUIRE(ApproxIntv(tube_primitive[3]) == Interval(1.5,4));
    REQUIRE(ApproxIntv(tube_primitive[4]) == Interval(2.0,5));
    REQUIRE(ApproxIntv(tube_primitive[5]) == Interval(2.5,6));
    REQUIRE(ApproxIntv(tube_primitive[6]) == Interval(3.0,7));
    REQUIRE(ApproxIntv(tube_primitive[7]) == Interval(3.5,8));
    REQUIRE(ApproxIntv(tube_primitive[8]) == Interval(4.0,9));
    REQUIRE(ApproxIntv(tube_primitive[9]) == Interval(4.5,10));
    REQUIRE(ApproxIntv(tube_primitive[10]) == Interval(5.0,11));
    REQUIRE(ApproxIntv(tube_primitive[11]) == Interval(5.5,12));
    REQUIRE(ApproxIntv(tube_primitive[12]) == Interval(6.0,13));
    REQUIRE(ApproxIntv(tube_primitive[13]) == Interval(6.5,14));
  }
}

TEST_CASE("Computing integration from 0, double argument", "[core]")
{
  SECTION("Test tube1")
  {
    Tube tube = tubeTest1();
    tube.set(Interval(-4,2), 14); // to test primitives pre-computation
    REQUIRE(ApproxIntv(tube.integral(0)) == Interval(0));
    REQUIRE(ApproxIntv(tube.integral(0.1)) == Interval(0.4,0.8));
    REQUIRE(ApproxIntv(tube.integral(1.1)) == Interval(4.2,8.7));
    REQUIRE(ApproxIntv(tube.integral(0.5)) == Interval(2,4));
    REQUIRE(ApproxIntv(tube.integral(1.0)) == Interval(4,8));
    REQUIRE(ApproxIntv(tube.integral(1.5)) == Interval(5,11.5));
    REQUIRE(ApproxIntv(tube.integral(2.0)) == Interval(6,15));
    REQUIRE(ApproxIntv(tube.integral(2.5)) == Interval(6.5,18));
    REQUIRE(ApproxIntv(tube.integral(2.9)) == Interval(6.9,20.4));
    REQUIRE(ApproxIntv(tube.integral(3.0)) == Interval(7,21));
    REQUIRE(ApproxIntv(tube.integral(3.1)) == Interval(6.6,21.4));
    REQUIRE(ApproxIntv(tube.integral(4.0)) == Interval(3,25));
  }

  SECTION("Test tube1(01)")
  {
    Tube tube = tubeTest1_01();
    REQUIRE(ApproxIntv(tube.integral(0)) == Interval(0));
    REQUIRE(ApproxIntv(tube.integral(0.1)) == Interval(0.4,0.8));
    REQUIRE(ApproxIntv(tube.integral(1.1)) == Interval(4.2,8.7));
    REQUIRE(ApproxIntv(tube.integral(0.5)) == Interval(2,4));
    REQUIRE(ApproxIntv(tube.integral(1.0)) == Interval(4,8));
    REQUIRE(ApproxIntv(tube.integral(1.5)) == Interval(5,11.5));
    REQUIRE(ApproxIntv(tube.integral(2.0)) == Interval(6,15));
    REQUIRE(ApproxIntv(tube.integral(2.5)) == Interval(6.5,18));
    REQUIRE(ApproxIntv(tube.integral(2.9)) == Interval(6.9,20.4));
    REQUIRE(ApproxIntv(tube.integral(3.0)) == Interval(7,21));
    REQUIRE(ApproxIntv(tube.integral(3.1)) == Interval(6.6,21.4));
    REQUIRE(ApproxIntv(tube.integral(4.0)) == Interval(3,25));
  }
  
  SECTION("Test tube4")
  {
    Tube tube = tubeTest4();
    REQUIRE(ApproxIntv(tube.integral(0.0)) == Interval(0.0));
    REQUIRE(ApproxIntv(tube.integral(0.1)) == Interval(0.1,0.2));
    REQUIRE(ApproxIntv(tube.integral(0.9)) == Interval(0.9,1.8));
    REQUIRE(ApproxIntv(tube.integral(1.0)) == Interval(1,2));
    REQUIRE(ApproxIntv(tube.integral(1.2)) == Interval(1.2,2.4));
    REQUIRE(ApproxIntv(tube.integral(2.0)) == Interval(2.0,4.0));
    REQUIRE(ApproxIntv(tube.integral(3.0)) == Interval(3.0,6.0));
    REQUIRE(ApproxIntv(tube.integral(5.0)) == Interval(5.0,10.0));
    REQUIRE(ApproxIntv(tube.integral(7.0)) == Interval(7.0,14.0));
    REQUIRE(ApproxIntv(tube.integral(9.0)) == Interval(9.0,18.0));
    REQUIRE(ApproxIntv(tube.integral(10.0)) == Interval(9.5,19.5));
    REQUIRE(ApproxIntv(tube.integral(10.1)) == Interval(9.4,19.6));
    REQUIRE(ApproxIntv(tube.integral(11.0)) == Interval(8.5,20.5));
    REQUIRE(ApproxIntv(tube.integral(11.5)) == Interval(7.75,20.25));
    REQUIRE(ApproxIntv(tube.integral(12.5)) == Interval(6.5,20.5));
    REQUIRE(ApproxIntv(tube.integral(12.6)) == Interval(6.4,20.6));
    REQUIRE(ApproxIntv(tube.integral(14.5)) == Interval(7,23.5));
  }
}

TEST_CASE("Computing integration from 0, interval argument", "[core]")
{
  SECTION("Test tube1")
  {
    Tube tube = tubeTest1();
    tube.set(Interval(-4,2), 14); // to test primitives pre-computation
    REQUIRE(ApproxIntv(tube.integral(Interval(0))) == Interval(0));
    REQUIRE(ApproxIntv(tube.integral(Interval(0.1))) == Interval(0.4,0.8));
    REQUIRE(ApproxIntv(tube.integral(Interval(0.5))) == Interval(2,4));
    REQUIRE(ApproxIntv(tube.integral(Interval(1.0))) == Interval(4,8));
    REQUIRE(ApproxIntv(tube.integral(Interval(1.5))) == Interval(5,11.5));
    REQUIRE(ApproxIntv(tube.integral(Interval(2.0))) == Interval(6,15));
    REQUIRE(ApproxIntv(tube.integral(Interval(2.5))) == Interval(6.5,18));
    REQUIRE(ApproxIntv(tube.integral(Interval(3.0))) == Interval(7,21));
    REQUIRE(ApproxIntv(tube.integral(Interval(3.0))) == Interval(7,21));
    REQUIRE(ApproxIntv(tube.integral(Interval(0.0,0.0))) == Interval(0,0));
    REQUIRE(ApproxIntv(tube.integral(Interval(0.0,0.5))) == Interval(0,4));
    REQUIRE(ApproxIntv(tube.integral(Interval(0.0,1.0))) == Interval(0,8));
    REQUIRE(ApproxIntv(tube.integral(Interval(0.0,1.5))) == Interval(0,11.5));
    REQUIRE(ApproxIntv(tube.integral(Interval(0.0,2.0))) == Interval(0,15));
    REQUIRE(ApproxIntv(tube.integral(Interval(1.0,1.0))) == Interval(4,8));
    REQUIRE(ApproxIntv(tube.integral(Interval(1.0,1.5))) == Interval(4,11.5));
    REQUIRE(ApproxIntv(tube.integral(Interval(1.0,2.0))) == Interval(4,15));
    REQUIRE(ApproxIntv(tube.integral(Interval(1.1,1.9))) == Interval(4.2,14.3));
    REQUIRE(ApproxIntv(tube.integral(Interval(1.1,2.1))) == Interval(4.2,15.6));
    REQUIRE(ApproxIntv(tube.integral(Interval(6,7))) == Interval(-23,19));
    REQUIRE(ApproxIntv(tube.integral(Interval(0,46))) == Interval(-85,194));
  }

  SECTION("Test tube1(01)")
  {
    Tube tube = tubeTest1_01();
    REQUIRE(ApproxIntv(tube.integral(Interval(0))) == Interval(0));
    REQUIRE(ApproxIntv(tube.integral(Interval(0.1))) == Interval(0.4,0.8));
    REQUIRE(ApproxIntv(tube.integral(Interval(0.5))) == Interval(2,4));
    REQUIRE(ApproxIntv(tube.integral(Interval(1.0))) == Interval(4,8));
    REQUIRE(ApproxIntv(tube.integral(Interval(1.5))) == Interval(5,11.5));
    REQUIRE(ApproxIntv(tube.integral(Interval(2.0))) == Interval(6,15));
    REQUIRE(ApproxIntv(tube.integral(Interval(2.5))) == Interval(6.5,18));
    REQUIRE(ApproxIntv(tube.integral(Interval(3.0))) == Interval(7,21));
    REQUIRE(ApproxIntv(tube.integral(Interval(3.0))) == Interval(7,21));
    REQUIRE(ApproxIntv(tube.integral(Interval(0.0,0.0))) == Interval(0,0));
    REQUIRE(ApproxIntv(tube.integral(Interval(0.0,0.5))) == Interval(0,4));
    REQUIRE(ApproxIntv(tube.integral(Interval(0.0,1.0))) == Interval(0,8));
    REQUIRE(ApproxIntv(tube.integral(Interval(0.0,1.5))) == Interval(0,11.5));
    REQUIRE(ApproxIntv(tube.integral(Interval(0.0,2.0))) == Interval(0,15));
    REQUIRE(ApproxIntv(tube.integral(Interval(1.0,1.0))) == Interval(4,8));
    REQUIRE(ApproxIntv(tube.integral(Interval(1.0,1.5))) == Interval(4,11.5));
    REQUIRE(ApproxIntv(tube.integral(Interval(1.0,2.0))) == Interval(4,15));
    REQUIRE(ApproxIntv(tube.integral(Interval(1.1,1.9))) == Interval(4.2,14.3));
    REQUIRE(ApproxIntv(tube.integral(Interval(1.1,2.1))) == Interval(4.2,15.6));
    REQUIRE(ApproxIntv(tube.integral(Interval(6,7))) == Interval(-23,19));
    REQUIRE(ApproxIntv(tube.integral(Interval(0,46))) == Interval(-85,194));
  }

  SECTION("Test tube4")
  {
    Tube tube = tubeTest4();
    REQUIRE(ApproxIntv(tube.integral(Interval(12.5))) == Interval(6.5,20.5));
    REQUIRE(ApproxIntv(tube.integral(Interval(14.5))) == Interval(7,23.5));
    REQUIRE(ApproxIntv(tube.integral(Interval(12.5,14.5))) == Interval(6.0,23.5));
    REQUIRE(ApproxIntv(tube.integral(Interval(0))) == Interval(0));
    REQUIRE(ApproxIntv(tube.integral(Interval(10.2))) == Interval(9.3,19.7));
    REQUIRE(ApproxIntv(tube.integral(Interval(1.5,3.5))) == Interval(1.5,7));
    REQUIRE(ApproxIntv(tube.integral(Interval(9,21))) == Interval(6,36.5));
  }

  SECTION("Test tube4(05)")
  {
    Tube tube = tubeTest4_05();
    REQUIRE(ApproxIntv(tube.integral(Interval(12.5))) == Interval(6.5,20.5));
    REQUIRE(ApproxIntv(tube.integral(Interval(14.5))) == Interval(7,23.5));
    REQUIRE(ApproxIntv(tube.integral(Interval(12.5,14.5))) == Interval(6.0,23.5));
    REQUIRE(ApproxIntv(tube.integral(Interval(0))) == Interval(0));
    REQUIRE(ApproxIntv(tube.integral(Interval(10.2))) == Interval(9.3,19.7));
    REQUIRE(ApproxIntv(tube.integral(Interval(1.5,3.5))) == Interval(1.5,7));
    REQUIRE(ApproxIntv(tube.integral(Interval(9,21))) == Interval(6,36.5));
  }
}

TEST_CASE("Computing integration from 0, partial integration", "[core]")
{
  SECTION("Test tube4")
  {
    Tube tube = tubeTest4();
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(0))) == make_pair(Interval(0.), Interval(0.)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(0.2))) == make_pair(Interval(0.2), Interval(0.4)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(0.1))) == make_pair(Interval(0.1), Interval(0.2)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(0.9))) == make_pair(Interval(0.9), Interval(1.8)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(1.0))) == make_pair(Interval(1), Interval(2)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(1.2))) == make_pair(Interval(1.2), Interval(2.4)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(2.0))) == make_pair(Interval(2.0), Interval(4.0)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(3.0))) == make_pair(Interval(3.0), Interval(6.0)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(5.0))) == make_pair(Interval(5.0), Interval(10.0)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(7.0))) == make_pair(Interval(7.0), Interval(14.0)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(9.0))) == make_pair(Interval(9.0), Interval(18.0)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(10.0))) == make_pair(Interval(9.5), Interval(19.5)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(10.1))) == make_pair(Interval(9.4), Interval(19.6)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(11.0))) == make_pair(Interval(8.5), Interval(20.5)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(12.5))) == make_pair(Interval(6.5), Interval(20.5)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(12.6))) == make_pair(Interval(6.4), Interval(20.6)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(14.5))) == make_pair(Interval(7), Interval(23.5)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(12.5,13))) == make_pair(Interval(6,6.5), Interval(20.5,21)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(13,14))) == make_pair(Interval(6,6.5), Interval(21,22.5)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(14,14.5))) == make_pair(Interval(6.5,7), Interval(22.5,23.5)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(12.5,14.5))) == make_pair(Interval(6.0,7.0), Interval(20.5,23.5)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(1.5,3.5))) == make_pair(Interval(1.5,3.5), Interval(3,7)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(9,21))) == make_pair(Interval(6,13.5), Interval(18,36.5)));
  }

  SECTION("Test tube4(05)")
  {
    Tube tube = tubeTest4_05();
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(0))) == make_pair(Interval(0.), Interval(0.)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(0.2))) == make_pair(Interval(0.2), Interval(0.4)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(0.1))) == make_pair(Interval(0.1), Interval(0.2)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(0.9))) == make_pair(Interval(0.9), Interval(1.8)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(1.0))) == make_pair(Interval(1), Interval(2)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(1.2))) == make_pair(Interval(1.2), Interval(2.4)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(2.0))) == make_pair(Interval(2.0), Interval(4.0)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(3.0))) == make_pair(Interval(3.0), Interval(6.0)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(5.0))) == make_pair(Interval(5.0), Interval(10.0)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(7.0))) == make_pair(Interval(7.0), Interval(14.0)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(9.0))) == make_pair(Interval(9.0), Interval(18.0)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(10.0))) == make_pair(Interval(9.5), Interval(19.5)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(10.1))) == make_pair(Interval(9.4), Interval(19.6)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(11.0))) == make_pair(Interval(8.5), Interval(20.5)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(12.5))) == make_pair(Interval(6.5), Interval(20.5)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(12.6))) == make_pair(Interval(6.4), Interval(20.6)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(14.5))) == make_pair(Interval(7), Interval(23.5)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(12.5,13))) == make_pair(Interval(6,6.5), Interval(20.5,21)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(13,14))) == make_pair(Interval(6,6.5), Interval(21,22.5)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(14,14.5))) == make_pair(Interval(6.5,7), Interval(22.5,23.5)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(12.5,14.5))) == make_pair(Interval(6.0,7.0), Interval(20.5,23.5)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(1.5,3.5))) == make_pair(Interval(1.5,3.5), Interval(3,7)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(9,21))) == make_pair(Interval(6,13.5), Interval(18,36.5)));
  }

  SECTION("Test tube1")
  {
    Tube tube = tubeTest1();
    tube.set(Interval(-4,2), 14); // to test primitives pre-computation
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(0.,46.))) == make_pair(Interval(-85,7), Interval(-16,194)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(7.))) == make_pair(Interval(-23), Interval(13)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(0.,7.))) == make_pair(Interval(-23,7.0), Interval(0,25)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(2.,6.))) == make_pair(Interval(-13,7), Interval(15,25)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(6.,7.))) == make_pair(Interval(-23,-13), Interval(13,19)));

    Interval integrale_lb = Interval::EMPTY_SET;
    Interval integrale_ub = Interval::EMPTY_SET;
    for(double t = 0.0 ; t <= 46.0 ; t += 0.2)
    {
      integrale_lb |= tube.partialIntegral(Interval(0, t)).first;
      integrale_ub |= tube.partialIntegral(Interval(0, t)).second;
    }
    REQUIRE(integrale_lb.is_subset(Interval(-85,7)));
    REQUIRE(integrale_ub.is_subset(Interval(-16,194)));
  }

  SECTION("Test tube1(01)")
  {
    Tube tube = tubeTest1_01();
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(0.,46.))) == make_pair(Interval(-85,7), Interval(-16,194)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(7.))) == make_pair(Interval(-23), Interval(13)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(0.,7.))) == make_pair(Interval(-23,7.0), Interval(0,25)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(2.,6.))) == make_pair(Interval(-13,7), Interval(15,25)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(6.,7.))) == make_pair(Interval(-23,-13), Interval(13,19)));
  }
}

TEST_CASE("Computing integration, two interval bounds", "[core]")
{
  SECTION("Test tube1")
  {
    Tube tube = tubeTest1();
    tube.set(Interval(-4,2), 14); // to test primitives pre-computation
    REQUIRE(ApproxIntv(tube.integral(Interval(20), Interval(20))) == Interval(0.));
    REQUIRE(ApproxIntv(tube.integral(Interval(2.1), Interval(2.1))) == Interval(0.));
    REQUIRE(ApproxIntv(tube.integral(Interval(12.2), Interval(12.2))) == Interval(0.));
    REQUIRE(ApproxIntv(tube.integral(Interval(12.2), Interval(12.2))) == Interval(0.));
    REQUIRE(ApproxIntv(tube.integral(Interval(17.), Interval(20.))) == Interval(12,24));
    REQUIRE(ApproxIntv(tube.integral(Interval(28.), Interval(33.))) == Interval(17,31));
    REQUIRE(ApproxIntv(tube.integral(Interval(20.), Interval(28.))) == Interval(51,88));
    REQUIRE(ApproxIntv(tube.integral(Interval(17.), Interval(33.))) == Interval(80,143));
    REQUIRE(ApproxIntv(tube.integral(Interval(22.5), Interval(23))) == Interval(4.5,6));
    REQUIRE(ApproxIntv(tube.integral(Interval(8.5), Interval(10.5))) == Interval(-18,-9.5));
    REQUIRE(ApproxIntv(tube.integral(Interval(0), Interval(7))) == Interval(-23,13));
    REQUIRE(ApproxIntv(tube.integral(Interval(2), Interval(6))) == Interval(-19,4));
    REQUIRE(ApproxIntv(tube.integral(Interval(0,2), Interval(6,7))) == Interval(-29,19));
    REQUIRE(ApproxIntv(tube.integral(Interval(0), Interval(17))) == Interval(-85,-4));
    REQUIRE(ApproxIntv(tube.integral(Interval(0), Interval(38))) == Interval(1,161));
    REQUIRE(ApproxIntv(tube.integral(Interval(0), Interval(17,38))) == Interval(-85,161));
    REQUIRE(ApproxIntv(tube.integral(Interval(10), Interval(13))) == Interval(-21,-11));
    REQUIRE(ApproxIntv(tube.integral(Interval(17), Interval(20))) == Interval(12,24));
    REQUIRE(ApproxIntv(tube.integral(Interval(13), Interval(17))) == Interval(-11,11));
    REQUIRE(ApproxIntv(tube.integral(Interval(10,13), Interval(17))) == Interval(-32,11));
    REQUIRE(ApproxIntv(tube.integral(Interval(13), Interval(17,20))) == Interval(-11,35));
    REQUIRE(ApproxIntv(tube.integral(Interval(10,13), Interval(17,20))) == Interval(-32,35));
    REQUIRE(ApproxIntv(tube.integral(Interval(5,10), Interval(31,42))) == Interval(-6,180));
  }

  SECTION("Test tube1(01)")
  {
    Tube tube = tubeTest1_01();
    REQUIRE(ApproxIntv(tube.integral(Interval(20), Interval(20))) == Interval(0.));
    REQUIRE(ApproxIntv(tube.integral(Interval(2.1), Interval(2.1))) == Interval(0.));
    REQUIRE(ApproxIntv(tube.integral(Interval(12.2), Interval(12.2))) == Interval(0.));
    REQUIRE(ApproxIntv(tube.integral(Interval(12.2), Interval(12.2))) == Interval(0.));
    REQUIRE(ApproxIntv(tube.integral(Interval(17.), Interval(20.))) == Interval(12,24));
    REQUIRE(ApproxIntv(tube.integral(Interval(28.), Interval(33.))) == Interval(17,31));
    REQUIRE(ApproxIntv(tube.integral(Interval(20.), Interval(28.))) == Interval(51,88));
    REQUIRE(ApproxIntv(tube.integral(Interval(17.), Interval(33.))) == Interval(80,143));
    REQUIRE(ApproxIntv(tube.integral(Interval(22.5), Interval(23))) == Interval(4.5,6));
    REQUIRE(ApproxIntv(tube.integral(Interval(8.5), Interval(10.5))) == Interval(-18,-9.5));
    REQUIRE(ApproxIntv(tube.integral(Interval(0), Interval(7))) == Interval(-23,13));
    REQUIRE(ApproxIntv(tube.integral(Interval(2), Interval(6))) == Interval(-19,4));
    REQUIRE(ApproxIntv(tube.integral(Interval(0,2), Interval(6,7))) == Interval(-29,19));
    REQUIRE(ApproxIntv(tube.integral(Interval(0), Interval(17))) == Interval(-85,-4));
    REQUIRE(ApproxIntv(tube.integral(Interval(0), Interval(38))) == Interval(1,161));
    REQUIRE(ApproxIntv(tube.integral(Interval(0), Interval(17,38))) == Interval(-85,161));
    REQUIRE(ApproxIntv(tube.integral(Interval(10), Interval(13))) == Interval(-21,-11));
    REQUIRE(ApproxIntv(tube.integral(Interval(17), Interval(20))) == Interval(12,24));
    REQUIRE(ApproxIntv(tube.integral(Interval(13), Interval(17))) == Interval(-11,11));
    REQUIRE(ApproxIntv(tube.integral(Interval(10,13), Interval(17))) == Interval(-32,11));
    REQUIRE(ApproxIntv(tube.integral(Interval(13), Interval(17,20))) == Interval(-11,35));
    REQUIRE(ApproxIntv(tube.integral(Interval(10,13), Interval(17,20))) == Interval(-32,35));
    REQUIRE(ApproxIntv(tube.integral(Interval(5,10), Interval(31,42))) == Interval(-6,180));
  }

  SECTION("Test tube4")
  {
    Tube tube = tubeTest4();
    REQUIRE(ApproxIntv(tube.integral(Interval(0,8), Interval(15,21))) == Interval(-0.5,36.5));
    REQUIRE(ApproxIntv(tube.integral(Interval(0,8), Interval(9,21))) == Interval(-2,36.5));
  }

  SECTION("Test tube4(05)")
  {
    Tube tube = tubeTest4_05();
    REQUIRE(ApproxIntv(tube.integral(Interval(0,8), Interval(15,21))) == Interval(-0.5,36.5));
    REQUIRE(ApproxIntv(tube.integral(Interval(0,8), Interval(9,21))) == Interval(-2,36.5));
  }
}

TEST_CASE("Computing partial integration, two interval bounds", "[core]")
{
  SECTION("Test tube1")
  {
    Tube tube = tubeTest1();
    tube.set(Interval(-4,2), 14); // to test primitives pre-computation
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(0), Interval(0))) == make_pair(Interval(0.), Interval(0.)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(20), Interval(20))) == make_pair(Interval(0.), Interval(0.)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(22,23), Interval(24,25))) == make_pair(Interval(8,24), Interval(13,37)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(17,20), Interval(28,33))) == make_pair(Interval(51,80), Interval(88,143)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(0.), Interval(46.))) == make_pair(Interval(-3), Interval(194)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(0.), Interval(7.))) == make_pair(Interval(-23), Interval(13)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(2.), Interval(6.))) == make_pair(Interval(-19), Interval(4)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(0,2), Interval(6,7))) == make_pair(Interval(-29,-13), Interval(-2,19)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(10,13), Interval(17,20))) == make_pair(Interval(-32,1), Interval(0,35)));
  }

  SECTION("Test tube1(01)")
  {
    Tube tube = tubeTest1_01();
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(0), Interval(0))) == make_pair(Interval(0.), Interval(0.)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(20), Interval(20))) == make_pair(Interval(0.), Interval(0.)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(22,23), Interval(24,25))) == make_pair(Interval(8,24), Interval(13,37)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(17,20), Interval(28,33))) == make_pair(Interval(51,80), Interval(88,143)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(0.), Interval(46.))) == make_pair(Interval(-3), Interval(194)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(0.), Interval(7.))) == make_pair(Interval(-23), Interval(13)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(2.), Interval(6.))) == make_pair(Interval(-19), Interval(4)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(0,2), Interval(6,7))) == make_pair(Interval(-29,-13), Interval(-2,19)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(10,13), Interval(17,20))) == make_pair(Interval(-32,1), Interval(0,35)));
  }

  SECTION("Test tube4")
  {
    Tube tube = tubeTest4();
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(0.1,1.1), Interval(2.6,3.2))) == make_pair(Interval(1.5,3.1), Interval(3.0,6.2)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(8.6,9.9), Interval(13.2,13.6))) == make_pair(Interval(-3.35,-2.3), Interval(1.95,4.7)));
  }

  SECTION("Test tube4(05)")
  {
    Tube tube = tubeTest4_05();
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(0.1,1.1), Interval(2.6,3.2))) == make_pair(Interval(1.5,3.1), Interval(3.0,6.2)));
    REQUIRE(ApproxIntvPair(tube.partialIntegral(Interval(8.6,9.9), Interval(13.2,13.6))) == make_pair(Interval(-3.35,-2.3), Interval(1.95,4.7)));
  }
}