/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_Vector.h>
#include <codac2_Matrix.h>
#include <codac2_IntervalVector.h>
#include <codac2_IntervalMatrix.h>

using namespace std;
using namespace codac2;

TEST_CASE("arithmetic add")
{
  // inline Vector operator+(const Vector& x1, const Vector& x2)
  CHECK(Vector({1,2,3})+Vector({5,6,7}) == Vector({6,8,10}));

  // inline IntervalVector operator+(const Vector& x1, const IntervalVector& x2)
  CHECK(Vector({1,2,3})+IntervalVector({{-1,1},{-2,2},{-3,3}}) == IntervalVector({{0,2},{0,4},{0,6}}));

  // Matrix operator+(const M& x1, const M_& x2)
  CHECK(Matrix({{1,2},{3,4}})+Matrix({{3,4},{5,6}}) == Matrix({{4,6},{8,10}}));
  CHECK(Matrix({{1,2},{3,4}}).block(0,0,2,2)+Matrix({{3,4},{5,6}}) == Matrix({{4,6},{8,10}}));
  CHECK(Matrix({{1,2},{3,4}})+Matrix({{3,4},{5,6}}).block(0,0,2,2) == Matrix({{4,6},{8,10}}));
  CHECK(Matrix({{1,2},{3,4}}).block(0,0,2,2)+Matrix({{3,4},{5,6}}).block(0,0,2,2) == Matrix({{4,6},{8,10}}));

  // IntervalMatrix operator+(const M& x1, const IM& x2)
  CHECK(Matrix({{1,2},{3,4}})+IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}}) == IntervalMatrix({{{0,2},{0,4}},{{0,6},{0,8}}}));
  CHECK(Matrix({{1,2},{3,4}}).block(0,0,2,2)+IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}}) == IntervalMatrix({{{0,2},{0,4}},{{0,6},{0,8}}}));
  CHECK(Matrix({{1,2},{3,4}})+IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}}).block(0,0,2,2) == IntervalMatrix({{{0,2},{0,4}},{{0,6},{0,8}}}));
  CHECK(Matrix({{1,2},{3,4}}).block(0,0,2,2)+IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}}).block(0,0,2,2) == IntervalMatrix({{{0,2},{0,4}},{{0,6},{0,8}}}));

  // inline IntervalVector operator+(const IntervalVector& x1, const Vector& x2)
  CHECK(IntervalVector({{-1,1},{-2,2},{-3,3}})+Vector({1,2,3}) == IntervalVector({{0,2},{0,4},{0,6}}));

  // inline IntervalVector operator+(const IntervalVector& x1, const IntervalVector& x2)
  CHECK(IntervalVector({{-1,1},{-2,2},{-3,3}})+IntervalVector({1,2,3}) == IntervalVector({{0,2},{0,4},{0,6}}));

  // IntervalMatrix operator+(const IM& x1, const M& x2)
  CHECK(IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}})+Matrix({{1,2},{3,4}}) == IntervalMatrix({{{0,2},{0,4}},{{0,6},{0,8}}}));
  CHECK(IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}}).block(0,0,2,2)+Matrix({{1,2},{3,4}}) == IntervalMatrix({{{0,2},{0,4}},{{0,6},{0,8}}}));
  CHECK(IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}})+Matrix({{1,2},{3,4}}).block(0,0,2,2) == IntervalMatrix({{{0,2},{0,4}},{{0,6},{0,8}}}));
  CHECK(IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}}).block(0,0,2,2)+Matrix({{1,2},{3,4}}).block(0,0,2,2) == IntervalMatrix({{{0,2},{0,4}},{{0,6},{0,8}}}));

  // IntervalMatrix operator+(const IM& x1, const IM_& x2)
  CHECK(IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}})+IntervalMatrix({{{1},{2}},{{3},{4}}}) == IntervalMatrix({{{0,2},{0,4}},{{0,6},{0,8}}}));
  CHECK(IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}}).block(0,0,2,2)+IntervalMatrix({{{1},{2}},{{3},{4}}}) == IntervalMatrix({{{0,2},{0,4}},{{0,6},{0,8}}}));
  CHECK(IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}})+IntervalMatrix({{{1},{2}},{{3},{4}}}).block(0,0,2,2) == IntervalMatrix({{{0,2},{0,4}},{{0,6},{0,8}}}));
  CHECK(IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}}).block(0,0,2,2)+IntervalMatrix({{{1},{2}},{{3},{4}}}).block(0,0,2,2) == IntervalMatrix({{{0,2},{0,4}},{{0,6},{0,8}}}));
}