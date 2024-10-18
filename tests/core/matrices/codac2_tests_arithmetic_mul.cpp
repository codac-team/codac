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
#include <codac2_arithmetic_mul.h>

using namespace std;
using namespace codac2;

TEST_CASE("arithmetic mul")
{
  // inline Vector operator*(double x1, const Vector& x2)
  CHECK(2.*Vector({1,2}) == Vector({2,4}));

  // Matrix operator*(double x1, const M& x2)
  CHECK(2.*Matrix({{1,2},{3,4}}) == Matrix({{2,4},{6,8}}));
  CHECK(2.*Matrix({{1,2},{3,4}}).block(0,0,2,2) == Matrix({{2,4},{6,8}}));

  // inline IntervalVector operator*(double x1, const IntervalVector& x2)
  CHECK(2.*IntervalVector({{-1,1},{-2,2}}) == IntervalVector({{-2,2},{-4,4}}));

  // IntervalMatrix operator*(double x1, const IM& x2)
  CHECK(2.*IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}}) == IntervalMatrix({{{-2,2},{-4,4}},{{-6,6},{-8,8}}}));
  CHECK(2.*IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}}).block(0,0,2,2) == IntervalMatrix({{{-2,2},{-4,4}},{{-6,6},{-8,8}}}));

  // inline IntervalVector operator*(const Interval& x1, const Vector& x2)
  CHECK(Interval(-1,1)*Vector({1,2}) == IntervalVector({{-1,1},{-2,2}}));

  // IntervalMatrix operator*(const Interval& x1, const M& x2)
  CHECK(Interval(-1,1)*Matrix({{1,2},{3,4}}) == IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}}));
  CHECK(Interval(-1,1)*Matrix({{1,2},{3,4}}).block(0,0,2,2) == IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}}));

  // inline IntervalVector operator*(const Interval& x1, const IntervalVector& x2)
  CHECK(Interval(2,oo)*IntervalVector({{1,2},{4,8}}) == IntervalVector({{2,oo},{8,oo}}));

  // IntervalMatrix operator*(const Interval& x1, const IM& x2)
  CHECK(Interval(-1,1)*IntervalMatrix({{{1,2},{2,3}},{{3,4},{4,5}}}) == IntervalMatrix({{{-2,2},{-3,3}},{{-4,4},{-5,5}}}));
  CHECK(Interval(-1,1)*IntervalMatrix({{{1,2},{2,3}},{{3,4},{4,5}}}).block(0,0,2,2) == IntervalMatrix({{{-2,2},{-3,3}},{{-4,4},{-5,5}}}));

  // inline Vector operator*(const Vector& x1, double x2)
  CHECK(Vector({1,2})*2. == Vector({2,4}));

  // inline IntervalVector operator*(const Vector& x1, const Interval& x2)
  CHECK(Vector({1,2})*Interval(-1,1) == IntervalVector({{-1,1},{-2,2}}));

  // Matrix operator*(const M& x1, double x2)
  CHECK(Matrix({{1,2},{3,4}})*2. == Matrix({{2,4},{6,8}}));
  CHECK(Matrix({{1,2},{3,4}}).block(0,0,2,2)*2. == Matrix({{2,4},{6,8}}));

  // IntervalMatrix operator*(const M& x1, const Interval& x2)
  CHECK(Matrix({{1,2},{3,4}})*Interval(-1,1) == IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}}));
  CHECK(Matrix({{1,2},{3,4}}).block(0,0,2,2)*Interval(-1,1) == IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}}));

  // Vector operator*(const M& x1, const Vector& x2)
  CHECK(Matrix({{1,2},{3,4}})*Vector({5,6}) == Vector({17,39}));
  CHECK(Matrix({{1,2},{3,4}}).block(0,0,2,2)*Vector({5,6}) == Vector({17,39}));

  // Matrix operator*(const M& x1, const M_& x2)
  CHECK(Matrix({{1,2},{3,4}})*Matrix({{5,6},{7,8}}) == Matrix({{19,22},{43,50}}));
  CHECK(Matrix({{1,2},{3,4}}).block(0,0,2,2)*Matrix({{5,6},{7,8}}) == Matrix({{19,22},{43,50}}));
  CHECK(Matrix({{1,2},{3,4}})*Matrix({{5,6},{7,8}}).block(0,0,2,2) == Matrix({{19,22},{43,50}}));
  CHECK(Matrix({{1,2},{3,4}}).block(0,0,2,2)*Matrix({{5,6},{7,8}}).block(0,0,2,2) == Matrix({{19,22},{43,50}}));

  // IntervalVector operator*(const M& x1, const IntervalVector& x2)
  CHECK(Matrix({{1,2},{3,4}})*IntervalVector({5,6}) == IntervalVector({17,39}));
  CHECK(Matrix({{1,2},{3,4}}).block(0,0,2,2)*IntervalVector({5,6}) == IntervalVector({17,39}));

  // IntervalMatrix operator*(const M& x1, const IM& x2)
  CHECK(Matrix({{1,2},{3,4}})*IntervalMatrix({{{5,6},{6,7}},{{7,8},{8,9}}}) == IntervalMatrix({{{19,22},{22,25}},{{43,50},{50,57}}}));
  CHECK(Matrix({{1,2},{3,4}}).block(0,0,2,2)*IntervalMatrix({{{5,6},{6,7}},{{7,8},{8,9}}}) == IntervalMatrix({{{19,22},{22,25}},{{43,50},{50,57}}}));
  CHECK(Matrix({{1,2},{3,4}})*IntervalMatrix({{{5,6},{6,7}},{{7,8},{8,9}}}).block(0,0,2,2) == IntervalMatrix({{{19,22},{22,25}},{{43,50},{50,57}}}));
  CHECK(Matrix({{1,2},{3,4}}).block(0,0,2,2)*IntervalMatrix({{{5,6},{6,7}},{{7,8},{8,9}}}).block(0,0,2,2) == IntervalMatrix({{{19,22},{22,25}},{{43,50},{50,57}}}));

  // inline IntervalVector operator*(const IntervalVector& x1, double x2)
  CHECK(IntervalVector({{-1,1},{-2,2}})*2. == IntervalVector({{-2,2},{-4,4}}));

  // inline IntervalVector operator*(const IntervalVector& x1, const Interval& x2)
  CHECK(IntervalVector({{1,2},{4,8}})*Interval(2,oo) == IntervalVector({{2,oo},{8,oo}}));

  // IntervalMatrix operator*(const IM& x1, double x2)
  CHECK(IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}})*2. == IntervalMatrix({{{-2,2},{-4,4}},{{-6,6},{-8,8}}}));
  CHECK(IntervalMatrix({{{-1,1},{-2,2}},{{-3,3},{-4,4}}}).block(0,0,2,2)*2. == IntervalMatrix({{{-2,2},{-4,4}},{{-6,6},{-8,8}}}));

  // IntervalMatrix operator*(const IM& x1, const Interval& x2)
  CHECK(IntervalMatrix({{{1,2},{2,3}},{{3,4},{4,5}}})*Interval(-1,1) == IntervalMatrix({{{-2,2},{-3,3}},{{-4,4},{-5,5}}}));
  CHECK(IntervalMatrix({{{1,2},{2,3}},{{3,4},{4,5}}}).block(0,0,2,2)*Interval(-1,1) == IntervalMatrix({{{-2,2},{-3,3}},{{-4,4},{-5,5}}}));

  // IntervalVector operator*(const IM& x1, const Vector& x2)
  CHECK(IntervalMatrix({{{1,2},{2,3}},{{3,4},{4,5}}})*Vector({5,6}) == IntervalVector({{17,28},{39,50}}));
  CHECK(IntervalMatrix({{{1,2},{2,3}},{{3,4},{4,5}}}).block(0,0,2,2)*Vector({5,6}) == IntervalVector({{17,28},{39,50}}));

  // IntervalMatrix operator*(const IM& x1, const M& x2)
  CHECK(IntervalMatrix({{{5,6},{6,7}},{{7,8},{8,9}}})*Matrix({{1,2},{3,4}}) == IntervalMatrix({{{23,27},{34,40}},{{31,35},{46,52}}}));
  CHECK(IntervalMatrix({{{5,6},{6,7}},{{7,8},{8,9}}}).block(0,0,2,2)*Matrix({{1,2},{3,4}}) == IntervalMatrix({{{23,27},{34,40}},{{31,35},{46,52}}}));
  CHECK(IntervalMatrix({{{5,6},{6,7}},{{7,8},{8,9}}})*Matrix({{1,2},{3,4}}).block(0,0,2,2) == IntervalMatrix({{{23,27},{34,40}},{{31,35},{46,52}}}));
  CHECK(IntervalMatrix({{{5,6},{6,7}},{{7,8},{8,9}}}).block(0,0,2,2)*Matrix({{1,2},{3,4}}).block(0,0,2,2) == IntervalMatrix({{{23,27},{34,40}},{{31,35},{46,52}}}));

  // IntervalVector operator*(const IM& x1, const IntervalVector& x2)
  CHECK(IntervalMatrix({{{1,2},{2,3}},{{3,4},{4,5}}})*IntervalVector({{5,6},{7,8}}) == IntervalVector({{19,36},{43,64}}));
  CHECK(IntervalMatrix({{{1,2},{2,3}},{{3,4},{4,5}}}).block(0,0,2,2)*IntervalVector({{5,6},{7,8}}) == IntervalVector({{19,36},{43,64}}));

  // IntervalMatrix operator*(const IM& x1, const IM_& x2)
  CHECK(IntervalMatrix({{{5,6},{6,7}},{{7,8},{8,9}}})*IntervalMatrix({{{1,2},{2,3}},{{3,4},{4,5}}}) == IntervalMatrix({{{23,40},{34,53}},{{31,52},{46,69}}}));
  CHECK(IntervalMatrix({{{5,6},{6,7}},{{7,8},{8,9}}}).block(0,0,2,2)*IntervalMatrix({{{1,2},{2,3}},{{3,4},{4,5}}}) == IntervalMatrix({{{23,40},{34,53}},{{31,52},{46,69}}}));
  CHECK(IntervalMatrix({{{5,6},{6,7}},{{7,8},{8,9}}})*IntervalMatrix({{{1,2},{2,3}},{{3,4},{4,5}}}).block(0,0,2,2) == IntervalMatrix({{{23,40},{34,53}},{{31,52},{46,69}}}));
  CHECK(IntervalMatrix({{{5,6},{6,7}},{{7,8},{8,9}}}).block(0,0,2,2)*IntervalMatrix({{{1,2},{2,3}},{{3,4},{4,5}}}).block(0,0,2,2) == IntervalMatrix({{{23,40},{34,53}},{{31,52},{46,69}}}));
}