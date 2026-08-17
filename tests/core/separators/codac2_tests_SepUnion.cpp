/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_SepInter.h>
#include <codac2_SepUnion.h>
#include <codac2_SepInverse.h>

using namespace std;
using namespace codac2;

TEST_CASE("SepUnion")
{
  VectorVar v(1);
  SepInverse s1(AnalyticFunction({v}, v), IntervalVector({{-1,1}}));
  SepInverse s2(AnalyticFunction({v}, v), IntervalVector({{5,6}}));

  SepInter s3(s1,s2);

  // Testing constructors
  SepUnion s4({s1,s2}); // works if c1,c2 of same types
  SepUnion s5(s1,s2,s3); // different types
  CHECK(s4.nb() == 2);
  CHECK(s5.nb() == 3);
}