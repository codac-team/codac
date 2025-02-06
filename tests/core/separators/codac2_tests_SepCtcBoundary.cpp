/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_SepCtcBoundary.h>
#include <codac2_CtcSegment.h>
#include <codac2_CtcUnion.h>
#include <codac2_pave.h>

using namespace std;
using namespace codac2;

BoolInterval test_inside_diamond(const Vector& x)
{
  if((x[1] < x[0]+1) && (x[1] > x[0]-1) && (x[1] < -x[0]+1) && (x[1] > -x[0]-1))
    return BoolInterval::TRUE;
  else if(!(x[1] < x[0]+1) || !(x[1] > x[0]-1) || !(x[1] < -x[0]+1) || !(x[1] > -x[0]-1))
    return BoolInterval::FALSE;
  else
    return BoolInterval::UNKNOWN;
}

TEST_CASE("SepCtcBoundary")
{
  auto ctc_bound_diamond = CtcSegment({-1,0}, {0,-1}) | CtcSegment({0,-1}, {1,0})
    | CtcSegment({1,0}, {0,1}) | CtcSegment({0,1}, {-1,0});

  SepCtcBoundary sep_diamond(ctc_bound_diamond,test_inside_diamond);

  //draw_while_paving(IntervalVector({{-2,2},{-2,2}}), sep_diamond, 0.1);

  IntervalVector x(2);
  auto xs = sep_diamond.separate(IntervalVector(2));
  //DefaultView::draw_box(xs.inner,Color::dark_green());
  //DefaultView::draw_box(xs.outer,Color::blue());
  CHECK(xs.inner == IntervalVector(2));
  CHECK(xs.outer == IntervalVector({{-1,1},{-1,1}}));

  x = IntervalVector({{0,10},{0,10}});
  xs = sep_diamond.separate(x);
  //DefaultView::draw_box(xs.inner,Color::dark_green());
  //DefaultView::draw_box(xs.outer,Color::blue());
  CHECK(xs.inner == x);
  CHECK(xs.outer == IntervalVector({{0,1},{0,1}}));

  x = IntervalVector({{0.2,10},{0.2,10}});
  xs = sep_diamond.separate(x);
  //DefaultView::draw_box(xs.inner,Color::dark_green());
  //DefaultView::draw_box(xs.outer,Color::blue());
  CHECK(xs.inner == x);
  CHECK(xs.outer == IntervalVector({{0.2,0.8},{0.2,0.8}}));

  x = IntervalVector({{0.5,10},{0.5,10}});
  xs = sep_diamond.separate(x);
  //DefaultView::draw_box(xs.inner,Color::dark_green());
  //DefaultView::draw_box(xs.outer,Color::blue());
  CHECK(xs.inner == x);
  CHECK(xs.outer == IntervalVector({0.5,0.5}));
}