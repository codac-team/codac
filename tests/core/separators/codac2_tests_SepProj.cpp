/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_SepProj.h>
#include <codac2_AnalyticFunction.h>
#include <codac2_SepInverse.h>
#include <codac2_pave.h>
#include <codac2_Approx.h>

using namespace std;
using namespace codac2;

TEST_CASE("SepProj")
{
  VectorVar x(3);
  AnalyticFunction f_ellipsoid({x}, 2*sqr(x[0])+x[0]*x[1]+x[0]*x[2]+sqr(x[1])+sqr(x[2]));
  SepInverse sep_ellipsoid(f_ellipsoid, {0.7,1});
  SepProj sep_proj(sep_ellipsoid, {0,1}, {{0.1,0.2}});

  // todo: test with SepProj sep_proj(sep_ellipsoid, {0,1}, {{-oo,oo}});

  //pave(IntervalVector({{-2,2},{-2,2}}), sep_proj, 0.1);

  {
    IntervalVector b({{0.2,0.4},{0.0,1}});
    //DefaultView::draw_box(b, Color::blue());
    auto bs = sep_proj.separate(b,1e-3);
    //DefaultView::draw_box(bs.inner, Color::green());
    //DefaultView::draw_box(bs.outer, Color::black());
    CHECK(Approx(bs.outer,1e-2) == IntervalVector({{0.2,0.4},{0,0.94}}));
  }

  {
    IntervalVector b({{-0.4,-0.2},{0.95,1.5}});
    //DefaultView::draw_box(b, Color::blue());
    auto bs = sep_proj.separate(b,1e-3);
    //DefaultView::draw_box(bs.inner, Color::green());
    //DefaultView::draw_box(bs.outer, Color::black());
    CHECK(Approx(bs.outer,1e-2) == IntervalVector({{-0.4,-0.2},{0.95,1.15}}));
  }

  {
    IntervalVector b({{0.65},{-0.5,-0.4}});
    //DefaultView::draw_box(b, Color::blue());
    auto bs = sep_proj.separate(b,1e-3);
    CHECK(bs.inner.is_empty());
  }
}