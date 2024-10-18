/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_IntervalVector.h>
#include <codac2_IntervalMatrix.h>
#include <codac2_arithmetic_sub.h>

using namespace std;
using namespace codac2;

TEST_CASE("arithmetic sub")
{
  // inline Vector operator-(const Vector& x1, const Vector& x2)
  CHECK(Vector({1,2,3})-Vector({5,6,7}) == Vector({-4,-4,-4}));

  // inline IntervalVector operator-(const Vector& x1, const IntervalVector& x2)
  CHECK(Vector({1,2,3})-IntervalVector({{-1,1},{-2,2},{-3,3}}) == IntervalVector({{0,2},{0,4},{0,6}}));

  // Matrix operator-(const M& x1, const M_& x2)
  CHECK(Matrix({{1,2},{3,4}})-Matrix({{3,4},{5,6}}) == Matrix({{-2,-2},{-2,-2}}));
  CHECK(Matrix({{1,2},{3,4}}).block(0,0,2,2)-Matrix({{3,4},{5,6}}) == Matrix({{-2,-2},{-2,-2}}));
  CHECK(Matrix({{1,2},{3,4}})-Matrix({{3,4},{5,6}}).block(0,0,2,2) == Matrix({{-2,-2},{-2,-2}}));
  CHECK(Matrix({{1,2},{3,4}}).block(0,0,2,2)-Matrix({{3,4},{5,6}}).block(0,0,2,2) == Matrix({{-2,-2},{-2,-2}}));

  // IntervalMatrix operator-(const M& x1, const IM& x2)
  CHECK(Matrix({{1,2},{3,4}})-IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}}) == IntervalMatrix({{{0,2},{0,4}},{{0,6},{0,8}}}));
  CHECK(Matrix({{1,2},{3,4}}).block(0,0,2,2)-IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}}) == IntervalMatrix({{{0,2},{0,4}},{{0,6},{0,8}}}));
  CHECK(Matrix({{1,2},{3,4}})-IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}}).block(0,0,2,2) == IntervalMatrix({{{0,2},{0,4}},{{0,6},{0,8}}}));
  CHECK(Matrix({{1,2},{3,4}}).block(0,0,2,2)-IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}}).block(0,0,2,2) == IntervalMatrix({{{0,2},{0,4}},{{0,6},{0,8}}}));

  // inline IntervalVector operator-(const IntervalVector& x1, const Vector& x2)
  CHECK(IntervalVector({{-1,1},{-2,2},{-3,3}})-Vector({1,2,3}) == IntervalVector({{-2,0},{-4,0},{-6,0}}));

  // inline IntervalVector operator-(const IntervalVector& x1, const IntervalVector& x2)
  CHECK(IntervalVector({{-1,1},{-2,2},{-3,3}})-IntervalVector({{1},{2},{3}}) == IntervalVector({{-2,0},{-4,0},{-6,0}}));

  // IntervalMatrix operator-(const IM& x1, const M& x2)
  CHECK(IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}})-Matrix({{1,2},{3,4}}) == IntervalMatrix({{{-2,0},{-4,0}},{{-6,0},{-8,0}}}));
  CHECK(IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}}).block(0,0,2,2)-Matrix({{1,2},{3,4}}) == IntervalMatrix({{{-2,0},{-4,0}},{{-6,0},{-8,0}}}));
  CHECK(IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}})-Matrix({{1,2},{3,4}}).block(0,0,2,2) == IntervalMatrix({{{-2,0},{-4,0}},{{-6,0},{-8,0}}}));
  CHECK(IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}}).block(0,0,2,2)-Matrix({{1,2},{3,4}}).block(0,0,2,2) == IntervalMatrix({{{-2,0},{-4,0}},{{-6,0},{-8,0}}}));

  // IntervalMatrix operator-(const IM& x1, const IM_& x2)
  CHECK(IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}})-IntervalMatrix({{{1},{2}},{{3},{4}}}) == IntervalMatrix({{{-2,0},{-4,0}},{{-6,0},{-8,0}}}));
  CHECK(IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}}).block(0,0,2,2)-IntervalMatrix({{{1},{2}},{{3},{4}}}) == IntervalMatrix({{{-2,0},{-4,0}},{{-6,0},{-8,0}}}));
  CHECK(IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}})-IntervalMatrix({{{1},{2}},{{3},{4}}}).block(0,0,2,2) == IntervalMatrix({{{-2,0},{-4,0}},{{-6,0},{-8,0}}}));
  CHECK(IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}}).block(0,0,2,2)-IntervalMatrix({{{1},{2}},{{3},{4}}}).block(0,0,2,2) == IntervalMatrix({{{-2,0},{-4,0}},{{-6,0},{-8,0}}}));
}