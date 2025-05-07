/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_hull.h>
#include <codac2_Vector.h>
#include <codac2_Matrix.h>
#include <codac2_IntervalVector.h>
#include <codac2_IntervalMatrix.h>

using namespace std;
using namespace codac2;

TEST_CASE("hull IntervalVector")
{
  CHECK(hull(Vector({1,2})) == IntervalVector({{1,1},{2,2}}));
  CHECK(hull(Vector({1,2}),Vector({1,2}),Vector({1,2}),Vector({1,2})) == IntervalVector({{1,1},{2,2}}));
  CHECK(hull(Vector({1,2}),Vector({9,10})) == IntervalVector({{1,9},{2,10}}));
  CHECK(hull(Vector({1,2}),IntervalVector({9,10})) == IntervalVector({{1,9},{2,10}}));
  CHECK(hull(IntervalVector({1,2}),Vector({9,10})) == IntervalVector({{1,9},{2,10}}));
  CHECK(hull(IntervalVector({1,2}),IntervalVector({9,10})) == IntervalVector({{1,9},{2,10}}));
  CHECK(hull(IntervalVector({1,2}),IntervalVector::empty(2)) == IntervalVector({{1,1},{2,2}}));
  CHECK(hull(IntervalVector({1,2})) == IntervalVector({{1,1},{2,2}}));
  CHECK(hull(2.*IntervalVector({1,2})) == IntervalVector({{2,2},{4,4}}));
}

TEST_CASE("hull IntervalMatrix")
{
  CHECK(hull(Matrix({{1,2},{3,4}})) == IntervalMatrix(Matrix({{1,2},{3,4}})));
  CHECK(hull(3.*Matrix({{1,2},{3,4}})) == IntervalMatrix(Matrix({{3,6},{9,12}})));
}