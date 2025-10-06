/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_CtcPolygon.h>
#include <codac2_pave.h>
#include <codac2_Figure2D.h>

using namespace std;
using namespace codac2;

TEST_CASE("CtcPolygon")
{
  Polygon p({{3,-1},{3,4},{5,6},{-1,1}});
  CtcPolygon c(p);

  IntervalVector x(2);
  c.contract(x);
  CHECK(x == IntervalVector({{-1,5},{-1,6}}));

  x = IntervalVector({{3.02,3.16},{2.5,3.2}}); // possible bug
  c.contract(x);
  CHECK(x.is_empty());

  //DefaultFigure::pave({{-3,7},{-3,8}}, c, 0.1);
  //DefaultFigure::draw_polygon({{3,-1},{3,4},{5,6},{-1,1}});
}

TEST_CASE("CtcPolygon - tests from Codac1")
{
  // Polygone with hole
  {
    CtcPolygon c({
      // external border
      Segment({{6,-6},{7,9}}),
      Segment({{7,9},{0,5}}),
      Segment({{0,5},{-9,8}}),
      Segment({{-9,8},{-8,-9}}),
      Segment({{-8,-9},{6,-6}}),
      // hole
      Segment({{-2,3},{3.5,2}}),
      Segment({{3.5,2},{1.5,0.5}}),
      Segment({{1.5,0.5},{3,-4}}),
      Segment({{3,-4},{-3,-3}}),
      Segment({{-3,-3},{-2,3}})
    });
    
    //DefaultFigure::pave(IntervalVector({{-10,10},{-10,10}}), c, .1);

    // Check a box inside the hole
    {
      IntervalVector x = IntervalVector({0,0}).inflate(0.5);
      //DefaultFigure::draw_box(x,Color::purple());
      c.contract(x);
      CHECK(x.is_empty());
    }

    // Check a box inside the polygon
    {
      IntervalVector x = IntervalVector({5,-5}).inflate(0.5), _x(x);
      //DefaultFigure::draw_box(x,Color::purple());
      c.contract(x);
      CHECK(x == _x);
    }

    // Check a box outside the polygon
    {
      IntervalVector x = IntervalVector({-1,8}).inflate(0.5);
      //DefaultFigure::draw_box(x,Color::purple());
      c.contract(x);
      CHECK(x.is_empty());
    }
  }

  {
    CtcPolygon c({{6,-6},{7,9},{0,5},{-9,8},{-8,-9}});

    // Check a box inside the polygon
    {
      IntervalVector x = IntervalVector({5,-5}).inflate(0.5), _x(x);
      c.contract(x);
      CHECK(x == _x);
    }

    // Check a box outside the polygon
    {
      IntervalVector x = IntervalVector({-1,8}).inflate(0.5);
      c.contract(x);
      CHECK(x.is_empty());
    }
  }
}