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
#include <stdio.h>

using namespace std;
using namespace Catch;
using namespace Detail;
using namespace tubex;

bool testSerialization(const Tube& tube1)
{
  string filename = "test_serialization.tube";

  map<double,double> map_test1a, map_test1b, map_test2a, map_test2b;

  for(int i = 0 ; i < tube1.size() ; i++)
    if(!tube1[i].is_unbounded() && tube1[i] != Interval::EMPTY_SET)
      map_test1a[tube1.domain(i).mid()] = tube1[i].mid();

  for(int i = 0 ; i < tube1.size() ; i++)
    if(!tube1[i].is_unbounded() && tube1[i] != Interval::EMPTY_SET)
      map_test1b[tube1.domain(i).mid()] = 42.;

  vector<map<double,double> > v_maps;

  v_maps.push_back(map_test1a);
  v_maps.push_back(map_test1b);
  tube1.serialize(filename, v_maps); // serialization

  v_maps.clear();
  Tube tube2(filename, v_maps); // deserialization
  remove(filename.c_str());

  bool equality = tube1 == tube2;

  if(v_maps.size() != 2)
    return false;

  map_test2a = v_maps[0];
  map_test2b = v_maps[1];
  typename map<double,double>::const_iterator it;
  for(it = map_test2a.begin(); it != map_test2a.end(); it++)
    equality &= map_test1a[it->first] == it->second;
  for(it = map_test2b.begin(); it != map_test2b.end(); it++)
    equality &= map_test1b[it->first] == it->second;

  return equality;
}

TEST_CASE("(de)serializations on bounded tubes", "[core]")
{
  SECTION("Test tube1")
  {
    REQUIRE(testSerialization(tubeTest1()));
  }

  SECTION("Test tube1(01)")
  {
    REQUIRE(testSerialization(tubeTest1_01()));    
  }

  SECTION("Test tube2")
  {
    REQUIRE(testSerialization(tubeTest2()));
  }

  SECTION("Test tube3")
  {
    REQUIRE(testSerialization(tubeTest3()));
  }

  SECTION("Test tube4")
  {
    REQUIRE(testSerialization(tubeTest4()));
  }

  SECTION("Test tube4(05)")
  {
    REQUIRE(testSerialization(tubeTest4_05()));
  }
}

TEST_CASE("(de)serializations on unbounded tubes", "[core]")
{
  SECTION("Test POS_REALS")
  {
    Tube tube = tubeTest1();
    tube.set(Interval::POS_REALS, tube.size() / 2);
    REQUIRE(testSerialization(tube));
    tube.set(Interval::POS_REALS);
    REQUIRE(testSerialization(tube));
  }

  SECTION("Test NEG_REALS")
  {
    Tube tube = tubeTest2();
    tube.set(Interval::NEG_REALS, 5);
    REQUIRE(testSerialization(tube));
    tube.set(Interval::NEG_REALS);
    REQUIRE(testSerialization(tube));
  }

  SECTION("Test ALL_REALS")
  {
    Tube tube = tubeTest3();
    tube.set(Interval::ALL_REALS, 1);
    REQUIRE(testSerialization(tube));
    tube.set(Interval::ALL_REALS);
    REQUIRE(testSerialization(tube));
  }

  SECTION("Test EMPTY_SET")
  {
    Tube tube = tubeTest4();
    tube.set(Interval::EMPTY_SET, 0);
    tube.set(Interval::EMPTY_SET, 8);
    REQUIRE(testSerialization(tube));
    tube.set(Interval::EMPTY_SET);
    REQUIRE(testSerialization(tube));
  }
}