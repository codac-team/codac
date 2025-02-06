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
#include <codac2_Approx.h>

using namespace std;
using namespace codac2;

TEST_CASE("arithmetic div")
{
  //inline Vector operator/(const Vector& x1, double x2)
  CHECK(Approx<Vector>(Vector({1,2,3})/10.) == Vector({.1,.2,.3}));

  //inline IntervalVector operator/(const Vector& x1, const Interval& x2)
  CHECK(Approx<IntervalVector>(Vector({1,2,3})/Interval(10.,oo)) == IntervalVector({{0,.1},{0,.2},{0,.3}}));

  //Matrix operator/(const M& x1, double x2)
  CHECK(Approx<Matrix>(Matrix({{1,2},{3,4}})/10.) == Matrix({{.1,.2},{.3,.4}}));
  CHECK(Approx<Matrix>(Matrix({{1,2},{3,4}}).block(0,0,2,2)/10.) == Matrix({{.1,.2},{.3,.4}}));

  //IntervalMatrix operator/(const M& x1, const Interval& x2)
  CHECK(Approx<IntervalMatrix>(Matrix({{1,2},{3,4}})/Interval(10.,oo)) == IntervalMatrix({{{0,.1},{0,.2}},{{0,.3},{0,.4}}}));
  CHECK(Approx<IntervalMatrix>(Matrix({{1,2},{3,4}}).block(0,0,2,2)/Interval(10.,oo)) == IntervalMatrix({{{0,.1},{0,.2}},{{0,.3},{0,.4}}}));

  //inline IntervalVector operator/(const IntervalVector& x1, double x2)
  CHECK(Approx<IntervalVector>(IntervalVector({1,2,3})/10.) == IntervalVector({0.1,0.2,0.3}));

  //inline IntervalVector operator/(const IntervalVector& x1, const Interval& x2)
  CHECK(Approx<IntervalVector>(IntervalVector({1,2,3})/Interval(10.,oo)) == IntervalVector({{0,.1},{0,.2},{0,.3}}));

  //IntervalMatrix operator/(const IM& x1, double x2)
  CHECK(Approx<IntervalMatrix>(IntervalMatrix({{1,2},{3,4}})/10.) == IntervalMatrix({{.1,.2},{.3,.4}}));
  CHECK(Approx<IntervalMatrix>(IntervalMatrix({{1,2},{3,4}}).block(0,0,2,2)/10.) == IntervalMatrix({{.1,.2},{.3,.4}}));

  //IntervalMatrix operator/(const IM& x1, const Interval& x2)
  CHECK(Approx<IntervalMatrix>(IntervalMatrix({{1,2},{3,4}})/Interval(10.,oo)) == IntervalMatrix({{{0,.1},{0,.2}},{{0,.3},{0,.4}}}));
  CHECK(Approx<IntervalMatrix>(IntervalMatrix({{1,2},{3,4}}).block(0,0,2,2)/Interval(10.,oo)) == IntervalMatrix({{{0,.1},{0,.2}},{{0,.3},{0,.4}}}));
}