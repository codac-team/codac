/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_SepInverse.h>
#include <codac2_analytic_operations.h>
#include <codac2_pave.h>
#include <codac2_Approx.h>

using namespace std;
using namespace codac2;

TEST_CASE("SepInverse")
{
  VectorVar x(2);
  AnalyticFunction f { {x}, vec(x[0],sqr(x[0]/7.)+sqr(x[1]/3.)) };
  SepInverse<IntervalVector> s(f, {{0,oo},{-oo,1}});

  //pave(IntervalVector({{-10,10},{-10,10}}), s, 0.1);

  {
    IntervalVector b({{0,0.8},{-2.28,-1.56}});
    auto xs = s.separate(b);
    //DefaultView::draw_box(b,Color::purple());
    //DefaultView::draw_box(xs.inner,Color::dark_green());
    //DefaultView::draw_box(xs.outer,Color::blue());
    CHECK(xs.inner == IntervalVector({{0},{-2.28,-1.56}}));
    CHECK(xs.outer == b);
  }

  {
    IntervalVector b({{4,5.4},{-0.05,2.45}});
    auto xs = s.separate(b);
    //DefaultView::draw_box(b,Color::purple());
    //DefaultView::draw_box(xs.inner,Color::dark_green());
    //DefaultView::draw_box(xs.outer,Color::blue());
    CHECK(Approx(xs.inner,1e-2) == IntervalVector({{4.039,5.40},{1.908,2.45}}));
    CHECK(xs.outer == b);
  }

  {
    IntervalVector b({{6.25,6.7},{0.9,1.85}});
    auto xs = s.separate(b);
    //DefaultView::draw_box(b,Color::purple());
    //DefaultView::draw_box(xs.inner,Color::dark_green());
    //DefaultView::draw_box(xs.outer,Color::blue());
    CHECK(Approx(xs.inner,1e-2) == IntervalVector({{6.25,6.70},{0.9, 1.85}}));
    CHECK(Approx(xs.inner,1e-2) == IntervalVector({{6.25,6.70},{0.9, 1.85}}));
  }

  {
    IntervalVector b({{-6,-5},{0,2}});
    auto xs = s.separate(b);
    //DefaultView::draw_box(b,Color::purple());
    //DefaultView::draw_box(xs.inner,Color::dark_green());
    //DefaultView::draw_box(xs.outer,Color::blue());
    CHECK(xs.inner == b);
    CHECK(xs.outer == IntervalVector::empty(2));
  }

  {
    IntervalVector b({{2,3},{-1,1}});
    auto xs = s.separate(b);
    //DefaultView::draw_box(b,Color::purple());
    //DefaultView::draw_box(xs.inner,Color::dark_green());
    //DefaultView::draw_box(xs.outer,Color::blue());
    CHECK(xs.inner == IntervalVector::empty(2));
    CHECK(xs.outer == b);
  }
}

TEST_CASE("SepInverse - other test")
{
  VectorVar x(2);
  AnalyticFunction f { {x}, vec(sqr(x[0])+sqr(x[1])) };
  SepInverse<IntervalVector> s(f, {{0,1}});

  //pave({{-5,5},{-5,5}}, s, 0.01);

  auto xs = s.separate(IntervalVector({{0.1,0.2},{0.1,0.2}})); // fully inside
  CHECK(xs.inner.is_empty());
  CHECK(xs.outer == IntervalVector({{0.1,0.2},{0.1,0.2}}));

  xs = s.separate(IntervalVector({{-0.2,0.2},{-0.2,0.2}})); // fully inside
  CHECK(xs.inner.is_empty());
  CHECK(xs.outer == IntervalVector({{-0.2,0.2},{-0.2,0.2}}));

  xs = s.separate(IntervalVector(2));
  CHECK(xs.inner == IntervalVector(2));
  CHECK(xs.outer == IntervalVector({{-1,1},{-1,1}}));
}