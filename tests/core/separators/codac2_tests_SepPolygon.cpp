/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_SepPolygon.h>
#include <codac2_pave.h>

using namespace std;
using namespace codac2;

TEST_CASE("SepPolygon")
{
  SepPolygon s({{3,-1},{3,4},{5,6},{-1,1}});

  IntervalVector x(2);
  auto xs = s.separate(IntervalVector(2));
  CHECK(xs.inner == IntervalVector(2));
  CHECK(xs.outer == IntervalVector({{-1,5},{-1,6}}));

  x = IntervalVector({{3.02,3.16},{2.5,3.2}}); // possible bug
  xs = s.separate(x);
  CHECK(xs.inner == x);
  CHECK(xs.outer.is_empty());
}

TEST_CASE("SepPolygon - tests from Codac1")
{
  // Polygone with hole
  {
    SepPolygon s({
      // external border
      {{6,-6},{7,9}},
      {{7,9},{0,5}},
      {{0,5},{-9,8}},
      {{-9,8},{-8,-9}},
      {{-8,-9},{6,-6}},
      // hole
      {{-2,3},{3.5,2}},
      {{3.5,2},{1.5,0.5}},
      {{1.5,0.5},{3,-4}},
      {{3,-4},{-3,-3}},
      {{-3,-3},{-2,3}}
    });
    
    //pave(IntervalVector({{-10,10},{-10,10}}), s, 0.1);

    // Check a box inside the hole
    {
      IntervalVector x = IntervalVector({{0},{0}}).inflate(0.5);
      //DefaultView::draw_box(x,Color::purple());
      auto xs = s.separate(x);
      CHECK(xs.inner == x);
      CHECK(xs.outer.is_empty());
    }

    // Check a box inside the polygon
    {
      IntervalVector x = IntervalVector({{5},{-5}}).inflate(0.5);
      //DefaultView::draw_box(x,Color::purple());
      auto xs = s.separate(x);
      CHECK(xs.inner.is_empty());
      CHECK(xs.outer == x);
    }

    // Check a box outside the polygon
    {
      IntervalVector x = IntervalVector({{-1},{8}}).inflate(0.5);
      //DefaultView::draw_box(x,Color::purple());
      auto xs = s.separate(x);
      CHECK(xs.inner == x);
      CHECK(xs.outer.is_empty());
    }
  }

  {
    SepPolygon s({{6,-6},{7,9},{0,5},{-9,8},{-8,-9}});

    // Check a box inside the polygon
    {
      IntervalVector x = IntervalVector({{5},{-5}}).inflate(0.5);
      auto xs = s.separate(x);
      CHECK(xs.inner.is_empty());
      CHECK(xs.outer == x);
    }

    // Check a box outside the polygon
    {
      IntervalVector x = IntervalVector({{-1},{8}}).inflate(0.5);
      auto xs = s.separate(x);
      CHECK(xs.inner == x);
      CHECK(xs.outer.is_empty());
    }
  }
}