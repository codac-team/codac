/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_SepTransform.h>
#include <codac2_SepInverse.h>
#include <codac2_pave.h>
#include <codac2_Approx.h>
#include <codac2_Figure2D.h>

using namespace std;
using namespace codac2;

TEST_CASE("SepTransform")
{
  {
    VectorVar x(2);
    SepInverse s1(AnalyticFunction({x}, sqr(x[0])+sqr(x[1])), Interval(0,1));
    SepTransform s2(s1,
      AnalyticFunction({x}, x-Vector({2,2})),
      AnalyticFunction({x}, x+Vector({2,2})));

    //DefaultFigure::pave({{-5,5},{-5,5}}, s2, 0.01);

    auto xs = s2.separate(IntervalVector(2));
    CHECK(xs.inner == IntervalVector(2));
    CHECK(Approx(xs.outer,1e-1) == IntervalVector({{-3,-1},{-3,-1}}));

    xs = s2.separate(IntervalVector({{-2.2,-1.8},{-2.2,-1.8}}));
    CHECK(xs.inner.is_empty());
    CHECK(xs.outer == IntervalVector({{-2.2,-1.8},{-2.2,-1.8}}));

    IntervalVector b({{-2.5,-1.5},{-2,10}});
    //DefaultFigure::draw_box(b,{Color::purple(),Color::purple(.3)});
    xs = s2.separate(b);
    //DefaultFigure::draw_box(xs.inner,{Color::green(),Color::green(.3)});
    CHECK(Approx(xs.inner,1e-1) == IntervalVector({{-2.5,-1.5},{-1.134,10}}));
    CHECK(Approx(xs.outer,1e-1) == IntervalVector({{-2.5,-1.5},{-2,-1}}));
  }
}