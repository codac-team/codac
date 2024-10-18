/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_Interval.h>
#include <codac2_IntervalVector.h>
#include <codac2_Approx.h>
#include <codac2_arithmetic.h>

using namespace std;
using namespace codac2;

TEST_CASE("Approx")
{
  CHECK(Interval(0.1) != Interval(1.)/Interval(10.));
  CHECK(Approx(Interval(0.1)) == Interval(1.)/Interval(10.));
  CHECK(Interval(0.1) == Approx(Interval(1.)/Interval(10.)));

  CHECK(IntervalVector({{0.1}}) != IntervalVector({{1.}})/Interval(10.));
  CHECK(Approx(IntervalVector({{0.1}})) == IntervalVector({{1.}})/Interval(10.));
  CHECK(IntervalVector({{0.1}}) == Approx(IntervalVector({{1.}})/Interval(10.)));
}